#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "httpRequest.hpp"
class HTTPRequest;

class HTTPResponse
{
	private:
		std::string _protocol;
		std::string	_statusCode;
		std::string	_header;
		char		*_cheader;
	public:
		// TTPResponse(): _protocol(NULL), _statusCode(0), _header(NULL) { };
		// ~HTTPResponse() {}
		class STATUS
		{
		friend class HTTPResponse;
			private:
				std::string _code;
			//  std::string _spe;
			public:
				std::string      status(int code, int type);
				typedef void (STATUS::*ptr)(int);
				ptr			getStatus[3];
				void        err4xx(int type);
				void        status2xx(int type);
				void        err5xx( int type);
		};
		std::string	getHeader( void );
		void		header( void );
		void		response(int code, int type, HTTPResponse::STATUS *sc, HTTPRequest::HTTPMethod *m);
		void		body(int code, HTTPResponse::STATUS *sc, HTTPRequest::HTTPMethod *m);

};

#endif