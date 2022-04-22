<?php
  // The global $_POST variable allows you to access the data sent with the POST method by name
  // To access the data sent with the GET method, you can use $_GET
  if (isset($_POST['say']))
    $say = htmlspecialchars($_POST['say']);
  else
    $say = "hi";
  if (isset($POST['to']))
    $to = htmlspecialchars($_POST['to']);
  else
    $to = "mom";

  echo "<h1>welcome into my php example</h1>";
  echo  $say, ' ', $to;
?>
