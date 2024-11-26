--TEST--
test1() Basic test
--EXTENSIONS--
aspect
--FILE--
<?php
$ret = test1();

var_dump($ret);
?>
--EXPECT--
The extension aspect is loaded and working!
NULL
