const crypto = require('crypto');
const path = require('path');
const fs = require('fs');
const mod = require('../dist/js/modules/module');
const __POST = mod.env.__POST;
const __FILES = mod.env.__FILES;
const uploadDir = 'upload';

const md5 = (str) => crypto.createHash('md5').update(str).digest('hex');
const fileContent = (path) => fs.readFileSync(path, {encoding: 'utf-8'});

mod.start();
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
	mod.echo('No file pass');
}
mod.echo('<p><a href=\'index.js\'>Go to index</a></p>');
mod.end();
