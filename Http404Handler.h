#ifndef HTTP404HANDLER_H
#define HTTP404HANDLER_H

#include "RequestHandler.h"
namespace Team15 {
namespace server {

class Http404Handler : public RequestHandler {

	virtual RequestHandler::Status Init(const std::string& uri_prefix,
				NginxConfig config);
	virtual RequestHandler::Status HandleRequest(const Request& request, 
				Response* response);

};

//REGISTER_REQUEST_HANDLER(HTTP404Handler);

}
}

#endif // HTTP404HANDLER_H