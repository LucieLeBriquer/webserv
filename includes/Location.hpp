#ifndef LOCATION_HPP
# define LOCATION_HPP
# include <iostream>
# include <vector>
# define GET 0
# define POST 1
# define DELETE 2

class Location
{
	private:
		std::string			_root;
		std::string			_index;
		size_t				_maxClientBody;
		std::vector<int>	_methods;
		std::string			_redirUrl;
		bool				_autoindex;
		bool				_formatOk;

	public:
		Location(void);
		Location(std::string str);
		Location(const Location &location);
		virtual ~Location();

		Location	&operator=(const Location &location);

		bool	wellFormatted(void) const;

		static void		splitBlocks(std::vector<std::string> &splitted, std::string str,
			std::string pattern, std::string &otherInfo);
};

std::ostream	&operator<<(std::ostream &o, const Location &location);

#endif
