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
			<?php echo "Hello from PHP"; ?>
		</div>
	</body>
	<link rel="stylesheet" href="../dist/css/uikit.css" />
	<link rel="stylesheet" href="../dist/css/dark_theme.css" />
	<link rel="stylesheet" href="../dist/css/pacman.css" />
	<script defer src="../dist/js/uikit.min.js"></script>
	<script defer src="../dist/js/uikit-icons.min.js"></script>
</html>
