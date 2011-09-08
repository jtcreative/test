/**
   File : SmoothStreamPlayReadyDescrambleSample_jni.c

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
#define NEXPLAYERENGINE_PIFFPLAYREADY_DESCRAMBLE_CALLBACK_FUNC "nexPlayerSWP_RegisterPiffPlayReadyDescrambleCallBackFunc"
                                                        

#define  LOG_TAG    "PIFF_DRM_SAMPLE"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGV(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)


static unsigned long PiffPlayReadyDescrambleCallbackFunc(
											unsigned char* 	pInputBuffer,
											unsigned long		dwInputBufferSize, 
											unsigned char*	pOutputBuffer,
											unsigned long*	pdwOutputBufferSize,
											unsigned char*	pSampleEncBox,
											unsigned long		dwSampleEncBoxLen,
											unsigned long		dwSampleIDX,
											unsigned long		dwTrackID,
											void*			pUserData)
{
	LOGI("[	PiffPlayReadyDescrambleCallbackFunc ] inputBuffer:0x%x(%d), outputBuffer:0x%X, pSampleEncBox:0x%X(%d), SampleIDX(%d), TrackID(%d), UserData:0x%X \n",
				pInputBuffer, dwInputBufferSize, pOutputBuffer, 
				pSampleEncBox, dwSampleEncBoxLen, dwSampleIDX, 
				dwTrackID, pUserData );
				
	if(pInputBuffer == pOutputBuffer)
	{
		*pdwOutputBufferSize = dwInputBufferSize;		
	}
	else
	{
		*pdwOutputBufferSize = dwInputBufferSize;
		memcpy(pOutputBuffer, pInputBuffer, dwInputBufferSize);	
	}	
				
	return 0;	
}								 

	

jint Java_com_nextreaming_piffplayreadydescramblesample_PiffPlayReadyDRMManager_initDRMManager (JNIEnv * env, 
														   	   jobject clazz,
														   	   jstring libName)
{
    LOGI("[initDRMManager] Start \n");
    
    void *handle = NULL;

    int (*fptr)(NEXPLAYERPiffPlayReadyDescrambleCallbackFunc pDescrambleCallbackFunc, void *pUserData); 
    
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
    fptr = (int (*)(NEXPLAYERPiffPlayReadyDescrambleCallbackFunc pDescrambleCallbackFunc, void *pUserData))dlsym(handle, NEXPLAYERENGINE_PIFFPLAYREADY_DESCRAMBLE_CALLBACK_FUNC);
    
    LOGI("[initDRMManager] fptr = %p", fptr);
    if (fptr == NULL)
    {
        LOGI("[initDRMManager] error=%s", dlerror());
    }
    
    /* Register DRM descramble function */
    if (fptr != NULL)
        (*fptr)(PiffPlayReadyDescrambleCallbackFunc, NULL);		

    dlclose(handle); 
   
    return 0;
}