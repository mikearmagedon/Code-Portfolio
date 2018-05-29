#ifndef jqueryui_INCLUDED
#define jqueryui_INCLUDED


#include "Poco/Net/HTTPRequestHandler.h"


class jqueryui: public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
};


#endif // jqueryui_INCLUDED
