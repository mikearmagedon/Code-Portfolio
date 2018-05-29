#ifndef newmessagemp3_INCLUDED
#define newmessagemp3_INCLUDED


#include "Poco/Net/HTTPRequestHandler.h"


class newmessagemp3: public Poco::Net::HTTPRequestHandler
{
public:
	void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
};


#endif // newmessagemp3_INCLUDED
