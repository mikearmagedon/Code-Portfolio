#ifndef logo_INCLUDED
#define logo_INCLUDED


#include "Poco/Net/HTTPRequestHandler.h"


class logo: public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
};


#endif // logo_INCLUDED
