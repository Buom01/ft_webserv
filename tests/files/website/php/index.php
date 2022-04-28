<?php
	$cookie_name = 'daisy';
	if (!isset($_COOKIE['daisy']))
	{
		$cookie_value = 1;
		setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/");
	}
	else
	{
		$cookie_value = ++$_COOKIE['daisy'];
		setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/");
	}
	
?>
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
		<div class="uk-container uk-margin-large-top" style="margin-bottom: 2em;">
			<h2><?php echo "This page was generated via the php cgi"; ?></h2>
			<p><?php echo "Let's get to the heart of the matter quickly:"; ?></p>
			<hr/>
			<h3><?php  echo "🍪 Cookie time !"; ?></h3>
			<p><?php echo "When you opened this page, a cookie with the sweet name of <u>daisy</u> was created. A session was also opened. If you reload the page you will see the number of times you opened this page in your browser"; ?></p>
			<table class="uk-table uk-table-striped">
				<thead>
					<tr>
						<th>Cookie name</th>
						<th>Counter</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>
							<?php echo "$cookie_name"; ?>
						</td>
						<td>
							<?php echo "$cookie_value"; ?>
						</td>
					</tr>
				</tbody>
			</table>
			<hr/>
			<h2>GET method</h2>
			<form action="form_get.php" method="get">
				<p>First name: <input class="uk-input" type="text" name="first_name" /></p>
				<p>Last name: <input class="uk-input" type="text" name="last_name" /></p>
				<input type="submit" class="uk-button uk-button-primary" value="Send GET form" />
			</form>
			<hr/>
			<h2>POST method</h2>
			<form action="form_post.php" method="post" enctype="multipart/form-data">
				<p>First name : <input class="uk-input" type="text" name="first_name" /></p>
				<p>Last name : <input class="uk-input" type="text" name="last_name" /></p>
				<div class="uk-margin">
					<div uk-form-custom>
						<input type="file" name="file"/>
						<button class="uk-button uk-button-default" type="button" tabindex="-1">Select file 🗏</button>
					</div>
				</div>
				<input type="submit" class="uk-button uk-button-primary" value="Send POST form" />
			</form>
		</div>
	</body>
	<link rel="stylesheet" href="../dist/css/uikit.css" />
	<link rel="stylesheet" href="../dist/css/dark_theme.css" />
	<link rel="stylesheet" href="../dist/css/pacman.css" />
	<script defer src="../dist/js/uikit.min.js"></script>
	<script defer src="../dist/js/uikit-icons.min.js"></script>
</html>
