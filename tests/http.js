const request = require('supertest');
const crypto = require("crypto");
const assert = require('assert');


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
			request.agent(endpoint(9007))
				.get('/')
				.expect('Content-Type', "text/plain")
				.expect(200, "Hi from domain-one.fr", done);
		});
		it('have defaults HTML error pages', function (done) {
			request.agent(endpoint(9008))
				.get('/a_file_that_does_not_exits')
				.expect('Content-Type', "text/html")
				.expect(404, /\<html/, done);
		});
		it('have customizable HTML per-error pages', function (done) {
			request.agent(endpoint(9009))
				.get('/static/a_file_that_does_not_exits')
				.expect('Content-Type', "text/html")
				.expect(404, '<html><body><h1>No ! File not found.</h1></body></html>', done);
		});
		it('have customizable HTML all-error pages', function (done) {
			request.agent(endpoint(9009))
				.get('/not-configured/location')
				.expect('Content-Type', "text/html")
				.expect(501, '<html><body><h1>This is a simple error page wich can be a template</h1></body></html>', done);
		});
	});

	describe('has a working CGI', function () {
		it('have php activate', (done) => {
			request.agent(endpoint(9200))
				.get('/info.php')
				.expect('Content-Type', 'text/html')
				.expect(200, /\<html/, done);
		});
		/*
- Chunked : https://github.com/visionmedia/superagent/blob/e196345074f57987c166283c302d06d661744f14/docs/index.md#piping-data
- Multipart : https://github.com/visionmedia/superagent/blob/e196345074f57987c166283c302d06d661744f14/docs/index.md#multipart
		*/
	});

});