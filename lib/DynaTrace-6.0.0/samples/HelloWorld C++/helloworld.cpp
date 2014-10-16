/********************************************************************************
 * dynaTrace Diagnostics (c) dynaTrace software GmbH
 *
 * Filename: helloworld.cpp
 * Author:   support@dynatrace.com
 *
 * Sample application for the Agent Development Kit (ADK)
 *******************************************************************************/
#include <iostream>
#include <stdexcept>
#include "dtplatform.h"

/* Including dynaTrace ADK header */
#include <dynatrace_adk.h>


/** A simple HelloWorld class. */
class HelloWorld {
private:
	static const char* answer;
public:
	/** Say what?
	 *  @param  what  What to say
	 *  @return A more or less meaningful answer
	 */
	static const char* say(const char *what) {
		// Place method sensor and capture the argument (using C++ macros)
		DYNATRACE_AUTO_NODE_CAPTURE(
			DYNATRACE_CAPTURE << what);

		// Say what need to be said...
		std::cout << what << std::endl;

		// Capture the return value; instead of 'return answer;' write:
		DYNATRACE_RETURN_STRING(answer);
	}
};
const char* HelloWorld::answer = "Hi!";


/** Our thread's function.
 *  It retrieves the dynaTrace tag and starts a server-side PurePath.
 */
dt_proc_t ThreadProc(void *data) {
	const char *tag = (const char*)data;	// 'read' tag...
	DYNATRACE_SET_TAG_FROM_STRING(tag);		// ...and set tag (attach it to this thread)
	DYNATRACE_START_SERVER_PUREPATH();		// start server-side PurePath

	// run 'server-side' code...
	HelloWorld::say("Hello from Server!");

	DYNATRACE_END_SERVER_PUREPATH();		// end server-side PurePath
	dt_thread_exit(0);
}


/** Function 'talking' to the server
 *	It places the client-side PurePath link and starts a thread
 *  @data	data passed on to the 'server' (simply the tag in this case)
 */
void TalkToServer(const char* data) {
	// Place method sensor and capture argument, using the C macros
	DYNATRACE_ENTER_CAPTURE(
		DYNATRACE_CAPTURE_STRING(data)
	);

	// Place the client-side link *before* sending tag to server
	DYNATRACE_LINK_CLIENT_PUREPATH_BY_STRING(true, data);	// true ... insert a synchronous link

	// Create 'server' thread and wait for completion
	dt_thread thread;
	dt_thread_create(thread, ThreadProc, data);
	dt_thread_join(thread);

	// When using the C-macros, sensor exit point needs to be placed, too!
	DYNATRACE_EXIT();
}

/**
 *	Example demonstrating exception handling
 */
int fooException(bool arg1) {
	//DYNATRACE_ENTER_CAPTURE(DYNATRACE_CAPTURE_BOOL(arg1)); // C style
	DYNATRACE_AUTO_NODE_CAPTURE(DYNATRACE_CAPTURE << arg1;); // C++ style
	
	if (arg1) {
		// original code: throw std::invalid_argument("Got invalid parameter!");
		DYNATRACE_EXCEPTION_THROW(std::invalid_argument, std::invalid_argument("Got invalid parameter!"));
	}

	// original code: return 0;
	//DYNATRACE_EXIT_RETURN_INT32(0); // C style
	DYNATRACE_RETURN_INT32(0); // C++ style
}


/** Program Entry Point
 *  @param  argc  Number of arguments, is being corrected by DYNATRACE_INITIALIZE if command line arguments
 *                contain optional --dt_* arguments
 *  @param  argv  May contain --dt_* parameters, which will be filtered out by DYNATRACE_INITIALIZE
 *	@return exit code
 */
int main(int argc, char **argv) {
	// Put configuration settings before initialization step
	//DYNATRACE_SET("agentname=helloworld_set_by_program");

	// Initialize dynaTrace ADK, connecting to dynaTrace Server
	// Configuration parameters are taken either from --dt_* command line parameters
	// or from the corresponding DT_* environment variables
	DYNATRACE_INITIALIZE(&argc, &argv);

	// Start a PurePath here
	DYNATRACE_START_PUREPATH();

	// Doing a direct call
	HelloWorld::say("Hello World!");

	// Doing a 'remote' call that requires tagging (a thread is used to simulate a 'server');
	// It doesn't matter how we pass on the tag. Also it doesn't matter whether we are tagging
	// to another Agent or not - so this could also be a call via socket to another application
	char tag[256];
	DYNATRACE_GET_TAG_AS_STRING(tag, sizeof(tag));	// get the current tag
	TalkToServer(tag);								// communicate with server, 'sending' the tag

	// exception sample
	try {
		fooException(true);
	} catch (std::exception &e) {
		std::cerr << "Expected Error (generate exception node on PurePath): " << e.what() << std::endl;
	}

	// End PurePath here
	DYNATRACE_EXIT();

	// Uninitialize dynaTrace ADK, close agent connection, free resources
	DYNATRACE_UNINITIALIZE();
	return 0;
}
