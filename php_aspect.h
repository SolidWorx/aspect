/* aspect extension for PHP */

#ifndef PHP_ASPECT_H
# define PHP_ASPECT_H

extern zend_module_entry aspect_module_entry;
# define phpext_aspect_ptr &aspect_module_entry

# define PHP_ASPECT_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_ASPECT)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_ASPECT_H */
