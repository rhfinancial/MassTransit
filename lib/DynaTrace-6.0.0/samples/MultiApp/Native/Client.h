#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "SocketException.h"

/** Our client implementation
 */
class Client {
private:
	struct addrinfo *info;
public:
	Client() {};
	virtual ~Client() {};
	SOCKET connect(const char * const host, int port) throw(SocketException);
	void run(const char * const host, int port, bool use_dt_tag = true);
};

#endif // __CLIENT_H__
