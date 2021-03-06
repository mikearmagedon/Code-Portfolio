#include "regist.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTMLForm.h"


#include "Poco/DateTime.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#line 9 "C:\\LP2\\LP2\\resources\\Pages\\regist.cpsp"


// .\Pages\regist.html
static const unsigned char data[] = {
	0x3C, 0x21, 0x44, 0x4F, 0x43, 0x54, 0x59, 0x50, 0x45, 0x20, 0x68, 0x74, 0x6D, 0x6C, 0x3E, 0x0D, 0x0A, 
	0x3C, 0x68, 0x74, 0x6D, 0x6C, 0x3E, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x68, 0x65, 0x61, 0x64, 
	0x3E, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x6C, 0x69, 0x6E, 0x6B, 0x20, 
	0x72, 0x65, 0x6C, 0x3D, 0x22, 0x73, 0x74, 0x79, 0x6C, 0x65, 0x73, 0x68, 0x65, 0x65, 0x74, 0x22, 0x20, 
	0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x74, 0x65, 0x78, 0x74, 0x2F, 0x63, 0x73, 0x73, 0x22, 0x20, 0x68, 
	0x72, 0x65, 0x66, 0x3D, 0x22, 0x73, 0x74, 0x79, 0x6C, 0x65, 0x73, 0x2E, 0x63, 0x73, 0x73, 0x22, 0x3E, 
	0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x74, 0x69, 0x74, 0x6C, 0x65, 0x3E, 
	0x52, 0x65, 0x67, 0x69, 0x73, 0x74, 0x65, 0x72, 0x3C, 0x2F, 0x74, 0x69, 0x74, 0x6C, 0x65, 0x3E, 0x0D, 
	0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x73, 0x63, 0x72, 0x69, 0x70, 0x74, 0x20, 
	0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x74, 0x65, 0x78, 0x74, 0x2F, 0x4A, 0x61, 0x76, 0x61, 0x53, 0x63, 
	0x72, 0x69, 0x70, 0x74, 0x22, 0x20, 0x73, 0x72, 0x63, 0x3D, 0x22, 0x6A, 0x71, 0x75, 0x65, 0x72, 0x79, 
	0x2E, 0x6A, 0x73, 0x22, 0x3E, 0x3C, 0x2F, 0x73, 0x63, 0x72, 0x69, 0x70, 0x74, 0x3E, 0x0D, 0x0A, 0x20, 
	0x20, 0x20, 0x20, 0x3C, 0x2F, 0x68, 0x65, 0x61, 0x64, 0x3E, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 
	0x62, 0x6F, 0x64, 0x79, 0x3E, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x64, 
	0x69, 0x76, 0x20, 0x69, 0x64, 0x3D, 0x22, 0x46, 0x74, 0x48, 0x65, 0x61, 0x64, 0x65, 0x72, 0x22, 0x3E, 
	0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x68, 0x31, 
	0x20, 0x61, 0x6C, 0x69, 0x67, 0x6E, 0x3D, 0x22, 0x63, 0x65, 0x6E, 0x74, 0x65, 0x72, 0x22, 0x3E, 0x3C, 
	0x69, 0x6D, 0x67, 0x20, 0x73, 0x72, 0x63, 0x3D, 0x22, 0x6C, 0x6F, 0x67, 0x6F, 0x2E, 0x6A, 0x70, 0x67, 
	0x22, 0x20, 0x61, 0x6C, 0x74, 0x3D, 0x22, 0x73, 0x6F, 0x6D, 0x65, 0x5F, 0x74, 0x65, 0x78, 0x74, 0x22, 
	0x20, 0x2F, 0x3E, 0x3C, 0x2F, 0x68, 0x31, 0x3E, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
	0x20, 0x3C, 0x2F, 0x64, 0x69, 0x76, 0x3E, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
	0x3C, 0x64, 0x69, 0x76, 0x20, 0x69, 0x64, 0x3D, 0x22, 0x46, 0x74, 0x49, 0x6E, 0x42, 0x6F, 0x78, 0x65, 
	0x73, 0x22, 0x3E, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
	0x3C, 0x66, 0x6F, 0x72, 0x6D, 0x20, 0x6D, 0x65, 0x74, 0x68, 0x6F, 0x64, 0x3D, 0x22, 0x50, 0x4F, 0x53, 
	0x54, 0x22, 0x20, 0x61, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x3D, 0x22, 0x2F, 0x72, 0x65, 0x67, 0x69, 0x73, 
	0x74, 0x22, 0x3E, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
	0x20, 0x20, 0x20, 0x20, 0x3C, 0x66, 0x6F, 0x6E, 0x74, 0x20, 0x63, 0x6F, 0x6C, 0x6F, 0x72, 0x3D, 0x22, 
	0x23, 0x30, 0x30, 0x33, 0x33, 0x39, 0x39, 0x22, 0x20, 0x66, 0x61, 0x63, 0x65, 0x3D, 0x22, 0x6D, 0x6F, 
	0x6E, 0x6F, 0x73, 0x70, 0x61, 0x63, 0x65, 0x22, 0x3E, 0x3C, 0x70, 0x3E, 0x55, 0x73, 0x65, 0x72, 0x6E, 
	0x61, 0x6D, 0x65, 0x3A, 0x3C, 0x2F, 0x70, 0x3E, 0x3C, 0x2F, 0x66, 0x6F, 0x6E, 0x74, 0x3E, 0x3C, 0x69, 
	0x6E, 0x70, 0x75, 0x74, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x74, 0x65, 0x78, 0x74, 0x22, 0x20, 
	0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x75, 0x73, 0x65, 0x72, 0x22, 0x20, 0x6D, 0x61, 0x78, 0x6C, 0x65, 
	0x6E, 0x67, 0x74, 0x68, 0x3D, 0x22, 0x32, 0x30, 0x22, 0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x32, 
	0x30, 0x22, 0x3E, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 
	0x20, 0x20, 0x20, 0x20, 0x3C, 0x66, 0x6F, 0x6E, 0x74, 0x20, 0x63, 0x6F, 0x6C, 0x6F, 0x72, 0x3D, 0x22, 
	0x23, 0x30, 0x30, 0x33, 0x33, 0x39, 0x39, 0x22, 0x20, 0x66, 0x61, 0x63, 0x65, 0x3D, 0x22, 0x6D, 0x6F, 
	0x6E, 0x6F, 0x73, 0x70, 0x61, 0x63, 0x65, 0x22, 0x3E, 0x3C, 0x70, 0x3E, 0x50, 0x61, 0x73, 0x73, 0x77, 
	0x6F, 0x72, 0x64, 0x3A, 0x3C, 0x2F, 0x70, 0x3E, 0x3C, 0x2F, 0x66, 0x6F, 0x6E, 0x74, 0x3E, 0x3C, 0x69, 
	0x6E, 0x70, 0x75, 0x74, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6F, 
	0x72, 0x64, 0x22, 0x20, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6F, 0x72, 
	0x64, 0x22, 0x20, 0x6D, 0x61, 0x78, 0x6C, 0x65, 0x6E, 0x67, 0x74, 0x68, 0x3D, 0x22, 0x32, 0x30, 0x22, 
	0x20, 0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x22, 0x3E, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x66, 0x6F, 0x6E, 0x74, 
	0x20, 0x63, 0x6F, 0x6C, 0x6F, 0x72, 0x3D, 0x22, 0x23, 0x30, 0x30, 0x33, 0x33, 0x39, 0x39, 0x22, 0x20, 
	0x66, 0x61, 0x63, 0x65, 0x3D, 0x22, 0x6D, 0x6F, 0x6E, 0x6F, 0x73, 0x70, 0x61, 0x63, 0x65, 0x22, 0x3E, 
	0x3C, 0x70, 0x3E, 0x52, 0x65, 0x70, 0x65, 0x61, 0x74, 0x20, 0x50, 0x61, 0x73, 0x73, 0x77, 0x6F, 0x72, 
	0x64, 0x3A, 0x3C, 0x2F, 0x70, 0x3E, 0x3C, 0x2F, 0x66, 0x6F, 0x6E, 0x74, 0x3E, 0x3C, 0x69, 0x6E, 0x70, 
	0x75, 0x74, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6F, 0x72, 0x64, 
	0x22, 0x20, 0x6E, 0x61, 0x6D, 0x65, 0x3D, 0x22, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6F, 0x72, 0x64, 0x32, 
	0x22, 0x20, 0x6D, 0x61, 0x78, 0x6C, 0x65, 0x6E, 0x67, 0x74, 0x68, 0x3D, 0x22, 0x32, 0x30, 0x22, 0x20, 
	0x73, 0x69, 0x7A, 0x65, 0x3D, 0x22, 0x32, 0x30, 0x22, 0x3E, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x69, 0x6E, 0x70, 0x75, 0x74, 
	0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x73, 0x75, 0x62, 0x6D, 0x69, 0x74, 0x22, 0x20, 0x76, 0x61, 
	0x6C, 0x75, 0x65, 0x3D, 0x22, 0x52, 0x65, 0x67, 0x69, 0x73, 0x74, 0x65, 0x72, 0x22, 0x3E, 0x0D, 0x0A, 
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x2F, 0x66, 0x6F, 0x72, 
	0x6D, 0x3E, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x2F, 0x64, 0x69, 0x76, 
	0x3E, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x2F, 0x62, 0x6F, 0x64, 0x79, 0x3E, 0x0D, 0x0A, 0x3C, 
	0x2F, 0x68, 0x74, 0x6D, 0x6C, 0x3E, 0x0D, 0x0A, 
};


static bool checkModified(Poco::Net::HTTPServerRequest& request)
{
	Poco::DateTime modified(2014, 7, 4, 13, 19, 18);
	request.response().setChunkedTransferEncoding(false);
	request.response().set("Last-Modified", Poco::DateTimeFormatter::format(modified, Poco::DateTimeFormat::HTTP_FORMAT));
	if (request.has("If-Modified-Since"))
	{
		Poco::DateTime modifiedSince;
		int tzd;
		Poco::DateTimeParser::parse(request.get("If-Modified-Since"), modifiedSince, tzd);
		if (modified <= modifiedSince)
		{
			request.response().setContentLength(0);
			request.response().setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_MODIFIED);
			request.response().send();
			return false;
		}
	}
	request.response().setContentLength(static_cast<int>(sizeof(data)));
	return true;
}


void regist::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	response.setChunkedTransferEncoding(true);
	response.setContentType("text/html");

	if (!(checkModified(request))) return;

	std::ostream& responseStream = response.send();
	responseStream << "";
	responseStream << "";
	responseStream << "";
	responseStream << "";
#line 95 "C:\\LP2\\LP2\\resources\\Pages\\regist.cpsp"

	responseStream.write(reinterpret_cast<const char*>(data), sizeof(data));
	responseStream << "";
}
