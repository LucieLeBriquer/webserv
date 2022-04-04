#ifndef REQUEST_HPP
# define REQUEST_HPP

class HTTPResponse;

class HTTPRequest
{
	protected:
	//	std::string	_scheme;  protocole de requete (HTTP/HTTPS)
		std::string _body; // a voir 
		std::string _GET; // tous les parametres get
		std::string _POST;
		std::string _COOKIES;
		std::string _FILES;
		std::string _META;// contenant tous les en-tetes http dispo
		std::string _OPTION;// les methods prises en charge par le serveur
		std::string _method; // type de method
		typedef void (HTTPRequest::*ptr)();
		ptr			getFct[4];
		void		get( void );
		void		post( void );
		void		mdelete( void );
	public:
		class HTTPMethod
		{
			friend class HTTPRequest;
			protected:
				std::string	_httpv; // version 1.0 ou 1.1
				std::string _url; // le chemin vers la page demandee
			public:
				int		parsePath(const std::string url);
				int 	parseProtocol(const std::string prot);
				int		parseMethod(const std::string cmd, const std::string *methods);
				std::string		getMethod();
				std::string		getUrl();
				std::string		getProtocol( void );
		};
		class HTTPHeader
		{
			friend class HTTPRequest;
			protected:
				std::string	_host;
				std::string	_useragent;
				std::string	_accept;
			public:
				void	getContext();
		};
		int		method(char buf[30000], HTTPResponse *deliver);
		int		header(char buf[30000]);
};
#endif