--TEST--
Handle different argument types
--EXTENSIONS--
aspect
--FILE--
<?php

#[Memoize]
function test_int(int $value) {
    return bin2hex(random_bytes($value));
}

#[Memoize]
function test_string(string $value) {
    return bin2hex(random_bytes(strlen($value)));
}

#[Memoize]
function test_array(array $value) {
    return bin2hex(random_bytes(count($value)));
}

#[Memoize]
function test_mixed($value) {
    return bin2hex(random_bytes($value));
}

#[Memoize]
function test_closure(\Closure $value) {
    return bin2hex(random_bytes(5));
}

#[Memoize]
function test_object(object $value) {
    return bin2hex(random_bytes(5));
}

#[Memoize]
function test_resource($value) {
    return bin2hex(random_bytes(5));
}

$closure1 = fn() => 5;
$closure2 = fn() => 6;
$class1 = new stdClass;
$class2 = new stdClass;
$resource1 = fopen(__FILE__, 'r');

var_dump(test_int(5) === test_int(5));
var_dump(test_string('hello') === test_string('hello'));
var_dump(test_array([1, 2, 3]) === test_array([1, 2, 3]));
var_dump(test_mixed(5) === test_mixed(5));
var_dump(test_closure($closure1) === test_closure($closure1));
var_dump(test_object($class1) === test_object($class1));
var_dump(test_resource($resource1) === test_resource($resource1));

var_dump(test_int(6) !== test_int(5));
var_dump(test_string('hello') !== test_string('world'));
var_dump(test_array([1, 2, 3]) !== test_array([1, 2, 4]));
var_dump(test_mixed(6) !== test_mixed(5));
var_dump(test_closure($closure1) !== test_closure($closure2));
var_dump(test_object($class1) !== test_object($class2));
var_dump(test_resource(fopen(__FILE__, 'r')) !== test_resource(fopen('php://memory', 'r')));

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
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
