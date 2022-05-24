<?php
	if (isset($_REQUEST['say']))
		$say = htmlspecialchars($_REQUEST['say']);
	else
		$say = 'say';
	if (isset($_REQUEST['to']))
		$to = htmlspecialchars($_REQUEST['to']);
	else
		$to = 'to';
	
    echo "<!DOCTYPE html>\n";
    echo "<html>\n";
    echo "<head>\n";
    echo "<title>Webserv</title>\n";
    echo '<link rel="shortcut icon" href="https://lucie-lebriquer.fr/webserv/favicon.png"/>', "\n";
    echo '<meta charset="utf-8"/>', "\n";
	echo '<link rel="stylesheet" href="https://lucie-lebriquer.fr/webserv/main.css"/>', "\n";
    echo '<script src="https://kit.fontawesome.com/58cd01063a.js" crossorigin="anonymous"></script>', "\n";
    echo "</head>\n\n";
    echo "<header>\n";
	echo "\t", '<div class="logo">', "\n";
	echo "\t\t", '<img src="https://lucie-lebriquer.fr/webserv/logo.png">', "\n";
	echo "\t\t", '<div class="titlename">Webserv</div>', "\n";
	echo "\t\t", '<div class="title">euses</div>', "\n";
	echo "\t", '</div>', "\n";
	echo "\t", '<span class="buttonTab">', "\n";
    echo "\t\t", '<a href="../index.html"><i class="fas fa-home"></i><span class="tabname">Index</span></a>', "\n";
    echo "\t", '</span>', "\n";
	echo "\t", '<span class="buttonTab active">', "\n";
    echo "\t\t", '<a href="../form.html"><i class="fas fa-file"></i><span class="tabname">Form</span></a>', "\n";
    echo "\t", '</span>', "\n";
	echo "\t", '<span class="buttonTab">', "\n";
	echo "\t\t", '<a href="../upload.html"><i class="fas fa-upload"></i><span class="tabname">Upload</span></a>', "\n";
	echo "\t", '</span>', "\n";
	echo "\t", '<span class="buttonTab">', "\n";
    echo "\t\t", '<a href="https://github.com/LucieLeBriquer/webserv.git"><i class="fab fa-github"></i><span class="tabname">Source</span></a>', "\n";
    echo "\t", '</span>', "\n";
    echo '</header>', "\n\n";
    echo '<body class="main">', "\n";
	echo '<div class="vertical">', "\n";
	echo "\t", "<center>";
    echo "\t", '<h6 class="title404_1">You said <strong>', $say, '</strong></h6><br><br><br>';
	echo '<h6 class="title404_2">to <strong>', $to, '</strong></h6>', "\n";
	echo "</div>\n";
    echo '</body>', "\n\n";
    echo "</html>\n";
?>
