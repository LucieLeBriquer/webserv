#ifndef RESPONSE_HPP
# define RESPONSE_HPP

class HTTPResponse
{
	private:
		std::string _protocol;
		int _statusCode;
		std::string _header;
	public:
		HTTPResponse();
		~HTTPResponse();
		class STATUS
		{
			private:
				int _code;
			//  std::string _spe;
			public:
				void        status(int code, int type);
				typedef void (STATUS::*ptr)(int);
				ptr			getStatus[3];
				void        err4xx(int type);
				void        status2xx(int type);
				void        err5xx( int type);
		};
		void	header( void );
};

#endif