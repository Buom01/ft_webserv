#ifndef __LIB_FILE_HPP
# define __LIB_FILE_HPP
# include <vector>
# include <string>
# include <unistd.h>
# include <dirent.h>
# include <sys/stat.h>
# include "Regex.hpp"

std::vector<std::string>	listFile(const std::string &);
std::string					sanitizeRelativePath(std::string);
std::string					relativeToAbsoluteDir(const std::string &);
std::string					concatPath(const std::string &, const std::string &);
std::string					getExtension(std::string);
off_t						fdFileSize(int);
bool						hasPermissions(const std::string &);
bool						hasReadPermissions(const std::string &);
bool						hasExecPermissions(const std::string &);
bool						isDirectory(const std::string &);
bool						directoryExists(const std::string &);
bool						fileExists(const std::string &);
void						nothrow_close(int);

#endif
