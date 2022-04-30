const crypto = require('crypto');
const fs = require('fs');
const path = require('path');
const process = require('process');
const mime = require('./mimetypes');

let argv = process.argv;
let env = {};

class bodyParseClass
{
	constructor(contentType = String, body = String)
	{
		this.rawData = body;
		this.body = body.split(/(?<=\r\n)/g);
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
				let el = __var.split("=");
				el[0] = el[0].replace(/\+/g, ' ');
				el[1] = el[1].replace(/\+/g, ' ');
				env['__GET'][decodeURI(el[0])] = decodeURI(el[1]);
			}
		}
		for (const el in this.args.POST)
			env['__POST'][el] = this.args.POST[el];
		for (const el in this.args.FILE)
		{
			if (!this.args.FILE[el].name.length)
				continue;
			env['__FILES'][el] = this.args.FILE[el];
		}
		return false;
	}

	urlencoded()
	{
		for (const line of this.body)
		{
			const split = line.trim().split('&');
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
		};
		const boundaryRegex = /^\s*boundary=([^\s]*)\s*$/gm;
		const limit = '--' + boundaryRegex.exec(this.contentType[1])[1];
		const end = limit + '--';
		let cursor = {
			current: Number(0),
			start: Number(0),
			end: Number(0),
		};
		for (const line of this.body)
		{
			if (line.trim() === limit || line.trim() === end)
			{
				if (info.filename !== undefined)
				{
					cursor.end -= 2;
					const _path = path.join(`/tmp/${crypto.randomUUID()}${(info.fileExtension) ? `.${info.fileExtension}` : ''}`);
					fs.writeFileSync(_path, Buffer.from(this.rawData.slice(cursor.start, cursor.end)));
					let ext = mime.lookup(path.extname(info.filename));
					if (ext === false)
						ext = path.extname(info.filename);
					this.args.FILE[info.name] = {
						name: info.filename,
						type: ext,
						size: fs.statSync(_path).size,
						tmp_name: _path,
					};
				}
				else if (info.name !== undefined)
				{
					this.args.POST[info.name] = info.fileBody.toString();
					info.fileBody = [];
				}
				info.isContent = true;
				info.isBody = false;
				if (line.trim() === end)
					break;
			}
			else if (info.isContent === true && info.isBody === false)
			{
				const split = line.trim().split(';');
				
				info.name = undefined;
				info.filename = undefined;
				info.isBody = true;
				info.fileBody = [];
				info.fileCharset = 'utf-8';
				info.separator = true;
				cursor.start = 0;
				cursor.end = 0;
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
					cursor.end += line.length;
				else
					info.fileBody.push(line);
			}
			else if (info.isContent === true && info.isBody === true && info.separator === true)
			{
				cursor.start = cursor.current;
				if (info.filename !== undefined)
				{
					if (line !== '\r\n')
					{
						const check = /^content-type:[ \t]*(.*)[ \t]/gm.exec(line.trim().toLowerCase());
						if (check)
						{
							info.fileCharset =  mime.charset(check[1]);
							if (info.fileCharset === false)
								info.fileCharset = 'utf8';
							info.fileExtension = mime.extension(check[1]);
						}
						cursor.current += line.length;
						continue;
					}
					cursor.start += 2;
					cursor.end = cursor.start;
				}
				info.separator = false;
			}
			cursor.current += line.length;
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
		data += buf.slice(0, ret);
	}
	const init = new bodyParseClass(process.env['CONTENT_TYPE'], data);
	init.start();
};

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

init();

module.exports = { argv, env, echo, start, end };
