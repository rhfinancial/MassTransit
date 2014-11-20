/********************************************************************************
 * (c)2013 Compuware APM
 * File: Client.cpp
 * ADK client sample application
 *******************************************************************************/
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma warning(disable:4996)
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
typedef int SOCKET;
#define INVALID_SOCKET (-1)
#endif
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <dynatrace_adk.h>

#include "SocketStream.h"
#include "Client.h"
#include "MultiAppNative.h"


void Client::run(const char * const host, int port, bool use_dt_tag) {
	// create a PurePath entry point
	DYNATRACE_API("Client");
	DYNATRACE_START_PUREPATH();

	try {
		SOCKET client = connect(host, port);
		SocketStream stream(client);

		long long a = rand();
		long long b = rand();

		// Either use dynaTrace tags or "custom tags" (passive tagging) for linking remote calls
		if (use_dt_tag) {
			// get the dynaTrace tag and transmit it to the server-side
			unsigned char tag[256];
			int length = DYNATRACE_GET_TAG(tag, sizeof(tag));
			stream.putByte((unsigned char)length);
			stream.put(tag, length);
			// insert a synchronous link node
			DYNATRACE_LINK_CLIENT_PUREPATH(true, tag);
		} else {
			// Get the "custom tag" - this is unique to this demo app. Normally the custom tag
 			// is a unique identifier that already exists in the monitored applications architecture.
			// Here we're using the data itself as "custom tag" (getCustomTagFromData here is just some kind of type cast)
			unsigned char customTag[CTAG_SIZE];
			MultiAppNative::getCustomTagFromData(customTag, a, b);
            // write a 0 byte, indicating that *no* tag is being sent (it's part of the payload)
			stream.putByte((unsigned char)0);
			// insert a synchronous link node using our specific customTag
			DYNATRACE_LINK_CLIENT_PUREPATH_BY_CUSTOMTAG(true, customTag, sizeof(customTag), NULL, 0);
		}

		// This is the application logic sending the application payload to the remote process.
		stream.putInt64(a);
		stream.putInt64(b);

		// read response from server
		long long r = stream.getInt64();

		std::cout << a << "*" << b << "=" << r << std::endl;

		// exit the usual way
		DYNATRACE_EXIT();
	}
	catch (SocketException &e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		// exit by exception, PurePath state will be set accordingly
		DYNATRACE_EXIT_EXCEPTION(e.what());
		return;
	}
}

SOCKET Client::connect(const char * const host, int port) throw (SocketException) {
	DYNATRACE_API("Socket");
	DYNATRACE_AUTO_NODE_CAPTURE(DYNATRACE_CAPTURE << host << port);

	struct addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	char port_as_string[10];
	sprintf(port_as_string, "%d", port);
	int result = getaddrinfo(host, port_as_string, &hints, &info);
	if (result) {
		DYNATRACE_EXCEPTION_THROW(SocketException, SocketException(LAST_SOCKET_ERROR()));
	}

	SOCKET client = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (client == INVALID_SOCKET) {
		throw SocketException(LAST_SOCKET_ERROR());
	}

	result = ::connect(client, info->ai_addr, (int)info->ai_addrlen);
	if (result) {
		DYNATRACE_EXCEPTION_THROW(SocketException, SocketException(LAST_SOCKET_ERROR()));
	}
	return client;
}
