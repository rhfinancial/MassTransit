#ifndef _DYNATRACE_ADK_PRIVATE_H_
#define _DYNATRACE_ADK_PRIVATE_H_

#include <stddef.h>

#include "dynatrace_adk_exports.h"

#ifdef __cplusplus
#include "dynatrace_adk_cpp.h"
#endif /* __cplusplus */

#undef __DYNATRACE_CURRENT_FUNCTION__

/*
#ifdef __GNUC__
#define __DYNATRACE_CURRENT_FUNCTION__ __PRETTY_FUNCTION__
#endif

#ifndef __DYNATRACE_CURRENT_FUNCTION__
#if _MSC_VER >= 1300
#define __DYNATRACE_CURRENT_FUNCTION__ __FUNCSIG__
#endif
#endif
*/

#ifndef __DYNATRACE_CURRENT_FUNCTION__
#if defined(_MSC_VER) || defined(__GNUC__)
#define __DYNATRACE_CURRENT_FUNCTION__ __FUNCTION__
#else /* defined(_MSC_VER) || defined(__GNUC__) */
#define __DYNATRACE_CURRENT_FUNCTION__ __func__
#endif /* defined(_MSC_VER) || defined(__GNUC__) */
#endif /* __DYNATRACE_CURRENT_FUNCTION__ */

#define __DYNATRACE_CURRENT_SOURCE_FILE__ __FILE__
#define __DYNATRACE_CURRENT_SOURCE_LINENO__ __LINE__

#define __DYNATRACE_BEGIN_BLOCK__ do {
#ifdef _MSC_VER
#define __DYNATRACE_END_BLOCK__ } __pragma(warning(push)) __pragma(warning(disable:4127)) while(0) __pragma(warning(pop))
#else /* _MSC_VER */
#define __DYNATRACE_END_BLOCK__ } while(0)
#endif /* _MSC_VER */

#if !defined(_MSC_VER) || _MSC_VER >= 1400
#define __DYNATRACE_HAS_VARIADIC_MACROS__ 1
#endif /* !defined(_MSC_VER) || _MSC_VER >= 1400 */

/* ---- API ---- */
#define __DYNATRACE_API_DEFAULT__ "ADK"
#define __DYNATRACE_API_EXCEPTION__ "Exception"
#define __DYNATRACE_API_LOGGING__ "Log"

#define __DYNATRACE_API__(api) \
	static const char *__dynatrace_api__ = (api)

__DYNATRACE_API__(__DYNATRACE_API_DEFAULT__);

#define __DYNATRACE_ENTER_PRE__(entry_point) \
	static int __dynatrace_method_id__ = 0; \
	int __dynatrace_serial_no__; \
	if (!__dynatrace_method_id__) { \
		__dynatrace_method_id__ = dynatrace_get_method_id(__DYNATRACE_CURRENT_FUNCTION__, __DYNATRACE_CURRENT_SOURCE_FILE__, __DYNATRACE_CURRENT_SOURCE_LINENO__, __dynatrace_api__, 0); \
	} \
	__dynatrace_serial_no__ = dynatrace_get_serial_no(__dynatrace_method_id__, entry_point)

#define __DYNATRACE_ENTER_POST__() \
	__dynatrace_serial_no__ = dynatrace_enter(__dynatrace_method_id__, __dynatrace_serial_no__)

#define __DYNATRACE_ENTER__(entry_point) \
	__DYNATRACE_ENTER_PRE__(entry_point); \
	__DYNATRACE_ENTER_POST__()

#if __DYNATRACE_HAS_VARIADIC_MACROS__

#define __DYNATRACE_ENTER_CAPTURE__(entry_point, ...) \
	__DYNATRACE_ENTER_PRE__(entry_point); \
	for (__VA_ARGS__;;) break; \
	__DYNATRACE_ENTER_POST__()

#endif /* __DYNATRACE_HAS_VARIADIC_MACROS__ */

#define __DYNATRACE_ENTER_CAPTURE_9__(entry_point, capture1, capture2, capture3, capture4, capture5, capture6, capture7, capture8, capture9) \
	__DYNATRACE_ENTER_PRE__(entry_point); \
	capture1; capture2; capture3; capture4; capture5; capture6; capture7; capture8; capture9; \
	__DYNATRACE_ENTER_POST__()

#define __DYNATRACE_EXIT_RETURN__(return_type, capture_type, return_value) \
	__DYNATRACE_BEGIN_BLOCK__ \
		return_type __dynatrace_tmp_rv__ = (return_value); \
		DYNATRACE_CAPTURE_##capture_type(__dynatrace_tmp_rv__); \
		dynatrace_exit(__dynatrace_method_id__, __dynatrace_serial_no__); \
		return __dynatrace_tmp_rv__; \
	__DYNATRACE_END_BLOCK__

#define __DYNATRACE_EXIT_RETURN_2__(return_type, capture_type, return_value, arg1, arg2) \
	__DYNATRACE_BEGIN_BLOCK__ \
		return_type __dynatrace_tmp_rv__ = (return_value); \
		DYNATRACE_CAPTURE_##capture_type(__dynatrace_tmp_rv__, arg1, arg2); \
		dynatrace_exit(__dynatrace_method_id__, __dynatrace_serial_no__); \
		return __dynatrace_tmp_rv__; \
	__DYNATRACE_END_BLOCK__

#define __DYNATRACE_LOG__(level, message, logger_name) \
	__DYNATRACE_BEGIN_BLOCK__ \
		static int __dynatrace_method_id__ = 0; \
		if (!__dynatrace_method_id__) { \
		__dynatrace_method_id__ = dynatrace_get_method_id(__DYNATRACE_CURRENT_FUNCTION__, __DYNATRACE_CURRENT_SOURCE_FILE__, __DYNATRACE_CURRENT_SOURCE_LINENO__, __DYNATRACE_API_LOGGING__, 0); \
		} \
		dynatrace_custom_log(__dynatrace_method_id__, message, level, logger_name); \
	__DYNATRACE_END_BLOCK__

#define __DYNATRACE_LOG_WITH_CHARSET__(level, message, message_length, message_ccsid, logger_name, logger_name_length, logger_name_ccsid) \
	__DYNATRACE_BEGIN_BLOCK__ \
		static int __dynatrace_method_id__ = 0; \
		if (!__dynatrace_method_id__) { \
		__dynatrace_method_id__ = dynatrace_get_method_id(__DYNATRACE_CURRENT_FUNCTION__, __DYNATRACE_CURRENT_SOURCE_FILE__, __DYNATRACE_CURRENT_SOURCE_LINENO__, __DYNATRACE_API_LOGGING__, 0); \
		} \
		dynatrace_custom_log_with_charset(__dynatrace_method_id__, message, message_length, message_ccsid, level, logger_name, logger_name_length, logger_name_ccsid); \
	__DYNATRACE_END_BLOCK__

#ifdef __cplusplus

#define __DYNATRACE_AUTO__(entry_point) \
	static int __dynatrace_method_id__ = 0; \
	int __dynatrace_serial_no__; \
	if (!__dynatrace_method_id__) { \
		__dynatrace_method_id__ = dynatrace_get_method_id(__DYNATRACE_CURRENT_FUNCTION__, __DYNATRACE_CURRENT_SOURCE_FILE__, __DYNATRACE_CURRENT_SOURCE_LINENO__, __dynatrace_api__, 0); \
	} \
	__dynatrace_serial_no__ = dynatrace_get_serial_no(__dynatrace_method_id__, entry_point); \
	__DYNATRACE_QUALIFIED__ PurePathNode __dynatrace_auto__(__dynatrace_method_id__, __dynatrace_serial_no__); \
	__dynatrace_auto__.enter();

#define __DYNATRACE_AUTO_CAPTURE__(entry_point, code) \
	static int __dynatrace_method_id__ = 0; \
	int __dynatrace_serial_no__; \
	if (!__dynatrace_method_id__) { \
		__dynatrace_method_id__ = dynatrace_get_method_id(__DYNATRACE_CURRENT_FUNCTION__, __DYNATRACE_CURRENT_SOURCE_FILE__, __DYNATRACE_CURRENT_SOURCE_LINENO__, __dynatrace_api__, 0); \
	} \
	__dynatrace_serial_no__ = dynatrace_get_serial_no(__dynatrace_method_id__, entry_point); \
	__DYNATRACE_QUALIFIED__ PurePathNode __dynatrace_auto__(__dynatrace_method_id__, __dynatrace_serial_no__); \
	{ code; } \
	__dynatrace_auto__.enter();

#define __DYNATRACE_RETURN__(return_type, return_value) \
	__DYNATRACE_BEGIN_BLOCK__ \
		return_type __dynatrace_tmp_rv__ = (return_value); \
		__DYNATRACE_QUALIFIED__ dynatrace_capture_arg(__dynatrace_serial_no__, __dynatrace_tmp_rv__); \
		return __dynatrace_tmp_rv__; \
	__DYNATRACE_END_BLOCK__

#define __DYNATRACE_RETURN_2__(return_type, return_value, arg1, arg2) \
	__DYNATRACE_BEGIN_BLOCK__ \
		return_type __dynatrace_tmp_rv__ = (return_value); \
		__DYNATRACE_QUALIFIED__ dynatrace_capture_arg(__dynatrace_serial_no__, __dynatrace_tmp_rv__); \
		return __dynatrace_tmp_rv__; \
	__DYNATRACE_END_BLOCK__

#define __DYNATRACE_EXCEPTION_THROW__(exception_type, exception_value) \
	__DYNATRACE_BEGIN_BLOCK__ \
		exception_type __dynatrace_tmp_exception__ = (exception_value); \
		int method_id = dynatrace_get_method_id(#exception_type, NULL, 0, __DYNATRACE_API_EXCEPTION__, 0); \
		dynatrace_exception_throw(method_id, __dynatrace_tmp_exception__.what(), #exception_type); \
		throw __dynatrace_tmp_exception__; \
	__DYNATRACE_END_BLOCK__

#endif /* __cplusplus */

#endif /* _DYNATRACE_ADK_PRIVATE_H_ */
