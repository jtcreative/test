/**
   File : HLSTsDescrambleSample_jni.c

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
#define NEXPLAYERENGINE_HLSTS_DESCRAMBLE_CALLBACK_FUNC "nexPlayerSWP_RegisterHLSTSDescrambleCallBackFunc"
                                                        

#define  LOG_TAG    "HLSTS_DRM_SAMPLE"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGV(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)


static unsigned long HLSTSDescrambleCallbackFunc(unsigned char*	pInputBuffer,  
												 unsigned int	uiInputBufferSize, 
												 unsigned char*	pOutputBuffer, 
												 unsigned int*	puiOutputBufferSize,
												 void *			pUserData)
{
	LOGI("[	HLSTSDescrambleCallbackFunc ] inputBuffer[%d]:0x%x, outputBuffer;0x%X, UserData:0x%X \n",
				uiInputBufferSize, pInputBuffer, pOutputBuffer, pUserData );
				
	if(pInputBuffer == pOutputBuffer)
	{
		*puiOutputBufferSize = uiInputBufferSize;		
	}
	else
	{
		*puiOutputBufferSize = uiInputBufferSize;
		memcpy(pOutputBuffer, pInputBuffer, uiInputBufferSize);	
	}	
				
	return 0;	
}								 

	

jint Java_com_nextreaming_hlstsdescramblesample_HLSTsDRMManager_initDRMManager (JNIEnv * env, 
														   	   jobject clazz,
														   	   jstring libName)
{
    LOGI("[initDRMManager] Start \n");
    
    void *handle = NULL;

    int (*fptr)(NEXPLAYERHLSTSDescrambleCallbackFunc pDescrambleCallbackFunc, void *pUserData); 
    
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
    fptr = (int (*)(NEXPLAYERHLSTSDescrambleCallbackFunc pDescrambleCallbackFunc, void *pUserData))dlsym(handle, NEXPLAYERENGINE_HLSTS_DESCRAMBLE_CALLBACK_FUNC);
    
    LOGI("[initDRMManager] fptr = %p", fptr);
    if (fptr == NULL)
    {
        LOGI("[initDRMManager] error=%s", dlerror());
    }
    
    /* Register DRM descramble function */
    if (fptr != NULL)
        (*fptr)(HLSTSDescrambleCallbackFunc, NULL);		

    dlclose(handle); 
   
    return 0;
}