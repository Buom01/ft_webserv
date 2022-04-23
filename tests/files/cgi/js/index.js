const process = require('process');

function out(line = '')
{
	if (line)
		process.stdout.write(`${line}\r\n`);
	else
		process.stdout.write('\r\n');
}

out('Status: 200 OK');
out('Content-type: text/html; charset=UTF-8');
out();
out('<body>');
out('	<h1>Hello World !</h1>');
out('	<h3>Je suis un homme comblé</h3>');
out('</body>');
