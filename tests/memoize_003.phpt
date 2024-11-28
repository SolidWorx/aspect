--TEST--
Memoize function calls
--EXTENSIONS--
aspect
--FILE--
<?php
function bar() {
    return random_int(100, 200);
}

#[Memoize]
function foo() {
    echo 'running foo'.PHP_EOL;
    return random_int(100, 200);
}

$foo = foo();

var_dump(bar() === bar());
var_dump(foo() === foo());
var_dump($foo === foo());

?>
--EXPECT--
running foo
bool(false)
bool(true)
bool(true)
