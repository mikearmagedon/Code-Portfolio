#ifndef chat_INCLUDED
#define chat_INCLUDED


#include "Poco/Net/HTTPRequestHandler.h"


class chat: public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
};


#endif // chat_INCLUDED
