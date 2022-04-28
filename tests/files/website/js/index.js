const path = require('path');
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
	mod.echo('		<tr>');
	mod.echo(`			<td>${arg}</td>`);
	mod.echo(`			<td>${(!mod.env[arg].length) ? 'null' : mod.env[arg]}</td>`);
	mod.echo('		</tr>');
}
mod.echo('	</tbody>');
mod.echo('</table>');
mod.echo('<hr/>');
//#endregion env

mod.end();
