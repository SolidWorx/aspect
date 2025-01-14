/* aspect extension for PHP */

#ifndef PHP_ASPECT_H
# define PHP_ASPECT_H

extern zend_module_entry aspect_module_entry;
# define phpext_aspect_ptr &aspect_module_entry

# define PHP_ASPECT_VERSION "0.1.1"

PHP_MINIT_FUNCTION(aspect);
PHP_MSHUTDOWN_FUNCTION(aspect);
PHP_RINIT_FUNCTION(aspect);
PHP_RSHUTDOWN_FUNCTION(aspect);

ZEND_BEGIN_MODULE_GLOBALS(aspect)
    HashTable memoize_cache; // Global cache
ZEND_END_MODULE_GLOBALS(aspect)

#define ASPECT_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(aspect, v)

BEGIN_EXTERN_C()
extern ZEND_API zend_class_entry *zend_ce_memoize;
END_EXTERN_C()

# if defined(ZTS) && defined(COMPILE_DL_ASPECT)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_ASPECT_H */
