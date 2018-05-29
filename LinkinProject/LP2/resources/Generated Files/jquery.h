#ifndef jquery_INCLUDED
#define jquery_INCLUDED


#include "Poco/Net/HTTPRequestHandler.h"


class jquery: public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
};


#endif // jquery_INCLUDED
