#ifndef _DYNATRACE_ADK_TYPES_H_
#define _DYNATRACE_ADK_TYPES_H_

#ifdef __cplusplus
typedef bool dynatrace_bool_t;
#else /* __cplusplus */
typedef int dynatrace_bool_t;
#endif /* __cplusplus */

typedef unsigned char dynatrace_byte_t;
typedef short dynatrace_int16_t;
typedef unsigned short dynatrace_uint16_t;
typedef int dynatrace_int32_t;
typedef unsigned int dynatrace_uint32_t;
typedef long long dynatrace_int64_t;
typedef unsigned long long dynatrace_uint64_t;

#endif /* _DYNATRACE_ADK_TYPES_H_ */
