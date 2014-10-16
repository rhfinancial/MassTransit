#include "SocketException.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <string.h>
#endif

SocketException::SocketException(int error) : m_error(error) {
	m_what[0] = 0;
}

SocketException::~SocketException() throw() {
}

int SocketException::error() const {
	return m_error;
}

const char *SocketException::what() const throw() {
	if (!m_what[0]) {
#ifdef _WIN32
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, m_error,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
				(char *)m_what, sizeof(m_what), NULL);
#else
		strncpy((char *)m_what, strerror(m_error), sizeof(m_what));	
#endif
	}
	return m_what;
}
