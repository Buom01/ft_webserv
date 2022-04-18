const request = require('supertest');
const net = require('net');


const endpoint = (port) => (
	`http://127.0.0.1:${port}`
)

const send = (content, port) => (new Promise((resolve, reject) => {
	const socket = net.connect(port, '127.0.0.1', () => {
		socket.write(content);;
		resolve(socket)
	});
}));

describe('Connection', function () {

	describe('manage broken request', function () {
		it('does not crash with bad startline', function (done) {
			send("Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01-Buom_01\n\r\n\r", 9300)
				.then((socket) => {
					request(endpoint(9300))
						.get('/')
						.expect(200, done);
					setTimeout(socket.end.bind(socket), 1000);
				})
		});
		it('does not crash with unknown methods', function (done) {
			send("TOTO / HTTP/1.1\n\r\n\r", 9300)
				.then((socket) => {
					request(endpoint(9300))
						.get('/')
						.expect(200, done);
					setTimeout(socket.end.bind(socket), 1000);
				})
		});
		it('does not crash with malicious path', function (done) {
			send("GET /../../package.json HTTP/1.1\n\r\n\r", 9300)
				.then((socket) => {
					request(endpoint(9300))
						.get('/')
						.expect(200, done);
					setTimeout(socket.end.bind(socket), 1000);
				})
		});
		it('does not crash with unknown HTTP version', function (done) {
			send("GET / HTTP/77\n\r\n\r", 9300)
				.then((socket) => {
					request(endpoint(9300))
						.get('/')
						.expect(200, done);
					setTimeout(socket.end.bind(socket), 1000);
				})
		});
	});

});