const crypto = require('crypto');
const path = require('path');
const fs = require('fs');
const mod = require('../../website/dist/js/modules/module');
const __POST = mod.env.__POST;
const __FILES = mod.env.__FILES;
const uploadDir = 'upload';

const md5 = (str) => crypto.createHash('md5').update(str).digest('hex');
const fileContent = (path) => fs.readFileSync(path, {encoding: 'utf-8'});

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
mod.echo('<h1>POST node</h1>');
mod.echo(`<h3>Hello ${__POST.first_name} ${__POST.last_name} !</h3>`);
if (Object.entries(__FILES).length)
{
	const data = fileContent(__FILES['file']['tmp_name']);
	mod.echo(`File is an ${__FILES['file']['type']}, with an MD5 as : ${md5(data)}`);
	if (!fs.existsSync(path.join(__dirname, uploadDir)))
		fs.mkdirSync(path.join(__dirname, uploadDir));
	const target = path.join(__dirname, uploadDir, __FILES['file']['name']);
	fs.writeFileSync(target, data, { encoding: 'utf-8', flag: 'w' });
	mod.echo(`<p>Go to ./${uploadDir}/${__FILES['file']['name']} for get your file</p>`);
}
else
{
	mod.echo('<p>No file pass</p>');
}
mod.echo('<p><a href=\'index.js\'>Go to index</a></p>');
mod.echo('</body>');
mod.echo('</html>');
