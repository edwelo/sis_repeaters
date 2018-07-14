<?php

ini_set('display_errors', 1);
error_reporting(E_ALL ^ E_NOTICE); //get rid of undefined variable notice
#error_reporting(E_ALL | E_STRICT);
date_default_timezone_set("Pacific/Palau");


//Page Setup and Print

$main_html = file_get_contents("frontend.tpl");

$page_names = array("Summary", "By Grade Level");

if(!$_GET["pn"]) {
	$page_name="Summary";
} else {
	if(in_array($_GET["pn"], $page_names)) {
		$page_name = $_GET["pn"];
	} else {
		$page_name = "Bad";
	}
}

$main_html = str_replace("{page_name}", $page_name, $main_html);

$page_content = file_get_contents("frontend_" . strtolower(str_replace(" ", "", $page_name)) . ".tpl");
$main_html = str_replace("<!-- page content -->", $page_content, $main_html);

print $main_html;

?>