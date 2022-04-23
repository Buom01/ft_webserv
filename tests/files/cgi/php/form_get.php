<?php
	echo "<meta charset=\"UTF-8\">";
	$firstname = $_GET['first_name'];
	$lastname = $_GET['last_name'];
	echo "GET form";
	echo "<h3>Hello ".$firstname.' '.$lastname." !</h3>";
	echo "<p><a href='index.php'>Go to index</a></p>";
?>
