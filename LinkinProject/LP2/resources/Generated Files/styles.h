#ifndef styles_INCLUDED
#define styles_INCLUDED


#include "Poco/Net/HTTPRequestHandler.h"


class styles: public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
};


#endif // styles_INCLUDED
