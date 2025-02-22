/* -LICENSE-START-
** Copyright (c) 2009 Blackmagic Design
**  
** Permission is hereby granted, free of charge, to any person or organization 
** obtaining a copy of the software and accompanying documentation (the 
** "Software") to use, reproduce, display, distribute, sub-license, execute, 
** and transmit the Software, and to prepare derivative works of the Software, 
** and to permit third-parties to whom the Software is furnished to do so, in 
** accordance with:
** 
** (1) if the Software is obtained from Blackmagic Design, the End User License 
** Agreement for the Software Development Kit ("EULA") available at 
** https://www.blackmagicdesign.com/EULA/DeckLinkSDK; or
** 
** (2) if the Software is obtained from any third party, such licensing terms 
** as notified by that third party,
** 
** and all subject to the following:
** 
** (3) the copyright notices in the Software and this entire statement, 
** including the above license grant, this restriction and the following 
** disclaimer, must be included in all copies of the Software, in whole or in 
** part, and all derivative works of the Software, unless such copies or 
** derivative works are solely in the form of machine-executable object code 
** generated by a source language processor.
** 
** (4) THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
** OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
** FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT 
** SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
** FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, 
** ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
** DEALINGS IN THE SOFTWARE.
** 
** A copy of the Software is available free of charge at 
** https://www.blackmagicdesign.com/desktopvideo_sdk under the EULA.
** 
** -LICENSE-END-
**/

#include <stdio.h>
#include <pthread.h>
#include <dlfcn.h>

#include "DeckLinkAPI_v7_6.h"

#define kDeckLinkAPI_Name "libDeckLinkAPI.so"
#define KDeckLinkPreviewAPI_Name "libDeckLinkPreviewAPI.so"

typedef IDeckLinkIterator* (*CreateIteratorFunc_v7_6)(void);
typedef IDeckLinkGLScreenPreviewHelper_v7_6* (*CreateOpenGLScreenPreviewHelperFunc_v7_6)(void);
typedef IDeckLinkVideoConversion_v7_6* (*CreateVideoConversionInstanceFunc_v7_6)(void);

static pthread_once_t					gDeckLinkOnceControl = PTHREAD_ONCE_INIT;
static pthread_once_t					gPreviewOnceControl = PTHREAD_ONCE_INIT;

static CreateIteratorFunc_v7_6						gCreateIteratorFunc			= NULL;
static CreateOpenGLScreenPreviewHelperFunc_v7_6		gCreateOpenGLPreviewFunc	= NULL;
static CreateVideoConversionInstanceFunc_v7_6		gCreateVideoConversionFunc	= NULL;

static void	InitDeckLinkAPI_v7_6 (void)
{
	void *libraryHandle;
	
	libraryHandle = dlopen(kDeckLinkAPI_Name, RTLD_NOW|RTLD_GLOBAL);
	if (!libraryHandle)
	{
		fprintf(stderr, "%s\n", dlerror());
		return;
	}
	
	gCreateIteratorFunc = (CreateIteratorFunc_v7_6)dlsym(libraryHandle, "CreateDeckLinkIteratorInstance");
	if (!gCreateIteratorFunc)
		fprintf(stderr, "%s\n", dlerror());
	gCreateVideoConversionFunc = (CreateVideoConversionInstanceFunc_v7_6)dlsym(libraryHandle, "CreateVideoConversionInstance");
	if (!gCreateVideoConversionFunc)
		fprintf(stderr, "%s\n", dlerror());
}

static void	InitDeckLinkPreviewAPI_v7_6 (void)
{
	void *libraryHandle;
	
	libraryHandle = dlopen(KDeckLinkPreviewAPI_Name, RTLD_NOW|RTLD_GLOBAL);
	if (!libraryHandle)
	{
		fprintf(stderr, "%s\n", dlerror());
		return;
	}
	gCreateOpenGLPreviewFunc = (CreateOpenGLScreenPreviewHelperFunc_v7_6)dlsym(libraryHandle, "CreateOpenGLScreenPreviewHelper");
	if (!gCreateOpenGLPreviewFunc)
		fprintf(stderr, "%s\n", dlerror());
}

IDeckLinkIterator*		CreateDeckLinkIteratorInstance_v7_6 (void)
{
	pthread_once(&gDeckLinkOnceControl, InitDeckLinkAPI_v7_6);
	
	if (gCreateIteratorFunc == NULL)
		return NULL;
	return gCreateIteratorFunc();
}

IDeckLinkGLScreenPreviewHelper_v7_6*		CreateOpenGLScreenPreviewHelper_v7_6 (void)
{
	pthread_once(&gDeckLinkOnceControl, InitDeckLinkAPI_v7_6);
	pthread_once(&gPreviewOnceControl, InitDeckLinkPreviewAPI_v7_6);
	
	if (gCreateOpenGLPreviewFunc == NULL)
		return NULL;
	return gCreateOpenGLPreviewFunc();
}

IDeckLinkVideoConversion_v7_6* CreateVideoConversionInstance_v7_6 (void)
{
	pthread_once(&gDeckLinkOnceControl, InitDeckLinkAPI_v7_6);
	
	if (gCreateVideoConversionFunc == NULL)
		return NULL;
	return gCreateVideoConversionFunc();
}

