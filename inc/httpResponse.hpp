#ifndef RESPONSE_HPP
# define RESPONSE_HPP

class HTTPResponse
{
	private:
		std::string _header;
	public:
		class STATUS
		{
			private:
				int _code;
			//  std::string _spe;
			public:
				void        status(int code);
				typedef void (STATUS::*ptr)(int);
				ptr			getStatus[3];
				void        err4xx(int code);
				void        status2xx(int code);
				void        err5xx( int code);
		};
		void	header( void );
};

#endif