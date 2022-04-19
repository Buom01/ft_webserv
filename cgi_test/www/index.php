<!DOCTYPE html>
<html>
	<meta charset="UTF-8">
	<body>
		<h2>GET method</h2>
		<form action="form_get.php" method="get">
			<p>Votre nom : <input type="text" name="nom" /></p>
			<p>Votre prénom : <input type="text" name="prenom" /></p>
			<p><input type="submit" value="envoyer le formulaire" /></p>
		</form>

		<h2>POST method</h2>
		<form action="form_post.php" method="post" enctype="multipart/form-data">
			<p>Votre nom : <input type="text" name="nom" /></p>
			<p>Votre prénom : <input type="text" name="prenom" /></p>
			<p>Un fichier : <input type="file" name="fichier" /></p>
			<p><input type="submit" value="envoyer le formulaire" /></p>
		</form>
	<a href="info.php">Print php info</a>
	</body>
</html>