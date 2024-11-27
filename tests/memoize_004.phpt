--TEST--
Functions aren't memoized if they throw an exception
--EXTENSIONS--
aspect
--FILE--
<?php

#[Memoize]
function foo($message = '') {
    if ($message !== '') {
        throw new Exception($message);
    }

    return random_int(100, 200);
}

$first = foo();

var_dump($first === foo());

try {
    foo('foo');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

try {
    foo('foo');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

try {
    foo('bar');
} catch (Exception $e) {
    echo $e->getMessage(), PHP_EOL;
}

var_dump(foo() === foo());
var_dump($first === foo());

?>
--EXPECT--
bool(true)
foo
foo
bar
bool(true)
bool(true)
