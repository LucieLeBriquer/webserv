//#include "usefull.hpp"
#include "engine.hpp"

std::string HTTPResponse::getHeader( void )
{
	return this->_header;
}

void HTTPResponse::setStatus(std::string code)
{
	std::stringstream ss;
	std::map<int, std::string> getStatus;
	int status;
	
	ss << code;
	ss >> status;

	getStatus[400] = "/400.html";
	getStatus[404] = "/404.html";
	getStatus[405] = "/405.html";
	getStatus[505] = "/505.html";

	this->_statusCode = code;
	if (status > 299)
		this->_url = getStatus[status];
}

std::string HTTPResponse::checkUrl()
{
	std::string filename("html");
	std::string tmpname("html");
	int fd;

	this->setStatus(this->_statusCode);
	tmpname += this->_url;
	if (this->_url == "/")
		this->_url = "/index.html";
	else if ((fd = open(tmpname.c_str(), O_RDWR)) == -1)
	{
		this->setStatus("404");
	}
	filename += this->_url;
	close(fd);
	return filename;
}

void HTTPResponse::setContentLen(int len)
{
	std::ostringstream s;
	s << len;
	const std::string tot_len(s.str());
	this->_contentLen = tot_len;
}

void HTTPResponse::statusCode(std::string status, std::string firstLine)
{
	std::vector<std::string> line = splitThis(firstLine);

	this->_statusCode = status;
	this->_protocol = line[2];
	this->_url = line[1];
}

void HTTPResponse::rendering(void)
{
	time_t rawtime;
	time(&rawtime);
	this->_header = "\n"+ this->_protocol + ' ' + this->_statusCode + "\nContent-Type: text/html; charset=UTF-8\nReferrer-Policy: no-referrer\nContent-Length: " + this->_contentLen + "\nDate: " + ctime(&rawtime); 
	std::cout << this->_header << std::endl;
}

// added for CSS
void HTTPResponse::rendering(const std::string typeContent)
{
	time_t rawtime;
	time(&rawtime);
	this->_header = "\n"+ this->_protocol + ' ' + this->_statusCode + "\nContent-Type: " + typeContent + "\nReferrer-Policy: no-referrer\nContent-Length: " + this->_contentLen + "\nDate: " + ctime(&rawtime); 
	std::cout << this->_header << std::endl;
}