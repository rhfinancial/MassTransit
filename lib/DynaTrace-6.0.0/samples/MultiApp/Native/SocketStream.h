#ifndef __SOCKETSTREAM_H__
#define __SOCKETSTREAM_H__

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
typedef int SOCKET;
#endif

#include "SocketException.h"

/** Helper class for reading and writing to a socket in network byte order
 */
class SocketStream {
private:
	SOCKET socket;
public:
	SocketStream(SOCKET socket) : socket(socket) {
	}
	virtual ~SocketStream();

	void put(unsigned char *bytes, int length) throw(SocketException);
	void putByte(unsigned char value) throw(SocketException);
	void putInt64(long long value) throw(SocketException);

	void get(unsigned char *bytes, int length) throw(SocketException);
	unsigned char getByte() throw(SocketException);
	long long getInt64() throw(SocketException);
};

#endif // __SOCKETSTREAM_H__
