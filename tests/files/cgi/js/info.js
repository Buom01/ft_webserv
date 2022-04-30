const mod = require('../../website/dist/js/modules/module');

process.stdout.write('Content-type: text/html; charset=UTF-8\r\n');
process.stdout.write('\r\n');
mod.echo('<!DOCTYPE html>');
mod.echo('<html>');
mod.echo('<head>');
mod.echo('<meta charset="utf-8">');
mod.echo('<meta name="viewport" content="width=device-width, initial-scale=1">');
mod.echo('<meta http-equiv="Content-Security-Policy" content="default-src \'self\' \'unsafe-inline\' \'unsafe-eval\'">');
mod.echo('</head>');
mod.echo('<body>');
mod.echo('<h1>NODE js info</h1>');
//#region argv
mod.echo('<h2>Argv list</h2>');
mod.echo('<table class="uk-table uk-table-striped">');
mod.echo('<thead>');
mod.echo('<tr>');
mod.echo('<th>ID</th>');
mod.echo('<th>Value</th>');
mod.echo('</tr>');
mod.echo('</thead>');
mod.echo('<tbody>');
let i = Number(0);
for (const arg in mod.argv)
{
	mod.echo('<tr>');
	mod.echo(`<td>${i++}</td>`);
	mod.echo(`<td>${(!mod.argv[arg].length) ? 'null' : mod.argv[arg]}</td>`);
	mod.echo('</tr>');
}
mod.echo('</tbody>');
mod.echo('</table>');
//#endregion argv
//#region env
mod.echo('<h2>Env list</h2>');
mod.echo('<table class="uk-table uk-table-striped">');
mod.echo('<thead>');
mod.echo('<tr>');
mod.echo('<th>Name</th>');
mod.echo('<th>Value</th>');
mod.echo('</tr>');
mod.echo('</thead>');
mod.echo('<tbody>');
for (const arg in mod.env)
{
	if (arg === '__GET' || arg === '__POST' || arg === '__FILES')
		continue;
	mod.echo('<tr>');
	mod.echo(`<td>${arg}</td>`);
	mod.echo(`<td>${mod.env[arg]}</td>`);
	mod.echo('</tr>');
}
mod.echo('</tbody>');
mod.echo('</table>');
//#endregion env
mod.echo('</body>');
mod.echo('</html>');
