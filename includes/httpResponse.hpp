#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <string>
#include "httpRequest.hpp"

class HTTPResponse
{
	private:
		std::string _protocol;
		int			_statusCode;
		std::string	_header;
	public:
		// TTPResponse(): _protocol(NULL), _statusCode(0), _header(NULL) { };
		// ~HTTPResponse() {}
		class STATUS
		{
		friend class HTTPResponse;
			private:
				int _code;
			//  std::string _spe;
			public:
				int        status(int code, int type);
				typedef void (STATUS::*ptr)(int);
				ptr			getStatus[3];
				void        err4xx(int type);
				void        status2xx(int type);
				void        err5xx( int type);
		};
		std::string	getHeader( void );
		void		header( void );
		void		response(int code, int type, HTTPResponse::STATUS *sc, HTTPRequest::HTTPMethod *m);

};

#endif