#include <fstream>
#include "../headers/Lazyheader.h"
#include "../headers/MyPartHandler.h"
#include "../headers/WebSocketServer.h"
#include "../headers/LoginRequestHandler.h"
#include "../headers/RegisterRequestHandler.h"

//* Header files for all the request handlers
#include "../resources/Generated Files/index.h"
#include "../resources/Generated Files/regist.h"
#include "../resources/Generated Files/chat.h"
#include "../resources/Generated Files/logo.h"
#include "../resources/Generated Files/newmessagemp3.h"
#include "../resources/Generated Files/newmessageogg.h"
#include "../resources/Generated Files/styles.h"
#include "../resources/Generated Files/favicon.h"
#include "../resources/Generated Files/script.h"
#include "../resources/Generated Files/jquery.h"
#include "../resources/Generated Files/jqueryui.h"
#include "../resources/Generated Files/jqueryuiss.h"

//* Data structure to hold all registered users
std::map<std::string, std::string> usersDataBase;

//* Read file to hash table
void readFile(std::map<std::string, std::string>& DB) {
	std::ifstream fPtr("./resources/DB/users.txt");
	std::string key, value;
	if (fPtr.is_open()) {
		while (!fPtr.eof()) {
			fPtr >> key >> value;
			DB.insert(std::pair<std::string, std::string>(key, value));
		}
	}
	fPtr.close();
}

class FormRequestHandler : public HTTPRequestHandler
	/// Return a HTML document with the the requested URI.
	/// This will be the default Request Handler for unknown
	/// requests. It simply returns a HTML document with the 
	/// request that has been made
{
public:
	FormRequestHandler()
	{
	}

	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{

		MyPartHandler partHandler;
		HTMLForm form(request, request.stream(), partHandler);

		response.setChunkedTransferEncoding(true);
		response.setContentType("text/html");

		std::ostream& ostr = response.send();
		
		ostr << "<h2>Request</h2><p>\n";
		ostr << "Method: " << request.getMethod() << "<br>\n";
		ostr << "URI: " << request.getURI() << "<br>\n";
	}
};

//* Handles responses to the client's requests
class ChatRequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
	{
		if (request.getURI() == "/")
			return new index;
		if (request.getURI() == "/styles.css")
			return new styles;
		if (request.getURI() == "/logo.jpg")
			return new logo;
		if (request.getURI() == "/favicon.ico")
			return new favicon;
		if (request.getURI() == "/newmessage.ogg")
			return new newmessageogg;
		if (request.getURI() == "/newmessage.mp3")
			return new newmessagemp3;
		if (request.getURI() == "/script.js")
			return new script;
		if (request.getURI() == "/jquery.js")
			return new jquery;
		if (request.getURI() == "/jqueryui.js")
			return new jqueryui;
		if (request.getURI() == "/jqueryuiss.css")
			return new jqueryuiss;
		if (request.getURI() == "/regist.html?" || request.getURI() == "/regist.html")	//* '/regist.html' is here for Firefox compatibility
			return new regist;
		if (request.getURI() == "/chat.html?")
			return new chat;
		if (request.getURI() == "/ws")
			return new WebSocketRequestHandler;
		if (request.getURI() == "/regist")
			return new RegisterRequestHandler;
		if (request.getURI() == "/login")
			return new LoginRequestHandler;
		else
			return new FormRequestHandler;
	}
};


class HTTPChatServerApp : public Poco::Util::ServerApplication
	/// The main application class.
	///
	/// This class handles command-line arguments and
	/// configuration files.
	/// Start the LP2 executable with the help
	/// option (/help on Windows, --help on Unix) for
	/// the available command line options.
	///
	/// To test the Chat Server you can use any web browser (http://localhost/).
{
public:
	HTTPChatServerApp() : _helpRequested(false)
	{
		Poco::Net::initializeSSL();
	}

	~HTTPChatServerApp()
	{
		Poco::Net::uninitializeSSL();
	}

protected:
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		ServerApplication::initialize(self);
	}

	void uninitialize()
	{
		ServerApplication::uninitialize();
	}

	void defineOptions(OptionSet& options)
	{
		ServerApplication::defineOptions(options);

		options.addOption(
			Option("help", "h", "display help information on command line arguments")
			.required(false)
			.repeatable(false));
	}

	void handleOption(const std::string& name, const std::string& value)
	{
		ServerApplication::handleOption(name, value);

		if (name == "help")
			_helpRequested = true;
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("A web server that serves a chat service.");
		helpFormatter.format(std::cout);
	}

	int main(const std::vector<std::string>& args)
	{
		if (_helpRequested)
		{
			displayHelp();
		}
		else
		{
			readFile(usersDataBase);
			// get parameters from configuration file
			unsigned short port = (unsigned short)config().getInt("HTTPChatServer.port", 443);

			// set-up a server socket
			SecureServerSocket svs(port);
			// set-up a HTTPServer instance
			HTTPServer srv(new ChatRequestHandlerFactory, svs, new HTTPServerParams);
			// start the HTTPServer
			srv.start();
			// wait for CTRL-C or kill
			waitForTerminationRequest();
			// Stop the HTTPServer
			srv.stop();
		}
		return Application::EXIT_OK;
	}

private:
	bool _helpRequested;
};


int main(int argc, char** argv)
{
	HTTPChatServerApp app;
	return app.run(argc, argv);
}