const request = require('supertest');
const crypto = require("crypto");
const assert = require('assert');


const endpoint = (port) => (
	`http://127.0.0.1:${port}`
)

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
				.expect(200, "<html><body><h1>Hello from the INDEX.HTML</h1></body><html>", done);
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
		it('run on multiple ports', async function (done) {
			const port1 = await request(endpoint(9003)).get('/file.txt');
			assert.equal(port1.headers['Content-Type'], "text/plain");
			assert.equal(port1.status, 200);
			assert.equal(port1.body, "My plain text file\nWith a simple newline ;)");

			const port2 = await request(endpoint(9004)).get('/file.txt');
			assert.equal(port2.headers['Content-Type'], "text/plain");
			assert.equal(port2.status, 200);
			assert.equal(port2.body, "My plain text file\nWith a simple newline ;)");
			done();
		});
		it('works through 127.0.0.1 only', function (done) {
			request(endpoint(9005))
				.get('/file.txt')
				.expect('Content-Type', "text/plain")
				.expect(200, "My plain text file\nWith a simple newline ;)", done);
		});
		it('works with server_name', function (done) {
			request.agent(endpoint(9006))
				.host('mockservername.dev')
				.get('/file.txt')
				.expect('Content-Type', "text/plain")
				.expect(200, "My plain text file\nWith a simple newline ;)", done);
		});
	});

});