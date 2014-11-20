#ifndef _DYNATRACE_ADK_EXPORTS_H_
#define _DYNATRACE_ADK_EXPORTS_H_

#include "dynatrace_adk_types.h"

#ifdef _WIN32
	#define __DYNATRACE_CALL__ __cdecl
	#ifdef DYNATRACE_DECLARE_STATIC
		#define __DYNATRACE_DLLEXPORT__
	#else
		#define __DYNATRACE_DLLEXPORT__ __declspec(dllexport)
	#endif /* DYNATRACE_DECLARE_STATIC */
#else /* _WIN32 */
	#define __DYNATRACE_CALL__
	#if defined(DYNATRACE_DECLARE_STATIC) || (__GNUC__ < 4)
		#define __DYNATRACE_DLLEXPORT__
	#else
		#define __DYNATRACE_DLLEXPORT__ __attribute__((visibility("default")))
	#endif /* DYNATRACE_DECLARE_STATIC || __GNUC__ >= 4 */
#endif /* _WIN32 */

#ifdef __cplusplus
	#define __DYNATRACE_EXTERN__ extern "C" __DYNATRACE_DLLEXPORT__
#else
	#define __DYNATRACE_EXTERN__ extern __DYNATRACE_DLLEXPORT__
#endif /* __cplusplus */

#define __DYNATRACE_DECLARE__(return_type) __DYNATRACE_EXTERN__ return_type __DYNATRACE_CALL__

/* ---- Initializiation ---- */
__DYNATRACE_DECLARE__(void) dynatrace_set(const char *key_value);
__DYNATRACE_DECLARE__(int) dynatrace_initialize(int *argc, char **argv[]);
__DYNATRACE_DECLARE__(int) dynatrace_reinitialize();
__DYNATRACE_DECLARE__(void) dynatrace_uninitialize();

/* ---- Util ---- */
__DYNATRACE_DECLARE__(void) dynatrace_agent_log(const char *message, int level);

/* ---- PurePath ---- */
__DYNATRACE_DECLARE__(int) dynatrace_get_method_id(const char *method_name, const char *source_file_name, int source_line_no, const char *api, unsigned char is_artificial);
__DYNATRACE_DECLARE__(int) dynatrace_get_method_id_with_charset(const char *method_name, int method_name_length, unsigned short method_name_ccsid, const char *source_file_name, int source_file_name_length, unsigned short source_file_name_ccsid, int source_line_no, const char *api, int api_length, unsigned short api_ccsid, unsigned char is_artificial);
__DYNATRACE_DECLARE__(int) dynatrace_get_serial_no(int method_id, int entry_point);
__DYNATRACE_DECLARE__(int) dynatrace_enter(int method_id, int serial_no);
__DYNATRACE_DECLARE__(void) dynatrace_exit(int method_id, int serial_no);
__DYNATRACE_DECLARE__(void) dynatrace_exit_exception(int method_id, int serial_no, const char *message);
__DYNATRACE_DECLARE__(void) dynatrace_exception_throw(int method_id, const char *message, const char *throwable_class);
__DYNATRACE_DECLARE__(void) dynatrace_exception_throw_with_charset(int method_id, const char *message, int message_length, unsigned short message_ccsid, const char *throwable_class, int throwable_class_length, unsigned short throwable_class_ccsid);
__DYNATRACE_DECLARE__(void) dynatrace_custom_log(int method_id, const char *message, int level, const char *logger_name);
__DYNATRACE_DECLARE__(void) dynatrace_custom_log_with_charset(int method_id, const char *message, int message_length, unsigned short message_ccsid, int level, const char *logger_name, int logger_name_length, unsigned short logger_name_ccsid);

/* ---- Capturing ---- */
__DYNATRACE_DECLARE__(void) dynatrace_capture_bool(int serial_no, dynatrace_int32_t argument);
__DYNATRACE_DECLARE__(void) dynatrace_capture_char(int serial_no, char argument);
__DYNATRACE_DECLARE__(void) dynatrace_capture_int16(int serial_no, dynatrace_int16_t argument);
__DYNATRACE_DECLARE__(void) dynatrace_capture_int32(int serial_no, dynatrace_int32_t argument);
__DYNATRACE_DECLARE__(void) dynatrace_capture_int64(int serial_no, dynatrace_int64_t argument);
__DYNATRACE_DECLARE__(void) dynatrace_capture_float(int serial_no, float argument);
__DYNATRACE_DECLARE__(void) dynatrace_capture_double(int serial_no, double argument);
__DYNATRACE_DECLARE__(void) dynatrace_capture_string(int serial_no, const char *argument);
__DYNATRACE_DECLARE__(void) dynatrace_capture_string_with_charset(int serial_no, const char * argument, int length, dynatrace_uint16_t ccsid);

/* ---- Tagging ---- */
__DYNATRACE_DECLARE__(int) dynatrace_get_tag_as_string(char *buf, size_t size);
__DYNATRACE_DECLARE__(int) dynatrace_set_tag_from_string(const char *tag);
__DYNATRACE_DECLARE__(int) dynatrace_get_tag(unsigned char *buf, size_t size);
__DYNATRACE_DECLARE__(int) dynatrace_set_tag(unsigned char *buf);
__DYNATRACE_DECLARE__(int) dynatrace_set_customtag(const unsigned char *customTag, size_t tagSize);
__DYNATRACE_DECLARE__(int) dynatrace_get_tag_size();
__DYNATRACE_DECLARE__(void) dynatrace_link_client_purepath(int synchronous, const unsigned char *buf);
__DYNATRACE_DECLARE__(void) dynatrace_link_client_purepath_by_string(int synchronous, const char* tag);
__DYNATRACE_DECLARE__(void) dynatrace_link_client_purepath_by_customtag(int synchronous, const unsigned char *customTag, size_t tagSize, const unsigned char *prevCustomTag, size_t prevTagSize);
__DYNATRACE_DECLARE__(void) dynatrace_start_server_purepath();
__DYNATRACE_DECLARE__(void) dynatrace_end_server_purepath();
__DYNATRACE_DECLARE__(int) dynatrace_tag_to_string(const unsigned char* tag, char* buf, size_t size);
__DYNATRACE_DECLARE__(int) dynatrace_string_to_tag(const char* tag, unsigned char* buf, size_t size);
__DYNATRACE_DECLARE__(int) dynatrace_is_tag_valid(const unsigned char *buf);

#endif /* _DYNATRACE_ADK_EXPORTS_H_ */
