#include <jni.h>
#include "inlineHook.h"
#include "android_log.h"

void (*old_log)(const char *) = NULL;

void origin_log(const char *str) {
    LOGE("%s", str);
    char buf[10];
    buf[0] = '1';
    buf[1] = '2';
    buf[2] = '3';
    buf[3] = 0;
    LOGE("%s",buf);
}

void new_log(const char *str) {
    LOGE("hooked");
//    old_log("log hooked");
}

int hook() {
    if (registerInlineHook((uint32_t) origin_log, (uint32_t) new_log, (uint32_t **) &old_log)
        != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) origin_log) != ELE7EN_OK) {
        return -1;
    }

    return 0;
}

int unHook() {
    if (inlineUnHook((uint32_t) origin_log) != ELE7EN_OK) {
        return -1;
    }

    return 0;
}


jstring
Java_com_andr0day_androidinlinehook_MainActivity_stringFromJNI(JNIEnv *env, jobject obj) {
    origin_log("test1");
    int ret = hook();
    LOGE("%d", ret);
    origin_log("test2");
    unHook();
    origin_log("test3");
    return (*env)->NewStringUTF(env, "WTF");
}
