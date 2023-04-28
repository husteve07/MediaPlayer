#include <jni.h>
#include <string>
#include <android/native_window_jni.h>

#include "Log.h"

#include "IPlayerProxy.h"

//static JavaVM* vm = NULL;
extern "C"
JNIEXPORT
jint JNI_OnLoad(JavaVM* vm, void *res)
{
    IPlayerProxy::Get()->Init(vm);
    return JNI_VERSION_1_6;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_mediaplayer_PlayMedia_InitView(JNIEnv *env, jobject thiz, jobject surface) {
    // TODO: implement InitView()
    ANativeWindow* win =  ANativeWindow_fromSurface(env, surface);
    IPlayerProxy::Get()->InitView(win);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_mediaplayer_OpenPath_Open(JNIEnv *env, jobject thiz, jstring url) {

    const char* url_ = env->GetStringUTFChars(url,0);
    IPlayerProxy::Get()->Open(url_);
    IPlayerProxy::Get()->Start();


}
