#ifndef newmessageogg_INCLUDED
#define newmessageogg_INCLUDED


#include "Poco/Net/HTTPRequestHandler.h"


class newmessageogg: public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
};


#endif // newmessageogg_INCLUDED
