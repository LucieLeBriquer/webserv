#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include "httpRequest.hpp"

class STATUS;

class HTTPResponse
{
	protected:
		std::string _contentLen;
		std::string _protocol;
		std::string	_statusCode;
		std::string	_url;
		std::string	_header;
		std::string	_METHOD;
	public:
		HTTPResponse(){};
		~HTTPResponse(){};
		std::string	getUrl(void );
		std::string	getmethod( void );
		std::string	getHeader( void );
		void		rendering( void );
		void		rendering( std::string typeContent );
		void		setContentLen( int len );
		std::string	checkUrl();
		void		setStatus(std::string code, std::string str);
		void		statusCode(std::string status, std::string firstLine);
};

class STATUS: public HTTPResponse
{
	friend class HTTPResponse;
	protected:
		std::string	_code;
		typedef void (STATUS::*ptr)(int);
		ptr			getStatus[3];
	public:
		STATUS();
		 ~STATUS(){};
		std::string status(int code, int type);
		void        err4xx(int type);
		void        status2xx(int type);
		void        err5xx( int type);
};

#endif