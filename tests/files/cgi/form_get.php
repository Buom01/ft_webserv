<?php
	echo "<meta charset=\"UTF-8\">";
	$nom = $_GET['nom'];
	$prenom = $_GET['prenom'];
	echo "GET form";
	echo "<h3>Bienvenue à ".$prenom.' '.$nom,"</h3>";
	echo "<p><a href='index.php'>Retour au formulaire</a></p>";
?>
