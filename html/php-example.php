<?php
  // The global $_POST variable allows you to access the data sent with the POST method by name
  // To access the data sent with the GET method, you can use $_GET
  if (getenv("REQUEST_METHOD") == 'GET')
  {
    if (isset($_GET['say']))
      $say = htmlspecialchars($_GET['say']);
    else
      $say = 'say';
    if (isset($_GET['to']))
      $to = htmlspecialchars($_GET['to']);
    else
      $to = 'to';
  }
  else if (getenv("REQUEST_METHOD") == 'POST')
  {
    if (isset($_POST['say']))
      $say = htmlspecialchars($_POST['say']);
    else
      $say = 'say';
    if (isset($_POST['to']))
      $to = htmlspecialchars($_POST['to']);
    else
      $to = 'to';
  } 
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
    echo    '<h1 style="text-align:center;">welcome to our PHP file, form results</h1><br><br><br>';
    echo    '<h6>You said --> <strong>', $say, '</strong></h6><br>';
    echo    '<h6>to --> <strong>', $to, '</strong></h6><br>';
	  echo  '</div>';
    echo '</body>';

    echo "</html>";
?>
