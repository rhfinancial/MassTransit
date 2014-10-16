#ifndef _DYNATRACE_ADK_CPP_H_
#define _DYNATRACE_ADK_CPP_H_

#include <limits.h>

#include "dynatrace_adk_types.h"

#ifdef __cplusplus

#define __DYNATRACE_QUALIFIED__ ::dynatrace::adk::

namespace dynatrace {
namespace adk {

class PurePathNode {
private:
	int method_id;
	int serial_no;
public:
	PurePathNode(int method_id, int serial_no) : method_id(method_id), serial_no(serial_no) {}
	void enter() { serial_no = dynatrace_enter(method_id, serial_no); }
	~PurePathNode() { dynatrace_exit(method_id, serial_no); }
};

template<typename T>
void dynatrace_capture_arg(int serial_no, const T &argument);

template<int N>
inline void dynatrace_capture_arg(int serial_no, const char (&argument)[N]) {
	dynatrace_capture_arg(serial_no, static_cast<const char *>(argument));
}

#define __DYNATRACE_TRANSLATE__(from_type, to_type, suffix) \
	inline void dynatrace_capture_arg(int serial_no, const from_type &argument) { \
		dynatrace_capture_##suffix(serial_no, static_cast<to_type>(argument)); \
	}

__DYNATRACE_TRANSLATE__(dynatrace_bool_t, dynatrace_int32_t, bool)
__DYNATRACE_TRANSLATE__(char, char, char)
__DYNATRACE_TRANSLATE__(dynatrace_byte_t, char, char)
__DYNATRACE_TRANSLATE__(double, double, double)
__DYNATRACE_TRANSLATE__(float, float, float)

#if (SHRT_MIN == -32768) && (SHRT_MAX == 32767)
__DYNATRACE_TRANSLATE__(dynatrace_int16_t, dynatrace_int16_t, int16)
__DYNATRACE_TRANSLATE__(dynatrace_uint16_t, dynatrace_int16_t, int16)
#else
#	error short is not a 16bit type
#endif

#if (INT_MIN == (-2147483647 - 1)) && (INT_MAX == 2147483647)
__DYNATRACE_TRANSLATE__(dynatrace_int32_t, dynatrace_int32_t, int32)
__DYNATRACE_TRANSLATE__(dynatrace_uint32_t, dynatrace_int32_t, int32)
#else
#	error int is not a 32bit type
#endif

#if (LONG_MIN == (-2147483647L - 1)) && (LONG_MAX == 2147483647L)
__DYNATRACE_TRANSLATE__(long, dynatrace_int32_t, int32)
__DYNATRACE_TRANSLATE__(unsigned long, dynatrace_int32_t, int32)
#else
#if (LONG_MIN == (-9223372036854775807LL - 1)) && (LONG_MAX == 9223372036854775807LL)
__DYNATRACE_TRANSLATE__(long, dynatrace_int64_t, int64)
__DYNATRACE_TRANSLATE__(unsigned long, dynatrace_int64_t, int64)
#else
#	error long is neither a 32bit nor a 64bit type
#endif
#endif

#if (defined(LLONG_MIN) && (LLONG_MIN == (-9223372036854775807LL - 1)) && (LLONG_MAX == 9223372036854775807LL)) || (defined(LONG_LONG_MIN) && (LONG_LONG_MIN == (-9223372036854775807LL - 1)) && (LONG_LONG_MAX == 9223372036854775807LL))
__DYNATRACE_TRANSLATE__(long long, dynatrace_int64_t, int64)
__DYNATRACE_TRANSLATE__(unsigned long long, dynatrace_int64_t, int64)
#else
#	error long long is not a 64bit type
#endif

__DYNATRACE_TRANSLATE__(char * const, const char * const, string)

#undef __DYNATRACE_TRANSLATE__

class Capture {
private:
	int serial_no;
public:
	Capture(int serial_no) : serial_no(serial_no) {}

	template<typename T>
	const Capture &operator<<(const T& argument) const {
		dynatrace_capture_arg(serial_no, argument);
		return *this;
	}
};

} /* namespace adk */
} /* namespace dynatrace */

#endif /* __cplusplus */

#endif /* _DYNATRACE_ADK_CPP_H_ */
