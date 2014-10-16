#include <iostream>
#include <dynatrace_adk.h>
#include <string.h>
#include "MultiAppNative.h"
#include "Server.h"
#include "Client.h"
#include "SocketException.h"
#ifndef _WIN32
#include <arpa/inet.h>
#endif

#define HOST "localhost"
#define PORT 1234

/** Our sample application's entry point
 */
int main(int argc, char **argv) {
	// initialize the dynaTrace ADK
	DYNATRACE_INITIALIZE(&argc, &argv);

	MultiAppNative multiapp;
	multiapp.run();

	// uninitialize the dynaTrace ADK
	DYNATRACE_UNINITIALIZE();
	return 0;
}


MultiAppNative::MultiAppNative() {
#ifdef _WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
}

MultiAppNative::~MultiAppNative() {
#ifdef _WIN32
	WSACleanup();
#endif
}

void MultiAppNative::run() {
	char c = '0';
	while (c != 'q') {
		std::cout << "Available Options:" << std::endl
				  << "  1 ... start server" << std::endl
		          << "  2 ... do remote call (by dynaTrace tags)" << std::endl
		          << "  3 ... do remote call (by custom tagging)" << std::endl
		          << "  q ... quit" << std::endl;
		std::cin >> c;
	
		try {
			switch (c) {
				case '1': Server().run(PORT); break;
				case '2': Client().run(HOST, PORT); break;
				case '3': Client().run(HOST, PORT, false); break;
				case 'q': break;
			}
		} catch (SocketException &e) {
			std::cout << "Exception: " << e.what() << std::endl;
		}
	}
}

void MultiAppNative::getCustomTagFromData(unsigned char* buffer, long long a, long long b) {
	if (buffer != NULL) {
		a = (long long)htonl((unsigned long long)(a & 0xFFFFFFFF));
		b = (long long)htonl((unsigned long long)(b & 0xFFFFFFFF));
		memcpy(buffer,   (unsigned char *)&a, 4);
		memcpy(buffer+4, (unsigned char *)&b, 4);
	}
}
