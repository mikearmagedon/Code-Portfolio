#ifndef __LOGINREQUESTHANDLER_H__
#define __LOGINREQUESTHANDLER_H__

#include "C:\LP2\LP2\headers\Lazyheader.h"
#include "C:\LP2\LP2\headers\MyPartHandler.h"

#define WrongPassword 0
#define UserNotFound 1
#define CorrectLogin 2
#define UserAlreadyOnline 3

class LoginRequestHandler : public HTTPRequestHandler
	/// Return a HTML document.
{
public:
	LoginRequestHandler()
	{
	}

	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		response.setChunkedTransferEncoding(true);
		response.setContentType("text/html");

		std::ostream& ostr = response.send();

		ostr << "<!DOCTYPE html>";
		ostr << "<html>";
		ostr << "<head>";
		ostr << "<link rel = \"stylesheet\" type = \"text/css\" href = \"styles.css\">";
		ostr << "<title>Redirect Page</title>";
		ostr << "<script type = \"text/JavaScript\" src = \"jquery.js\"></script>";
		ostr << "</head>";
		ostr << "<body onload=\"setTimeout('document.forms.redirect.submit();',2000);\">";
		ostr << "<div id = \"FtHeader\">";
		ostr << "<H1 align = \"center\"><img src = \"logo.jpg\" alt = \"some_text\"></H1>";
		ostr << "</div>";
		ostr << "<div id = \"serverMessage\">";
		ostr << "<font face = \"Arial\" color = \"#003399\"><p><b>Server Message : </b></p></font>";
		ostr << "<div id = \"ErrorMessage\">";

			
		//* Form parsing i.e. "Request:/login"
		parsingUserData(new HTMLForm(request, request.stream(), *(new MyPartHandler)), request.begin(), request.end());
				
		switch (short aux = evaluateLogin()){
		case CorrectLogin:
			ostr << "<form id=\"redirect\" method=\"post\" action=\"/chat.html?\">";
			ostr << "<input type=\"text\" id = \"redirecting\" name=\"user\" value=\""<< username << "\"/>";
			ostr << "</form>";
			ostr << "Logging in with username: " << username << "\n";
			break;
		default:
			ostr << "<form id=\"redirect\" method=\"post\" action=\"/\">";
			ostr << "</form>";
			if (aux == WrongPassword) {
				ostr << "Wrong password, please try again. Redirecting...<br>\n";
			}
			if (aux == UserNotFound) {
				ostr << "User not found, please register. Redirecting...<br>\n";
			}
			if (aux == UserAlreadyOnline){
				ostr << "User is already online. Redirecting...<br>\n";
			}
		};
		ostr << "</div>";
		ostr << "</div>";
		ostr << "</body>";
		ostr << "</html>";		
	}

	private:
		std::string username, password;	//* Vars to hold user and password
		
		//* Parsing the data from the request
		void parsingUserData(HTMLForm* form, NameValueCollection::ConstIterator it, NameValueCollection::ConstIterator end){
			int i = 0;
			if (!form->empty())
			{
				it = form->begin();
				end = form->end();
				for (; it != end; ++it)
				{
					if (it->first == "user") {
						username = it->second;
                        // ignore whitespaces
						while (username[i] == ' ')
							i++;
						username = username.substr(i);
						i = 0;
					}
					if (it->first == "password") {
						password = it->second;
                        // ignore whitespaces
						while (password[i] == ' ')
							i++;
						password = password.substr(i);
						i = 0;
					}
				}
			}
		}

		//* Check if login is legal
		short evaluateLogin(){
			std::map<std::string, std::string>::iterator itdb = usersDataBase.find(username);
			if (itdb != usersDataBase.end())
			{
				std::map<std::string, WebSocket*>::iterator itonusers = onlineUsersDataBase.find(username);
				if (itonusers == onlineUsersDataBase.end())
				{

					if (itdb->second == password){
						return CorrectLogin;
					}
					else return WrongPassword;
				}
				else return UserAlreadyOnline;
			}
			else return UserNotFound;
		}
};
#endif //__LOGINREQUESTHANDLER_H__