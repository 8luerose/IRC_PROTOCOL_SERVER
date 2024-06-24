#ifndef UTIL_HPP
# define UTIL_HPP

#include "Command.hpp"
#include <sstream>

bool						isSpecialChar(char c);
std::vector<std::string>	split(std::string str, char delim);

#endif
