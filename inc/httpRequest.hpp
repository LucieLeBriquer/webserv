#ifndef REQUEST_HPP
# define REQUEST_HPP

class HTTPRequest
{
	private:
		std::string	_scheme; // protocole de requete (HTTP/HTTPS)
		std::string	_httpv; // version 1.0 ou 1.1
		std::string _body; // a voir 
		std::string _path; // le chemin vers la page demandee
		std::string _method; // type de method
		std::string _GET; // tous les parametres get
		std::string _POST;
		std::string _COOKIES;
		std::string _FILES;
		std::string _META;// contenant tous les en-tetes http dispo
		std::string _OPTION;// les methods prises en charge par le serveur
		typedef void (HTTPRequest::*ptr)();
		ptr			getFct[4];
		void		get( void );
		void		post( void );
		void		mdelete( void );
	public:
		int		request(char buf[1024]);
		int		parsePath(const std::string url);
		int 	parseProtocol(const std::string prot);
		int		parseMethod(const std::string cmd, const std::string *methods);
};

#endif