#ifndef script_INCLUDED
#define script_INCLUDED


#include "Poco/Net/HTTPRequestHandler.h"


class script: public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
};


#endif // script_INCLUDED
