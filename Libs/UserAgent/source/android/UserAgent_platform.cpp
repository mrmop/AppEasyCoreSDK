/*
 * android-specific implementation of the UserAgent extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "UserAgent_internal.h"

#include "s3eEdk.h"
#include "s3eEdk_android.h"
#include <jni.h>
#include "IwDebug.h"

static char* g_RetStr;
static int g_RetStrLen = 16384;

static jobject g_Obj;
static jmethodID g_getUserAgent;

struct JavaString
{
    jstring _Ref;

    JavaString ( const char *Buffer )
    {
        JNIEnv *env = s3eEdkJNIGetEnv();
        _Ref = env->NewStringUTF ( Buffer );
    }

    JavaString ( jstring Ref )
    {
        _Ref = Ref;
    }

    ~JavaString()
    {
        JNIEnv *env = s3eEdkJNIGetEnv();
        env->DeleteLocalRef ( _Ref );
    }

    operator jstring()
    {
        return _Ref;
    }
};

const char* getCString(jstring str)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    if (!str)
        return NULL;
    jboolean free;
    const char* res = env->GetStringUTFChars(str, &free);
    g_RetStrLen = strlen(res) + 1;
    s3eEdkReallocOS(g_RetStr, g_RetStrLen);
    strncpy(g_RetStr, res, g_RetStrLen);
    env->ReleaseStringUTFChars(str, res);
    return g_RetStr;
}

s3eResult UserAgentInit_platform()
{
    //Alloc buffer for returning strings
    g_RetStr = (char*)s3eEdkMallocOS(g_RetStrLen);

    // Get the environment from the pointer
    JNIEnv* env = s3eEdkJNIGetEnv();
    jobject obj = NULL;
    jmethodID cons = NULL;

    // Get the extension class
    jclass cls = s3eEdkAndroidFindClass("UserAgent");
    if (!cls)
        goto fail;

    // Get its constructor
    cons = env->GetMethodID(cls, "<init>", "()V");
    if (!cons)
        goto fail;

    // Construct the java class
    obj = env->NewObject(cls, cons);
    if (!obj)
        goto fail;

    // Get all the extension methods
    g_getUserAgent = env->GetMethodID(cls, "getUserAgent", "()Ljava/lang/String;");
    if (!g_getUserAgent)
        goto fail;



    IwTrace(USERAGENT, ("USERAGENT init success"));
    g_Obj = env->NewGlobalRef(obj);
    env->DeleteLocalRef(obj);
    env->DeleteGlobalRef(cls);

    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;

fail:
    jthrowable exc = env->ExceptionOccurred();
    if (exc)
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
        IwTrace(UserAgent, ("One or more java methods could not be found"));
    }
    return S3E_RESULT_ERROR;

}

void UserAgentTerminate_platform()
{
    // Add any platform-specific termination code here
    s3eEdkFreeOS(g_RetStr);
    g_RetStr = NULL;
}

const char* getUserAgent_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    JavaString str((jstring)env->CallObjectMethod(g_Obj, g_getUserAgent));
    return getCString((jstring)str);
}
