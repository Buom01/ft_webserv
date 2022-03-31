#ifndef __REGEX
# define __REGEX
# define NO_FLAG 0
# define GLOBAL_FLAG 1
# include <sys/types.h>
# include <regex.h>
# include <iostream>
# include <fstream>
# include <algorithm>
# include <cstring>
# include <exception>
# include <vector>

struct match_t
{
	int			start;
	int			end;
	int			width;
	std::string	group;
	std::string	occurence;
};

class Regex
{
	private:
		char		*_line;
		char		*_pos;
		char		*_pos_save;
		char 		*_reg;
		bool		_is_end;
		int			_size_line;
		size_t		_size;
		regex_t		_regex;
		match_t		*__match;

	public:
		Regex() : _line(NULL), _pos(NULL), _pos_save(NULL), _reg(NULL), _regex(), __match(NULL) {}
		~Regex()
		{
			if (_line)
				delete [] _line;
			if (_pos_save != NULL)
				_pos = _pos_save;
			if (_pos)
				delete [] _pos;
			if (_reg)
				delete [] _reg;
			if (_regex.re_nsub)
				regfree(&_regex);
			if (__match)
				delete [] __match;
		}

	private:
		bool compile_regex (regex_t * r, const char * regex_text)
		{
			int status = regcomp (r, regex_text, REG_EXTENDED | REG_NEWLINE);
			if (status != 0)
			{
				char error_message[0x1000];
				regerror(status, r, error_message, 0x1000);
				std::cerr << "Regex creation failed, " << regex_text << ":" << error_message << std::endl;
				return true;
			}
			return false;
		}

		void init(std::string regex, std::string line, bool reset)
		{
			if (reset == false && _line && _reg
				&& std::strcmp(line.c_str(), _line) == 0
				&& std::strcmp(regex.c_str(), _reg) == 0
			)
				return;
			_is_end = false;
			if (_line)
				delete [] _line;
			_line = new char[line.size() + 1];
			std::memcpy(_line, line.c_str(), line.size() + 1);
			
			if (_pos_save != NULL)
				_pos = _pos_save;
			if (_pos)
				delete [] _pos;
			_pos = new char[line.size() + 1];
			std::memcpy(_pos, line.c_str(), line.size() + 1);
			_pos_save = _pos;

			if (_reg)
				delete [] _reg;
			_reg = new char[regex.size() + 1];
			std::memcpy(_reg, regex.c_str(), regex.size() + 1);

			_size_line = static_cast<int>(line.size());
			if (_regex.re_nsub)
				regfree(&_regex);
			compile_regex(&_regex, regex.c_str());
			if (__match)
			{
				delete [] __match;
				__match = NULL;
			}
		}

		match_t	*parse(regex_t *r, const char *to_match, int flag)
		{
			size_t 					point, n_matches = _regex.re_nsub + 1;
			bool					occ_is_null = false;
			std::vector<match_t>	matches;
			regmatch_t				pmatch[n_matches];
			match_t					temp;
			std::string				tempGroup;

			for (size_t x = 0; regexec(r, _pos, n_matches, pmatch , 0) == 0 && !_is_end; x++)
			{
				if (x > 0 && flag != GLOBAL_FLAG)
					break;
				point = std::strlen(to_match) - std::strlen(_pos);
				for (size_t pass = 0; pass < n_matches; pass++)
				{
					if (pmatch[pass].rm_so == -1)
					{
						_is_end = true;
						break;
					}
					temp.start = pmatch[pass].rm_so + point;
					temp.end = pmatch[pass].rm_eo + point;
					temp.width = temp.end - temp.start;
					occ_is_null = !!(temp.width == 0);
					_is_end = !!(temp.start == _size_line || temp.end == _size_line);
					temp.occurence = std::string(_pos + pmatch[pass].rm_so, temp.width);
					if (pass > 0)
					{
						temp.group = tempGroup;
						matches.push_back(temp);
					}
					else
						tempGroup = temp.occurence;
				}
				_pos += pmatch[0].rm_eo;
				if (occ_is_null && !_is_end)
					_pos += 1;
			}
			_size = matches.size();
			if (__match)
				delete [] __match;
			__match = new match_t[matches.size()];
			for (size_t x = 0; x < _size; x++)
			{
				__match[x].group = matches[x].group;
				__match[x].end = matches[x].end;
				__match[x].occurence = matches[x].occurence;
				__match[x].start = matches[x].start;
				__match[x].width = matches[x].width;
			}
			return __match;
		}
	public:
		/**
		 * Get the number of last occurrence(s) in the array
		 * @return (size_t) number of occurence found
		 */
		size_t size() const { return _size; }

		/**
		 *	Get the array of last occurrence(s)
		 *	@return (match_t[]) array containing the match(s)
		 *	If no match, size() is zero
		 */
		match_t *match() const { return __match; }

		/**
		 *	Print occurrences in a valid json format
		 *	/!\ Replace tab with space /!\
		 */
		void	print()
		{
			std::cout << "[" << std::endl;
			for (size_t x = 0; x < _size; x++)
			{
				std::replace(__match[x].occurence.begin(), __match[x].occurence.end(),'\t',' ');
				std::replace(__match[x].group.begin(), __match[x].group.end(),'\t',' ');
				std::cout << "  {" << std::endl;
				std::cout << "     \"Group\"     : \""	<< __match[x].group		<< "\","	<< std::endl;
				std::cout << "     \"Start\"     : "	<< __match[x].start		<< ","		<< std::endl;
				std::cout << "     \"End\"       : "	<< __match[x].end		<< ","		<< std::endl;
				std::cout << "     \"Width\"     : "	<< __match[x].width		<< ","		<< std::endl;
				std::cout << "     \"Occurence\" : \""	<< __match[x].occurence	<< "\""		<< std::endl;
				std::cout << "  }";
				if (x < _size - 1)
					std::cout << ",";
				std::cout << std::endl;
			} 
			std::cout << "]" << std::endl;
		}

		/**
		 * 	Found occurence(s) in string
		 * 	@param line (std::string) line to search occurence
		 * 	@param regex (std::string) regex rules
		 * 	@param flag (int) (optional) by default regex stop at first occurence,
		 *	pass GLOBAL_FLAG for get all occurences in string
		 *	@param reset (bool) (optional) if set to true, regex position is reset to zero
		 *  @return match_t[size()] containing the match(s)
		 *	If no flag pass & no reset and function recall, regex saved last position and return next match
		 */
		match_t *exec(std::string line, std::string regex, int flag = NO_FLAG, bool reset = false)
		{
			init(regex, line, reset);
			parse(&_regex, _line, flag);
			return __match;
		}
};

#endif
