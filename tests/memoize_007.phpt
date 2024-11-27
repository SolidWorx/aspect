--TEST--
Static class methods can be memoized
--EXTENSIONS--
aspect
--FILE--
<?php

class Foo {
    #[Memoize]
    public static function foo() {
        return bin2hex(random_bytes(5));
    }

    #[Memoize]
    public static function bar($val1 = '', $val2 = '') {
        return $val1 . $val2 . bin2hex(random_bytes(5));
    }

    public static function baz() {
        return bin2hex(random_bytes(5));
    }
}

var_dump(Foo::foo() === Foo::foo());
var_dump(Foo::bar('a', 'b') === Foo::bar('a', 'b'));
var_dump(Foo::bar('a', 'c') !== Foo::foo('a', 'b'));
var_dump(Foo::baz() !== Foo::baz());

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
