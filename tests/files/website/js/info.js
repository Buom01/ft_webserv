const process = require('process');
const readline = require('readline');
const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
  terminal: false
});

function out(line = '')
{
	if (line)
		process.stdout.write(`${line}\n`);
	else
		process.stdout.write('\n');
}
const argv = process.argv;
const env = process.env;

process.stdout.write('Content-type: text/html; charset=UTF-8\r\n');
process.stdout.write('\r\n');
out('<body>');
out('	<h1>Hello World !</h1>');
out('	<h3>From Node.js</h3>');
out();
out('Argv variables:');
out('<ul>');
for (const arg of argv)
	out(`<li>${arg}</li>`);
out('</ul>');
out();
out('Env variables:');
out('<ul>');
for (const arg in env)
	out(`<li>${arg}=${env[arg]}</li>`);
out('</ul>');

out('Get content of stdin');
rl.on('line', (line) => out(line));

out('</body>');
