const cp = require('child_process');
const {accessSync} = require('fs');
const {resolve} = require('path');


const exists = (file) => {
	try {
		accessSync(file);
		return (true);
	}
	catch (err)
	{
		return (false);
	}
};

const command = (command, cwd, timeout, killSignal) => (
	cp.execSync(
		command,
		{
			cwd,
			stdio: 'ignore',
			encoding: 'utf-8',
			timeout,
			killSignal
		}
	)
)

const exec = (file, args, cwd, timeout, killSignal) => (
	cp.execFileSync(
		resolve(__dirname, file),
		args,
		{
			cwd,
			stdio: 'ignore',
			encoding: 'utf-8',
			timeout,
			killSignal
		}
	)
)

describe('Program', function () {
	describe('has a working Makefile', function () {
		it('should compile', function () {
			this.timeout(15000);
			command("make", "..");
			if (!exists("../webserv"))
				throw "../webserv were not created";
		});
		it('should cleanup the sources', function () {
			command("make clean", "..");
		});
		it('should cleanup the program', function () {
			command("make fclean", "..");
			if (exists("../webserv"))
				throw "../webserv were not removed";
		});
		it('should (re)compile', function () {
			this.timeout(15000);
			command("make re", "..");
			if (!exists("../webserv"))
				throw "../webserv were not created";
		});
		it('should not relink', function () {
			this.timeout(15000);
			command("make", "..");
			command("make -q", "..");
		});
	});
	describe('take a config file', function () {
		it('should stop when no config is provided and no default one is found', function () {
			this.timeout(15000);
			try {
				exec("../webserv", [], "..", 10000);
			} catch (error) {
				if (error.status != 1)
					throw "server should not start";
			}
		});
		it('works with the test config', function () {
			this.timeout(6000);
			try {
				exec("../webserv", ['./http.conf'], ".", 4000, 'SIGINT');
			} catch (error) {
				if (error.code != 'ETIMEDOUT')
					throw "server have not success to start"
			}
		});
	});
});