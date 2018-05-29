#ifndef __MYPARTHANDLER_H__
#define __MYPARTHANDLER_H__

#include "C:/LP2/LP2/headers/Lazyheader.h"

class MyPartHandler : public Poco::Net::PartHandler
{
public:
	MyPartHandler() :
		_length(0)
	{
	}

	void handlePart(const MessageHeader& header, std::istream& stream)
	{
		_type = header.get("Content-Type", "(unspecified)");
		if (header.has("Content-Disposition"))
		{
			std::string disp;
			NameValueCollection params;
			MessageHeader::splitParameters(header["Content-Disposition"], disp, params);
			_name = params.get("name", "(unnamed)");
			_fileName = params.get("filename", "(unnamed)");
		}

		CountingInputStream istr(stream);
		NullOutputStream ostr;
		StreamCopier::copyStream(istr, ostr);
		_length = istr.chars();
	}

	int length() const
	{
		return _length;
	}

	const std::string& name() const
	{
		return _name;
	}

	const std::string& fileName() const
	{
		return _fileName;
	}

	const std::string& contentType() const
	{
		return _type;
	}

private:
	int _length;
	std::string _type;
	std::string _name;
	std::string _fileName;
};
#endif //__MYPARTHANDLER_H__