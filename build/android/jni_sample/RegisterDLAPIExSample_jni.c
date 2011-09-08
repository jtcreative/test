/**
   File : registerDLAPIExSample_jni.c
   Copyright (c) 2010 Nextreaming Inc, all rights reserved.
 */


#include "dlfcn.h"
#include "nexplayer_jni.h"


#include <jni.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <android/log.h>


#define NEXPLAYERENGINE_LIB "/data/data/com.nextreaming.nexplayersample/lib/libnexplayerengine.so"
#define NEXPLAYERENGINE_GETDLAPI_CALLBACK_FUNC "nexPlayerSWP_RegisterDLAPICallbackFunc"
                                                        

#define  LOG_TAG    "GETDLAPI_SAMPLE"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGV(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)


void* DLOpenCallbackFunc(const char* filename, int flag)
{
	LOGI("[DLOpenCallbackFunc] \n");

	char strPath[1024];
	memset(strPath, 0, 1024);

	strcpy(strPath, "/data/data/com.nextreaming.nexplayersample/lib/");
	strcat(strPath, filename);
	return dlopen(strPath, flag);
}
void* DLSymCallbackFunc(void* hDL, const char* strFunc)
{
	LOGI("[DLSymCallbackFunc] \n");
	return dlsym(hDL, strFunc);
}

int  DLCloseCallbackFunc(void* hDL)
{
	LOGI("[DLSymCallbackFunc] \n");
	return dlclose(hDL);
}
const char* DLErrorCallbackFunc(void)
{
	LOGI("[DLErrorCallbackFunc] \n");
	return dlerror();
}

jint Java_com_nextreaming_registerdlapiexsample_RegisterDLAPIExManager_initDLAPIExManager (JNIEnv * env, 
														   	   jobject clazz,
														   	   jstring libName)
{
	LOGI("[initGetDLAPI] Start \n");
	void *handle = NULL;
	int (*fptr)( NEXPLAYERDLOpenCallbackFunc pDLOpenFunc, NEXPLAYERDLOpenCallbackFunc pDLSymFunc, NEXPLAYERDLCloseCallbackFunc pDLCloseFunc, NEXPLAYERDLErrorCallbackFunc pDLErrorFunc);

	
    char buf[512];
    const jbyte *str;
    str = (*env)->GetStringUTFChars(env, libName, NULL);
    if(str != NULL)
	{
		handle = dlopen(str, RTLD_LAZY);  
		
		LOGI("[initGetDLAPI] libName[0x%X]:%s",handle, str);
	}
    else
	{
	    /* Load Default NexPlayerEngine library */
	    handle = dlopen(NEXPLAYERENGINE_LIB, RTLD_LAZY);
	}

	
    LOGI("initializeAgent : nextreaming handle=%p", handle);
    if (handle == NULL)
    {
        LOGI("[initGetDLAPI] error=%s", dlerror());
        return -1;
    }
   
    /* Get DRM register function pointer*/   
    fptr = (int (*)( NEXPLAYERDLOpenCallbackFunc pDLOpenFunc, NEXPLAYERDLOpenCallbackFunc pDLSymFunc, NEXPLAYERDLCloseCallbackFunc pDLCloseFunc, NEXPLAYERDLErrorCallbackFunc pDLErrorFunc))dlsym(handle, NEXPLAYERENGINE_GETDLAPI_CALLBACK_FUNC);
    
    LOGI("[initGetDLAPI] fptr = %p", fptr);
    if (fptr == NULL)
    {
        LOGI("[initGetDLAPI] error=%s", dlerror());
    }
    
    /* Register DRM descramble function */
    if (fptr != NULL)
        (*fptr)(DLOpenCallbackFunc, DLSymCallbackFunc, DLCloseCallbackFunc, DLErrorCallbackFunc);

    dlclose(handle); 
   
    return 0;
}
