<?php

ini_set('display_errors', 1);
error_reporting(E_ALL ^ E_NOTICE); //get rid of undefined variable notice
#error_reporting(E_ALL | E_STRICT);
date_default_timezone_set("Pacific/Palau");

$db_host = "mysql.moe"; $db_user = "mysql"; $db_pass = "mysql";
if($_SERVER["REMOTE_ADDR"] == "127.0.0.1" || $_SERVER["REMOTE_ADDR"] == "::1") {
	$db_host = "localhost";
}

//The stuenroll_static table is extracted from the stuenroll table.
//It contains one student record per sy. That record is the latest
//record based on enrdate.
$db_tbl = "people.stuenroll_static";

if($_GET["a"]) {
	echo $_GET["pn"] . "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
	echo "sy: " . $_GET["sy"] . ",&nbsp;&nbsp;";
	echo "schid: " . $_GET["schid"] . ",&nbsp;&nbsp;";
	echo "glvl: " . $_GET["glvl"] . "<br />\n";
	echo "db connection: " . $db_host . " " . $db_user . " " . $db_pass . "<br />\n";

	$sqlPrintStr = "<p style='font-size: small;'>%s</p>\n";
}

# Having issues with 5.7, see comments below
$db = new mysqli($db_host, $db_user, $db_pass);

if(is_numeric($_GET["schid"])) $schid = $_GET["schid"];
if(is_numeric($_GET["sy"])) $sy = $_GET["sy"];
if(is_numeric($_GET["glvl"])) $glvl = $_GET["glvl"];

if($_GET["pn"] == "Summary") {
	require_once("backend_summary.inc");
} else if($_GET["pn"] == "By Grade Level") {
	require_once("backend_by_grade_level.inc");
} else {

}

if(isset($_GET["a"])) {
	echo "<pre>"; print_r($data); echo "</pre>";
} else {
	$myJSON = json_encode($data);
	echo $myJSON;
}

/* ### Had Problems with MySQL 5.7, still unresolved
   ### ...
mysqli_report(MYSQLI_REPORT_STRICT);
try {
     $db = new mysqli($db_host, $db_user, $db_pass);
} catch (Exception $e ) {
     echo "Service unavailable";
     echo "message: " . $e->getMessage;   // not in live code obviously...
     exit;
}
*/

?>