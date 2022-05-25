<?php

//Call getenv() function without argument

$env_array = getenv();

echo "<!DOCTYPE html>\n";
echo "<html>\n";

echo "<head>\n";
echo "\t", "<title>Webserv</title>\n";
echo "\t", '<link rel="shortcut icon" href="https://lucie-lebriquer.fr/webserv/favicon.png"/>', "\n";
echo "\t", '<meta charset="utf-8"/>', "\n";
echo "\t", '<link rel="stylesheet" href="https://lucie-lebriquer.fr/webserv/main.css"/>', "\n";
echo "\t", '<script src="https://kit.fontawesome.com/58cd01063a.js" crossorigin="anonymous"></script>', "\n";
echo "</head>\n";

echo "<header>", "\n";
echo "\t", '<div class="logo">', "\n";
echo "\t", "\t", '<img src="https://lucie-lebriquer.fr/webserv/logo.png">', "\n";
echo "\t", "\t", '<div class="titlename">Webserv</div>', "\n";
echo "\t", "\t", '<div class="title">euses</div>', "\n";
echo "\t", '</div>', "\n";
echo "\t", '<span class="buttonTab">', "\n";
echo "\t", "\t", '<a href="index.html"><i class="fas fa-home"></i><span class="tabname">Index</span></a>', "\n";
echo "\t", '</span>', "\n";
echo "\t", '<span class="buttonTab">', "\n";
echo "\t", "\t", '<a href="form.html"><i class="fas fa-file"></i><span class="tabname">Form</span></a>', "\n";
echo "\t", '</span>', "\n";
echo "\t", '<span class="buttonTab">', "\n";
echo "\t", "\t", '<a href="../upload.html"><i class="fas fa-upload"></i><span class="tabname">Upload</span></a>', "\n";
echo "\t", '</span>', "\n";
echo "\t", '<span class="buttonTab">', "\n";
echo "\t", "\t", '<a href="https://github.com/LucieLeBriquer/webserv.git"><i class="fab fa-github"></i><span class="tabname">Source</span></a>', "\n";
echo "\t", '</span>', "\n";
echo '</header>', "\n", "\n";

echo '<body class="main">', "\n";
echo "\t", "<h3>The list of environment variables with values are :</h3>", "\n";
foreach ($env_array as $key=>$value)
{
    echo "\t", "$key => $value <br>", "\n";
}
echo '</body>', "\n", "\n";
echo "</html>", "\n";

?>