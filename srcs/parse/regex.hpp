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
		size_t	_ContainerSize;
		match_t	*__match;
	public:
		Regex() : _ContainerSize(0), __match(NULL) {}
		~Regex() { delete [] __match; }
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
		void match_regex (regex_t * r, const char * to_match)
		{
			std::string	temp1 = to_match;
			const char * p = to_match;
			const int n_matches = r->re_nsub + 1;
			_ContainerSize = n_matches;
			regmatch_t m[n_matches];
			__match = new match_t[n_matches];
			while (1)
			{
				int i = 0;
				int nomatch = regexec (r, p, n_matches, m, 0);
				if (nomatch)
					break ;
				for (i = 0; i < n_matches; i++)
				{
					int start;
					int finish;
					if (m[i].rm_so == -1)
						break;
					start = m[i].rm_so + (p - to_match);
					finish = m[i].rm_eo + (p - to_match);
					std::string temp2 = temp1.substr(start, (finish - start));
					__match[i].start = start;
					__match[i].end = finish;
					__match[i].width = (finish - start);
					__match[i].occurence = temp2;
				}
				p += m[0].rm_eo;
			}
		}
	public:
		/**
		 * 	Return size of last generate tab of regex_t
		 */
		size_t GetSize() const { return _ContainerSize; }

		/**
		 * 	Return match_t[] of last generate tab of regex_t
		 */
		match_t *GetMatch() const { return __match; }

		/**
		 * 	Found occurence(s) in string
		 * 	@param line : line to search occurence
		 * 	@param regex : regex rule
		 */
		void Match(std::string line, std::string regex)
		{
			if (__match != NULL)
				delete [] __match;
			regex_t r;
			const char * regex_text = regex.c_str();
			const char * find_text = line.c_str();
			compile_regex(&r, regex_text);
			match_regex(&r, find_text);
			regfree (&r);
		}
};

#endif
