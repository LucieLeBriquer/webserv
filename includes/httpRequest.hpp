#ifndef REQUEST_HPP
# define REQUEST_HPP
# include "httpResponse.hpp"

class HTTPHeader;

class HTTPRequest
{
	protected:
		std::string _META;// contenant tous les en-tetes http dispo
		std::string _OPTION;// les methods prises en charge par le serveur
		std::string _method; // type de method
		std::string	_httpv; // version 1.0 ou 1.1
		std::string _url; // le chemin vers la page demandee
		int			_active;
		std::string _fLine;
		typedef void (HTTPRequest::*ptr)();
		ptr			getFct[4];
	public:
		HTTPRequest();
		~HTTPRequest() {};
		std::string		getFirstLine( void );
		std::string		getMethod();
		std::string		getUrl();
		void			get( void );
		void			post( void );
		void			mdelete( void );

};

class HTTPHeader : public HTTPRequest
{
	friend class HTTPRequest;
	protected:
		std::string	_host;
		std::string	_contentLen;
		std::string	_useragent;
		std::string	_accept;
		typedef void (HTTPHeader::*ptr)(std::string);
		ptr			setFct[3];
	public:
		HTTPHeader() {};
		~HTTPHeader() {};

		int		method(std::string buf, STATUS *code, HTTPResponse *deliver);
		int		parsePath(const std::string url);
		int 	parseProtocol(const std::string prot);
		int		parseMethod(const std::string cmd, const std::string *methods);

		int		header(std::string buf);
		int		getContext();
		void	setContentLen(std::string value);
		void	setHost(std::string value);
		void	setUserA(std::string value);
		void	setAccept(std::string value);
		void	checkContext();
};

#endif