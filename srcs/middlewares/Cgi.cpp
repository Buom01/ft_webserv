#include "Cgi.hpp"

cgiEnv::cgiEnv()
{
	predefinedHeaders.push_back("auth-scheme");
	predefinedHeaders.push_back("content-length");
	predefinedHeaders.push_back("content-type");
	predefinedHeaders.push_back("remote-host");
	predefinedHeaders.push_back("transfer-encoding");
	generateEnv = NULL;
}

cgiEnv::~cgiEnv() { clean(); }

void			cgiEnv::clean()
{
	if (generateEnv == NULL)
		return;
	for (int x = 0; generateEnv[x]; x++)
		delete [] generateEnv[x];
	delete [] generateEnv;
	generateEnv = NULL;
}

cgiEnv::s_environment	cgiEnv::getVariable(std::string key)
{
	for (std::vector<s_environment>::const_iterator it = ENV.begin(); it != ENV.end(); it++)
	{
		if ((*it).key == key)
			return (*it);
	}
	return s_environment();
}

void			cgiEnv::printVariable()
{
	for (std::vector<s_environment>::const_iterator it = ENV.begin(); it != ENV.end(); it++)
		std::cout << (*it).key << "=" << (*it).value << std::endl;
}

bool			cgiEnv::addVariable(std::string key, std::string value)
{
	s_environment temp;
	temp.key = key;
	temp.value = value;
	deleteVariable(key);
	ENV.push_back(temp);
	return true;
}

bool			cgiEnv::deleteVariable(std::string key)
{
	for (std::vector<s_environment>::iterator it = ENV.begin(); it != ENV.end(); it++)
		if ((*it).key == key)
		{
			ENV.erase(it);
			return true;
		}
	return false;
}

char 			**cgiEnv::envForCGI()
{
	std::string	tempString;
	int			x = 0;

	clean();
	generateEnv = new char *[ENV.size() + 1];
	x = 0;
	for (std::vector<s_environment>::iterator it = ENV.begin(); it != ENV.end(); it++)
	{
		tempString = (*it).key + "=" + (*it).value;
		generateEnv[x] = new char[tempString.size() + 1];
		generateEnv[x] = std::strcpy(generateEnv[x], tempString.c_str());
		tempString.clear();
		++x;
	}
	generateEnv[x] = NULL;
	return generateEnv;
}


CGI::CGI(Parse::s_cgi config, std::string location, std::string index):
	_config(config),
	_location(location),
	_index(index)
{}

CGI::~CGI() {};

std::string		CGI::toLowerCase(std::string _string)
{
	std::string ret = _string;
	std::transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
	return ret;
}

void			CGI::fileExtension(Request &req)
{
	size_t nposIndex(0);

	file.path_info = req.pathname;
	file.path = req.pathname;
	file.path.insert(0, ".");
	if (file.path == "./")
		file.path.append(_index);
	nposIndex = file.path.find_last_of("/");
	if (nposIndex == std::string::npos)
		return;
	file.file = file.path.substr(nposIndex + 1);
	file.extension = file.path.substr(file.path.find_last_of("."));
	file.path = concatPath(_config.root, file.path);
}

bool			CGI::isMethod(Request &req)
{
	std::string method = convertMethod(req.method);
	if ((method == "GET" && _config.allow.GET == true) ||
		(method == "HEAD" && _config.allow.HEAD == true) ||
		(method == "POST" && _config.allow.POST == true) ||
		(method == "PUT" && _config.allow.PUT == true) ||
		(method == "DELETE" && _config.allow.DELETE == true) ||
		(method == "CONNECT" && _config.allow.CONNECT == true) ||
		(method == "OPTIONS" && _config.allow.OPTIONS == true) ||
		(method == "TRACE" && _config.allow.TRACE == true) ||
		(method == "ALL" && _config.allow.ALL == true))
		return true;
	return false;
}

int 			CGI::stoi(std::string number)
{
	std::stringstream ss;
	int ret;

	ss << number;
	ss >> ret;
	return ret;
}

std::string		CGI::itos(int number)
{
	std::stringstream ss;

	ss << number;
	return ss.str();
}

std::string 	CGI::convertMethod(method_t method)
{
	if (method == 0)
		return "UNKNOWN";
	if (method == (1 << 0))
		return "GET";
	if (method == (1 << 1))
		return "HEAD";
	if (method == (1 << 2))
		return "POST";
	if (method == (1 << 3))
		return "PUT";
	if (method == (1 << 4))
		return "DELETE";
	if (method == (1 << 5))
		return "CONNECT";
	if (method == (1 << 6))
		return "OPTIONS";
	if (method == (1 << 7))
		return "TRACE";
	return "ALL";
}

std::string 	CGI::sval(std::string value, std::string _default)
{
	if (value.empty())
		return _default;
	return value;
}

std::string 	CGI::correctHeaderFormat(std::string name)
{
	if (name.empty())
		return "";
	for (std::string::iterator it = name.begin(); it != name.end(); it++)
	{
		if ((*it) == '-')
			(*it) = '_';
		else
			*it = std::toupper(*it);
	}
	name.insert(0, "HTTP_");
	return name;
}

void			CGI::setHeader(Request &req)
{
	Header::_headers mapHeaders = req.headers.map();
			
	#pragma region Not defined by http headers
		env.addVariable("GATEWAY_INTERFACE", GATEWAY_VERSION);
		env.addVariable("PATH_INFO", file.path_info);
		env.addVariable("PATH_TRANSLATED", file.path_info);
		if (!req.querystring.empty() && req.querystring.at(0) == '?')
			req.querystring.erase(0, 1);
		env.addVariable("QUERY_STRING", req.querystring);
		env.addVariable("REDIRECT_STATUS", "200");
		env.addVariable("REMOTE_ADDR", req.client_ip);
		env.addVariable("REMOTE_IDENT", req.username);
		env.addVariable("REMOTE_PASS", req.password);
		env.addVariable("REMOTE_USER", req.username);
		env.addVariable("REQUEST_METHOD", convertMethod(req.method));
		env.addVariable("REQUEST_URI", file.path_info);
		env.addVariable("SCRIPT_FILENAME", file.path);
		env.addVariable("SCRIPT_NAME", file.file);
		env.addVariable("SERVER_NAME", req.hostname);
		env.addVariable("SERVER_PORT", req.port);
		env.addVariable("SERVER_PROTOCOL", SERVER_PROTOCOL);
		env.addVariable("SERVER_SOFTWARE", SERVER_SOFTWARE);
	#pragma endregion Not defined by http headers

	#pragma region Predefined Headers
		env.addVariable("AUTH_TYPE", sval(req.headers.header("AUTH-SCHEME", true), ((!req.username.empty()) ? "Basic" : "")));
		env.addVariable("CONTENT_LENGTH",
			sval(req.headers.header("CONTENT-LENGTH", true), itos(req.body.size()))
		);
		env.addVariable("CONTENT_TYPE", req.headers.header("CONTENT-TYPE", true));
		env.addVariable("REMOTE_HOST", sval(req.headers.header("REMOTE_HOST", true), ENV_NULL));
	#pragma endregion Predefined Headers

	#pragma region HTTP_*
	for (Header::_headers::iterator it = mapHeaders.begin(); it != mapHeaders.end(); it++)
	{
		std::string headerName(toLowerCase((*it).first)), var("");

		if (std::find(predefinedHeaders.begin(), predefinedHeaders.end(), headerName) == predefinedHeaders.end())
		{
			env.addVariable(
				correctHeaderFormat((*it).first),
				req.headers.header(headerName, true)
			);
		}
	}
	#pragma endregion HTTP_*
}

bool			CGI::setGenerateHeader(Request &, Response &res)
{
	size_t npos(0);
	std::string	line;

	res.response_fd_buff.clear();
	while (get_next_line_string(res.response_fd, line, res.response_fd_buff, res.logger))
	{
		if (line.empty())
		{
			npos += 2;
			break;
		}
		else
		{
			npos += line.size() + 2;
			res.headers.set(line);
		}
	}
	res.response_fd_header_size = npos;
	return (true);
}

int			CGI::exec(Request &req, Response &res)
{
	char * const * _null = NULL;
	int 	pipeFD[2], saveFd[3], pipeOUT[2];
	int 	devNull = open("/dev/null", O_WRONLY);
	pid_t	pid;

	setHeader(req);
	saveFd[0] = dup(STDIN_FILENO);
	saveFd[1] = dup(STDOUT_FILENO);
	saveFd[2] = dup(STDERR_FILENO);
	res.code = C_OK;
	if ((pipe(pipeFD)) == -1 || (pipe(pipeOUT)) == -1)
		return EXIT_FAILURE;
	if ((pid = fork()) == -1)
		return EXIT_FAILURE;
	else if (pid == 0)
	{
		close(pipeFD[1]);
		dup2(pipeFD[0], STDIN_FILENO);
		close(pipeOUT[0]);
		dup2(pipeOUT[1], STDOUT_FILENO);
		if (req.logger.options.verbose == false)
			dup2(devNull, STDERR_FILENO);
		if (execve(_config.path.c_str(), _null, env.envForCGI()))
		{
			res.code = C_INTERNAL_SERVER_ERROR;
			std::cout << "Status: 500\r\n";
		}
	}
	else
	{
		close(pipeFD[0]);
		if (!req.body.empty())
			write(pipeFD[1], req.body.c_str(), req.body.size());
		close(pipeFD[1]);
		res.response_fd = pipeOUT[0];
		close(pipeOUT[1]);
	}
	dup2(saveFd[0], STDIN_FILENO);
	dup2(saveFd[1], STDOUT_FILENO);
	dup2(saveFd[2], STDERR_FILENO);
	close(saveFd[0]);
	close(saveFd[1]);
	close(saveFd[2]);
	close(devNull);
	if (pid == 0)
	{
		close(pipeOUT[1]);
		close(devNull);
		exit(EXIT_SUCCESS);
	}
	return EXIT_SUCCESS;
}

bool 			CGI::operator()(Request &req, Response &res)
{
	std::string empty("");

	if (res.code != C_NOT_IMPLEMENTED && res.code != C_NOT_FOUND)
		return (true);
	if (res.response_fd > 0 || res.body.length() > 0)
		return (true);
	if (req.finish())
		return (true);
	if (req.timeout())
	{
		res.code = C_REQUEST_TIMEOUT;
		return (true);
	}
	fileExtension(req);
	if (file.path == ".")
		return true;
	std::vector<std::string>::iterator it = std::find(_config.extensions.begin(), _config.extensions.end(), file.extension);
	if (it == _config.extensions.end() || !isMethod(req))
		return true;
	if (exec(req, res) == EXIT_FAILURE)
		return (true);
	setGenerateHeader(req, res);
	return true;
}
