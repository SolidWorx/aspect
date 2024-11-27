--TEST--
Functions are memoized with different parameters
--EXTENSIONS--
aspect
--FILE--
<?php

#[Memoize]
function foo($val1 = '', $val2 = '') {
    return $val1 . $val2 . bin2hex(random_bytes(5));
}

var_dump(foo('a', 'b') === foo('a', 'b'));
var_dump(foo('a', 'c') !== foo('a', 'b'));
var_dump(foo('a', 'c') === foo('a', 'c'));

// Functions should only be memoized with the same object instance
class Baz {
    public string $id;
    public function __construct() {
        $this->id = bin2hex(random_bytes(5));
    }
}

#[Memoize]
function bar(Baz $baz) {
    return bin2hex(random_bytes(5));
}

$baz1 = new Baz;

var_dump(bar($baz1) !== bar(new Baz));
var_dump(bar($baz1) === bar($baz1));

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
