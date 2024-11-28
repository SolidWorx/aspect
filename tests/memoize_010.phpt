--TEST--
It doesn't memoize if the function returns void
--EXTENSIONS--
aspect
--FILE--
<?php

#[Memoize]
function foo(): void {
    echo 'running function'.PHP_EOL;
}

var_dump(foo() === foo());

?>
--EXPECT--
running function
running function
bool(true)
