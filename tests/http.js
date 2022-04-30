const request = require('supertest');
const crypto = require("crypto");
const assert = require('assert');
const path = require('path');
const fs = require('fs');
const process = require('process');

const check = () =>
{
	let option = (process.argv[3]) ? process.argv[3] : undefined;
	if (option !== 'static'
		&& option !== 'config'
		&& option !== 'block'
		&& option !== 'php'
		&& option !== 'node'
	)
	option = undefined;
	return option;
};
const option = check();

const endpoint = (port) => (
	`http://127.0.0.1:${port}`
)

function getSimpleStatic(callback, port, root = '/', agent = request(endpoint(port)))
{
	agent
		.get(root+'file.txt')
		.expect('Content-Type', "text/plain")
		.expect(200, "My plain text file\nWith a simple newline ;)", callback);
}

describe('Server', function () {
if (option === undefined || option === 'static')
	describe('serve static files', function () {
		it('serve simple text file', function (done) {
			request(endpoint(9002))
				.get('/file.txt')
				.expect('Content-Type', "text/plain")
				.expect(200, "My plain text file\nWith a simple newline ;)", done);
		});
		it('can serve an index when no file is asked', function (done) {
			request(endpoint(9002))
				.get('/')
				.expect('Content-Type', "text/html")
				.expect(200, "<html><body><h1>Hello from the INDEX.HTML</h1></body></html>", done);
		});
		it('allow client to download binary files', function (done) {
			this.timeout(10000);
			request(endpoint(9002))
				.get('/thunderstorm.mp4')
				.expect('Content-Type', "video/mp4")
				.expect(200)
				.end((err, response) => {
					if (err) return done(err);
					assert.equal(
						crypto.createHash('md5').update(response.body).digest('hex'),
						'42ce4fa3bc42137305e29debb0a70803'
					);
					done();
				})
		});
		it("works with '\\0'", function (done) {
			request(endpoint(9002))
				.get('/nullfile-255.txt')
				.expect('Content-Type', "text/plain")
				.expect(
					200,
					'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
					done
				);
		});
	});
if (option === undefined || option === 'config')
	describe('is configurable', function () {
		it('run on multiple ports', function (done) {
			getSimpleStatic(() => {
				getSimpleStatic(done, 9004);
			}, 9003)
		});
		it('works through 127.0.0.1', function (done) {
			getSimpleStatic(done, 9005);
		});
		it('works with a simple server_name', function (done) {
			getSimpleStatic(
				done,
				9006,
				'/',
				request.agent(endpoint(9006)).host('mockservername.dev')
			);
		});
		it('route to the first domain with multiple server_name', function (done) {
			request.agent(endpoint(9007))
				.set('Host', 'domain-one.fr')
				.get('/')
				.expect('Content-Type', "text/plain")
				.expect(200, "Hi from domain-one.fr", done);
		});
		it('route to the second domain with multiple server_name', function (done) {
			request.agent(endpoint(9007))
				.set('Host', 'domain-two.fr')
				.get('/')
				.expect('Content-Type', "text/plain")
				.expect(200, "Hi from domain-two.fr", done);
		});
		it('route to the last domain with multiple server_name', function (done) {
			request.agent(endpoint(9007))
				.set('Host', 'domain-three.fr')
				.get('/')
				.expect('Content-Type', "text/plain")
				.expect(200, "Hi from domain-three.fr", done);
		});
		it('route to a default server block with multiple server_name', function (done) {
			request(endpoint(9007))
				.get('/')
				.expect('Content-Type', "text/plain")
				.expect(200, "Hi from domain-one.fr", done);
		});
		it('have defaults HTML error pages', function (done) {
			request(endpoint(9008))
				.get('/a_file_that_does_not_exits')
				.expect('Content-Type', "text/html")
				.expect(404, /\<html/, done);
		});
		it('have customizable HTML per-error pages', function (done) {
			request(endpoint(9009))
				.get('/static/a_file_that_does_not_exits')
				.expect('Content-Type', "text/html")
				.expect(404, '<html><body><h1>No ! File not found.</h1></body></html>', done);
		});
		it('have customizable HTML all-error pages', function (done) {
			request(endpoint(9009))
				.get('/not-configured/location')
				.expect('Content-Type', "text/html")
				.expect(403, '<html><body><h1>This is a simple error page wich can be a template</h1></body></html>', done);
		});
	});
if (option === undefined || option === 'block')
	describe('is per-location block configurable', function () {
		it('can define which methods are allowed, GET only', function (done) {
			request(endpoint(9100))
				.get('/allow-get-only/')
				.expect(200, function () {
					request(endpoint(9100))
						.post('/allow-get-only/')
						.expect(405, done);
				});
		});
		it('can define which methods are allowed, POST only', function (done) {
			request(endpoint(9100))
				.get('/allow-post-only/')
				.expect(405, function () {
					request(endpoint(9100))
						.post('/allow-post-only/')
						.expect(200, done);
				});
		});
		it('can define which methods are allowed, both POST and GET', function (done) {
			request(endpoint(9100))
				.get('/allow-get-n-post/')
				.expect(200, function () {
					request(endpoint(9100))
						.post('/allow-get-n-post/')
						.expect(200, done);
				});
		});
		it('can do simple redirection', function (done) {
			request(endpoint(9101))
				.get('/')
				.redirects(1)
				.expect(200, 'You have been redirected :)', done);
		});
		it('can redirect with $request_uri', function (done) {
			request(endpoint(9101))
				.get('/test.txt')
				.redirects(1)
				.expect(200, 'You have been redirected to the right file :)', done);
		});
		it('can serve static files correctly from a custom location to a custom directory', function (done) {
			request(endpoint(9102))
				.get('/my-route-to-serve/')
				.expect(200, '<html><body><h1>Hello from the INDEX.HTML</h1></body></html>', done);
		});
		it('can have a custom index file for static serving', function (done) {
			request(endpoint(9102))
				.get('/my-route-to-serve-with-custom-indexfile/')
				.expect(200, 'My plain text file\nWith a simple newline ;)', done);
		});
		it('can list files of directory for static serving', function (done) {
			request(endpoint(9102))
				.get('/my-route-to-serve-with-listing/')
				.expect(200, /thunderstorm\.mp4/, done);
		});
		it('can disallow directory\'s file listing for static serving', function (done) {
			request(endpoint(9102))
				.get('/my-route-to-serve-without-listing/')
				.expect(403, done);
		});
		it('serve index over list files for static serving', function (done) {
			request(endpoint(9102))
				.get('/my-route-to-serve-with-both/')
				.expect(200, 'My plain text file\nWith a simple newline ;)', done);
		});
		
		it('can accept uploaded file and decide where to save', function (done) {
			const req = request(endpoint(9103))
				.put('/test.jpg')
				.send(fs.readFileSync('./files/static/Bill_Gates_2017_(cropped).jpg'))
				.expect(201, '', done);
		});
		it('serve uploaded file', function (done) {
			request(endpoint(9103))
				.get('/test.jpg')
				.expect(200)
				.end((err, response) => {
					if (err) return done(err);
					assert.equal(
						crypto.createHash('md5').update(response.body).digest('hex'),
						'39bea58f55a6c930aa0b2f8eca3d4512'
					);
					done();
				})
		});
		it('can remove an uploaded file', function (done) {
			request(endpoint(9103))
				.delete('/test.jpg')
				.expect(204, '', done);
		});
		it('cannot access removed file anymore', function (done) {
			request(endpoint(9103))
				.get('/test.jpg')
				.expect(404, /404/, done);
		});
		
		it('can accept chunked file', function (done) {
			const req = request(endpoint(9103))
				.put('/plain.txt')
				.set('Transfer-Encoding', 'chunked')
				.set('Content-Type', "plain/text")
				.send("Testfile\n\r\n")
				.send("Testfile\n\r\n")
				.send("Testfile\n\r\n")
				.send("Testfile\n\r\n")
				.expect(201, '', done);
		});
		it('serve uploaded chunked file', function (done) {
			request(endpoint(9103))
				.get('/plain.txt')
				.expect(200, "Testfile\n\r\nTestfile\n\r\nTestfile\n\r\nTestfile\n\r\n", done)
		});
		it('can remove an uploaded chunked file', function (done) {
			request(endpoint(9103))
				.delete('/plain.txt')
				.expect(204, '', done);
		});
		it('cannot access removed chunked file anymore', function (done) {
			request(endpoint(9103))
				.get('/plain.txt')
				.expect(404, /404/, done);
		});
		
		it('can send body of 99 when limit is 100 bytes', function (done) {
			const req = request(endpoint(9104))
				.put('/test1')
				.set('Content-Type', "plain/text")
				.send("qwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuio")
				.expect(201, '', done);
		});
		it('can send body of 100 when limit is 100 bytes', function (done) {
			const req = request(endpoint(9104))
				.put('/test2')
				.set('Content-Type', "plain/text")
				.send("qwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiop")
				.expect(201, done);
		});
		it('cannot send body of 101 when limit is 100 bytes', function (done) {
			const req = request(endpoint(9104))
				.put('/test3')
				.set('Content-Type', "plain/text")
				.send("qwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopqwertyuiopq")
				.expect(413, done);
		});
	});
if (option === undefined || option === 'php')
	describe('has a working PHP CGI', function () {
		it('get php info', (done) => {
			request.agent(endpoint(9200))
				.get('/info.php')
				.expect('Content-Type', "text/html; charset=UTF-8")
				.expect(200, /\<!DOCTYPE html PUBLIC "-\/\/W3C\/\/DTD XHTML 1.0 Transitional\/\/EN" "DTD\/xhtml1-transitional.dtd">/, done);
		});
		it('send GET request', (done) => {
			request.agent(endpoint(9200))
				.get('/form_get.php')
				.query({first_name: 'John'})
				.query({last_name: 'Doe'})
				.expect(200, '<meta charset="UTF-8">GET form<h3>Hello John Doe !</h3><p><a href=\'index.php\'>Go to index</a></p>', done);
		});
		it('send POST request with multipart/form-data', (done) => {
			request.agent(endpoint(9200))
				.post('/form_post.php')
				.field('first_name', 'John')
				.field('last_name', 'Doe')
				.attach('file', path.join(__dirname, './files/static/Bill_Gates_2017_(cropped).jpg'), { contentType: 'image/jpeg'})
				.expect(200, '<meta charset="UTF-8">POST form<h3>Hello John Doe !</h3>File is an image/jpeg, with an MD5 as : 39bea58f55a6c930aa0b2f8eca3d4512<p>Go to uploads/Bill_Gates_2017_(cropped).jpg for get your file</p><p><a href=\'index.php\'>Go to index</a></p>', done);
		});
	});
if (option === undefined || option === 'node')
	describe('has a working NodeJS CGI', function () {
		it('get NodeJS info', (done) => {
			request.agent(endpoint(9200))
				.get('/js/info.js')
				.expect('Content-Type', "text/html; charset=UTF-8")
				.expect(200, /<!DOCTYPE html>\n<html>\n<head>\n<meta charset="utf-8">\n<meta name="viewport" content="width=device-width, initial-scale=1">\n<meta http-equiv="Content-Security-Policy" content="default-src 'self' 'unsafe-inline' 'unsafe-eval'">\n<\/head>\n/, done);
		});
		it('send GET request', (done) => {
			request.agent(endpoint(9200))
				.get('/js/form_get.js')
				.query({first_name: 'John'})
				.query({last_name: 'Doe'})
				.expect(200, '<!DOCTYPE html>\n<html>\n<head>\n<meta charset="utf-8">\n<meta name="viewport" content="width=device-width, initial-scale=1">\n<meta http-equiv="Content-Security-Policy" content="default-src \'self\' \'unsafe-inline\' \'unsafe-eval\'">\n</head>\n<body>\n<h1>GET node</h1>\n<h3>Hello John Doe !</h3>\n<p><a href=\'index.js\'>Go to index</a></p>\n</body>\n</html>\n', done);
		});
		it('send POST request', (done) => {
			request.agent(endpoint(9200))
				.post('/js/form_post.js')
				.field('first_name', 'John')
				.field('last_name', 'Doe')
				.attach('file', path.join(__dirname, './files/static/random.txt'), { contentType: 'text/plain'})
				.expect(200, '<!DOCTYPE html>\n<html>\n<head>\n<meta charset="utf-8">\n<meta name="viewport" content="width=device-width, initial-scale=1">\n<meta http-equiv="Content-Security-Policy" content="default-src \'self\' \'unsafe-inline\' \'unsafe-eval\'">\n</head>\n<body>\n<h1>POST node</h1>\n<h3>Hello John Doe !</h3>\nFile is an text/plain, with an MD5 as : 7121039cbcfe5c74af428d4ce882f3c7\n<p>Go to ./upload/random.txt for get your file</p>\n<p><a href=\'index.js\'>Go to index</a></p>\n</body>\n</html>\n', done);
		});
	});
});