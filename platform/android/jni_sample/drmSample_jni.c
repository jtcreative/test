/**
   File : drmSample_jni.c

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


#define		_SUPPORT_WMDRM_

#define NEXPLAYERENGINE_LIB "/data/data/com.nextreaming.nexplayersample/lib/libnexplayerengine.so"
#define NEXPLAYERENGINE_DESCRAMBLE_CALLBACK_FUNC "nexPlayerSWP_RegisterDRMDescrambleCallBackFunc"
#define	NEXPLAYERENGINE_WMDRMDESCRAMBLE_CALLBACK_FUNC  "nexPlayerSWP_RegisterWMDRMDescrambleCallBackFunc"

#define  LOG_TAG    "DRM_SAMPLE"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGV(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)


static int drmDescrambleCallbackFunc( 	unsigned int	uiType,				// 0:Video, 1:Audio
										 unsigned char*	pInputBuffer,  
										 unsigned int	uiInputBufferSize, 
										 unsigned char*	pOutputBuffer, 
										 unsigned int*	puiOutputBufferSize,
										 unsigned char* pIVBuffer,
										 unsigned long dwIVBufferSize, 
										 void *			pUserData)
{
	LOGI("[	drmDescrambleCallbackFunc ] type:%d, inputBuffer[%d]:0x%x, outputBuffer;0x%X, UserData:0x%X \n",
				uiType, uiInputBufferSize, pInputBuffer, pOutputBuffer, pUserData );
				
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

static int wmdrmDescrambleCallbackFunc(  unsigned char*	pInputBuffer,  
										 unsigned int	uiInputBufferSize, 
										 unsigned char*	pOutputBuffer, 
										 unsigned int*	puiOutputBufferSize,
										 void *			pUserData)
{
	LOGI("[	wmdrmDescrambleCallbackFunc ] inputBuffer[%d]:0x%x, outputBuffer;0x%X, UserData:0x%X \n",
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

jint Java_com_nextreaming_drmsample_DRMManager_initDRMManager (JNIEnv * env, 
														   	   jobject clazz,
														   	   jstring libName)
{
    LOGI("[initDRMManager] Start \n");
    
    void *handle = NULL;

#ifndef _SUPPORT_WMDRM_    
    int (*fptr)(NEXPLAYERDRMDescrambleCallbackFunc pDescrambleCallbackFunc, void *pUserData); 
#else
	int (*fptr)(unsigned int uiDRMType, NEXPLAYERDRMDescrambleCallbackFunc pDescrambleCallbackFunc, void *pUserData); 
#endif	// _SUPPORT_WMDRM_
    
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
    
#ifndef _SUPPORT_WMDRM_
    /* Get DRM register function pointer*/
    
    fptr = (int (*)(NEXPLAYERDRMDescrambleCallbackFunc pDescrambleCallbackFunc, void *pUserData))dlsym(handle, NEXPLAYERENGINE_DESCRAMBLE_CALLBACK_FUNC);
    
    LOGI("[initDRMManager] fptr = %p", fptr);
    if (fptr == NULL)
    {
        LOGI("[initDRMManager] error=%s", dlerror());
    }
    
    /* Register DRM descramble function */
    if (fptr != NULL)
        (*fptr)(drmDescrambleCallbackFunc, NULL);		

    dlclose(handle); 
#else	// _SUPPORT_WMDRM_
	/* Get WMDRM register function pointer*/
    fptr = (int (*)(unsigned int uiDRMType, NEXPLAYERWMDRMDescrambleCallbackFunc pDescrambleCallbackFunc, void *pUserData))dlsym(handle, NEXPLAYERENGINE_WMDRMDESCRAMBLE_CALLBACK_FUNC);

    LOGI("[initDRMManager] fptr = %p", fptr);
    if (fptr == NULL)
    {
        LOGI("[initDRMManager] error=%s", dlerror());
    }
    
    /* Register WMDRM descramble function */
    if (fptr != NULL)
        (*fptr)( /*NXWMDRM_PAYLOAD_TYPE */ NXWMDRM_PACKET_TYPE, wmdrmDescrambleCallbackFunc, 100);

    dlclose(handle); 
#endif	// _SUPPORT_WMDRM_    
   
    return 0;
}