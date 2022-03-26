#ifndef LOCATION_HPP
# define LOCATION_HPP
# include <iostream>
# include <vector>
# define GET 0
# define POST 1
# define DELETE 2
# define RED "\x1B[31m"
# define GREEN "\x1B[32m"
# define YELLOW "\x1B[33m"
# define ORANGE "\x1B[34m"
# define PURPLE "\x1B[35m"
# define BLUE "\x1B[36m"
# define BOLD "\x1B[1m"
# define END "\x1B[0m"

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
		static void		splitLines(std::vector<std::string> &splitted, std::string str);
		static void		printFormatError(void);
};

std::ostream	&operator<<(std::ostream &o, const Location &location);

#endif
