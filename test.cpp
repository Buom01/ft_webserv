#include <unistd.h>
#include <iostream>
#include <cstring>

int main()
{
	char *_argv[2] = { "daisy", "/mnt/c/Users/Clement/Desktop/webserv/tests/./files/cgi/js/index.js" };
	char *_envp[3] = { "ONE=1", "SCRIPT_FILENAME=/mnt/c/Users/Clement/Desktop/webserv/tests/./files/cgi/js/index.js", "THREE=3" };
	if (execve(
		"/mnt/c/Users/Clement/Desktop/webserv/tests/files/cgi/executable/cgi-node", _argv,
		_envp)
	)
	{
		std::cerr << "FAILED" << std::endl;
	}
	return 0;
}