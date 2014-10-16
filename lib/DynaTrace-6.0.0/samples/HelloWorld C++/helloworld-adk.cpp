/********************************************************************************
 * dynaTrace Diagnostics (c) dynaTrace software GmbH
 *
 * Filename: helloworld.cpp
 * Author:   support@dynatrace.com
 *
 * Sample application for the Agent Development Kit (ADK)
 *******************************************************************************/
#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#endif
#include <iostream>
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
		// Place method sensor and capture the argument
		static int dt_method_id = 0; // the 'method id' identifies a certain method
		if (!dt_method_id) {
			// the first call registers the method (by it's name, file and line number)
			// optionally, an 'API' can be defined for a method (by default "ADK" is used)
			dt_method_id = dynatrace_get_method_id(__FUNCTION__, __FILE__, __LINE__, __dynatrace_api__, 0);
		}
		// getting the 'serial number' identifying a call to this method
		int dt_serial_no = dynatrace_get_serial_no(dt_method_id, 0);

		// A PurePath node can be uniquely identified by method_id + serial_no
		// using the PurePathNode (auto node) object here (does not require to place an exit)
		::dynatrace::adk::PurePathNode dt_auto_node(dt_method_id, dt_serial_no);
		// stream arguments to Capture object (C++ way of argument capturing)
		::dynatrace::adk::Capture(dt_serial_no) << what;
		// mark method entry point (C++)
		dt_auto_node.enter();

		// Say what need to be said...
		std::cout << what << std::endl;

		// Capture the return value; instead of 'return answer;' write:
		::dynatrace::adk::dynatrace_capture_arg(dt_serial_no, (const char *)answer);
		return answer;
	}
};
const char* HelloWorld::answer = "Hi!";


/** Platform specific defines */
#ifdef _WIN32
  #define dt_proc_t void
  #define dt_thread HANDLE
  #define dt_thread_create(thread, proc, data) \
	thread = (HANDLE)_beginthread(proc, 0, (void*)data)
  #define dt_thread_exit(rc) _endthread()
  #define dt_thread_join(thread) \
	WaitForSingleObject(thread, INFINITE); \
	CloseHandle(thread)
#else
  #define dt_proc_t void*
  #define dt_thread pthread_t
  #define dt_thread_create(thread, proc, data) \
	pthread_create(&thread, NULL, proc, (void*)data)
  #define dt_thread_exit(rc) pthread_exit(rc); return 0
  #define dt_thread_join(thread) pthread_join(thread, NULL);
#endif


/** Our thread's function.
 *  It retrieves the dynaTrace tag and starts a server-side PurePath.
 */
dt_proc_t ThreadProc(void *data) {
	const char *tag = (const char*)data;	// 'read' tag...
	dynatrace_set_tag_from_string(tag);		// ...and set tag (attach it to this thread)
	dynatrace_start_server_purepath();		// start server-side PurePath

	// run 'server-side' code...
	HelloWorld::say("Hello from Server!");

	dynatrace_end_server_purepath();		// end server-side PurePath
	dt_thread_exit(0);
}


/** Function 'talking' to the server
 *	It places the client-side PurePath link and starts a thread
 *  @data	data passed on to the 'server' (simply the tag in this case)
 */
void TalkToServer(const char* data) {
	// Place method sensor and capture argument
	static int dt_method_id = 0;
	if (!dt_method_id) {
		dt_method_id = dynatrace_get_method_id(__FUNCTION__, __FILE__, __LINE__, __dynatrace_api__, 0);
	}
	int dt_serial_no = dynatrace_get_serial_no(dt_method_id, 0);
	// C-style argument capturing
	dynatrace_capture_string(dt_serial_no, data);
	// mark entry point for this PurePath node (C-style)
	dt_serial_no = dynatrace_enter(dt_method_id, dt_serial_no);

	// Place the client-side link *before* sending tag to server
	dynatrace_link_client_purepath_by_string(true, data);	// true ... insert a synchronous link

	// Create 'server' thread and wait for completion
	dt_thread thread;
	dt_thread_create(thread, ThreadProc, data);
	dt_thread_join(thread);

	// mark method exit point (C-style)
	dynatrace_exit(dt_method_id, dt_serial_no);
}


/** Program Entry Point
 *  @param  argc  Number of arguments, is being corrected by DYNATRACE_INITIALIZE if command line arguments
 *                contain optional --dt_* arguments
 *  @param  argv  May contain --dt_* parameters, which will be filtered out by DYNATRACE_INITIALIZE
 *	@return exit code
 */
int main(int argc, char **argv) {
	// Initialize dynaTrace ADK, connecting to dynaTrace Server
	// Configuration parameters are taken either from --dt_* command line parameters
	// or from the corresponding DT_* environment variables
	dynatrace_initialize(&argc, &argv);

	// Start a PurePath here
	static int dt_method_id = 0;
	int dt_serial_no;
	if (!dt_method_id) {
		dt_method_id = dynatrace_get_method_id(__FUNCTION__, __FILE__, __LINE__, __dynatrace_api__, 0);
	}
	// get serial number for this method
	// (note that the 2nd parameter is '1' in this case, which will start a PurePath here!)
	dt_serial_no = dynatrace_get_serial_no(dt_method_id, 1);
	// mark entry point for this PurePath node (C-style)
	dt_serial_no = dynatrace_enter(dt_method_id, dt_serial_no);

	// Doing a direct call
	HelloWorld::say("Hello World!");

	// Doing a 'remote' call that requires tagging (a thread is used to simulate a 'server');
	// It doesn't matter how we pass on the tag. Also it doesn't matter whether we are tagging
	// to another Agent or not - so this could also be a call via socket to another application
	char tag[256];
	dynatrace_get_tag_as_string(tag, sizeof(tag));	// get the current tag
	TalkToServer(tag);								// communicate with server, 'sending' the tag

	// End PurePath here
	dynatrace_exit(dt_method_id, dt_serial_no);

	// Uninitialize dynaTrace ADK, close agent connection, free resources
	dynatrace_uninitialize();
	return 0;
}
