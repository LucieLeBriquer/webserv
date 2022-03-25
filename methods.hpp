#ifndef REQUESTS_HPP
# define REQUESTS_HPP

# include "webserv.h"

class Requests
{
	private:
		std::string	_level;
		void	    get( void );
		void		post( void );
		void		mdelete( void );
		typedef void (Requests::*ptr)();
		ptr			getFct[4];
    public:
        void treatRequest(char buf[1024]);
};

#endif