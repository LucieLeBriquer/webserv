<?php

$target_dir = "../uploads/";
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
$uploadOk = 1;
$imageFileType = strtolower(pathinfo($target_file, PATHINFO_EXTENSION));


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
echo    '<a href="../index.html"><i class="fas fa-home"></i><span class="tabname">Index</span></a>';
echo  '</span>';
echo  '<span class="buttonTab">';
echo    '<a href="../form.html"><i class="fas fa-file"></i><span class="tabname">Form</span></a>';
echo  '</span>';
echo  '<span class="buttonTab active">';
echo    '<a href="../upload.html"><i class="fas fa-upload"></i><span class="tabname">Upload</span></a>';
echo  '</span>';
echo  '<span class="buttonTab">';
echo    '<a href="https://github.com/LucieLeBriquer/webserv.git"><i class="fab fa-github"></i><span class="tabname">Source</span></a>';
echo  '</span>';
echo '</header>';

echo '<body class="main">';
  // Check if file already exists
  if (file_exists($target_file))
  {
    echo "Sorry, file already exists.<br>";
    $uploadOk = 0;
  }

  // Check if $uploadOk is set to 0 by an error
  if ($uploadOk == 0)
    echo "Your file was not uploaded.<br>";
  else
  {
    // if everything is ok, try to upload file
    if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file))
      echo "The file ". htmlspecialchars( basename( $_FILES["fileToUpload"]["name"])). " has been uploaded.<br>";
    else
      echo "Sorry, there was an error uploading your file.<br>";
  }
echo '</body>';
echo "</html>";

?>