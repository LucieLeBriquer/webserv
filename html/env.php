<?php

//Call getenv() function without argument

$env_array = getenv();

echo "<!DOCTYPE html>";
echo "<html>";

echo "<head>";
echo  "<title>Webserv</title>";
echo  '<link rel="shortcut icon" href="https://lucie-lebriquer.fr/webserv/favicon.png"/>';
echo  '<meta charset="utf-8"/>';
echo  '<link rel="stylesheet" href="https://lucie-lebriquer.fr/webserv/main.css"/>';
echo  '<script src="https://kit.fontawesome.com/58cd01063a.js" crossorigin="anonymous"></script>';
echo "</head>";

echo "<header>";
echo  '<div class="logo">';
echo    '<img src="https://lucie-lebriquer.fr/webserv/logo.png">';
echo    '<div class="titlename">Webserv</div>';
echo    '<div class="title">euses</div>';
echo  '</div>';
echo  '<span class="buttonTab">';
echo    '<a href="index.html"><i class="fas fa-home"></i><span class="tabname">Index</span></a>';
echo  '</span>';
echo  '<span class="buttonTab">';
echo    '<a href="form.html"><i class="fas fa-file"></i><span class="tabname">Form</span></a>';
echo  '</span>';
echo  '<span class="buttonTab">';
echo    '<a href="https://github.com/LucieLeBriquer/webserv.git"><i class="fab fa-github"></i><span class="tabname">Source</span></a>';
echo  '</span>';
echo '</header>';

echo '<body class="main">';
echo  '<div class="vertical">';
echo    "<center>";
echo    "<h3>The list of environment variables with values are :</h3>";

//Print all environment variable names with values

foreach ($env_array as $key=>$value)
{
    echo "$key => $value <br />";
}

echo  '</div>';
echo '</body>';

echo "</html>";

?>