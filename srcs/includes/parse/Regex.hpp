#ifndef __REGEX
# define __REGEX
# include <sys/types.h>
# include <regex.h>
# include <iostream>
# include <fstream>
# include <cstring>
# include <exception>

struct match_t
{
	int			start;
	int			end;
	int			width;
	std::string	occurence;
};

class Regex
{
	private:
		const char	*_line;
		const char	*_pos;
		regex_t		_regex;
		match_t		*__match;

	public:
		Regex() : _line(NULL), _pos(NULL), _regex(), __match(NULL) {}
		Regex(const std::string &regex, const std::string &line) : _regex(), __match(NULL)
		{
			Init(regex, line);
		}
		~Regex()
		{
			if (_regex.re_nsub)
				regfree(&_regex);
			if (__match)
				delete [] __match;
		}

	private:
		bool compile_regex (regex_t * r, const char * regex_text)
		{
			int status = regcomp (r, regex_text, REG_EXTENDED|REG_NEWLINE);
			if (status != 0)
			{
				char error_message[0x1000];
				regerror(status, r, error_message, 0x1000);
				std::cerr << "Regex creation failed, " << regex_text << ":" << error_message << std::endl;
				return true;
			}
			return false;
		}

		match_t *match_regex (regex_t * r, const char * to_match)
		{
			const size_t n_matches = GetSize();
			regmatch_t m[n_matches];
			if (__match)
				delete [] __match;
			__match = new match_t[n_matches];
			if (!regexec (r, _pos, n_matches, m, 0))
			{
				size_t i = 0;
				for (i = 0; i < n_matches; ++i)
				{
					int start;
					int finish;
					if (m[i].rm_so == -1)
						break;
					start = m[i].rm_so + (_pos - to_match);
					finish = m[i].rm_eo + (_pos - to_match);
					__match[i].start = start;
					__match[i].end = finish;
					__match[i].width = (finish - start);
					__match[i].occurence = std::string(to_match, start, (finish - start));
				}
				_pos += m[0].rm_eo;
				return (__match);
			}
			return (NULL);
		}

	public:
		/**
		 * 	Return size of last generate tab of regex_t
		 */
		size_t GetSize() const { return (_regex.re_nsub + 1); }

		/**
		 *	Return match_t[] containing the match followed by groups
		 *	or NULL if there are no more match
		 *  of the last call to Exec or Match
		 */
		match_t *GetMatch() const { return __match; }

		/**
		 * 	Initialize the regex and prepare the line
		 * 	@param line : line to search occurence
		 * 	@param regex : regex rule
		 */
		void Init(const std::string &regex, const std::string &line)
		{
			_line = line.c_str();
			_pos = line.c_str();
			if (_regex.re_nsub)
				regfree(&_regex);
			compile_regex(&_regex, regex.c_str());
			if (__match)
			{
				delete [] __match;
				__match = NULL;
			}
		}

		/**
		 * 	Found next occurence in string
		 *	Return match_t[] containing the match followed by groups
		 *	or NULL if there are no more match
		 */
		match_t	*Exec()	{ return match_regex(&_regex, _line); }

		/**
		 * 	Found occurence(s) in string
		 * 	@param line : line to search occurence
		 * 	@param regex : regex rule
		 *  Return match_t[] containing the match followed by groups
		 *	or NULL if there are no match
		 */
		match_t *Match(const std::string line, const std::string regex)
		{
			Init(regex, line);
			return Exec();
		}
};

#endif
