#ifndef _DYNATRACE_ADK_H_
#define _DYNATRACE_ADK_H_

#include "private/dynatrace_adk_private.h"

/** \mainpage dynaTrace ADK for C/C++ Documentation
 *  Website: http://www.dynatrace.com/
 */

/** \defgroup Initialization Initialization
 *  Lifetime Management of the Library
\code
int main(int argc, char **argv) {
	DYNATRACE_SET("agentname=myApp");
	DYNATRACE_SET("server=myserver");
	int result = DYNATRACE_INITIALIZE(&argc, &argv);
	...
	DYNATRACE_UNINITIALIZE();
	return 0;
}
\endcode
 *  @{
 */

#define DYNATRACE_CCSID_UTF16BE  1201
#define DYNATRACE_CCSID_UTF16LE  1203
#define DYNATRACE_CCSID_UTF16    1205
#define DYNATRACE_CCSID_UTF8     1209

#define DYNATRACE_LOG_LEVEL_FINEST		0
#define DYNATRACE_LOG_LEVEL_FINER		1
#define DYNATRACE_LOG_LEVEL_FINE		2
#define DYNATRACE_LOG_LEVEL_CONFIG		3
#define DYNATRACE_LOG_LEVEL_INFO		4
#define DYNATRACE_LOG_LEVEL_WARNING		5
#define DYNATRACE_LOG_LEVEL_SEVERE		6
#define DYNATRACE_LOG_LEVEL_DEBUG		7

/** Sets a variable for the dynaTrace ADK resp. the dynaTrace Agent.
 *  @param key_value a key value pair of the form "key=value"
 *  @remarks Variables set like this override environment variables as well as
 *           arguments provided via \ref DYNATRACE_INITIALIZE.
 */
#define DYNATRACE_SET(key_value) \
	dynatrace_set(key_value)

/** Initializes the dynaTrace ADK. Any calls happening before this is called
 *  are being ignored, except \ref DYNATRACE_SET.
 *  @param argc count of arguments
 *  @param argv arguments
 *  @return 0 on success, non-zero otherwise
 *  @remarks dynaTrace ADK specific arguments will be removed from
 *           argv and argc. These are arguments starting with "--dt_".
 * \n\n
 *  @remarks Variables affecting this call:
 *           \li debugadk=[true|false]
 *           \li agentactive=[true|false]
 *           \li agentlibrary=\<path\>
 *
 *           All other variables which are deemed dynaTrace specific are being
 *           passed to the dynaTrace Agent.
 * \n\n
 *           Variables may be set via
 *           \li argc/argv ("--dt_" prefix)
 *           \li environment variables ("DT_" prefix and all upper-case)
 *           \li source code (see \ref DYNATRACE_SET)
 */
#define DYNATRACE_INITIALIZE(argc, argv) \
	dynatrace_initialize(argc, argv)

/** Reinitializes the dynaTrace ADK.
 *  @return 0 on success, non-zero otherwise
 *  @remarks This call is needed by the child process after fork(), as the dynaTrace
 *           Agent was left in a defunct state.
 */
#define DYNATRACE_REINITIALIZE() \
	dynatrace_reinitialize()

/** Uninitializes the dynaTrace ADK. Any calls happening after this is called
 *  are being ignored.
 *  @remarks An application that initialized the dynaTrace ADK via
 *           \ref DYNATRACE_INITIALIZE should call this to ensure
 *           release of resources.
 */
#define DYNATRACE_UNINITIALIZE() \
	dynatrace_uninitialize()

/** @} */

/** \defgroup SensorPlacement Sensor Placement
 *  Placing Sensors
\code
int baz(const char *bar) {
	DYNATRACE_API("BAZAPI");
	DYNATRACE_ENTER_CAPTURE(DYNATRACE_CAPTURE_STRING(bar));
	...
	DYNATRACE_EXIT_RETURN_INT32(0);
}

void foo(const char *bar) {
	DYNATRACE_START_PUREPATH();
	baz(bar);
	DYNATRACE_EXIT();
}
\endcode
 *  @{
 */

/** Defines the API for a sensor.
 *  @remarks This has to be placed BEFORE entering a sensor.
 */
#define DYNATRACE_API(api) \
	__DYNATRACE_API__(api)

/** Places a sensor.
 *  @see DYNATRACE_START_PUREPATH for marking this as entry point.
 */
#define DYNATRACE_ENTER() \
	__DYNATRACE_ENTER__(0)

/** Places a sensor and marks it as entry point.
 *  @see DYNATRACE_ENTER for placing a sensor and NOT marking is as entry point.
 */
#define DYNATRACE_START_PUREPATH() \
	__DYNATRACE_ENTER__(1)

/** Marks the end of a sensor.
 */
#define DYNATRACE_EXIT() \
	dynatrace_exit(__dynatrace_method_id__, __dynatrace_serial_no__)

/** Marks the end of a sensor by exception
 *  (this will change the PurePath state if the node is the top-most one of this PurePath)
 */
#define DYNATRACE_EXIT_EXCEPTION(message) \
	dynatrace_exit_exception(__dynatrace_method_id__, __dynatrace_serial_no__, message)

/** \defgroup ArgumentCapturing Argument Capturing
 *  Capturing Arguments passed to functions
 *  @remarks DYNATRACE_CAPTURE_xxx should only be used within any of \ref DYNATRACE_ENTER_CAPTURE,
 *           \ref DYNATRACE_START_PUREPATH_CAPTURE, \ref ArgumentCapturingNonVariadic
 *
\code
void foo(int i, const char *cp, double d, boolean b) {
	DYNATRACE_ENTER_CAPTURE(
		DYNATRACE_CAPTURE_INT32(i),
		DYNATRACE_CAPTURE_STRING(cp),
		DYNATRACE_CAPTURE_DOUBLE(d),
		DYNATRACE_CAPTURE_BOOL(b)
	);
	...
	DYNATRACE_EXIT();
}
\endcode
 *  @{
 */

#if __DYNATRACE_HAS_VARIADIC_MACROS__

/** Places a sensor enabling argument capturing.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_START_PUREPATH_CAPTURE for marking this as entry point.
 */
#define DYNATRACE_ENTER_CAPTURE(...) \
	__DYNATRACE_ENTER_CAPTURE__(0, __VA_ARGS__)

/** Places a sensor enabling argument capturing and marks it as entry point.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_ENTER_CAPTURE for placing a sensor and NOT marking is as entry point.
 */
#define DYNATRACE_START_PUREPATH_CAPTURE(...) \
	__DYNATRACE_ENTER_CAPTURE__(1, __VA_ARGS__)

#endif /* __DYNATRACE_HAS_VARIADIC_MACROS__ */

/** \defgroup ArgumentCapturingNonVariadic Argument Capturing for Compilers not supporting Variadic Macros
 *  Capturing Arguments passed to functions
 *  @{
 */

/** Places a sensor enabling argument capturing.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_START_PUREPATH_CAPTURE_1 for marking this as entry point.
 */
#define DYNATRACE_ENTER_CAPTURE_1(capture1) \
	__DYNATRACE_ENTER_CAPTURE_9__(0, capture1, , , , , , , , )

/** Places a sensor enabling argument capturing.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_START_PUREPATH_CAPTURE_2 for marking this as entry point.
 */
#define DYNATRACE_ENTER_CAPTURE_2(capture1, capture2) \
	__DYNATRACE_ENTER_CAPTURE_9__(0, capture1, capture2, , , , , , , )

/** Places a sensor enabling argument capturing.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_START_PUREPATH_CAPTURE_3 for marking this as entry point.
 */
#define DYNATRACE_ENTER_CAPTURE_3(capture1, capture2, capture3) \
	__DYNATRACE_ENTER_CAPTURE_9__(0, capture1, capture2, capture3, , , , , , )

/** Places a sensor enabling argument capturing.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_START_PUREPATH_CAPTURE_4 for marking this as entry point.
 */
#define DYNATRACE_ENTER_CAPTURE_4(capture1, capture2, capture3, capture4) \
	__DYNATRACE_ENTER_CAPTURE_9__(0, capture1, capture2, capture3, capture4, , , , , )

/** Places a sensor enabling argument capturing.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_START_PUREPATH_CAPTURE_5 for marking this as entry point.
 */
#define DYNATRACE_ENTER_CAPTURE_5(capture1, capture2, capture3, capture4, capture5) \
	__DYNATRACE_ENTER_CAPTURE_9__(0, capture1, capture2, capture3, capture4, capture5, , , , )

/** Places a sensor enabling argument capturing.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_START_PUREPATH_CAPTURE_6 for marking this as entry point.
 */
#define DYNATRACE_ENTER_CAPTURE_6(capture1, capture2, capture3, capture4, capture5, capture6) \
	__DYNATRACE_ENTER_CAPTURE_9__(0, capture1, capture2, capture3, capture4, capture5, capture6, , , )

/** Places a sensor enabling argument capturing.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_START_PUREPATH_CAPTURE_7 for marking this as entry point.
 */
#define DYNATRACE_ENTER_CAPTURE_7(capture1, capture2, capture3, capture4, capture5, capture6, capture7) \
	__DYNATRACE_ENTER_CAPTURE_9__(0, capture1, capture2, capture3, capture4, capture5, capture6, capture7, , )

/** Places a sensor enabling argument capturing.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_START_PUREPATH_CAPTURE_8 for marking this as entry point.
 */
#define DYNATRACE_ENTER_CAPTURE_8(capture1, capture2, capture3, capture4, capture5, capture6, capture7, capture8) \
	__DYNATRACE_ENTER_CAPTURE_9__(0, capture1, capture2, capture3, capture4, capture5, capture6, capture7, capture8, )

/** Places a sensor enabling argument capturing.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_START_PUREPATH_CAPTURE_9 for marking this as entry point.
 */
#define DYNATRACE_ENTER_CAPTURE_9(capture1, capture2, capture3, capture4, capture5, capture6, capture7, capture8, capture9) \
	__DYNATRACE_ENTER_CAPTURE_9__(0, capture1, capture2, capture3, capture4, capture5, capture6, capture7, capture8, capture9)

/** Places a sensor enabling argument capturing and marks it as entry point.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_ENTER_CAPTURE_1 for placing a sensor and NOT marking is as entry point.
 */
#define DYNATRACE_START_PUREPATH_CAPTURE_1(capture1) \
	__DYNATRACE_ENTER_CAPTURE_9__(1, capture1, , , , , , , , )

/** Places a sensor enabling argument capturing and marks it as entry point.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_ENTER_CAPTURE_2 for placing a sensor and NOT marking is as entry point.
 */
#define DYNATRACE_START_PUREPATH_CAPTURE_2(capture1, capture2) \
	__DYNATRACE_ENTER_CAPTURE_9__(1, capture1, capture2, , , , , , , )

/** Places a sensor enabling argument capturing and marks it as entry point.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_ENTER_CAPTURE_3 for placing a sensor and NOT marking is as entry point.
 */
#define DYNATRACE_START_PUREPATH_CAPTURE_3(capture1, capture2, capture3) \
	__DYNATRACE_ENTER_CAPTURE_9__(1, capture1, capture2, capture3, , , , , , )

/** Places a sensor enabling argument capturing and marks it as entry point.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_ENTER_CAPTURE_4 for placing a sensor and NOT marking is as entry point.
 */
#define DYNATRACE_START_PUREPATH_CAPTURE_4(capture1, capture2, capture3, capture4) \
	__DYNATRACE_ENTER_CAPTURE_9__(1, capture1, capture2, capture3, capture4, , , , , )

/** Places a sensor enabling argument capturing and marks it as entry point.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_ENTER_CAPTURE_5 for placing a sensor and NOT marking is as entry point.
 */
#define DYNATRACE_START_PUREPATH_CAPTURE_5(capture1, capture2, capture3, capture4, capture5) \
	__DYNATRACE_ENTER_CAPTURE_9__(1, capture1, capture2, capture3, capture4, capture5, , , , )

/** Places a sensor enabling argument capturing and marks it as entry point.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_ENTER_CAPTURE_6 for placing a sensor and NOT marking is as entry point.
 */
#define DYNATRACE_START_PUREPATH_CAPTURE_6(capture1, capture2, capture3, capture4, capture5, capture6) \
	__DYNATRACE_ENTER_CAPTURE_9__(1, capture1, capture2, capture3, capture4, capture5, capture6, , , )

/** Places a sensor enabling argument capturing and marks it as entry point.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_ENTER_CAPTURE_7 for placing a sensor and NOT marking is as entry point.
 */
#define DYNATRACE_START_PUREPATH_CAPTURE_7(capture1, capture2, capture3, capture4, capture5, capture6, capture7) \
	__DYNATRACE_ENTER_CAPTURE_9__(1, capture1, capture2, capture3, capture4, capture5, capture6, capture7, , )

/** Places a sensor enabling argument capturing and marks it as entry point.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_ENTER_CAPTURE_8 for placing a sensor and NOT marking is as entry point.
 */
#define DYNATRACE_START_PUREPATH_CAPTURE_8(capture1, capture2, capture3, capture4, capture5, capture6, capture7, capture8) \
	__DYNATRACE_ENTER_CAPTURE_9__(1, capture1, capture2, capture3, capture4, capture5, capture6, capture7, capture8, )

/** Places a sensor enabling argument capturing and marks it as entry point.
 *  @see ArgumentCapturing
 *  @see DYNATRACE_ENTER_CAPTURE_9 for placing a sensor and NOT marking is as entry point.
 */
#define DYNATRACE_START_PUREPATH_CAPTURE_9(capture1, capture2, capture3, capture4, capture5, capture6, capture7, capture8, capture9) \
	__DYNATRACE_ENTER_CAPTURE_9__(1, capture1, capture2, capture3, capture4, capture5, capture6, capture7, capture8, capture9)

/** @} */

/** Captures a bool value.
 */
#define DYNATRACE_CAPTURE_BOOL(argument) \
	dynatrace_capture_bool(__dynatrace_serial_no__, !!(argument))

/** Captures an ASCII char value (or EBCDIC on zOS).
 */
#define DYNATRACE_CAPTURE_CHAR(argument) \
	dynatrace_capture_char(__dynatrace_serial_no__, (char)(argument))

/** Captures an int16 value.
 */
#define DYNATRACE_CAPTURE_INT16(argument) \
	dynatrace_capture_int16(__dynatrace_serial_no__, (dynatrace_int16_t)(argument))

/** Captures an int32 value.
 */
#define DYNATRACE_CAPTURE_INT32(argument) \
	dynatrace_capture_int32(__dynatrace_serial_no__, (dynatrace_int32_t)(argument))

/** Captures an int64 value.
 */
#define DYNATRACE_CAPTURE_INT64(argument) \
	dynatrace_capture_int64(__dynatrace_serial_no__, (dynatrace_int64_t)(argument))

/** Captures a float value.
 */
#define DYNATRACE_CAPTURE_FLOAT(argument) \
	dynatrace_capture_float(__dynatrace_serial_no__, (float)(argument))

/** Captures a double value.
 */
#define DYNATRACE_CAPTURE_DOUBLE(argument) \
	dynatrace_capture_double(__dynatrace_serial_no__, (double)(argument))

/** Captures a zero-terminated 7-bit ASCII (or EBCDIC on zOS) string value.
 *  @remarks The maximum string length captured depends on the dynaTrace Agent.
 */
#define DYNATRACE_CAPTURE_STRING(argument) \
	dynatrace_capture_string(__dynatrace_serial_no__, (const char *)(argument))

/** Captures a string value of the specified length (in bytes) and charset (CCSID).
 *  @remarks The maximum string length captured depends on the dynaTrace Agent.
 */
#define DYNATRACE_CAPTURE_STRING_WITH_CHARSET(argument, length, ccsid) \
	dynatrace_capture_string_with_charset(__dynatrace_serial_no__, (const char *)(argument), length, ccsid)

/** @} */

/** \defgroup ReturnValueCapturing Return Value Capturing
 *  Capturing Return Values of functions
 *
\code
int answer_to_life_the_universe_and_everything(void) {
	DYNATRACE_ENTER();
	...
	DYNATRACE_EXIT_RETURN_INT32(42);
}
\endcode
 *  @{
 */

/** Marks the end of a sensor, captures a bool return value and returns it.
 */
#define DYNATRACE_EXIT_RETURN_BOOL(return_value) \
	__DYNATRACE_EXIT_RETURN__(dynatrace_bool_t, BOOL, return_value)

/** Marks the end of a sensor, captures a char return value and returns it.
 */
#define DYNATRACE_EXIT_RETURN_CHAR(return_value)\
	__DYNATRACE_EXIT_RETURN__(char, CHAR, return_value)

/** Marks the end of a sensor, captures an int16 return value and returns it.
 */
#define DYNATRACE_EXIT_RETURN_INT16(return_value) \
	__DYNATRACE_EXIT_RETURN__(dynatrace_int16_t, INT16, return_value)

/** Marks the end of a sensor, captures an int32 return value and returns it.
 */
#define DYNATRACE_EXIT_RETURN_INT32(return_value) \
	__DYNATRACE_EXIT_RETURN__(dynatrace_int32_t, INT32, return_value)

/** Marks the end of a sensor, captures an int64 return value and returns it.
 */
#define DYNATRACE_EXIT_RETURN_INT64(return_value) \
	__DYNATRACE_EXIT_RETURN__(dynatrace_int64_t, INT64, return_value)

/** Marks the end of a sensor, captures a float return value and returns it.
 */
#define DYNATRACE_EXIT_RETURN_FLOAT(return_value) \
	__DYNATRACE_EXIT_RETURN__(float, FLOAT, return_value)

/** Marks the end of a sensor, captures a double return value and returns it.
 */
#define DYNATRACE_EXIT_RETURN_DOUBLE(return_value) \
	__DYNATRACE_EXIT_RETURN__(double, DOUBLE, return_value)

/** Marks the end of a sensor, captures a zero-terminated 7-bit ASCII (or EBCDIC on zOS)
 *  string return value and returns it.
 *  @remarks The maximum string length captured depends on the dynaTrace Agent.
 */
#define DYNATRACE_EXIT_RETURN_STRING(return_value) \
	__DYNATRACE_EXIT_RETURN__(const char *, STRING, return_value)

/** Marks the end of a sensor, captures a string of the specified
 *  length (in bytes) and codeset (CCSID) as return value and returns it.
 *  @remarks The maximum string length captured depends on the dynaTrace Agent.
 */
#define DYNATRACE_EXIT_RETURN_STRING_WITH_CHARSET(return_value, length, ccsid) \
	__DYNATRACE_EXIT_RETURN_2__(const char *, STRING_WITH_CHARSET, return_value, length, ccsid)

/** @} */

/** \defgroup Logging Custom Log Events
\code
void foo(int i) {
	DYNATRACE_ENTER();
	switch(i) {
	case 0:
		DYNATRACE_LOG(0, "case 0", "foo");
		...
		break;
	default:
		DYNATRACE_LOG(0, "case default", "foo");
		...
		break;
	}
	DYNATRACE_EXIT();
}
\endcode
 *  @{
 */

/** Inserts a custom log message in zero-terminated 7-bit ASCII (or EBCDIC on zOS) format.
 *  @remarks The maximum string length captured depends on the dynaTrace Agent.
 */
#define DYNATRACE_LOG(level, message, logger_name) \
	__DYNATRACE_LOG__(level, message, logger_name)

/** Inserts a custom log message in the format specified using the CCSID.
 *  @remarks The maximum string length captured depends on the dynaTrace Agent.
 */
#define DYNATRACE_LOG_WITH_CHARSET(level, message, message_length, message_ccsid, logger_name, logger_name_length, logger_name_ccsid) \
	__DYNATRACE_LOG_WITH_CHARSET__(level, message, message_length, message_ccsid, logger_name, logger_name_length, logger_name_ccsid)

/** @} */

/** @} */

#ifdef __cplusplus

/** \defgroup CppSensorPlacement C++ Sensor Placement
 *  Placing Sensors
 *  @remarks This is merely an alternative to \ref SensorPlacement.
 *           Macros from there may still be used in combination with these.
 *
\code
void Foo::bar(int i, const char *cp, double d, boolean b) {
	DYNATRACE_API("BARAPI");
	DYNATRACE_AUTO_PUREPATH_CAPTURE(DYNATRACE_CAPTURE << i << cp << d << b);
	...
}

int Foo::add(int a, int b) {
	DYNATRACE_AUTO_NODE_CAPTURE(DYNATRACE_CAPTURE << a << b);
	DYNATRACE_RETURN_INT32(a+b);
}
\endcode
 *  @{
 */

/** \defgroup CppCapturing C++ Argument Capturing
 *  These should only be used within any of \ref DYNATRACE_AUTO_NODE_CAPTURE,
 *  \ref DYNATRACE_AUTO_PUREPATH_CAPTURE
\code
void Foo::bar(int i, const char *cp, double d, boolean b) {
	DYNATRACE_AUTO_PUREPATH_CAPTURE(DYNATRACE_CAPTURE << i << cp << d << b);
	...
}

int Foo::add(int a, int b) {
	DYNATRACE_AUTO_NODE_CAPTURE(DYNATRACE_CAPTURE << a << b);
	DYNATRACE_RETURN_INT32(a+b);
}
\endcode
 *  @{
 */

/** Creates a Capture instance that you can stream arguments onto.
 *  @see ArgumentCapturing for supported types.
 *  @return an instance of Capture
 */
#define DYNATRACE_CAPTURE \
	__DYNATRACE_QUALIFIED__ Capture(__dynatrace_serial_no__)

/** @} */

/** Places a sensor.
 *  @see DYNATRACE_AUTO_PUREPATH for marking this as entry point.
 */
#define DYNATRACE_AUTO_NODE() \
	__DYNATRACE_AUTO__(0)

/** Places a sensor enabling argument capturing.
 *  @see CppCapturing
 *  @see DYNATRACE_AUTO_PUREPATH_CAPTURE for marking this as entry point.
 */
#define DYNATRACE_AUTO_NODE_CAPTURE(code) \
	__DYNATRACE_AUTO_CAPTURE__(0, code)

/** Places a sensor and marks it as entry point.
 *  @see DYNATRACE_AUTO_NODE for placing a sensor and NOT marking this as entry point.
 */
#define DYNATRACE_AUTO_PUREPATH() \
	__DYNATRACE_AUTO__(1)

/** Places a sensor enabling argument capturing and marks it as entry point.
 *  @see CppCapturing
 *  @see DYNATRACE_AUTO_NODE_CAPTURE for placing a sensor and NOT marking this as entry point.
 */
#define DYNATRACE_AUTO_PUREPATH_CAPTURE(code) \
	__DYNATRACE_AUTO_CAPTURE__(1, code)

/** \defgroup CppReturnValue C++ Return Value Capturing
\code
int Foo::add(int a, int b) {
	DYNATRACE_AUTO_NODE();
	DYNATRACE_RETURN_INT32(a+b);
}

char *Foo::concat(const char *a, const char *b) {
	DYNATRACE_AUTO_NODE();
	char *cp = malloc(strlen(a) + strlen(b) + 1);
	strcpy(cp, a);
	strcat(cp, b);
	DYNATRACE_RETURN_STRING(cp);
}
\endcode
 *  @{
 */

/** Captures a bool return value and returns it.
 *  @see CppSensorPlacement
 */
#define DYNATRACE_RETURN_BOOL(return_value) \
	__DYNATRACE_RETURN__(dynatrace_bool_t, return_value)

/** Captures a char return value and returns it.
 *  @see CppSensorPlacement
 */
#define DYNATRACE_RETURN_CHAR(return_value) \
	__DYNATRACE_RETURN__(char, return_value)

/** Captures an int16 return value and returns it.
 *  @see CppSensorPlacement
 */
#define DYNATRACE_RETURN_INT16(return_value) \
	__DYNATRACE_RETURN__(dynatrace_int16_t, return_value)

/** Captures an int32 return value and returns it.
 *  @see CppSensorPlacement
 */
#define DYNATRACE_RETURN_INT32(return_value) \
	__DYNATRACE_RETURN__(dynatrace_int32_t, return_value)

/** Captures an int64 return value and returns it.
 *  @see CppSensorPlacement
 */
#define DYNATRACE_RETURN_INT64(return_value) \
	__DYNATRACE_RETURN__(dynatrace_int64_t, return_value)

/** Captures a float return value and returns it.
 *  @see CppSensorPlacement
 */
#define DYNATRACE_RETURN_FLOAT(return_value) \
	__DYNATRACE_RETURN__(float, return_value)

/** Captures a double return value and returns it.
 *  @see CppSensorPlacement
 */
#define DYNATRACE_RETURN_DOUBLE(return_value) \
	__DYNATRACE_RETURN__(double, return_value)

/** Captures a zero-terminated 7-bit ASCII (or EBCDIC on zOS) string
 *  return value and returns it.
 *  @remarks The maximum length captured depends on the dynaTrace Agent.
 *  @see CppSensorPlacement
 */
#define DYNATRACE_RETURN_STRING(return_value) \
	__DYNATRACE_RETURN__(const char *, return_value)

/** Captures a string of the specified length (in bytes) and
 *  codeset (CCSID) as return value and returns it.
 *  @remarks The maximum length captured depends on the dynaTrace Agent.
 *  @see CppSensorPlacement
 */
#define DYNATRACE_RETURN_STRING_WITH_CODESET(return_value, length, ccsid) \
	__DYNATRACE_RETURN_2__(const char *, return_value, length, ccsid)

/** @} */

/** \defgroup CppExceptions C++ Exceptions
\code
class TestException : public std::exception {
private:
	const char *m;
public:
	TestException() throw() : m("unknown") {}
	TestException(const char *message) throw() : m(message) {}
	virtual ~TestException() {}
	virtual const char *what() const throw() {
		return m;
	}
};

void Foo::baz() throw() {
	...
	DYNATRACE_EXCEPTION_THROW(TestException, TestException);
	...
	DYNATRACE_EXCEPTION_THROW(TestException, TestException("test exception"));
	...
}

void Foo::bar() {
	DYNATRACE_AUTO_NODE();
	try {
		baz();
	} catch (TestException &) {
		...
	}
}
\endcode
 *  @{
 */

/** Throws an std::exception (or subclass thereof) and captures it.
 *  @remarks The exception type needs a working assignment operator.
 *           The message is retrieved via std::exception::what().
 *  @remarks The maximum length captured depends on the dynaTrace Agent.
 */
#define DYNATRACE_EXCEPTION_THROW(exception_type, exception_value) \
	__DYNATRACE_EXCEPTION_THROW__(exception_type, exception_value)

/** @} */

/** @} */

#endif /* __cplusplus */

/** \defgroup Tagging Tagging
\code
void handle_request(int client) {
	unsigned char buf[256];

	read(client, buf, 1);	// tag size
	read(client, buf, (size_t)buf[0]); // actual tag

	DYNATRACE_SET_TAG(buf);
	DYNATRACE_START_SERVER_PUREPATH();

	...

	DYNATRACE_END_SERVER_PUREPATH();

	close(client);
}

void server() {
	int s = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(12345);
	bind(s, (struct sockaddr *)&sin, sizeof(sin));
	listen(s, 5);
	int cl = accept(s, NULL, NULL);
	handle_request(cl);
	close(s);
}

void client() {
	struct hostent *hp = gethostbyname("localhost");
	int s = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
	sin.sin_port = htons(12345);
	connect(s, (struct sockaddr *)&sin, sizeof(sin);

	unsigned char buf[256];
	int size = DYNATRACE_GET_TAG(buf+1, sizeof(buf)-1);
	if (size < 0) {
		// error
		buf[0] = 0;
	} else {
		buf[0] = (unsigned char)size;
	}
	write(s, buf, buf[0]+1);

	DYNATRACE_LINK_CLIENT_PUREPATH(0, buf);

	...

	close(s);
}
\endcode
 *  @{
 */

/** Gets the current dynaTrace tag on the client side represented
 *  as zero-terminated 7-bit ASCII (or EBCDIC on zOS) string for serialization by the user.
 *  @return the size of the tag including the terminating zero, negative in case of error
 *  @remarks This may be useful when tagging via HTTP headers for example
 *  @see DYNATRACE_SET_TAG_FROM_STRING
 */
#define DYNATRACE_GET_TAG_AS_STRING(buf, size) \
	dynatrace_get_tag_as_string(buf, size)

/** Sets the dynaTrace tag on the server side from a zero-terminated
 *  7-bit ASCII (or EBCDIC on zOS) string as deserialized by the user.
 *  @see DYNATRACE_GET_TAG_AS_STRING
 */
#define DYNATRACE_SET_TAG_FROM_STRING(tag) \
	dynatrace_set_tag_from_string(tag)

/** Gets the current dynaTrace tag on the client side represented
 *  as byte array for serialization by the user.
 *  @return the size of the tag, negative in case of error
 *  @see DYNATRACE_SET_TAG
 */
#define DYNATRACE_GET_TAG(buf, size) \
	dynatrace_get_tag(buf, size)

/** Sets the dynaTrace tag on the server side from a byte array
 *  as deserialized by the user.
 *  @see DYNATRACE_GET_TAG
 */
#define DYNATRACE_SET_TAG(tag) \
	dynatrace_set_tag(tag)

/** Sets the dynaTrace tag on the server side from a byte array using a custom tag
 *  as deserialized by the user.
 *  @param customTag	pointer to a byte array representing the custom tag
 *  @param tagSize		size of custom tag
 */
#define DYNATRACE_SET_CUSTOMTAG(customTag, tagSize) \
	dynatrace_set_customtag(customTag, tagSize)

/** Returns the amount of space required for a dynaTrace tag in bytes.
 *  @return the size of the tag, negative in case of an error
 */
#define DYNATRACE_GET_TAG_SIZE() \
	dynatrace_get_tag_size()

/** Inserts a synchronous or asynchronous link on the client side.
 *  @param synchronous true if synchronous, false if asynchronous
 *  @param buffer containing the corresponding dynaTrace tag
 */
#define DYNATRACE_LINK_CLIENT_PUREPATH(synchronous, buffer) \
	dynatrace_link_client_purepath(synchronous, buffer)

/** Inserts a synchronous or asynchronous link on the client side.
 *  @param synchronous true if synchronous, false if asynchronous
 *  @param buffer zero-terminated 7-bit ASCII (or EBCDIC on zOS) string
 *                representation of the corresponding dynaTrace tag
 */
#define DYNATRACE_LINK_CLIENT_PUREPATH_BY_STRING(synchronous, buffer) \
	dynatrace_link_client_purepath_by_string(synchronous, buffer)

/** Inserts a synchronous or asynchronous link on the client side using a "custom tag"
 *  @param synchronous true if synchronous, false if asynchronous
 *  @param customTag		pointer to a byte array representing the custom tag
 *  @param tagSize			size of custom tag
 *  @param prevCustomTag	pointer to a previous custom tag, to be linked with the current tag
 *                          (NULL if not present or not used) - obsolete, no longer required!
 *  @param prevTagSize		size of previous custom tag - obsolete, no longer required!
 */
#define DYNATRACE_LINK_CLIENT_PUREPATH_BY_CUSTOMTAG(synchronous, customTag, tagSize, prevCustomTag, prevTagSize) \
	dynatrace_link_client_purepath_by_customtag(synchronous, customTag, tagSize, prevCustomTag, prevTagSize)

/** Starts a server-side PurePath.
 */
#define DYNATRACE_START_SERVER_PUREPATH() \
	dynatrace_start_server_purepath()

/** Ends a server-side PurePath.
 */
#define DYNATRACE_END_SERVER_PUREPATH() \
	dynatrace_end_server_purepath()

/** Convert a dynaTrace tag to its string representation
 *  @param tag     pointer to the buffer containing the tag of DYNATRACE_TAG_SIZE bytes size
 *  @param buf     string buffer to write the tag to as 7-bit ASCII (or EBCDIC in zOS) string
 *  @param size    size of the target buffer (should be at least 128 characters)
 *  @return number of bytes written to the buffer on success, -1 otherwise
 */
#define DYNATRACE_TAG_TO_STRING(tag, buf, size) \
    dynatrace_tag_to_string((tag), (buf), (size))

/** Convert a dynaTrace string tag into the tag of DYNATRACE_TAG_SIZE bytes size
 *  @param str     string buffer containing the tag as 7-bit ASCII (or EBCDIC in zOS) string
 *  @param tag     pointer to a (at least) DYNATRACE_TAG_SIZE bytes buffer for receiving the tag
 *  @param size    size of the target buffer
 *  @return number of bytes written to the buffer on success, -1 otherwise
 */
#define DYNATRACE_STRING_TO_TAG(str, tag, size) \
    dynatrace_string_to_tag((str), (tag), (size))

/**
 *	Check if specified dynaTrace tag is valid.
 *	@param	tag	pointer to buffer containing the dynaTrace tag of DYNATRACE_TAG_SIZE bytes size
 *	@return		1 if tag is valid
 *				0 if tag is invalid
 *			   -1 internal function error (e.g. ADK library could not be loaded)
 */
#define DYNATRACE_IS_TAG_VALID(tag) \
	dynatrace_is_tag_valid(tag)

/** @} */

#endif /* _DYNATRACE_ADK_H_ */
