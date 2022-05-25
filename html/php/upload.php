<?php

$target_dir = "../uploads/";
if (isset($_FILES["fileToUpload"]))
{
	$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
	$uploadOk = 1;
	$imageFileType = strtolower(pathinfo($target_file, PATHINFO_EXTENSION));
}
else {
	$nofile = true;
	$uploadOk = 0;
}

echo "<!DOCTYPE html>", "\n";
echo "<html>", "\n", "\n";
echo "<head>", "\n";
echo "\t", "<title>Webserv</title>", "\n";
echo "\t", '<link rel="shortcut icon" href="https://lucie-lebriquer.fr/webserv/favicon.png"/>', "\n";
echo "\t", '<meta charset="utf-8"/>', "\n";
echo "\t", '<link rel="stylesheet" href="https://lucie-lebriquer.fr/webserv/main.css"/>', "\n";
echo "\t", '<script src="https://kit.fontawesome.com/58cd01063a.js" crossorigin="anonymous"></script>', "\n";
echo "</head>", "\n", "\n";
echo "<header>", "\n";
echo "\t", '<div class="logo">', "\n";
echo "\t", "\t", '<img src="https://lucie-lebriquer.fr/webserv/logo.png">', "\n";
echo "\t", "\t", '<div class="titlename">Webserv</div>', "\n";
echo "\t", "\t", '<div class="title">euses</div>', "\n";
echo "\t", '</div>', "\n";
echo "\t", '<span class="buttonTab">', "\n";
echo "\t", "\t", '<a href="../index.html"><i class="fas fa-home"></i><span class="tabname">Index</span></a>', "\n";
echo "\t", '</span>', "\n";
echo "\t", '<span class="buttonTab">', "\n";
echo "\t", "\t", '<a href="../form.html"><i class="fas fa-file"></i><span class="tabname">Form</span></a>', "\n";
echo "\t", '</span>', "\n";
echo "\t", '<span class="buttonTab active">';
echo "\t", "\t", '<a href="../upload.html"><i class="fas fa-upload"></i><span class="tabname">Upload</span></a>', "\n";
echo "\t", '</span>', "\n";
echo "\t", '<span class="buttonTab">', "\n";
echo "\t", "\t", '<a href="https://github.com/LucieLeBriquer/webserv.git"><i class="fab fa-github"></i><span class="tabname">Source</span></a>', "\n";
echo "\t", '</span>', "\n";
echo '</header>', "\n";
echo '<body class="main">', "\n";

if ($nofile)
	echo "Please, choose a file.<br>", "\n";
else if (file_exists($target_file))
{
	echo "Sorry, file already exists.<br>", "\n";
	$uploadOk = 0;
}

if ($uploadOk == 0)
	echo "Your file was not uploaded.<br>", "\n";
else
{
	if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file))
		echo "The file ". htmlspecialchars( basename( $_FILES["fileToUpload"]["name"])). " has been uploaded.<br>", "\n";
	else
		echo "Sorry, there was an error uploading your file.<br>", "\n";
}
echo '</body>', "\n";
echo "</html>", "\n";

?>