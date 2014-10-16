#ifndef __SOCKETEXCEPTION_H__
#define __SOCKETEXCEPTION_H__

#include <exception>
#ifndef _WIN32
#include <errno.h>
#endif

/** An exception wrapping a socket error
 */
class SocketException : public std::exception {
private:
	int m_error;
	char m_what[2048];
public:
	SocketException(int error);
	virtual ~SocketException() throw();

	int error() const;
	virtual const char *what() const throw();
};

#ifdef _WIN32
#define LAST_SOCKET_ERROR() WSAGetLastError()
#else
#define LAST_SOCKET_ERROR() (errno)
#endif

#endif // __SOCKETEXCEPTION_H__
