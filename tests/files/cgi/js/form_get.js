const mod = require('../../website/dist/js/modules/module');
const __GET = mod.env.__GET;

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
mod.echo('<h1>GET node</h1>');
mod.echo(`<h3>Hello ${__GET.first_name} ${__GET.last_name} !</h3>`);
mod.echo('<p><a href=\'index.js\'>Go to index</a></p>');
mod.echo('</body>');
mod.echo('</html>');
