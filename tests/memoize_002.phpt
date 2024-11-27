--TEST--
Memoize attribute exists
--EXTENSIONS--
aspect
--FILE--
<?php
var_dump(class_exists('Memoize'));

#[Memoize]
function foo() {
    return 'foo';
}

var_dump((new ReflectionFunction('foo'))->getAttributes()[0]->getName());

class Bar {
    #[Memoize]
    public function bar() {
        return 'bar';
    }
}

$bar = new Bar();
var_dump((new ReflectionMethod($bar, 'bar'))->getAttributes()[0]->getName());

?>
--EXPECT--
bool(true)
string(7) "Memoize"
string(7) "Memoize"
