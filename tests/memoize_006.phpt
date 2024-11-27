--TEST--
Class methods can be memoized
--EXTENSIONS--
aspect
--FILE--
<?php

class Foo {
    #[Memoize]
    public function foo() {
        return bin2hex(random_bytes(5));
    }

    #[Memoize]
    public function bar($val1 = '', $val2 = '') {
        return bin2hex(random_bytes(5));
    }

    public function baz() {
        return bin2hex(random_bytes(5));
    }
}

$foo = new Foo;
var_dump($foo->foo() === $foo->foo());
var_dump($foo->baz() !== $foo->baz());
var_dump($foo->bar('a', 'b') === $foo->bar('a', 'b'));
var_dump($foo->bar('a', 'c') !== $foo->bar('a', 'b'));
var_dump($foo->bar('a', 'c') === $foo->bar('a', 'c'));

// only instances of the same class is memoized
$foo1 = new Foo;
$foo2 = new Foo;
var_dump($foo1->foo() === $foo1->foo());
var_dump($foo2->foo() === $foo2->foo());
var_dump($foo1->foo() !== $foo2->foo());
var_dump($foo1->bar('a', 'b') !== $foo2->bar('a', 'b'));

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
