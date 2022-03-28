#ifndef REQUESTS_HPP
# define REQUESTS_HPP

# include "webserv.h"

class HTTPRequest
{
	private:
		std::string	_scheme; // protocole de requete (HTTP/HTTPS)
		std::string _body;
		std::string _path; // le chemin vers la page demandee
		std::string _method;
		std::string _GET; // tous les parametres get
		std::string _POST;
		std::string _COOKIES;
		std::string _FILES;
		std::string _META;// contenant tous les en-tetes http dispo
		typedef void (HTTPRequest::*ptr)();
		ptr			getFct[4];
		void	    get( void );
		void		post( void );
		void		mdelete( void );
    public:
        int request(char buf[1024]);
};

#endif