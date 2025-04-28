#ifndef ROUTER_H
#define ROUTER_H

#include "request.h"
#include "response.h"

response_t *route_request(request_t *req);

#endif
