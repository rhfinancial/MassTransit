#ifndef __SERVER_H__
#define __SERVER_H__

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
typedef int SOCKET;
#define INVALID_SOCKET (-1)
#endif

#include "SocketException.h"

/** Our server implementation
 */
class Server {
private:
	struct addrinfo *info;
	SOCKET listen;

	long long multiply(long long a, long long b);

public:
	Server();
	virtual ~Server();
	void listening(int port);
	void run(int port) throw (SocketException);
};

#endif // __SERVER_H__
