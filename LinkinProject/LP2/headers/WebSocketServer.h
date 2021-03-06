#ifndef __WEBSOCKETSERVER_H__
#define __WEBSOCKETSERVER_H__

//
// WebSocketServer.cpp
//
// $Id: //poco/1.4/Net/samples/WebSocketServer/src/WebSocketServer.cpp#1 $
//
// This sample demonstrates the WebSocket class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include <iostream>
#include <vector>
#include "Lazyheader.h"

//* Header files for all the request handlers
#include "C:/LP2/LP2/resources/Generated Files/index.h"
#include "C:/LP2/LP2/resources/Generated Files/regist.h"
#include "C:/LP2/LP2/resources/Generated Files/logo.h"
#include "C:/LP2/LP2/resources/Generated Files/styles.h"
#include "C:/LP2/LP2/resources/Generated Files/favicon.h"

std::map <std::string, WebSocket*>onlineUsersDataBase;

class WebSocketRequestHandler : public HTTPRequestHandler
	//* Handle a WebSocket connection.
{
private:
	void sendOnlineUsers(int& len, int &flags)
	{
		std::string obuf = ""; 
		std::map<std::string, WebSocket*>::iterator it;
		for (it = onlineUsersDataBase.begin(); it != onlineUsersDataBase.end(); ++it) {
			if (obuf == "")
				obuf = "\\onU";
			obuf += it->first + ";";
		}
		for (it = onlineUsersDataBase.begin(); it != onlineUsersDataBase.end(); ++it) {
			len = it->second->sendFrame(obuf.c_str(), obuf.length(), flags);
		}
	}

	//* parsing all client requests types (\\usr, \\msg, \\off and \\pvt)
	void parsingClientData(char buf[], int &len, WebSocket ws, int &flags)
	{
		std::string type = static_cast<std::string>(buf).substr(0, 4);
		if (type == "\\usr") {
			//* When an online request handled, add the user to the
			//* onlineUsersDataBase and open a websocket connection
			//* and then send the online users list
			std::string content = static_cast<std::string>(buf).substr(4);
			onlineUsersDataBase.insert(std::pair<std::string, WebSocket*>(content, new WebSocket(ws)));
			sendOnlineUsers(len,flags);
		}
		if (type == "\\msg") {
			std::map<std::string, WebSocket*>::iterator it;
			for (it = onlineUsersDataBase.begin(); it != onlineUsersDataBase.end(); ++it) {
				len = it->second->sendFrame(buf, len, flags);
			}
		}
		if (type == "\\off" || type == "\x3�sr") {	//* '\x3�sr' is here for Firefox compatibility
			//* When an offline request is handled, find the username, 
			//* close the websocket and deallocate it and clear its 
			//* entry on the onlineUsersDataBase and then send the
			//* updated online users list
			std::string content;
			std::map<std::string, WebSocket*>::iterator it;

			content = static_cast<std::string>(buf).substr(4);

			it = onlineUsersDataBase.find(content);
			it->second->close();
			delete it->second;
			onlineUsersDataBase.erase(content);
				
			sendOnlineUsers(len,flags);
			flags = 0x88;
			len = 0;
		}
		if (type == "\\pvt") {
			std::vector<std::string> usersArray;
			std::vector<std::string>::iterator it;
			unsigned int now = 0, before, userExists = 0;

			std::string content = static_cast<std::string>(buf).substr(4);
			unsigned int i = content.find('_');
			std::string contentToUsers = content.substr(0, i) + ";";
			std::string contentMessage = content.substr(i+1);
			i = contentMessage.find(':');
			usersArray.push_back(contentMessage.substr(0, i));
			contentMessage = type + contentMessage;
			while (now < (contentToUsers.length())) {
				before = now;
				now = contentToUsers.find(';', before);
				for (it = usersArray.begin(); it != usersArray.end(); it++) {
					std::map<std::string, WebSocket*>::iterator it2;
					it2 = onlineUsersDataBase.find(contentToUsers.substr(before, (now - before)));
					if (it2 != onlineUsersDataBase.end()){
						if (contentToUsers.substr(before, (now - before)) == *it) {
							userExists = 1;
						}
					}
					else userExists = 1;
				}
				if (!userExists)
					usersArray.push_back(contentToUsers.substr(before, (now - before)));
				now++;
				userExists = 0;
			}
			if (usersArray.size() > 1){
				for (it = usersArray.begin(); it != usersArray.end(); it++) {
					std::map<std::string, WebSocket*>::iterator it2;
					it2 = onlineUsersDataBase.find(*it);
					if (it2 != onlineUsersDataBase.end())
						it2->second->sendFrame(contentMessage.c_str(), contentMessage.length(), flags);
				}
			}
		}
	}
	

public:
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		Application& app = Application::instance();
		try
		{
			WebSocket ws(request, response);
			ws.setReceiveTimeout(Poco::Timespan(10, 0, 0, 0, 0));

			char buffer[1024];
			int flags;
			int n;
			do
			{
				n = ws.receiveFrame(buffer, sizeof(buffer), flags);
				parsingClientData(buffer, n, ws, flags);
			} while (n > 0 || (flags & WebSocket::FRAME_OP_BITMASK) != WebSocket::FRAME_OP_CLOSE);
		}
		catch (WebSocketException& exc)
		{
			switch (exc.code())
			{
			case WebSocket::WS_ERR_HANDSHAKE_UNSUPPORTED_VERSION:
				response.set("Sec-WebSocket-Version", WebSocket::WEBSOCKET_VERSION);
			case WebSocket::WS_ERR_NO_HANDSHAKE:
			case WebSocket::WS_ERR_HANDSHAKE_NO_VERSION:
			case WebSocket::WS_ERR_HANDSHAKE_NO_KEY:
				response.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
				response.setContentLength(0);
				response.send();
				break;
			}
		}
	}
};
#endif //__WEBSOCKETSERVER_H__