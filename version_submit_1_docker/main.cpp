#include "includes/Server.hpp"

int	filter(int *port, std::string *password, char *av1, char *av2)
{
	for (int i = 0; av1[i]; i++)
	{
		if (!isdigit(av1[i]))
			return (std::cerr << "Port number is not a number" << std::endl || 1);
	}

	*port = atoi(av1);
	if (*port < 1024 || *port > 65535)
		return (std::cerr << "Wrong port number" << std::endl || 1);

	*password = std::stringstream(av2).str();
	if (password->empty())
		return (std::cerr << "Password empty" << std::endl || 1);

	if (password->length() > 10) // Password is too long
		return (std::cerr << "Password too long" << std::endl || 1);
	
	return (0);
}

int main(int argc, char *argv[])
{
	int			port;
	std::string	password;

	if (argc != 3)
		return (std::cerr << "Usage: ./ircserv [port] [password]" << std::endl || 1);

	if (filter(&port, &password, argv[1], argv[2]))
		return (1);

	try
	{
		Server server(port, password);
		server.run();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	return (0);
}
