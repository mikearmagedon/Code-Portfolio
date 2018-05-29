#ifndef regist_INCLUDED
#define regist_INCLUDED


#include "Poco/Net/HTTPRequestHandler.h"


class regist: public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
};


#endif // regist_INCLUDED
