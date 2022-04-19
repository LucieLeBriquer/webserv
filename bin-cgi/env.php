<?php

echo "HTTP/1.1 200 OK";
echo "Host: localhost:8080";
echo "Content-Type: text/html";
echo "";

//Call getenv() function without argument

$env_array = getenv();

echo "<h3>The list of environment variables with values are :</h3>";

//Print all environment variable names with values

foreach ($env_array as $key=>$value)
{
    echo "$key => $value <br />";
}

?>