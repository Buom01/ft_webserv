const crypto = require('crypto');
const fs = require('fs');
const mime = require('./mimetypes');
const path = require('path');
const process = require('process');
const readline = require('./n-readlines');
const { exit } = require('process');

let argv = process.argv;
let env = {};

class bodyParseClass
{
	constructor(contentType = String, body = String)
	{
		this.tmpFile = path.join(`/tmp/${crypto.randomUUID()}`);
		fs.writeFileSync(this.tmpFile, body);
		this.contentType = contentType.split(';');
		this.args = {
			GET: {},
			POST: {},
			FILE: {},
		};
	}

	start()
	{
		if (!this.contentType.length)
			return true;
		switch (this.contentType[0])
		{
			case 'application/x-www-form-urlencoded':
				this.urlencoded();
				break;
			case 'multipart/form-data':
				this.multipart();
				break;
		}
		for (const _var in process.env)
			env[_var] = process.env[_var];
		env['__GET'] = {};
		env['__POST'] = {};
		env['__FILES'] = {};
		const _get = process.env['QUERY_STRING'];
		if (_get.length)
		{
			const __vars = _get.split('&');
			for (const __var of __vars)
			{
				const el = __var.split("=");
				env['__GET'][el[0].replace('+', ' ')] = el[1].replace('+', ' ');
			}
		}
		for (const el in this.args.POST)
			env['__POST'][el] = this.args.POST[el];
		for (const el in this.args.FILE)
			env['__FILES'][el] = this.args.FILE[el];
		fs.rm(this.tmpFile, (e) => {
			if (e)
				throw e;
		});
		return false;
	}

	urlencoded()
	{
		const stream = new readline(this.tmpFile);
		let line;
		while (line = stream.next())
		{
			line = line.toString('utf-8');
			const split = line.split('&');
			for (const arg of split)
			{
				const _var = arg.split('=');
				this.args.POST[_var[0]] = _var[1];
			}
		}
	}

	multipart()
	{
		let info = {
			isContent: Boolean(false),
			isBody: Boolean(false),
			separator: Boolean(false),
			name: undefined,
			filename: undefined,
			fileBody: [],
			fileCharset: String(''),
			fileExtension: String(''),
			fileBuffer: String(''),
		};
		const boundaryRegex = /^\s*boundary=([^\s]*)\s*$/gm;
		const limit = '--' + boundaryRegex.exec(this.contentType[1])[1];
		const end = limit + '--';
		const stream = new readline(this.tmpFile);
		let line;
		while (line = stream.next())
		{
			const lineTest = line.toString('utf-8');
			if (lineTest.trim() === limit || lineTest.trim() === end)
			{
				if (info.filename !== undefined)
				{
					const _path = path.join(`/tmp/${crypto.randomUUID()}${(info.fileExtension) ? `.${info.fileExtension}` : ''}`);
					const buffer = new Buffer.from(info.fileBuffer);
					const fd = fs.openSync(_path, 'w');
					fs.writeSync(fd, buffer);
					this.args.FILE[info.name] = {
						name: info.filename,
						type: mime.lookup(path.extname(info.filename)),
						size: fs.statSync(_path).size,
						tmp_name: _path,
					}
					info.fileBuffer = '';
				}
				else if (info.name !== undefined)
				{
					this.args.POST[info.name] = info.fileBody.toString();
					info.fileBody = [];
				}
				info.isContent = true;
				info.isBody = false;
				if (lineTest.trim() === end)
					break;
			}
			else if (info.isContent === true && info.isBody === false)
			{
				const split = lineTest.trim().split(';');
				
				info.name = undefined;
				info.filename = undefined;
				info.isBody = true;
				info.fileBody = [];
				info.fileBuffer = '';
				info.fileCharset = 'utf-8';
				info.separator = true;
				for (const el of split)
				{
					const _var = el.split('=');
					if (_var[0].trim() === 'name')
						info.name = _var[1].slice(1, _var[1].length - 1);
					else if (_var[0].trim() === 'filename')
						info.filename = _var[1].slice(1, _var[1].length - 1);
				}
			}
			else if (info.isContent === true && info.isBody === true && info.separator === false)
			{
				if (info.filename !== undefined)
					info.fileBuffer += line;
				else
					info.fileBody.push(lineTest);
			}
			else if (info.isContent === true && info.isBody === true && info.separator === true)
			{
				if (info.filename !== undefined && lineTest.length > 1)
				{
					const check = /^content-type:[ \t]*(.*)[ \t]*/gm.exec(lineTest.trim().toLowerCase());
					if (check)
					{
						info.fileCharset =  mime.charset(check[1]);
						if (info.fileCharset === false)
							info.fileCharset = 'ansi';
						info.fileExtension = mime.extension(check[1]);
					}
					continue;
				}
				info.separator = false;
			}
		}
	}
}

const init = () =>
{
	const BUFFER_SIZE = 256;
	const buf = new Buffer.allocUnsafe(BUFFER_SIZE);
	let ret = 0;
	let data = '';

	while (true)
	{
		ret = 0;
		try {
			ret = fs.readSync(process.stdin.fd, buf, 0, BUFFER_SIZE);
		} catch (err) {
			if (err.code === 'EAGAIN')
				throw 'Node-cgi/init: interactive stdin input not supported.';
			else if (err.code === 'EOF')
				break;
			throw err;
		}
		if (!ret)
			break;
		data += buf.toString('utf8', 0, ret);
	}
	const init = new bodyParseClass(process.env['CONTENT_TYPE'], data);
	init.start();
}; init();

//console.error(env);

const echo = (str) => process.stdout.write(`${str}\n`);

const start = () =>
{
	process.stdout.write('Content-type: text/html; charset=UTF-8\r\n');
	process.stdout.write('\r\n');
	echo('<!DOCTYPE html>');
	echo('<html>');
	echo('	<head>');
	echo('		<meta charset="utf-8">');
	echo('		<meta name="viewport" content="width=device-width, initial-scale=1">');
	echo('		<meta http-equiv="Content-Security-Policy" content="default-src \'self\' \'unsafe-inline\' \'unsafe-eval\' wikipedia.org *.wikipedia.org wikimedia.org *.wikimedia.org">');
	echo('		<link rel="icon" type="image/png" href="../dist/img/logo.png">');
	echo('		<title>Zoupladou</title>');
	echo('	</head>');
	echo('	<body>');
	echo('		<div class="uk-flex uk-flex-around uk-flex-middle" id="main-nav">');
	echo('			<a class="uk-navbar-item uk-logo" href="#">');
	echo('				<img src="../dist/img/logo.png" style="width: 15vw; max-width: 110px;"/>');
	echo('			</a>');
	echo('			<a class="nav-light uk-button uk-button-text" href="../index.html">Home</a>');
	echo('			<a class="nav-light uk-button uk-button-text" href="../random.html">Random article</a>');
	echo('			<a class="nav-light uk-button uk-button-text" href="../php/index.php">PHP test</a>');
	echo('			<a class="nav-light uk-button uk-button-text" href="./index.js">Node.js test</a>');
	echo('		</div>');
	echo('		<div class="uk-container uk-margin-large-top">');
}

const end = () =>
{
	echo('		</div>');
	echo('	</body>');
	echo('	<link rel="stylesheet" href="../dist/css/uikit.css" />');
	echo('	<link rel="stylesheet" href="../dist/css/uikit.css" />');
	echo('	<link rel="stylesheet" href="../dist/css/dark_theme.css" />');
	echo('	<link rel="stylesheet" href="../dist/css/pacman.css" />');
	echo('	<script defer src="../dist/js/uikit.min.js"></script>');
	echo('	<script defer src="../dist/js/uikit-icons.min.js"></script>');
	echo('	<script defer src="../dist/js/modules/node_cookie.js"></script>');
	echo('</html>');
}


module.exports = { argv, env, echo, start, end };
