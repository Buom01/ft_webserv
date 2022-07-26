<!DOCTYPE html>
<html>
	<head>
		<meta charset="utf-8">
		<meta name="viewport" content="width=device-width, initial-scale=1">
		<meta http-equiv="Content-Security-Policy" content="default-src 'self' 'unsafe-inline' 'unsafe-eval' wikipedia.org *.wikipedia.org wikimedia.org *.wikimedia.org">
		<link rel="icon" type="image/png" href="../dist/img/logo.png">
		<title>Zoupladou</title>
	</head>
	<body>
		<div class="uk-flex uk-flex-around uk-flex-middle" id="main-nav">
			<a class="uk-navbar-item uk-logo" href="#">
				<img src="../dist/img/logo.png" style="width: 15vw; max-width: 110px;"/>
			</a>
			<a class="nav-light uk-button uk-button-text" href="../index.html">Home</a>
			<a class="nav-light uk-button uk-button-text" href="../random.html">Random article</a>
			<a class="nav-light uk-button uk-button-text" href="./index.php">PHP test</a>
			<a class="nav-light uk-button uk-button-text" href="../js/index.js">Node.js test</a>
		</div>
		<div class="uk-container uk-margin-large-top">
			<?php
				echo "<meta charset=\"UTF-8\">";
				$firstname = $_POST['first_name'];
				$lastname = $_POST['last_name'];
				echo "POST form";
				echo "<h3>Hello ".$firstname.' '.$lastname." !</h3>";

				$target_dir = "uploads/";
				if (!empty($target_dir) && !is_dir($target_dir))
					mkdir($target_dir);    
				$target_file = $target_dir . basename($_FILES["file"]["name"]);
				$imageFileType = strtolower(pathinfo($target_file, PATHINFO_EXTENSION));
				echo "File is an ". $_FILES["file"]["type"] .", with an MD5 as : " . md5(file_get_contents($_FILES["file"]["tmp_name"]));
				move_uploaded_file($_FILES["file"]["tmp_name"], $target_file);
				echo "<p>Go to ".$target_file." for get your file</p>";
				echo "<p><a href='index.php'>Go to index</a></p>";
			?>
		</div>
	</body>
	<link rel="stylesheet" href="../dist/css/uikit.css" />
	<link rel="stylesheet" href="../dist/css/dark_theme.css" />
	<link rel="stylesheet" href="../dist/css/pacman.css" />
	<script defer src="../dist/js/uikit.min.js"></script>
	<script defer src="../dist/js/uikit-icons.min.js"></script>
</html>
