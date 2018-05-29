#ifndef favicon_INCLUDED
#define favicon_INCLUDED


#include "Poco/Net/HTTPRequestHandler.h"


class favicon: public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
};


#endif // favicon_INCLUDED
