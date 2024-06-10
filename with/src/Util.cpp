#include "../includes/Util.hpp"

std::vector<std::string> split(std::string str, char delim)
{
	std::vector<std::string> tmpVector;
	std::stringstream ss;
	std::string buffer;

	ss << str;
	while (getline(ss, buffer, delim))
	{
		tmpVector.push_back(buffer);
	}
	return (tmpVector);
}

bool isSpecialChar(char c)
{
	if (c == '_' || c == '|' || c == '[' || c == ']' \ 
		|| c == '{' || c == '}' || c == '\\')
	{
		return (true);
	}
	return (false);
}