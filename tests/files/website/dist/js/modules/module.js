const process = require('process');
const http = require('http');

const argv = process.argv;
const env = process.env;

const echo = (str) => process.stdout.write(`${str}\n`);

const start = () =>
{
	process.stdout.write('Content-type: text/html; charset=UTF-8\r\n');
	process.stdout.write('\r\n');
	echo('<!DOCTYPE html>');
	echo('<html>');
	echo('	<head>');
	echo('		<meta charset="utf-8">');
	echo('		<meta name="viewport" content="width=device-width, initial-scale=1">');
	echo('		<meta http-equiv="Content-Security-Policy" content="default-src \'self\' \'unsafe-inline\' \'unsafe-eval\' wikipedia.org *.wikipedia.org wikimedia.org *.wikimedia.org">');
	echo('		<link rel="icon" type="image/png" href="../dist/img/logo.png">');
	echo('		<title>Zoupladou</title>');
	echo('	</head>');
	echo('	<body>');
	echo('		<div class="uk-flex uk-flex-around uk-flex-middle" id="main-nav">');
	echo('			<a class="uk-navbar-item uk-logo" href="#">');
	echo('				<img src="../dist/img/logo.png" style="width: 15vw; max-width: 110px;"/>');
	echo('			</a>');
	echo('			<a class="nav-light uk-button uk-button-text" href="../index.html">Home</a>');
	echo('			<a class="nav-light uk-button uk-button-text" href="../random.html">Random article</a>');
	echo('			<a class="nav-light uk-button uk-button-text" href="../php/index.php">PHP test</a>');
	echo('			<a class="nav-light uk-button uk-button-text" href="./index.js">Node.js test</a>');
	echo('		</div>');
	echo('		<div class="uk-container uk-margin-large-top">');
}

const end = () =>
{
	echo('		</div>');
	echo('	</body>');
	echo('	<link rel="stylesheet" href="../dist/css/uikit.css" />');
	echo('	<link rel="stylesheet" href="../dist/css/uikit.css" />');
	echo('	<link rel="stylesheet" href="../dist/css/dark_theme.css" />');
	echo('	<link rel="stylesheet" href="../dist/css/pacman.css" />');
	echo('	<script defer src="../dist/js/uikit.min.js"></script>');
	echo('	<script defer src="../dist/js/uikit-icons.min.js"></script>');
	echo('	<script defer src="../dist/js/modules/node_cookie.js"></script>');
	echo('</html>');
}

module.exports = { argv, env, echo, start, end };
