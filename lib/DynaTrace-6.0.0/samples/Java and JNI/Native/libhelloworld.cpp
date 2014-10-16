#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __MVS__
#include <unistd.h>
#endif
#include <jni.h>
#include <dynatrace_adk.h>

extern "C" {

JNIEXPORT void JNICALL Java_com_dynatrace_samples_NativeCalls_say(JNIEnv *e, jclass, jstring what) {
    jboolean isCopy;
    const char *utf = e->GetStringUTFChars(what, &isCopy);

    DYNATRACE_START_PUREPATH_CAPTURE(
            DYNATRACE_CAPTURE_STRING_WITH_CHARSET(utf, strlen(utf), DYNATRACE_CCSID_UTF8));

    char *str = strdup(utf);
#ifdef __MVS__
    __atoe((char *)str);
#endif

    printf("%s\n", str);
    free(str);

    DYNATRACE_EXIT();

	e->ReleaseStringUTFChars(what, utf);
}

JNIEXPORT void JNICALL Java_com_dynatrace_samples_NativeCalls_setTag(JNIEnv *e, jclass, jbyteArray jtag) {
	jbyte *tag = e->GetByteArrayElements(jtag, NULL);

	DYNATRACE_SET_TAG((unsigned char *)tag);

	e->ReleaseByteArrayElements(jtag, tag, JNI_ABORT);
}

JNIEXPORT void JNICALL Java_com_dynatrace_samples_NativeCalls_startServerPurePath(JNIEnv *, jclass) {
	DYNATRACE_START_SERVER_PUREPATH();
}

JNIEXPORT void JNICALL Java_com_dynatrace_samples_NativeCalls_endServerPurePath(JNIEnv *, jclass) {
	DYNATRACE_END_SERVER_PUREPATH();
}

JNIEXPORT void JNICALL Java_com_dynatrace_samples_NativeCalls_initialize(JNIEnv *, jclass) {
	DYNATRACE_SET("agentname=libhelloworld");
	DYNATRACE_INITIALIZE(NULL, NULL);
}

JNIEXPORT void JNICALL Java_com_dynatrace_samples_NativeCalls_uninitialize(JNIEnv *, jclass) {
	DYNATRACE_UNINITIALIZE();
}

}
