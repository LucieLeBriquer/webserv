#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <fstream>
# include "Server.hpp"

class Config
{
	private:
		std::vector<Server>	_servers;

		Config(const Config &config);
		Config(void);
		
	public:
		Config(std::string file);
		virtual ~Config();

		Config	&operator=(const Config &config);
};

std::ostream	&operator<<(std::ostream &o, const Config &config);

#endif
