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
	public:
		HTTPResponse() {};
		~HTTPResponse() {};
		std::string	getHeader(void);

		void		rendering(void);
		void		rendering(const std::string typeContent);

		void		setContentLen( int len );
		std::string	checkUrl();
		void		setStatus(std::string code);
		void		statusCode(std::string status, std::string firstLine);
	//	void		body(int code, STATUS *sc, HTTPMethod *m);

		std::string	getUrl(void) const
		{
			return this->_url;
		}
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