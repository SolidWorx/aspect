/* aspect extension for PHP */

#ifdef HAVE_CONFIG_H

# include <config.h>
# include <zend_smart_str.h>

#endif

#include "php.h"
#include "ext/standard/info.h"
#include "ext/standard/php_standard.h"
#include "php_aspect.h"
#include "aspect_arginfo.h"
#include "zend_attributes.h"
#include "zend_exceptions.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
 ZEND_PARSE_PARAMETERS_START(0, 0) \
 ZEND_PARSE_PARAMETERS_END()
#endif

ZEND_API zend_class_entry *zend_ce_memoize;

ZEND_DECLARE_MODULE_GLOBALS(aspect)

static void (*original_zend_execute_ex)(zend_execute_data *execute_data);

ZEND_API static void aspect_execute_ex(zend_execute_data *execute_data);

static void handle_memoize_functions(zend_execute_data *execute_data);

static zend_string *compute_cache_key(zend_execute_data *execute_data) {
    smart_str key = {0};
    zend_function *func = execute_data->func;

    // Check if it's a method call
    if (func->common.scope) {
        // Check if it's a static method call
        if (func->common.fn_flags & ZEND_ACC_STATIC) {
            // Append fully qualified class name
            smart_str_appends(&key, ZSTR_VAL(func->common.scope->name));
        } else {
            // Append object handle
            smart_str_append_long(&key, (long) Z_OBJ_HANDLE(execute_data->This));
        }
        // Append method name
        smart_str_appends(&key, "::");
        smart_str_appends(&key, ZSTR_VAL(func->common.function_name));
    } else {
        // Append function name
        smart_str_appends(&key, ZSTR_VAL(func->common.function_name));
    }

    // Serialize arguments
    zval *params = ZEND_CALL_ARG(execute_data, 1);
    for (uint32_t i = 0; i < ZEND_CALL_NUM_ARGS(execute_data); i++) {
        switch (Z_TYPE(params[i])) {
            case IS_OBJECT: {
                // Use object handle (unique for each object instance)
                smart_str_append_long(&key, (long) Z_OBJ_HANDLE(params[i]));
                break;
            }
            case IS_CALLABLE: {
                // Serialize callable as string
                smart_str_appendc(&key, ':');
                zend_string *callable_str = zval_get_string(&params[i]);
                smart_str_appends(&key, ZSTR_VAL(callable_str));
                zend_string_release(callable_str);
                break;
            }
            case IS_RESOURCE: {
                // Serialize resource as string
                smart_str_appendc(&key, ':');
                smart_str_append_long(&key, Z_RES_HANDLE(params[i]));
                break;
            }
            default: {
                // Serialize other types as usual
                smart_str_appendc(&key, ':');
                php_serialize_data_t var_hash;
                PHP_VAR_SERIALIZE_INIT(var_hash);
                php_var_serialize(&key, &params[i], &var_hash);
                PHP_VAR_SERIALIZE_DESTROY(var_hash);
                break;
            }
        }
    }

    smart_str_0(&key);

    return key.s;
}

static void aspect_execute_ex(zend_execute_data *execute_data) {
    zend_function *func = execute_data->func;

    // Check if function name is set
    if (!func->common.function_name) {
        original_zend_execute_ex(execute_data);
        return;
    }

    if (func->common.attributes && zend_get_attribute_str(func->common.attributes, "memoize", sizeof("memoize") - 1)) {
        handle_memoize_functions(execute_data);
        return;
    }

    // Default execution for non-memoized functions
    original_zend_execute_ex(execute_data);
}

static void handle_memoize_functions(zend_execute_data *execute_data) {
    zend_string *cache_key = compute_cache_key(execute_data);
    zval *cached_value = zend_hash_find(&ASPECT_G(memoize_cache), cache_key);

    if (cached_value) {
        // Return the cached value
        ZVAL_COPY(EX(return_value), cached_value);
        zend_string_release(cache_key);
        return;
    }

    // Call the original function
    original_zend_execute_ex(execute_data);

    if (EG(exception) || EG(exit_status)) {
        zend_string_release(cache_key);
        return;
    }

    if (Z_TYPE_P(EX(return_value)) == IS_UNDEF) {
        php_error_docref(NULL, E_WARNING, "Return value is undefined");
        zend_string_release(cache_key);
        return;
    }

    // Cache the result if no exception or exit occurred
    zval cache_copy;
    ZVAL_DUP(&cache_copy, EX(return_value));

    if (zend_hash_add(&ASPECT_G(memoize_cache), cache_key, &cache_copy) == NULL) {
        php_error_docref(NULL, E_WARNING, "Failed to add cache entry");
        zval_ptr_dtor(&cache_copy); // Clean up if adding fails
        zend_string_release(cache_key);
        return;
    }

    zend_string_release(cache_key);
}

PHP_MINIT_FUNCTION (aspect) {
    // Initialize the memoize cache
    zend_hash_init(&ASPECT_G(memoize_cache), 8, NULL, ZVAL_PTR_DTOR, 1);

    original_zend_execute_ex = zend_execute_ex;

    zend_execute_ex = aspect_execute_ex;

    // Register the Memoize attribute
    zend_ce_memoize = register_class_Memoize();

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION (aspect) {
    // Destroy the memoize cache
    zend_hash_destroy(&ASPECT_G(memoize_cache));

    zend_execute_ex = original_zend_execute_ex;
    return SUCCESS;
}

PHP_RINIT_FUNCTION (aspect) {
#if defined(ZTS) && defined(COMPILE_DL_ASPECT)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION (aspect) {
    zend_hash_clean(&ASPECT_G(memoize_cache));
    return SUCCESS;
}

PHP_MINFO_FUNCTION (aspect) {
    php_info_print_table_start();
    php_info_print_table_row(2, "aspect support", "enabled");
    php_info_print_table_row(2, "Version", PHP_ASPECT_VERSION);
    php_info_print_table_end();
}

zend_module_entry aspect_module_entry = {
        STANDARD_MODULE_HEADER,
        "aspect",                  /* Extension name */
        NULL,                      /* zend_function_entry */
        PHP_MINIT(aspect),         /* PHP_MINIT - Module initialization */
        PHP_MSHUTDOWN(aspect),     /* PHP_MSHUTDOWN - Module shutdown */
        PHP_RINIT(aspect),         /* PHP_RINIT - Request initialization */
        PHP_RSHUTDOWN(aspect),     /* PHP_RSHUTDOWN - Request shutdown */
        PHP_MINFO(aspect),         /* PHP_MINFO - Module info */
        PHP_ASPECT_VERSION,        /* Version */
        STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_ASPECT
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif

ZEND_GET_MODULE(aspect)

#endif
