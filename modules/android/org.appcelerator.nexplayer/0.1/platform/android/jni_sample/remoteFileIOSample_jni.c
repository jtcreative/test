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
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NEXPLAYERENGINE_LIB "/data/data/com.nextreaming.nexplayersample/lib/libnexplayerengine.so"
#define	NEXPLAYERENGINE_REGISTER_REMOTE_FILE_IO_FUNC  "nexPlayerSWP_RegisterRemoteFileIOInterface"

#define  LOG_TAG    "REMOTE_FILE_IO_SAMPLE"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGV(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)


NEXFileHandle nexRemoteFile_OpenFt ( char* pFileName, NEXFileMode iMode, void *pUserData )
{
	NEXFileHandle hRetFile;
	int flags = 0;
	
	LOGI("nexRemoteFile_OpenFt" );

	if ( iMode & NEX_FILE_CREATE )
	{
		flags |= O_CREAT;
	}

	if ( ( iMode & NEX_FILE_READ ) && !(iMode & NEX_FILE_WRITE) )
		flags |= O_RDONLY;
	else if ( !( iMode & NEX_FILE_READ ) && (iMode & NEX_FILE_WRITE) )
		flags |= O_WRONLY;
	else
		flags |= O_RDWR;
		
	hRetFile = (NEXFileHandle)open( pFileName, flags );
	
	LOGI( "File handle is 0x%0x.", hRetFile );

	return hRetFile;
}

int nexRemoteFile_CloseFt ( NEXFileHandle hFile, void *pUserData )
{
	LOGI("in Remote File Close, %d", hFile );
	return close( hFile );
}

int nexRemoteFile_ReadFt ( NEXFileHandle hFile, void *pBuf, unsigned int uiSize, unsigned int uiCount, void *pUserData )
{
	int nRead = read( hFile, pBuf, uiSize*uiCount );
	LOGI("in RemoteFile Read, %d, %d", uiSize*uiCount, nRead );
	return nRead;	
}

int nexRemoteFile_SeekFt ( NEXFileHandle hFile, int iOffset, NEXFileSeekOrigin iOrigin, void *pUserData )
{
	int origin;

	switch ( iOrigin )
	{
	case NEX_SEEK_BEGIN : origin = SEEK_SET; break;
	case NEX_SEEK_CUR : origin = SEEK_CUR; break;
	case NEX_SEEK_END : origin = SEEK_END; break;
	}
	
	int iRet = lseek( hFile, iOffset, origin );
	LOGI("in RemoteFile Seek, %d, %d, %d, %d", hFile, iOffset, origin, iRet );
	return iRet;	
	
}

long long nexRemoteFile_Seek64Ft ( NEXFileHandle hFile, long long iOffset, NEXFileSeekOrigin iOrigin, void *pUserData )
{
	int origin;

	switch ( iOrigin )
	{
	case NEX_SEEK_BEGIN : origin = SEEK_SET; break;
	case NEX_SEEK_CUR : origin = SEEK_CUR; break;
	case NEX_SEEK_END : origin = SEEK_END; break;
	}
	
	long long iRet = (long long)lseek( hFile, (int)iOffset, origin );
	LOGI("in RemoteFile Seek64, %d, %d, %d, %d", hFile, iOffset, origin, iRet );
	return iRet;
}

long long nexRemoteFile_SizeFt ( NEXFileHandle hFile, void *pUserData )
{
	int nCurrentPos;
	int nSize;

	nCurrentPos = lseek(hFile, 0, SEEK_CUR);
	nSize = lseek(hFile, 0, SEEK_END);
	lseek(hFile, nCurrentPos, SEEK_SET);

	LOGI("in RemoteFile Size, %d, %d, %d, %d", hFile, nSize );
	
	return nSize;
}

jint Java_com_nextreaming_remoteFileIO_remoteFileIOManager_registerRemoteFileIO (  JNIEnv * env, 
																			   	   jobject clazz,
																			   	   jstring libName)
{
    LOGI("[registerRemoteFileIO] Start \n");
    
    void *handle = NULL;
    int (*fptr)(NEXPLAYERRemoteFileIOInterface *pRemoteFileIOInterface, void *pUserData); 
    
    char buf[256];
    const jbyte *str;
    str = (*env)->GetStringUTFChars(env, libName, NULL);
    if(str != NULL)
	{
		handle = dlopen(str, RTLD_LAZY);
		
		LOGI("[registerRemoteFileIO] libName[0x%X]:%s",handle, buf);
	}
    else
    {
	    /* Load Default NexPlayerEngine library */
	    handle = dlopen(NEXPLAYERENGINE_LIB, RTLD_LAZY);
	}
	
    LOGI("initializeAgent : nextreaming handle=%p", handle);
    if (handle == NULL)
    {
        LOGI("[registerRemoteFileIO] error=%s", dlerror());
        return -1;
    }
    
	/* Get remote file IO interface register function pointer*/
    fptr = (int (*)(NEXPLAYERRemoteFileIOInterface *pRemoteFileIOInterface, void *pUserData))dlsym(handle, NEXPLAYERENGINE_REGISTER_REMOTE_FILE_IO_FUNC);

    LOGI("[registerRemoteFileIO] fptr = %p", fptr);
    if (fptr == NULL)
    {
        LOGI("[registerRemoteFileIO] error=%s", dlerror());
    }
    
    /* Register remote file IO inferface function */
    if (fptr != NULL)
    {
    	void * pUserData = NULL;
    	NEXPLAYERRemoteFileIOInterface stFileIO;
    	
    	stFileIO.Open 	= nexRemoteFile_OpenFt;
    	stFileIO.Close 	= nexRemoteFile_CloseFt;
    	stFileIO.Seek 	= nexRemoteFile_SeekFt;
    	stFileIO.Seek64 = nexRemoteFile_Seek64Ft;
    	stFileIO.Read 	= nexRemoteFile_ReadFt;
		stFileIO.Write 	= NULL;
		stFileIO.Size 	= nexRemoteFile_SizeFt;    	
    	
        (*fptr)(&stFileIO, pUserData);
	}
    dlclose(handle); 
 
   
    return 0;
}