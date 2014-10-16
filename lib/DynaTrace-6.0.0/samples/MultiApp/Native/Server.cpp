/********************************************************************************
 * (c)2013 Compuware APM
 * File: Server.cpp
 * ADK sample application
 *******************************************************************************/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include "Server.h"
#include "SocketStream.h"
#include "MultiAppNative.h"

#include <dynatrace_adk.h>

#ifdef _WIN32
#pragma warning(disable:4996)
#pragma warning(disable:4127)
#endif

Server::Server() : info(NULL), listen(INVALID_SOCKET) {
}

long long Server::multiply(long long a, long long b) {
	// create a PurePath node capturing arguments and return value
	DYNATRACE_API("Server");
	DYNATRACE_AUTO_NODE_CAPTURE(
		DYNATRACE_CAPTURE << a << b
	);
	DYNATRACE_RETURN_INT64(a * b);
}


void Server::run(int port) throw (SocketException) {
	listening(port);
	std::cout << "server awaiting connections..." << std::endl;

	while (true) {
		SOCKET client = accept(listen, NULL, NULL);
		std::cout << "accepted client" << std::endl;

		SocketStream stream(client);

		// read the dynaTrace tag (if present) and set it
		unsigned char length = stream.getByte();
        // if length != 0, the message contains a dynaTrace tag
        if (length != 0) {
			unsigned char tag[256];
			stream.get(tag, length);
			DYNATRACE_SET_TAG(tag);
        }

		// reading "payload"
		long long a = stream.getInt64();
		long long b = stream.getInt64();

        // no dynaTrace tag was sent => we assume that the data itself were used as "custom tag"
        if (length == 0) {
			unsigned char customTag[CTAG_SIZE];
            MultiAppNative::getCustomTagFromData(customTag, a, b);
			DYNATRACE_SET_CUSTOMTAG(customTag, sizeof(customTag));
        }
		// start server-side PurePath
		DYNATRACE_START_SERVER_PUREPATH();

		long long r = multiply(a, b);
		std::cout << a << "*" << b << "=" << r << std::endl;
		stream.putInt64(r);

		// end server-side PurePath
		DYNATRACE_END_SERVER_PUREPATH();
	}
}


Server::~Server() {
	if (listen) {
#ifdef _WIN32
		closesocket(listen);
#else
		close(listen);
#endif
		listen = INVALID_SOCKET;
	}
	if (info) {
		freeaddrinfo(info);
		info = NULL;
	}
}


void Server::listening(int port) {
	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	char port_as_string[10];
	sprintf(port_as_string, "%d", port);
	int result = getaddrinfo(NULL, port_as_string, &hints, &info);
	if (result) {
		throw SocketException(LAST_SOCKET_ERROR());
	}

	listen = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (listen == INVALID_SOCKET) {
		throw SocketException(LAST_SOCKET_ERROR());
	}

	result = bind(listen, info->ai_addr, (int)info->ai_addrlen);
	if (result) {
		throw SocketException(LAST_SOCKET_ERROR());
	}

	result = ::listen(listen, SOMAXCONN);
	if (result) {
		throw SocketException(LAST_SOCKET_ERROR());
	}
}
