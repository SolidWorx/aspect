#include <zend_attributes.h>

/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 7fe2851260165fea167af14286eceaf94f1a6699 */




static const zend_function_entry class_Memoize_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_Memoize(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "Memoize", class_Memoize_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL|ZEND_ACC_NO_DYNAMIC_PROPERTIES;

	zend_string *attribute_name_Attribute_class_Memoize_0 = zend_string_init_interned("Attribute", sizeof("Attribute") - 1, 1);
	zend_attribute *attribute_Attribute_class_Memoize_0 = zend_add_class_attribute(class_entry, attribute_name_Attribute_class_Memoize_0, 1);
	zend_string_release(attribute_name_Attribute_class_Memoize_0);
	zval attribute_Attribute_class_Memoize_0_arg0;
	ZVAL_LONG(&attribute_Attribute_class_Memoize_0_arg0, ZEND_ATTRIBUTE_TARGET_FUNCTION | ZEND_ATTRIBUTE_TARGET_METHOD);
	ZVAL_COPY_VALUE(&attribute_Attribute_class_Memoize_0->args[0].value, &attribute_Attribute_class_Memoize_0_arg0);

	return class_entry;
}
