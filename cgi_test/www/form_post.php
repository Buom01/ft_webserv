<?php
	echo "<meta charset=\"UTF-8\">";
	$nom = $_POST['nom'];
	$prenom = $_POST['prenom'];
	echo "POST form";
	echo "<h3>Bienvenue à ".$prenom.' '.$nom,"</h3>";

	$target_dir = "uploads/";
	if (!empty($target_dir) && !is_dir($target_dir))
		mkdir($target_dir);    
	$target_file = $target_dir . basename($_FILES["fichier"]["name"]);
	$imageFileType = strtolower(pathinfo($target_file, PATHINFO_EXTENSION));
	$check = getimagesize($_FILES["fichier"]["tmp_name"]);
	if ($check !== false)
		echo "File is an image - " . $check["mime"] . ".";
	else
		echo "File is not an image.";
	move_uploaded_file($_FILES["fichier"]["tmp_name"], $target_file);
	echo "<p>Go to ".$target_file." for get your file</p>";
	echo "<p><a href='index.php'>Retour au formulaire</a></p>";
?>
