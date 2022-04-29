const mod = require('../dist/js/modules/module');

mod.start();

mod.echo('<h1>This page was generated via node js</h1>');
mod.echo('<p>Let\'s get to the heart of the matter quickly:</p>');
mod.echo('<hr/>');

//#region cookie
mod.echo('<h2>🍪 Cookie time !</h2>');
mod.echo('<p>When you opened this page, a cookie with the sweet name of <u>daisy_js</u> was created. A session was also opened. If you reload the page you will see the number of times you opened this page in your browser</p>');
mod.echo('<table class="uk-table uk-table-striped">');
mod.echo('	<thead>');
mod.echo('		<tr>');
mod.echo('			<th>Cookie name</th>');
mod.echo('			<th>Counter</th>');
mod.echo('		</tr>');
mod.echo('	</thead>');
mod.echo('	<tbody>');
mod.echo('		<tr>');
mod.echo(`			<td>daisy_js</td>`);
mod.echo(`			<td id="cookie_value"></td>`);
mod.echo('		</tr>');
mod.echo('	</tbody>');
mod.echo('</table>');
mod.echo('<hr/>');
//#endregion

//#region argv
mod.echo('<h2>Argv list</h2>');
mod.echo('<table class="uk-table uk-table-striped">');
mod.echo('	<thead>');
mod.echo('		<tr>');
mod.echo('			<th>ID</th>');
mod.echo('			<th>Value</th>');
mod.echo('		</tr>');
mod.echo('	</thead>');
mod.echo('	<tbody>');
let i = Number(0);
for (const arg in mod.argv)
{
	mod.echo('		<tr>');
	mod.echo(`			<td>${i++}</td>`);
	mod.echo(`			<td>${(!mod.argv[arg].length) ? 'null' : mod.argv[arg]}</td>`);
	mod.echo('		</tr>');
}
mod.echo('	</tbody>');
mod.echo('</table>');
//#endregion argv

//#region env
mod.echo('<h2>Env list</h2>');
mod.echo('<table class="uk-table uk-table-striped">');
mod.echo('	<thead>');
mod.echo('		<tr>');
mod.echo('			<th>Name</th>');
mod.echo('			<th>Value</th>');
mod.echo('		</tr>');
mod.echo('	</thead>');
mod.echo('	<tbody>');
for (const arg in mod.env)
{
	if (arg === '__GET' || arg === '__POST' || arg === '__FILES')
		continue;
	mod.echo('		<tr>');
	mod.echo(`			<td>${arg}</td>`);
	mod.echo(`			<td>${mod.env[arg]}</td>`);
	mod.echo('		</tr>');
}
mod.echo('	</tbody>');
mod.echo('</table>');
mod.echo('<hr/>');
//#endregion env

//#region form
mod.echo('<h2>GET method</h2>');
mod.echo('<form action="form_get.js" method="get">');
mod.echo('	<p>First name : <input class="uk-input" type="text" name="first_name" /></p>');
mod.echo('	<p>Last name : <input class="uk-input" type="text" name="last_name" /></p>');
mod.echo('	<input type="submit" class="uk-button uk-button-primary" value="Send GET form" />');
mod.echo('</form>');
mod.echo('<hr/>');
mod.echo('<h2>POST method</h2>');

mod.echo('<div uk-alert>');
mod.echo('	<h3>Notice</h3>');
mod.echo('	<p>The file post works on text files, but very badly on binary files (like images or pdf). This is due to the complexity of the encodings and of the way Node stores the data. Therefore, this form is only present for demonstration purposes, but the files created are very often corrupted (and if you want more details on why, don\'t hesitate to ask)</p>');
mod.echo('</div>');


mod.echo('<form action="form_post.js" method="post" enctype="multipart/form-data">');
mod.echo('	<p>First name : <input class="uk-input" type="text" name="first_name" /></p>');
mod.echo('	<p>Last name : <input class="uk-input" type="text" name="last_name" /></p>');
mod.echo('	<div class="uk-margin">');
mod.echo('		<div uk-form-custom>');
mod.echo('			<input type="file" name="file"/>');
mod.echo('			<button class="uk-button uk-button-default" type="button" tabindex="-1">Select file 🗏</button>');
mod.echo('		</div>');
mod.echo('	</div>');
mod.echo('	<input type="submit" class="uk-button uk-button-primary" value="Send POST form" />');
mod.echo('</form>');
//#endregion form

mod.end();
