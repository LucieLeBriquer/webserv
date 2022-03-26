#ifndef CONFIG_HPP
# define CONFIG_HPP
# include <fstream>
# include "Server.hpp"

typedef	std::vector<Server>	vecSrv;

class Config
{
	private:
		vecSrv	_servers;

		Config(const Config &config);
		Config(void);
		
	public:
		Config(std::string file);
		virtual ~Config();

		Config	&operator=(const Config &config);
};

std::ostream	&operator<<(std::ostream &o, const Config &config);

#endif
