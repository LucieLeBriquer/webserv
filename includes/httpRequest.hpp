#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "httpResponse.hpp"
class HTTPMethod;
class HTTPHeader;

class HTTPRequest
{
	protected:
	//	std::string	_scheme;  protocole de requete (HTTP/HTTPS)
	//	std::string _GET; // tous les parametres get
	//	std::string _POST;
	//	std::string _COOKIES;
	//	std::string _FILES;
		std::string _META;// contenant tous les en-tetes http dispo
		std::string _OPTION;// les methods prises en charge par le serveur
		std::string _body; // a voir 
		std::string _method; // type de method
		std::string	_httpv; // version 1.0 ou 1.1
		std::string _url; // le chemin vers la page demandee
		int			_active;
		typedef void (HTTPRequest::*ptr)();
		ptr			getFct[4];
		void		get( void );
		void		post( void );
		void		mdelete( void );
	public:
		int		method(std::string buf, HTTPMethod *m, STATUS *code);
		int		header(std::string buf, HTTPHeader *h);
};

class HTTPMethod : public HTTPRequest
{
	public:
		friend class HTTPRequest;
		HTTPMethod() {};
		~HTTPMethod() {};
		int		parsePath(const std::string url);
		int 	parseProtocol(const std::string prot);
		int		parseMethod(const std::string cmd, const std::string *methods);
		std::string		getMethod();
		std::string		getUrl();
		std::string		getProtocol( void );
};

class HTTPHeader : protected HTTPRequest
{
	friend class HTTPRequest;
	protected:
		std::string	_host;
		std::string	_useragent;
		std::string	_accept;
	public:
		HTTPHeader(): _host(NULL), _useragent(NULL), _accept(NULL){ };
		~HTTPHeader() {};
		typedef void (HTTPHeader::*ptr)(std::string);
		ptr			setFct[3];
		int		getContext();
		void	setHost(std::string value);
		void	setUserA(std::string value);
		void	setAccept(std::string value);
		void	checkContext();	
};

#endif