/**
   File : GetAESKeyFuncSample_jni.c
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
#define NEXPLAYERENGINE_GETCREDENTIAL_CALLBACK_FUNC "nexPlayerSWP_RegisterGetCredentialCallbackFunc"
                                                        

#define  LOG_TAG    "GETHTTPCREDENTIAL_SAMPLE"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGV(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)


#define HTTPCREDENTIAL_STRING "id: test1\r\npw: 12345\r\n"

static unsigned long GetCredentialCallbackFunc(
												char* pBuf, 
									 		unsigned long dwBufSize,
									 		unsigned long* pdwNewBufSize,
									 		unsigned long pUserData)
{
	unsigned int uiLen = 0;

	LOGI("[	GetCredentialCallbackFunc ]\n");

	uiLen = strlen(HTTPCREDENTIAL_STRING);

	if (dwBufSize < uiLen + 1)
	{
	          *pdwNewBufSize = uiLen + 1;
	          return 1;                                          // 1 means that the buffer is not enough. And if 1 is returned, then this callback will be called again with dwBufSize as same value with *pdwNewBufSize.
	}

	strcpy(pBuf, HTTPCREDENTIAL_STRING); // Copy credentials to pBuf. Do be careful that the credential MUST BE a NULL terminated string, and each line MUST BE ended with "\r\n".
	return 0;               // 0 means that the credential is set correctly. All the other return value except 0 and 1 will be treated as an error.
}								 

	
jint Java_com_nextreaming_gethttpcredentialsample_GetHttpCredentialManager_initManager (JNIEnv * env, 
														   	   jobject clazz,
														   	   jstring libName)
{
    LOGI("[GetHttpCredentialManager initManager] Start \n");
    
    void *handle = NULL;

    int (*fptr)(NEXPLAYERGetCredentialCallbackFunc pGetKeyExtFunc, void *pUserData); 
    
    char buf[512];
    const jbyte *str;
    str = (*env)->GetStringUTFChars(env, libName, NULL);
    if(str != NULL)
	{
		handle = dlopen(str, RTLD_LAZY);  
		
		LOGI("[initDRMManager] libName[0x%X]:%s",handle, str);
	}
    else
	{
	    /* Load Default NexPlayerEngine library */
	    handle = dlopen(NEXPLAYERENGINE_LIB, RTLD_LAZY);
	}

	
    LOGI("initializeAgent : nextreaming handle=%p", handle);
    if (handle == NULL)
    {
        LOGI("[initDRMManager] error=%s", dlerror());
        return -1;
    }
   
    /* Get DRM register function pointer*/   
    fptr = (int (*)(NEXPLAYERGetCredentialCallbackFunc pGetKeyExtFunc, void *pUserData))dlsym(handle, NEXPLAYERENGINE_GETCREDENTIAL_CALLBACK_FUNC);
    
    LOGI("[initManager] fptr = %p", fptr);
    if (fptr == NULL)
    {
        LOGI("[initManager] error=%s", dlerror());
    }
    
    /* Register DRM descramble function */
    if (fptr != NULL)
        (*fptr)(GetCredentialCallbackFunc, NULL);

    dlclose(handle); 
   
    return 0;
}
