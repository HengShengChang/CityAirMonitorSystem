<?php
require("dbinfo.php");
// Start XML file, create parent node
$dom = new DOMDocument("1.0");
$node = $dom->createElement("markers");
$parnode = $dom->appendChild($node);

// Opens a connection to a MySQL server
$connection=mysql_connect ('localhost', $username, $password);
if (!$connection) {  die('Not connected : ' . mysql_error());}

// Set the active MySQL database
$db_selected = mysql_select_db($database, $connection);
if (!$db_selected) {
  die ('Can\'t use db : ' . mysql_error());
}

// Select data in the UAM table
$query = "SELECT * FROM UAM where DATE='".$_GET['date']."' AND TIME BETWEEN ADDTIME('".$_GET['time']."','-0:30:0') AND ADDTIME('".$_GET['time']."','0:30:0')";
$result = mysql_query($query);
if (!$result) {
  die('Invalid query: ' . mysql_error());
}
header("Content-type: text/xml");
// Iterate through the rows, adding XML nodes for each

while ($row = @mysql_fetch_assoc($result)){
  // ADD TO XML DOCUMENT NODE
  $node = $dom->createElement("marker");
  $newnode = $parnode->appendChild($node);
  $newnode->setAttribute("lat", $row['LATITUDE']);
  $newnode->setAttribute("lng", $row['LONGITUDE']);
  $newnode->setAttribute("temp", $row['TEMP']);
  $newnode->setAttribute("humid", $row['HUMID']);
  $newnode->setAttribute("mq5", $row['MQ5']);
  $newnode->setAttribute("mq9", $row['MQ9']);
  $newnode->setAttribute("dust", $row['Dust']);
}
echo $dom->saveXML();
?>
