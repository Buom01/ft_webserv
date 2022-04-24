<!DOCTYPE html>
<html>
	<meta charset="UTF-8">
	<body>
		<h2>GET method</h2>
		<form action="form_get.php" method="get">
			<p>First name: <input type="text" name="first_name" /></p>
			<p>Last name: <input type="text" name="last_name" /></p>
			<p><input type="submit" value="Send GET form" /></p>
		</form>
		<h2>POST method</h2>
		<form action="form_post.php" method="post" enctype="multipart/form-data">
			<p>First name : <input type="text" name="first_name" /></p>
			<p>Last name : <input type="text" name="last_name" /></p>
			<p>A random file : <input type="file" name="file" /></p>
			<p><input type="submit" value="Send POST form" /></p>
		</form>
	<a href="info.php">Print php info</a>
	</body>
</html>
