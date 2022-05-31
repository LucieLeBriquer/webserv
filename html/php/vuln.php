<?php

$page = $_GET['page'];
if(isset($page))
    include $page;
else
    include '../403.html';

?>

<!--
To solve the vulnerability add pages that are allowed by yourself :

...
$allowed = ['env.php','info.php','../index.html'];
if (in_array($page, $allowed))
...

-->