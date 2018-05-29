#include "chat.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTMLForm.h"
#include "C:/LP2/LP2/headers/Lazyheader.h"
#include "C:/LP2/LP2/headers/MyPartHandler.h"
#include "Poco/DateTime.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#line 9 "C:\\LP2\\LP2\\resources\\Pages\\chat.cpsp"



void chat::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
	std::string nick;

	MyPartHandler partHandler;
	HTMLForm form(request, request.stream(), partHandler);

	response.setChunkedTransferEncoding(true);
	response.setContentType("text/html");

	if (!form.empty())
	{
		NameValueCollection::ConstIterator it;
		NameValueCollection::ConstIterator end;

		it = form.begin();
		end = form.end();
		for (; it != end; ++it)
		{
			if (it->first == "user")
				nick = it->second;
		}
	}

	std::ostream& responseStream = response.send();
	responseStream << "<!DOCTYPE html>\n";
	responseStream << "<html>\n";
	responseStream << "\t<head>\n";
	responseStream << "\t\t<link rel = \"stylesheet\" type = \"text/css\" href = \"styles.css\">\n";
	responseStream << "\t\t<title>Chat</title>\n";
	responseStream << "\t\t<script type = \"text/JavaScript\" src = \"jquery.js\"></script>\n";
	responseStream << "\t\t<script type = \"text/JavaScript\" src = \"jqueryui.js\"></script>\n";
	responseStream << "\t\t<script type = \"text/JavaScript\" src = \"script.js\"></script>\n";
	responseStream << "<script>\n";
	responseStream << "\tvar nick=\"" << nick << "\";\n";
	responseStream << "</script>\n";
	responseStream << "\t\t<link rel = \"stylesheet\" type = \"text/css\" href = \"jqueryuiss.css\">\n";
	responseStream << "\t</head>\n";
	responseStream << "\t<body>\n";
	responseStream << "\t\t<audio id=\"myAudio\">\n";
	responseStream << "\t\t\t<source src=\"newmessage.mp3\" type=\"audio/mpeg\">\n";
	responseStream << "\t\t\t<source src=\"newmessage.ogg\" type=\"audio/ogg\">\n";
	responseStream << "\t\t\tYour browser does not suport the audio tag\n";
	responseStream << "</audio>\n";
	responseStream << "\t\t<div id = \"FtHeader\">\n";
	responseStream << "\t\t\t<h1 align = \"center\"><img src = \"logo.jpg\" alt = \"some_text\"></h1>\n";
	responseStream << "\t\t</div>\n";
	responseStream << "\t\t<div id = \"chatTable2\">\n";
	responseStream << "\t\t\t<table align = \"center\">\n";
	responseStream << "\t\t\t\t<tr>\n";
	responseStream << "\t\t\t\t<td>\n";
	responseStream << "\t\t\t\t<div id=\"tabs\">\n";
	responseStream << "\t\t\t\t\t<ul>\n";
	responseStream << "\t\t\t\t\t\t<li><a href=\"#chatWindow\">Public</a></li>\n";
	responseStream << "\t\t\t\t\t\t<li><a href=\"#privateChatWindow\">Private</a></li>\n";
	responseStream << "\t\t\t\t\t</ul>\n";
	responseStream << "\t\t\t\t\t<div id = \"chatWindow\"></div>\n";
	responseStream << "\t\t\t\t\t<div id = \"privateChatWindow\"></div>\n";
	responseStream << "\t\t\t\t</div>\n";
	responseStream << "\t\t\t\t</td>\n";
	responseStream << "\t\t\t\t<td>\n";
	responseStream << "\t\t\t\t\t<div id = \"OnlineBox\">\n";
	responseStream << "\t\t\t\t\t</div>\n";
	responseStream << "\t\t\t\t</td>\n";
	responseStream << "\t\t\t\t</tr>\n";
	responseStream << "\t\t\t\t<tr>\n";
	responseStream << "\t\t\t\t<td>\n";
	responseStream << "\t\t\t\t\t<textarea maxlength = \"320\" id = \"TxTBox\" type = \"text\" style = \"resize:none\" name = \"TxTBox\" onkeypress = \"if(event.keyCode == 13){event.preventDefault(); sendMsgToServer();}\"></textarea>\n";
	responseStream << "\t\t\t\t</td>\n";
	responseStream << "\t\t\t\t<td>\n";
	responseStream << "\t\t\t\t\t<Button id = \"SendButton\" onclick = \"  sendMsgToServer();\"  onkeypress = \"if (event.keyCode == 13) {event.preventDefault(); sendMsgToServer(); }\">Send!</Button><br>\n";
	responseStream << "\t\t\t\t\t<form method=\"POST\" action=\"/\">\n";
	responseStream << "\t\t\t\t\t\t<input type=\"submit\" value=\"Logout\" id=\"LogoutButton\" onclick=\"loggingOut();\">\n";
	responseStream << "\t\t\t\t\t</form>\n";
	responseStream << "\t\t\t\t</td>\n";
	responseStream << "\t\t\t\t</tr>\n";
	responseStream << "\t\t\t</table>\n";
	responseStream << "\t\t</div>\n";
	responseStream << "\t</body>\n";
	responseStream << "</html>";
}