/** \file nexplayer_jni.h
 *  \brief Main header file for the NexPlayer&trade;&nbsp;DRM API on Android
 */

#ifndef _NEXPLAYER_JNI_H_
#define	_NEXPLAYER_JNI_H_

/**
 * \mainpage NexPlayer&trade;&nbsp;DRM&nbsp;API&nbsp;for&nbsp;Android
 *
 * \section legal Legal Notices
 *
 * \par Disclaimer for Intellectual property
 * <i>This product is designed for general purpose, and accordingly customer is 
 * responsible for all or any of intellectual property licenses required for 
 * actual application. Nextreaming Corporation does not provide any 
 * indemnification for any intellectual properties owned by third party.</i>
 * 
 * \par Copyright
 * Copyright for all documents, drawings and programs related with this 
 * specification are owned by Nextreaming Corporation. All or any part of the 
 * specification shall not be reproduced nor distributed without prior written 
 * approval by Nextreaming Corporation. Content and configuration of all or any
 * part of the specification shall not be modified nor distributed without prior 
 * written approval by Nextreaming Corporation.
 * 
 * \par
 * &copy; Copyright 2010-2011 Nextreaming Corporation. All rights reserved.
 *
 * \section abstract Abstract
 *
 * This document describes the NexPlayer&trade;&nbsp;SDK for Android <em>DRM API</em>.
 * 
 * This is a JNI (Java Native Interface) API that allows applications to perform
 * DRM descrambling.  The main Java API for the NexPlayer&trade;&nbsp; engine is
 * documented separately.
 *
 * \section overview DRM Descrambling Overview
 *
 * NexPlayer&trade;&nbsp; supports DRM descrambling by allowing the application to
 * register one or more callback functions.  Each callback receives pointers to
 * input and output buffers; it descrambles the data in the input buffer and
 * places the descrambled data in the output buffer.
 *
 * Different callbacks can be registered to allow access to data at different
 * points in the decoding process.  The exact point at which the descrambling must
 * occur differs between the different DRM schemes.  In addition, some callbacks
 * provide additional parameters necessary for a given type of DRM.  See the
 * individual function descriptions for more information.
 *
 * The normal flow is as follows:
 * -# Your application (written in Java) calls your JNI library
 * -# Your JNI library in turn calls the appropriate NexPlayer&trade;&nbsp;API 
 *    functions to register the necessary callbacks.
 * -# Your application calls the NexPlayer&trade;&nbsp;API normally to play
 *    content; when necessary, the player will automatically call the registered
 *    callbacks to descramble the content.
 *
 * <b>CAUTION:</b> In many cases, the input and output buffer pointers point
 * to the same location.  Your code should be able to handle cases where they
 * point to the same location, and cases when they are different.  For example
 * a typical no-op descrambling function that just outputs what it is given
 * as input might be written as follows:
 *
 * \code
 * static int wmdrmDescrambleCallbackFunc(unsigned char*    pInputBuffer,  
 *                                        unsigned int      uiInputBufferSize, 
 *                                        unsigned char*	pOutputBuffer, 
 *                                        unsigned int*     puiOutputBufferSize,
 *                                        void *            pUserData)
 * {
 * 	if(pInputBuffer == pOutputBuffer) {
 * 		*puiOutputBufferSize = uiInputBufferSize;		
 * 	} else {
 * 		*puiOutputBufferSize = uiInputBufferSize;
 * 		memcpy(pOutputBuffer, pInputBuffer, uiInputBufferSize);	
 * 	}	
 * 	return 0;	
 * }	
 *
 * \endcode
 *
 * \see ::nexPlayerSWP_RegisterDRMDescrambleCallBackFunc for general DRM
 * \see ::nexPlayerSWP_RegisterWMDRMDescrambleCallBackFunc for WM-DRM
 * \see ::nexPlayerSWP_RegisterHLSTSDescrambleCallBackFunc for HLS/TS DRM
 * \see ::nexPlayerSWP_RegisterSmoothStreamFragmentDescrambleCallBackFunc for Smooth Streaming fragment-based DRM
 * \see ::nexPlayerSWP_RegisterSmoothStreamPlayReadyDescrambleCallBackFunc for Smooth Streaming PlayReady DRM
 * \see ::nexPlayerSWP_RegisterPiffPlayReadyDescrambleCallBackFunc for PIFF PlayReady DRM
 * \see ::nexPlayerSWP_RegisterAsfPlayReadyDescrambleCallBackFunc for ASF PlayReady DRM
 *
 * \section rfio Remote File I/O
 *
 * NexPlayer&trade;&nbsp;also provides remote file I/O callbacks that allows
 * an applicaiton to provide custom open, close, read and write implementations.
 * This allows an application to retrieve the file data from another source, or
 * to perform DRM descrambling on the data as it is read.
 *
 * \see ::nexPlayerSWP_RegisterRemoteFileIOInterface for details
 * 
 * \section include Header File
 *
 * The main header file for the NexPlayer&trade;&nbsp;DRM API on Android is
 * <i>nexplayer_jni.h</i>, which you should include anywhere you need to
 * call the callback registration functions:
 *
 * \code
 * #include "nexplayer_jni.h"
 * \endcode
 *
 * \section deckey HLS Decryption Key Support
 *
 * For HTTP Live Streaming (HLS) encrypted data, an AES-128 key is required for 
 * decryption.  This file is normally referenced from the HLS index file, and
 * NexPlayer&trade;&nbsp;will download it automatically and perform decryption.  
 * 
 * However, there may be cases where the application needs to handle key 
 * retreival, such as if authorization is required to download the key.  In this
 * case, the application can register a callback function, which will be called
 * whenever it is necessary to retrieve a key.
 *
 * \see ::nexPlayerSWP_RegisterGetKeyExtCallBackFunc for details
 *
 * \section dllsec Dynamic Library Security
 *
 * NexPlayer&trade;&nbsp;uses a series of dynamic libraries, in order to allow unnecessary components to be
 * removed from an application, thus reducing the size of the application.  
 *
 * Because these libraries are loaded dynamically, a user may attempt to gain access to decrypted data
 * by replacing these dynamic libraries.
 *
 * In order to prevent tampering, applications that support DRM should verify that the libraries being 
 * loaded are the correct libraries that were distributed with the application.
 *
 * In order to accomplish this, application can register callback functions which are used in place of the
 * normal system calls to load the dynamic libraries.  This allows the application to verify that the
 * correct libraries are present before calling through to the system to perform the dynamic load.
 *
 * These callbacks are registered using ::nexPlayerSWP_RegisterDLAPICallbackFunc.
 *
 * \see ::nexPlayerSWP_RegisterDLAPICallbackFunc for details.
 *
 */

#ifdef __cplusplus
extern "C"
{
#endif
    
    /** \defgroup apifunc API Functions
     *
     * NexPlayer&trade;&nbsp;API functions
     *
     */
    
    /** \defgroup types Types 
     * 
     * Types that are part of the NexPlayer&trade;&nbsp;API.
     */
    
    /** \defgroup cbtypes Callback Types 
     * 
     * Callback function typedefs that are part of the NexPlayer&trade;&nbsp;API.
     */
    
    /** \defgroup ppmacro Preprocessor Constants and Macros
     *
     * Preprocessor constants and macros that are part of the NexPlayer&trade;&nbsp;API.
     */
    
    /**
     * \ingroup cbtypes
     * \brief   General callback function for descrambling  DRM encrypted content.
     *
     * This callback can be registered by calling
     * ::nexPlayerSWP_RegisterDRMDescrambleCallBackFunc.
     *
     * When registered, this callback function will be called for each frame of
     * audio or video data before that frame is decoded.  This provides a general
     * opportunity for descrambling the frame.  For types of DRM where descrambling
     * must occur at a different point in the playback process or where additional
     * information is needed, more specific functions are provided and should be
     * used instead of this one where necessary.
     *
     * \param[in]  uiType               The type of frame to be descrambled:
     *                                      - <b>0:</b> Video
     *                                      - <b>1:</b> Audio
     *
     * \param[in]  pInputBuffer         The encrypted data to be descrambled
     *
     * \param[in]  uiInputBufferSize    The size of the encrypted data, in bytes
     *
     * \param[out] pOutputBuffer        The location at which to place the descrambled 
     *                                  output data.  This may point to the same
     *                                  location as the input buffer, or it may point
     *                                  to a separate location. The size available for
     *                                  the output buffer is the same as the size of
     *                                  the input buffer.  That is, the decrypted data
     *                                  may be smaller than the encrypted data, but
     *                                  not larger.
     *
     * \param[out] puiOutputBufferSize  The size of the decrypted data.  The callback
     *                                  must set this value.  This may be equal to or
     *                                  smaller than \c uiInputBufferSize, but not larger.
     *
     * \param[in]  pUserData            The user data passed when the callback was
     *                                  originally registered.
     * 
     * \returns                         The callback should return zero if the data
     *                                  was successfully descrambled.  In the case of
     *                                  an error, it should return -1.
     */
    
    typedef int ( *NEXPLAYERDRMDescrambleCallbackFunc ) (unsigned int	uiType,
                                                         unsigned char*	pInputBuffer,  
                                                         unsigned int	uiInputBufferSize, 
                                                         unsigned char*	pOutputBuffer, 
                                                         unsigned int*	puiOutputBufferSize,
                                                         void *			pUserData);
    
    /**
     * \ingroup cbtypes
     * \brief   Callback function for descrambling WM-DRM encrypted content.
     *
     * This callback can be registered by calling
     * ::nexPlayerSWP_RegisterWMDRMDescrambleCallBackFunc.
     *
     * \param[in]  pInputBuffer         The encrypted data to be descrambled
     *
     * \param[in]  uiInputBufferSize    The size of the encrypted data, in bytes
     *
     * \param[out] pOutputBuffer        The location at which to place the descrambled 
     *                                  output data.  This may point to the same
     *                                  location as the input buffer, or it may point
     *                                  to a separate location. The size available for
     *                                  the output buffer is the same as the size of
     *                                  the input buffer.  That is, the decrypted data
     *                                  may be smaller than the encrypted data, but
     *                                  not larger.
     *
     * \param[out] puiOutputBufferSize  The size of the decrypted data.  The callback
     *                                  must set this value.  This may be equal to or
     *                                  smaller than \c uiInputBufferSize, but not larger.
     *
     * \param[in]  pUserData            The user data passed when the callback was
     *                                  originally registered.
     * 
     * \returns                         The callback should return zero if the data
     *                                  was successfully descrambled.  In the case of
     *                                  an error, it should return -1. 
     */
    typedef int ( *NEXPLAYERWMDRMDescrambleCallbackFunc ) (unsigned char*	pInputBuffer,  
                                                           unsigned int		uiInputBufferSize, 
                                                           unsigned char*	pOutputBuffer, 
                                                           unsigned int*	puiOutputBufferSize,
                                                           unsigned char* 	pIVBuffer,
                                                           unsigned long	dwIVBufferSize,
                                                           void *			pUserData);
    
    /**
     * \ingroup cbtypes
     * \brief   Callback function for descrambling HLS-TS encrypted content (HTTP
     *          Live Streaming content encrypted at the segment level).
     *
     * This callback can be registered by calling
     * ::nexPlayerSWP_RegisterHLSTSDescrambleCallBackFunc.
     *
     * This callback is called every time an HLS segment is received.  The segment
     * may be either a TS file or an audio file.  The player does not attempt to
     * detect whether a segment is encrypted, but rather passes all segments 
     * directly to the callback, if one is registered.
     *
     * \param[in]  pInputBuffer         The segment (TS file or audio file) that
     *                                  has been received.
     *
     * \param[in]  uiInputBufferSize    The size of the data at pInputBuffer, in bytes
     *
     * \param[out] pOutputBuffer        The location at which to place the descrambled 
     *                                  output data.  This may point to the same
     *                                  location as the input buffer, or it may point
     *                                  to a separate location. The size available for
     *                                  the output buffer is the same as the size of
     *                                  the input buffer.  That is, the decrypted data
     *                                  may be smaller than the encrypted data, but
     *                                  not larger.
     *
     * \param[out] puiOutputBufferSize  The size of the decrypted data.  The callback
     *                                  must set this value.  This may be equal to or
     *                                  smaller than \c uiInputBufferSize, but not larger.
     *
     * \param[in]  pUserData            The user data passed when the callback was
     *                                  originally registered.
     * 
     * \returns                         The callback should return zero if the data
     *                                  was successfully descrambled.  In the case of
     *                                  an error, it should return -1. 
     */
    typedef unsigned long ( *NEXPLAYERHLSTSDescrambleCallbackFunc ) (unsigned char*	pInputBuffer,  
                                                                     unsigned int	uiInputBufferSize, 
                                                                     unsigned char*	pOutputBuffer, 
                                                                     unsigned int*	puiOutputBufferSize,
                                                                     void *			pUserData);
    
    
    /**
     * \ingroup cbtypes
     * \brief   Callback function for descrambling Smooth Streaming fragments.
     *
     * This callback can be registered by calling
     * ::nexPlayerSWP_RegisterSmoothStreamFragmentDescrambleCallBackFunc.
     *
     * When registered, this callback function is called every time a Smooth
     * Streaming fragment is received, regardless of whether or not the
     * fragment is encrypted.  The callback should perform any necessary 
     * descrambling on the fragment, and return the descrambled fragment.
     *
     * \param[in]  pInputBuffer         The fragment that has just been received.
     *
     * \param[in]  uiInputBufferSize    The size of the fragment, in bytes
     *
     * \param[out] pOutputBuffer        The location at which to place the descrambled 
     *                                  output data.  This may point to the same
     *                                  location as the input buffer, or it may point
     *                                  to a separate location. The size available for
     *                                  the output buffer is the same as the size of
     *                                  the input buffer.  That is, the decrypted data
     *                                  may be smaller than the encrypted data, but
     *                                  not larger.
     *
     * \param[out] puiOutputBufferSize  The size of the decrypted data.  The callback
     *                                  must set this value.  This may be equal to or
     *                                  smaller than \c uiInputBufferSize, but not larger.
     *
     * \param[in]  pUserData            The user data passed when the callback was
     *                                  originally registered.
     * 
     * \returns                         The callback should return zero if the data
     *                                  was successfully descrambled.  In the case of
     *                                  an error, it should return -1. 
     */
    typedef unsigned long ( *NEXPLAYERSmoothStreamFragmentDescrambleCallbackFunc ) (unsigned char*	pInputBuffer,  
                                                                                    unsigned int	uiInputBufferSize, 
                                                                                    unsigned char*	pOutputBuffer, 
                                                                                    unsigned int*	puiOutputBufferSize,
                                                                                    void *			pUserData);
    
    
    /**
     * \ingroup cbtypes
     * \brief   Callback function for descrambling Smooth Streaming PlayReady encrypted content.
     *
     * This callback can be registered by calling
     * ::nexPlayerSWP_RegisterSmoothStreamPlayReadyDescrambleCallBackFunc.
     *
     * \param[in]  pInputBuffer         The encrypted data to be descrambled
     *
     * \param[in]  dwInputBufferSize    The size of the encrypted data, in bytes
     *
     * \param[out] pOutputBuffer        The location at which to place the descrambled 
     *                                  output data.  This may point to the same
     *                                  location as the input buffer, or it may point
     *                                  to a separate location. The size available for
     *                                  the output buffer is the same as the size of
     *                                  the input buffer.  That is, the decrypted data
     *                                  may be smaller than the encrypted data, but
     *                                  not larger.
     *
     * \param[out] pdwOutputBufferSize  The size of the decrypted data.  The callback
     *                                  must set this value.  This may be equal to or
     *                                  smaller than \c uiInputBufferSize, but not larger.
     *
     * \param[in]  pSampleEncBox        The \c SampleEncryptionBox, as detailed in the
     *                                  <em>[MS-SSTR] Smooth Streaming Protocol Specification</em>.
     *
     * \param[in]  dwSampleEncBoxLen    The length, in bytes, of the data at \c pSampleEncBox.
     *
     * \param[in]  dwSampleIDX          The index of the media object (frame or sample, depending
     *                                  on media format) being descrambled.
     *
     * \param[in]  dwTrackID            Media Track ID, from \c TfhdBox, as defined in the
     *                                  <em>[MS-SSTR] Smooth Streaming Protocol Specification</em>.
     *
     * \param[in]  pUserData            The user data passed when the callback was
     *                                  originally registered.
     * 
     * \returns                         The callback should return zero if the data
     *                                  was successfully descrambled.  In the case of
     *                                  an error, it should return -1. 
     */
    typedef int ( *NEXPLAYERSmoothStreamPlayReadyDescrambleCallbackFunc ) ( 	 
                                                                           unsigned char*   pInputBuffer,
                                                                           unsigned long	dwInputBufferSize, 
                                                                           unsigned char*	pOutputBuffer,
                                                                           unsigned long*	pdwOutputBufferSize,
                                                                           unsigned char*	pSampleEncBox,
                                                                           unsigned long	dwSampleEncBoxLen,
                                                                           unsigned long	dwSampleIDX,
                                                                           unsigned long	dwTrackID,
                                                                           void*			pUserData);
    
    /**
     * \ingroup cbtypes
     * \brief   Callback function for descrambling PlayReady encrypted content in a PIFF file.
     *
     * This callback can be registered by calling
     * ::nexPlayerSWP_RegisterPiffPlayReadyDescrambleCallBackFunc.
     *
     * \param[in]  pInputBuffer         The encrypted data to be descrambled
     *
     * \param[in]  dwInputBufferSize    The size of the encrypted data, in bytes
     *
     * \param[out] pOutputBuffer        The location at which to place the descrambled 
     *                                  output data.  This may point to the same
     *                                  location as the input buffer, or it may point
     *                                  to a separate location. The size available for
     *                                  the output buffer is the same as the size of
     *                                  the input buffer.  That is, the decrypted data
     *                                  may be smaller than the encrypted data, but
     *                                  not larger.
     *
     * \param[out] pdwOutputBufferSize  The size of the decrypted data.  The callback
     *                                  must set this value.  This may be equal to or
     *                                  smaller than \c uiInputBufferSize, but not larger.
     *
     * \param[in]  pSampleEncBox        The \c SampleEncryptionBox, as detailed in the
     *                                  <em>[MS-SSTR] Smooth Streaming Protocol Specification</em>.
     *
     * \param[in]  dwSampleEncBoxLen    The length, in bytes, of the data at \c pSampleEncBox.
     *
     * \param[in]  dwSampleIDX          The index of the media object (frame or sample, depending
     *                                  on media format) being descrambled.
     *
     * \param[in]  dwTrackID            Media Track ID, from \c TfhdBox, as defined in the
     *                                  <em>[MS-SSTR] Smooth Streaming Protocol Specification</em>.
     *
     * \param[in]  pUserData            The user data passed when the callback was
     *                                  originally registered.
     * 
     * \returns                         The callback should return zero if the data
     *                                  was successfully descrambled.  In the case of
     *                                  an error, it should return -1. 
     */
    typedef int ( *NEXPLAYERPiffPlayReadyDescrambleCallbackFunc ) ( 	 
                                                                   unsigned char*   pInputBuffer,
                                                                   unsigned long	dwInputBufferSize, 
                                                                   unsigned char*	pOutputBuffer,
                                                                   unsigned long*	pdwOutputBufferSize,
                                                                   unsigned char*	pSampleEncBox,
                                                                   unsigned long	dwSampleEncBoxLen,
                                                                   unsigned long	dwSampleIDX,
                                                                   unsigned long	dwTrackID,
                                                                   void*			pUserData);
    
    
    /**
     * \ingroup cbtypes
     * \brief   Callback function for descrambling PlayReady encrypted content in an ASF file.
     *
     * This callback can be registered by calling
     * ::nexPlayerSWP_RegisterAsfPlayReadyDescrambleCallBackFunc.
     *
     * \param[in]  pInBuf               The encrypted data to be descrambled
     *
     * \param[in]  dwInBufSize          The size of the encrypted data, in bytes
     *
     * \param[out] pOutBuf              The location at which to place the descrambled 
     *                                  output data.  This may point to the same
     *                                  location as the input buffer, or it may point
     *                                  to a separate location. The size available for
     *                                  the output buffer is the same as the size of
     *                                  the input buffer.  That is, the decrypted data
     *                                  may be smaller than the encrypted data, but
     *                                  not larger.
     *
     * \param[out] pdwOutSize           The size of the decrypted data.  The callback
     *                                  must set this value.  This may be equal to or
     *                                  smaller than \c uiInputBufferSize, but not larger.
     *
     * \param[in]  pIVBuf               Initialization vector.
     *
     * \param[in]  dwIVBufSize          Size (in bytes) of the initialization vector.
     *
     * \param[in]  pUserData            The user data passed when the callback was
     *                                  originally registered.
     * 
     * \returns                         The callback should return zero if the data
     *                                  was successfully descrambled.  In the case of
     *                                  an error, it should return -1. 
     */
    typedef int (*NEXPLAYERAsfPlayReadyDescrambleCallbackFunc)(unsigned char*	pInBuf,
                                                               unsigned long	dwInBufSize, 
                                                               unsigned char*	pOutBuf,
                                                               unsigned long*	pdwOutSize,
                                                               unsigned char*	pIVBuf,
                                                               unsigned long	dwIVBufSize,
                                                               void*			pUserData);		
    
    /**
     * \ingroup cbtypes
     * \brief   Callback function for retrieving an HTTP Live Streaming decryption key.
     *
     * When this callback is registered, it is called instead of NexPlayer&trade;'s internal
     * key retrieval function.  This function must retrieve the key from the given
     * URL and return the KEY to NexPlayer.
     *
     * This callback can be registered by calling
     * ::nexPlayerSWP_RegisterGetKeyExtCallBackFunc.
     *
     * \param[in]  pKeyUrl          The key URL from the HLS index file.
     * 
     * \param[in]  dwKeyUrlLen      The length of \c pKeyUrl in bytes
     *
     * \param[out] pKeyBuf          Buffer to receive the key
     *
     * \param[in]  dwKeyBufSize     Size of the buffer (maximum size of key that
     *                              can be received).
     *
     * \param[out] pdwKeySize       Actual size of key (must be equal to or less
     *                              than \c dwKeyBufSize).
     *
     * \param[in]  pUserData        The user data passed when the callback was
     *                              originally registered.
     *
     * \returns                     The callback should return zero if the data
     *                              was successfully descrambled.  In the case of
     *                              an error, it should return -1. 
     *
     * \see \ref deckey
     * \see ::nexPlayerSWP_RegisterGetKeyExtCallBackFunc
     */
    typedef int (*NEXPLAYERGetKeyExtCallbackFunc)(char*             pKeyUrl,
                                                  unsigned long		dwKeyUrlLen,
                                                  unsigned char*	pKeyBuf,
                                                  unsigned long		dwKeyBufSize,
                                                  unsigned long*	pdwKeySize,
                                                  unsigned long 	pUserData);


    /**
     * \ingroup cbtypes
     * \brief Callback function for dynamically providing credentials for
     *          retrying after an HTTP 401 response.
     *
     * This callback can be registered by calling
     * ::nexPlayerSWP_RegisterGetCredentialCallbackFunc.
     *
     * When this callback is registered, it is called whenever an HTTP 401
     * response is received during streaming play.  The callback can provide
     * additional HTTP headers to include when retrying the failed request
     * (for the purpose of providing credentials to the server).
     *
     * The string returned by this callback function should be in the form of 
     * zero or more HTTP headers (header name and value), and each header 
     * (including the last) should be terminated with a CRLF sequence, for example:
     * \code
     * "id: test1\r\npw: 12345\r\n"
     * \endcode
     * The particulars of the headers depend on the server and the authentication
     * method being used.
     *
     * When this function is registered, the string it returns is used instead of the
     * value of the \c HTTP_CREDENTIAL property.
     *
     * The first time this function is called, the buffer provided may not be big
     * enough to hold the headers, or it may not even have been allocated.  If
     * \c dwBufSize is too small or zero, the callback must set \c *pdwNewBufSize
     * to the correct size and return <b>1</b>.  In this case, the caller will
     * reallocate a buffer of the correct size and try the call again.
     *
     * If the buffer is big enough to hold the string AND the terminating null,
     * then the callback should simply fill in the buffer with the correct string
     * and then return <b>0</b>. It is not necessary to set \c *pdwNewBufSize in 
     * this case, because the string is null terminated, and the caller will
     * ignore the value of \c *pdwNewBufSize if the function returns zero.
     *
     * All other return values are reserved and are treated as an error.
     *
     * \param[out]  pBuf            Buffer to receive the new credentials string
     * \param[in]   dwBufSize       Size, in bytes, of the buffer at \c pBuf
     * \param[out]  pdwNewBufSize   Number of bytes needed to store the headers,
     *                              including the terminating null, if the return
     *                              value is 1. Undefined for other return values.
     * \param[in]   pUserData       User data passed when the callback was registered.
     * \returns     0 if successful; 1 if the buffer wasn't big enough (in which case
     *              pdwNewBufSize must be set to the required buffer size).
     */
    typedef int(*NEXPLAYERGetCredentialCallbackFunc)( 	char* pBuf, 
									 		unsigned long dwBufSize,
									 		unsigned long* pdwNewBufSize,
									 		unsigned long pUserData);
    
    /**
     * \ingroup cbtypes
     * \brief Callback replacement for \c dlopen system call.
     *
     * This is one of several callback functions that can be registered using
     * ::nexPlayerSWP_RegisterDLAPICallbackFunc that can replace the system
     * calls normally used for loading and accessing dynamic libraries.
     *
     * Arguments and return values are the same as for \c dlopen
     */
    typedef void* (*NEXPLAYERDLOpenCallbackFunc)(const char* filename, int flag);
    
    /**
     * \ingroup cbtypes
     * \brief Callback replacement for \c dlsym system call.
     *
     * This is one of several callback functions that can be registered using
     * ::nexPlayerSWP_RegisterDLAPICallbackFunc that can replace the system
     * calls normally used for loading and accessing dynamic libraries.
     *
     * Arguments and return values are the same as for \c dlsym
     */
    typedef void* (*NEXPLAYERDLSymCallbackFunc)(void* hDL, const char* strFunc);
    
    /**
     * \ingroup cbtypes
     * \brief Callback replacement for \c dlclose system call.
     *
     * This is one of several callback functions that can be registered using
     * ::nexPlayerSWP_RegisterDLAPICallbackFunc that can replace the system
     * calls normally used for loading and accessing dynamic libraries.
     *
     * Arguments and return values are the same as for \c dlclose
     */
    typedef int (*NEXPLAYERDLCloseCallbackFunc)(void* hDL);
    
    /**
     * \ingroup cbtypes
     * \brief Callback replacement for \c dlerror system call.
     *
     * This is one of several callback functions that can be registered using
     * ::nexPlayerSWP_RegisterDLAPICallbackFunc that can replace the system
     * calls normally used for loading and accessing dynamic libraries.
     *
     * Arguments and return values are the same as for \c dlerror
     */
    typedef const char* (*NEXPLAYERDLErrorCallbackFunc)(void);
        
    /**
     * \ingroup     types
     * \brief       File handle used in <em>Remote File I/O</em> callbacks.
     *
     * This is the file handle used in calls to the various <em>Remote File I/O</em>
     * callback functions.  This value is returned by the file-open callback, and
     * can be any value that the remote file callbacks can use to uniquely 
     * identify the open file instance.
     */
   typedef 		unsigned int	NEXFileHandle;
	
    /**
     * \ingroup     types
     * \brief       File open mode.
     *
     * This is passed by NexPlayer&trade;&nbsp; in calls to the
     * ::NEXPLAYERRemoteFile_OpenFt callback.
     *
     * This is a bitfield, so the constants can be combined with the bitwise-or
     * operator.
     *
     * \code
     * NEX_FILE_WRITE | NEX_FILE_CREATE // Open file for writing; create if it doesn't exist
     * NEX_FILE_READ | NEX_FILE_WRITE // Same as NEX_FILE_READWRITE
     * \endcode
     */
    typedef enum _NEXFileMode
    {
        /** Open for reading                                                                           */
        NEX_FILE_READ         	= 1,	
        /** Open for writing                                                                           */
        NEX_FILE_WRITE        	= 2, 	
        /** Open for reading and writing                                                               */
        NEX_FILE_READWRITE    	= 3, 	        
        /** Create the file if it doesn't exist                                                        */
        NEX_FILE_CREATE       	= 4  	
        
    } NEXFileMode;
    
    /**
     * \ingroup     types
     * \brief       Origin for Remove File I/O callback seek operations.
     *
     * \see ::NEXPLAYERRemoteFile_SeekFt
     * \see ::NEXPLAYERRemoteFile_Seek64Ft
     */
    typedef enum _NEXFileSeekOrigin
    {
        /** Beginning of file         */
        NEX_SEEK_BEGIN        	= 0,  
        /** Current position          */
        NEX_SEEK_CUR          	= 1,  
        /** End of file               */
        NEX_SEEK_END          	= 2    
        
    } NEXFileSeekOrigin;	
	
    /**
     * \ingroup cbtypes
     * \brief   Remote File I/O callback for opening a file. 
     *
     * This is one of several callback functions that can be registered using
     * ::nexPlayerSWP_RegisterRemoteFileIOInterface that can replace the system
     * calls normally used for opening and accessing files.
     *
     * \param[in]  pFileName        Path and filename of the file to be opened.  This is the path 
     *                              that the application originally passed to NexPlayer&trade;, so
     *                              the application may treat it in any way appropriate in the callback.
     *
     * \param[in]  iMode            Specifies how the file is to be opened; see ::NEXFileMode for details.
     *
     * \param[in]  pUserData        The user data passed when the callback was
     *                              originally registered.
     *
     * \returns                     The handle of the opened file, or -1 if an error occurred.
     */
    typedef NEXFileHandle ( *NEXPLAYERRemoteFile_OpenFt ) ( char* pFileName, NEXFileMode iMode, void *pUserData );
    
    /**
     * \ingroup cbtypes
     * \brief   Remote File I/O callback for closing a file. 
     *
     * This is one of several callback functions that can be registered using
     * ::nexPlayerSWP_RegisterRemoteFileIOInterface that can replace the system
     * calls normally used for opening and accessing files.
     *
     * \param[in]  hFile            File handle (as returned by ::NEXPLAYERRemoteFile_OpenFt) of file
     *                              to be closed.
     *
     * \param[in]  pUserData        The user data passed when the callback was
     *                              originally registered.
     *
     * \returns                     0 if successful, or -1 if an error occurred.
     */
    typedef int ( *NEXPLAYERRemoteFile_CloseFt ) ( NEXFileHandle hFile, void *pUserData );
    
    /**
     * \ingroup cbtypes
     * \brief   Remote File I/O callback for reading a file. 
     *
     * This is one of several callback functions that can be registered using
     * ::nexPlayerSWP_RegisterRemoteFileIOInterface that can replace the system
     * calls normally used for opening and accessing files.
     *
     * The actual number of bytes to read is \code (uiSize * uiCount) \endcode.
     *
     * \param[in]  hFile            File handle (as returned by ::NEXPLAYERRemoteFile_OpenFt) of file
     *                              to read from.
     *
     * \param[out] pBuf             Buffer to receive the data.
     *
     * \param[in]  uiSize           Record size, in bytes.
     *
     * \param[in]  uiCount          Number of records to read.
     *
     * \param[in]  pUserData        The user data passed when the callback was
     *                              originally registered.
     *
     * \returns                     
     *                              - &gt;0: The number of bytes actually read
     *                              - 0: Reached the end of the file
     *                              - -1: An error occurred
     */
    typedef int ( *NEXPLAYERRemoteFile_ReadFt ) ( NEXFileHandle hFile, void *pBuf, unsigned int uiSize, unsigned int uiCount, void *pUserData );
    
    /**
     * \ingroup cbtypes
     * \brief   Remote File I/O callback for seeking a file. 
     *
     * This is one of several callback functions that can be registered using
     * ::nexPlayerSWP_RegisterRemoteFileIOInterface that can replace the system
     * calls normally used for opening and accessing files.
     *
     * This sets the location in the file at which the next <em>read</em> operation
     * will occur.
     *
     * \note    This supports seek offsets of up to 32-bits.  For large offsets, 
     *          ::NEXPLAYERRemoteFile_Seek64Ft will be called instead.  If the
     *          64-bit callback is not registered, file with sizes over 2GB will
     *          not be supported.
     *
     * \param[in]  hFile            File handle (as returned by ::NEXPLAYERRemoteFile_OpenFt) of file
     *                              to be seeked.
     *
     * \param[in]  iOffset          Seek destination, as an offset in bytes from \c iOrigin
     *
     * \param[in]  iOrigin          Origin for \c iOffset.  See ::NEXFileSeekOrigin for possible values.
     *
     * \param[in]  pUserData        The user data passed when the callback was
     *                              originally registered.
     *
     * \returns                     New offset from beginning of file, or -1 if an error occurred.
     */
    typedef int ( *NEXPLAYERRemoteFile_SeekFt ) ( NEXFileHandle hFile, int iOffset, NEXFileSeekOrigin iOrigin, void *pUserData );
    
    /**
     * \ingroup cbtypes
     * \brief   Remote File I/O callback for seeking a file. 
     *
     * This is one of several callback functions that can be registered using
     * ::nexPlayerSWP_RegisterRemoteFileIOInterface that can replace the system
     * calls normally used for opening and accessing files.
     *
     * This sets the location in the file at which the next <em>read</em> operation
     * will occur.
     *
     * \note    This supports seek offsets of up to 64-bits.  Implement this
     *          callback if you wish to support files over 2GB in size.
     *
     * \param[in]  hFile            File handle (as returned by ::NEXPLAYERRemoteFile_OpenFt) of file
     *                              to be seeked.
     *
     * \param[in]  iOffset          Seek destination, as an offset in bytes from \c iOrigin
     *
     * \param[in]  iOrigin          Origin for \c iOffset.  See ::NEXFileSeekOrigin for possible values.
     *
     * \param[in]  pUserData        The user data passed when the callback was
     *                              originally registered.
     *
     * \returns                     New offset from beginning of file, or -1 if an error occurred.
     */
    typedef long long ( *NEXPLAYERRemoteFile_Seek64Ft ) ( NEXFileHandle hFile, long long iOffset, NEXFileSeekOrigin iOrigin, void *pUserData );

    /**
     * \ingroup cbtypes
     * \brief   Remote File I/O callback for writing to a file. 
     *
     * This is one of several callback functions that can be registered using
     * ::nexPlayerSWP_RegisterRemoteFileIOInterface that can replace the system
     * calls normally used for opening and accessing files.
     *
     * \param[in]  hFile            File handle (as returned by ::NEXPLAYERRemoteFile_OpenFt) of file
     *                              to be written to.
     *
     * \param[in]  pBuf             Data to write to file
     *
     * \param[in]  dwSize           Number of bytes to write to file
     *
     * \param[in]  pUserData        The user data passed when the callback was
     *                              originally registered.
     *
     * \returns                     Actual number of bytes wrtten, or -1 if an error occurred.
     */
    typedef long long ( *NEXPLAYERRemoteFile_WriteFt ) ( NEXFileHandle hFile, char* pBuf, unsigned int dwSize, void *pUserData );			// JDKIM 2010/12/17

    /**
     * \ingroup cbtypes
     * \brief   Remote File I/O callback for getting the size of a file. 
     *
     * This is one of several callback functions that can be registered using
     * ::nexPlayerSWP_RegisterRemoteFileIOInterface that can replace the system
     * calls normally used for opening and accessing files.
     *
     * This callback should return the size of the file <em>without</em> modifying
     * the position to which the file has been seeked (if the seek location must
     * be moved to determine the size, this function should move it back afterwards).
     *
     * \param[in]  hFile            File handle (as returned by ::NEXPLAYERRemoteFile_OpenFt) of the ile
     *                              for which the size should be retrieved.
     *
     * \param[in]  pUserData        The user data passed when the callback was
     *                              originally registered.
     *
     * \returns                     Actual number of bytes wrtten, or -1 if an error occurred.
     */
    typedef long long ( *NEXPLAYERRemoteFile_SizeFt ) ( NEXFileHandle hFile, void *pUserData );			// JDKIM 2010/12/17
    
    
    /**
     * \ingroup     types
     * \brief       A structure holding function pointers to all of the functions that 
     *              comprise the Remote File I/O interface.
     *
     * This structure is passed to ::nexPlayerSWP_RegisterRemoteFileIOInterface when
     * registering the callbacks.
     */
    typedef struct NEXPLAYERRemoteFileIOInterface_
    {
        /** Open callback (see ::NEXPLAYERRemoteFile_OpenFt) */    
        NEXPLAYERRemoteFile_OpenFt		Open;        
        /** Close callback (see ::NEXPLAYERRemoteFile_CloseFt) */  
        NEXPLAYERRemoteFile_CloseFt		Close;       
        /** Read callback (see ::NEXPLAYERRemoteFile_ReadFt) */    
        NEXPLAYERRemoteFile_ReadFt		Read;        
        /** Seek callback (see ::NEXPLAYERRemoteFile_SeekFt) */    
        NEXPLAYERRemoteFile_SeekFt		Seek;        
        /** Seek64 callback (see ::NEXPLAYERRemoteFile_Seek64Ft) */
        NEXPLAYERRemoteFile_Seek64Ft	Seek64;      
        /** Write callback (see ::NEXPLAYERRemoteFile_WriteFt) */  
        NEXPLAYERRemoteFile_WriteFt		Write;       
        /** Size callback (see ::NEXPLAYERRemoteFile_SizeFt)  */   
        NEXPLAYERRemoteFile_SizeFt		Size;            
    } NEXPLAYERRemoteFileIOInterface;

    /**
     * \ingroup     ppmacro
     * \brief       Payload DRM type
     *
     * This is a possible value that can be used when calling ::nexPlayerSWP_RegisterWMDRMDescrambleCallBackFunc 
     */
#define	NXWMDRM_PAYLOAD_TYPE	0x01
    
    /**
     * \ingroup     ppmacro
     * \brief       Packet DRM type
     *
     * This is a possible value that can be used when calling ::nexPlayerSWP_RegisterWMDRMDescrambleCallBackFunc 
     */
#define	NXWMDRM_PACKET_TYPE		0x10
    
    /**
     * \ingroup     ppmacro
     * \brief       Frame DRM type
     *
     * This is a possible value that can be used when calling ::nexPlayerSWP_RegisterWMDRMDescrambleCallBackFunc 
     */
#define NXWMDRM_FRAME_TYPE		0x20
    
    
    /**
     * \ingroup apifunc
     * \brief Registers a general DRM descrambling callback function.
     *
     * \param[in]  pDescrambleCallbackFunc  Callback function to register
     *
     * \param[in]  pUserData                Additional data to pass to callback function when it is called
     *
     * \returns 
     *     - 0 if the operation succeeded
     *     - a non-zero error code if the operation failed
     *
     * \see ::NEXPLAYERDRMDescrambleCallbackFunc
     */
    int nexPlayerSWP_RegisterDRMDescrambleCallBackFunc(NEXPLAYERDRMDescrambleCallbackFunc pDescrambleCallbackFunc, 
                                                       void *pUserData);													 
    
    /**
     * \ingroup apifunc
     * \brief Registers a WMDRM descrambling callback function.
     *
     * \param[in] uiDRMType
     *		        The type of DRM. This is one of the following constants:
     *		        - ::NXWMDRM_PAYLOAD_TYPE <b>(0x01)</b>
     *		        - ::NXWMDRM_PACKET_TYPE <b>(0x10)</b> 
     *		        - ::NXWMDRM_FRAME_TYPE <b>(0x20)</b>
     *
     * \param[in]  pDescrambleCallbackFunc  Callback function to register
     *
     * \param[in]  pUserData                Additional data to pass to callback function when it is called
     *
     * \returns 
     *     - 0 if the operation succeeded
     *     - a non-zero error code if the operation failed
     *
     * \see ::NEXPLAYERWMDRMDescrambleCallbackFunc
     */
    int nexPlayerSWP_RegisterWMDRMDescrambleCallBackFunc(unsigned int uiDRMType, NEXPLAYERWMDRMDescrambleCallbackFunc pDescrambleCallbackFunc, 		
                                                         void *pUserData);
    /**
     * \ingroup apifunc
     * \brief Registers an HLS/TS descrambling callback function.
     *
     * \param[in]  pDescrambleCallbackFunc  Callback function to register
     *
     * \param[in]  pUserData                Additional data to pass to callback function when it is called
     *
     * \returns 
     *     - 0 if the operation succeeded
     *     - a non-zero error code if the operation failed
     *
     * \see ::NEXPLAYERHLSTSDescrambleCallbackFunc
     */
    int nexPlayerSWP_RegisterHLSTSDescrambleCallBackFunc(NEXPLAYERHLSTSDescrambleCallbackFunc pDescrambleCallbackFunc, 
                                                         void *pUserData);
    
    /**
     * \ingroup apifunc
     * \brief Registers a Smooth Streaming fragment descrambling callback function.
     *
     * \param[in]  pDescrambleCallbackFunc  Callback function to register
     *
     * \param[in]  pUserData                Additional data to pass to callback function when it is called
     *
     * \returns 
     *     - 0 if the operation succeeded
     *     - a non-zero error code if the operation failed
     *
     * \see ::NEXPLAYERSmoothStreamFragmentDescrambleCallbackFunc
     */
    int nexPlayerSWP_RegisterSmoothStreamFragmentDescrambleCallBackFunc(NEXPLAYERSmoothStreamFragmentDescrambleCallbackFunc pDescrambleCallbackFunc, 
                                                                        void *pUserData);
    
    
    /**
     * \ingroup apifunc
     * \brief Registers a Smooth Streaming PlayReady descrambling callback function.
     *
     * \param[in]  pDescrambleCallbackFunc  Callback function to register
     *
     * \param[in]  pUserData                Additional data to pass to callback function when it is called
     *
     * \returns 
     *     - 0 if the operation succeeded
     *     - a non-zero error code if the operation failed
     *
     * \see ::NEXPLAYERSmoothStreamPlayReadyDescrambleCallbackFunc
     */
    int nexPlayerSWP_RegisterSmoothStreamPlayReadyDescrambleCallBackFunc(NEXPLAYERSmoothStreamPlayReadyDescrambleCallbackFunc pDescrambleCallbackFunc, 
                                                                         void *pUserData);
    
    /**
     * \ingroup apifunc
     * \brief Registers a PIFF PlayReady descrambling callback function.
     *
     * \param[in]  pDescrambleCallbackFunc  Callback function to register
     *
     * \param[in]  pUserData                Additional data to pass to callback function when it is called
     *
     * \returns 
     *     - 0 if the operation succeeded
     *     - a non-zero error code if the operation failed
     *
     * \see ::NEXPLAYERPiffPlayReadyDescrambleCallbackFunc
     */
    int nexPlayerSWP_RegisterPiffPlayReadyDescrambleCallBackFunc(NEXPLAYERPiffPlayReadyDescrambleCallbackFunc pDescrambleCallbackFunc, 
                                                                 void *pUserData);
    
    
    /**
     * \ingroup apifunc
     * \brief Registers an ASF PlayReady descrambling callback function.
     *
     * \param[in]  pDescrambleCallbackFunc  Callback function to register
     *
     * \param[in]  ulDescrambleType         
     *                  The type of DRM. This is one of the following constants:
     *                  - ::NXWMDRM_PAYLOAD_TYPE <b>(0x01)</b>
     *                  - ::NXWMDRM_PACKET_TYPE <b>(0x10)</b>
     *                  - ::NXWMDRM_FRAME_TYPE <b>(0x20)</b>
     *
     * \param[in]  pUserData                Additional data to pass to callback function when it is called
     *
     * \returns 
     *     - 0 if the operation succeeded
     *     - a non-zero error code if the operation failed
     *
     * \see ::NEXPLAYERAsfPlayReadyDescrambleCallbackFunc
     */
    int nexPlayerSWP_RegisterAsfPlayReadyDescrambleCallBackFunc(NEXPLAYERAsfPlayReadyDescrambleCallbackFunc pDescrambleCallbackFunc, 
                                                                unsigned long ulDescrambleType,  
                                                                void *pUserData);
    
    /**
     * \ingroup apifunc
     * \brief Registers a callback function for obtaining external decryption keys.
     *
     * \param[in]  pGetKeyExtFunc           Callback function to register
     *
     * \param[in]  pUserData                Additional data to pass to callback function when it is called
     *
     * \returns 
     *     - 0 if the operation succeeded
     *     - a non-zero error code if the operation failed
     *
     * \see \ref deckey
     * \see ::NEXPLAYERGetKeyExtCallbackFunc
     */
    int nexPlayerSWP_RegisterGetKeyExtCallBackFunc(	NEXPLAYERGetKeyExtCallbackFunc pGetKeyExtFunc, 
                                                   void* pUserData);
    
    /**
     * \ingroup apifunc
     * \brief Registers a set of callback functions for Remote File I/O.
     *
     * See \ref rfio for more information.
     *
     * \param[in]  pRemoteFileIOInterface   Structure containing pointers to functions to register
     *
     * \param[in]  pUserData                Additional data to pass to callback function when it is called
     *
     * \returns 
     *     - 0 if the operation succeeded
     *     - a non-zero error code if the operation failed
     *
     * \see ::NEXPLAYERRemoteFileIOInterface
     */
    int nexPlayerSWP_RegisterRemoteFileIOInterface(NEXPLAYERRemoteFileIOInterface *pRemoteFileIOInterface, void *pUserData);		// JDKIM 2010/08/02
    
    /**
     * \ingroup apifunc
     * \brief Registers a set of callback functions to use in place of the normal dynamic library system calls.
     *
     * NexPlayer&trade;&nbsp;uses a series of dynamic libraries, in order to allow unnecessary components to be
     * removed from an application, thus reducing the size of the application.  In order to prevent tampering,
     * DRM solutions can handle the loading of this dynalic libraries themselves, allowing verification of the
     * library.
     *
     * To facilitate this, NexPlayer&trade;&nbsp;allows applications to substitute their own functions for
     * loading and managing dynamic libraries.  These callbacks use exactly the same interface as the
     * normal system calls.
     *
     * \param[in]  pDLOpenFunc    Pointer to replacement function for \c dlopen
     *
     * \param[in]  pDLSymFunc     Pointer to replacement function for \c dlsym
     *
     * \param[in]  pDLCloseFunc   Pointer to replacement function for \c dlclose
     *
     * \param[in]  pDLErrorFunc   Pointer to replacement function for \c dlerror
     *
     * \returns 
     *     - 0 if the operation succeeded
     *     - a non-zero error code if the operation failed
     *
     * \see ::NEXPLAYERDLOpenCallbackFunc
     * \see ::NEXPLAYERDLSymCallbackFunc
     * \see ::NEXPLAYERDLCloseCallbackFunc
     * \see ::NEXPLAYERDLErrorCallbackFunc
     */
    int nexPlayerSWP_RegisterDLAPICallbackFunc( NEXPLAYERDLOpenCallbackFunc pDLOpenFunc, NEXPLAYERDLSymCallbackFunc pDLSymFunc, NEXPLAYERDLCloseCallbackFunc pDLCloseFunc, NEXPLAYERDLErrorCallbackFunc pDLErrorFunc);

    /**
     * \ingroup apifunc
     * \brief Registers a callback function to provide credentials (in the form of HTTP headers) when a 401 response is received from the server.
     *
     * \param[in]  pGetCredentialFunc       Callback function to register
     *
     * \param[in]  pUserData                Additional data to pass to callback function when it is called
     *
     * \returns 
     *     - 0 if the operation succeeded
     *     - a non-zero error code if the operation failed
     *
     * \see ::NEXPLAYERGetCredentialCallbackFunc
     */
    int nexPlayerSWP_RegisterGetCredentialCallbackFunc( NEXPLAYERGetCredentialCallbackFunc pGetCredentialFunc, void* pUserData);

#ifdef __cplusplus
}
#endif

#endif	// _NEXPLAYER_JNI_H_
