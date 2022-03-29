#ifndef ERROR_HPP
# define ERROR_HPP

# include "webserv.hpp"

class ERRORS
{
	private:
    public:
};

class ERR4: public ERRORS
{
	private:
    public:
        void error4xx(std::string str, int code);
};

class ERR5: public ERRORS
{
	private:
    public:
};

class ERR2: public ERRORS
{
	private:
    public:
};

#endif