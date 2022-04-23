function out(line = '')
{
	if (line)
		console.log(`${line}\r\n`);
	else
		console.log('\r\n');
}

out('Content-Type: text/html; charset=UTF-8');
out();
out('<body>');
out('	<h1>Hello World !</h1>');
out('	<h3>Je suis un homme comblé</h3>');
out('</body>');
