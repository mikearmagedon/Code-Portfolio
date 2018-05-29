#ifndef jqueryuiss_INCLUDED
#define jqueryuiss_INCLUDED


#include "Poco/Net/HTTPRequestHandler.h"


class jqueryuiss: public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
};


#endif // jqueryuiss_INCLUDED
