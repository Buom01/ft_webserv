#include "split.hpp"

/**
 * 	Split string to token with delimiter(s), return s_split of element(s)
 * 	@param str: string to split
 * 	@param delimiter: string of delimiter(s)
 */
s_split split(std::string str, std::string delimiter)
{
	s_split data;
	int start = 0;
	int end = str.find(delimiter);
	while (end != -1)
    {
        data.push_back(str.substr(start, end - start));
		start = end + delimiter.size();
		end = str.find(delimiter, start);
	}
    data.push_back(str.substr(start, end - start));
    return data;
}