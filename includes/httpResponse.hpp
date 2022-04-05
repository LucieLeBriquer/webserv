#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "httpRequest.hpp"
class STATUS;

class HTTPResponse
{
	protected:
		std::string _contentLen;
		std::string _protocol;
		std::string	_statusCode;
		std::string	_header;
	public:
		HTTPResponse() {};
		~HTTPResponse() {};
		std::string	getHeader( void );
		void		rendering( void );
		void		setContentLen( int len );
		void		statusCode(std::string status, std::string prot);
	//	void		body(int code, STATUS *sc, HTTPMethod *m);
};

class STATUS: public HTTPResponse
{
	friend class HTTPResponse;
	protected:
		std::string	_code;
		typedef void (STATUS::*ptr)(int);
		ptr			getStatus[3];
	public:
		std::string status(int code, int type);
		void        err4xx(int type);
		void        status2xx(int type);
		void        err5xx( int type);
};

#endif