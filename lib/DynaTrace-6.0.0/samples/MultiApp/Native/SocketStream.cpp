#include "SocketStream.h"

#ifndef _WIN32
#include <unistd.h>
#include <arpa/inet.h>
#define SOCKET_ERROR (-1)
#endif

SocketStream::~SocketStream() {
#ifdef _WIN32
	closesocket(socket);
#else
	close(socket);
#endif
}

void SocketStream::put(unsigned char *bytes, int length) throw(SocketException) {
	while (length > 0) {
		int count = send(socket, (char *)bytes, length, 0);
		if (count == SOCKET_ERROR) throw SocketException(LAST_SOCKET_ERROR());
		length -= count;
		bytes += count;
	}
}

void SocketStream::putByte(unsigned char value) throw(SocketException) {
	put(&value, 1);
}

void SocketStream::putInt64(long long value) throw(SocketException) {
	value = (long long)htonl((unsigned long long)(value >> 32)) | (long long)htonl((unsigned long long)(value & 0xFFFFFFFF)) << 32;
	put((unsigned char *)&value, sizeof(long long));
}

void SocketStream::get(unsigned char *bytes, int length) throw(SocketException) {
	while (length > 0) {
		int count = recv(socket, (char *)bytes, length, 0);
		if (count == SOCKET_ERROR) throw SocketException(LAST_SOCKET_ERROR());
		length -= count;
		bytes += count;
	}
}

unsigned char SocketStream::getByte() throw(SocketException) {
	unsigned char b;
	get(&b, 1);
	return b;
}

long long SocketStream::getInt64() throw(SocketException) {
	unsigned char buffer[sizeof(long long)];
	get(buffer, sizeof(buffer));
	long long value = *((long long *)buffer);
	return (long long)ntohl((unsigned long long)(value >> 32)) | (long long)ntohl((unsigned long long)(value & 0xFFFFFFFF)) << 32;
}
