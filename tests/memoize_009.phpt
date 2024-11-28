--TEST--
It returns the same object
--EXTENSIONS--
aspect
--FILE--
<?php

#[Memoize]
function foo() {
    return new DateTime();
}

var_dump(foo() === foo());

?>
--EXPECT--
bool(true)
