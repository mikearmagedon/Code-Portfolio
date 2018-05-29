#ifndef __REGISTERREQUESTHANDLER_H__
#define __REGISTERREQUESTHANDLER_H__

#include "C:\LP2\LP2\headers\Lazyheader.h"
#include "C:\LP2\LP2\headers\MyPartHandler.h"
#include <fstream>

#define DifferentPasswords 0
#define UserExists 1
#define CorrectRegist 2
#define EmptyUser 3


class RegisterRequestHandler : public HTTPRequestHandler
	/// Return a HTML document.
{
public:
	RegisterRequestHandler()
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
		
		//* Form parsing i.e. "Request:/register"
		parsingUserData(new HTMLForm(request, request.stream(), *(new MyPartHandler)), request.begin(), request.end());
		
		switch (short aux = evaluateUserInsertion()){
		case CorrectRegist:
			usersDataBase.insert(std::pair<std::string, std::string>(username, password));
			writeFile(username + " " + password);
			ostr << "<form id=\"redirect\" method=\"post\" action=\"/\">";
			ostr << "</form>";
			ostr << "Registed with success! Enjoy it! Redirecting...\n";
			break;
		default:
			ostr << "<form id=\"redirect\" method=\"post\" action=\"/regist.html?\">";
			ostr << "</form>";
			if (aux == UserExists) {
				ostr << "Username already in use. Choose another one. Redirecting...\n";
			}
			if (aux == DifferentPasswords) {
				ostr << "The passwords don't match. Try again. Redirecting...\n";
			}
			if (aux == EmptyUser) {
				ostr << "Username or password not allowed. Choose another one. Redirecting...\n";
			}
		};
		ostr << "</div>";
		ostr << "</div>";
		ostr << "</body>";
		ostr << "</html>";
	}
private:

	std::string username, password, password2;	//* Vars to hold user, password and password2
	
	//* write to file: user+" "+password
	bool writeFile(std::string value){
		std::ofstream fPtr("./resources/DB/users.txt", std::ofstream::app);

		if (fPtr.is_open()){
			fPtr << value << std::endl;
			return true;
		}
		return false;
	}

	//* Parsing the data from the request
	void parsingUserData(HTMLForm* form, NameValueCollection::ConstIterator it, NameValueCollection::ConstIterator end) {
		int i = 0;
		if (!form->empty())
		{
			it = form->begin();
			end = form->end();
			for (; it != end; ++it)
			{
				if (it->first == "user") {
					username = it->second;
					while (username[i] == ' ')
						i++;
					username = username.substr(i);
					i = 0;
				}
				if (it->first == "password"){
					password = it->second;
					while (password[i] == ' ')
						i++;
					password = password.substr(i);
					i = 0;
				}
				if (it->first == "password2") {
					password2 = it->second;
					while (password2[i] == ' ')
						i++;
					password2 = password2.substr(i);
					i = 0;
				}
			}
		}
		delete form;
	}

	//* Check if regist is legal
	short evaluateUserInsertion(){
		std::map<std::string, std::string>::iterator itdb = usersDataBase.find(username);
		if (itdb == usersDataBase.end())
		{
			if (username == "" || password == "") {
				return EmptyUser;
			}
			if (password2 == password){
				return CorrectRegist;
			}
			else return DifferentPasswords;
		}
		else return UserExists;
	}
};
#endif //__REGISTERREQUESTHANDLER_H__