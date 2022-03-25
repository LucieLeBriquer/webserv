#include "Config.hpp"

int	main(int argc, char **argv)
{
	std::string		configFile = "config/basic.conf";

	if (argc >= 2)
		configFile = argv[1];
	
	Config	config(configFile);
	
	return (0);
}
