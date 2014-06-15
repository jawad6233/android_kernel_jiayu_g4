/*************************************************************
 * 
 * Copyright (C), 2008-2012, OPPO Mobile Comm Corp., Ltd
 * 
 * VENDOR_EDIT
 *
 * Date Created: 2012-06-12 11:10
 * 
 * Author: Lycan.Wang@Prd.Camera.CameraSrv
 * 
 * Description : NULL
 * 
 * ------------------ Revision History: ---------------------
 *      <author>        <date>          <desc>
 *
 *************************************************************/ 
#ifndef ANDROID_HARDWARE_UTILS_H
#define ANDROID_HARDWARE_UTILS_H
#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>
#include <stdint.h>
#include "camera/CameraParameters.h"
extern "C" {

#include <linux/rtpm_prio.h>
#include <sched.h>
#include <sys/resource.h>
}

#include <inc/CamUtils.h>
#include <utils/CallStack.h>
/******************************************************************************
*
*******************************************************************************/
#if !defined(MY_LOGV)
    #define MY_LOGV(fmt, arg...)        CAM_LOGV("(%d)[%s] "fmt, ::gettid(), __FUNCTION__, ##arg)
#endif
#if !defined(MY_LOGD)
    #define MY_LOGD(fmt, arg...)        CAM_LOGD("(%d)[%s] "fmt, ::gettid(), __FUNCTION__, ##arg)
#endif
#if !defined(MY_LOGI)
    #define MY_LOGI(fmt, arg...)        CAM_LOGI("(%d)[%s] "fmt, ::gettid(), __FUNCTION__, ##arg)
#endif
#if !defined(MY_LOGW)
    #define MY_LOGW(fmt, arg...)        CAM_LOGW("(%d)[%s] "fmt, ::gettid(), __FUNCTION__, ##arg)
#endif
#if !defined(MY_LOGE) 
    #define MY_LOGE(fmt, arg...)        CAM_LOGE("(%d)[%s] "fmt, ::gettid(), __FUNCTION__, ##arg)
#endif

#if !defined(MY_LOGV_IF) 
    #define MY_LOGV_IF(cond, arg...)    if (cond) { MY_LOGV(arg); }
#endif
#if !defined(MY_LOGD_IF) 
    #define MY_LOGD_IF(cond, arg...)    if (cond) { MY_LOGD(arg); }
#endif
#if !defined(MY_LOGI_IF) 
    #define MY_LOGI_IF(cond, arg...)    if (cond) { MY_LOGI(arg); }
#endif
#if !defined(MY_LOGW_IF) 
    #define MY_LOGW_IF(cond, arg...)    if (cond) { MY_LOGW(arg); }
#endif
#if !defined(MY_LOGE_IF) 
    #define MY_LOGE_IF(cond, arg...)    if (cond) { MY_LOGE(arg); }
#endif

namespace android {

long long getTimeMs();

void storeToFile(sp<IMemory> mem, const char * name);

void mergeImage(uint8_t * a, uint8_t * b, uint8_t * c, uint8_t * d,
                uint8_t * _out, int width, int height);

void imageBlur(uint8_t *image, uint8_t level, int width, int height);

class AutoTimer {
public:
	AutoTimer(const char* functionName) {
		_start = getTimeMs();
		_name = functionName;
	}
	~AutoTimer() {
	    CAM_LOGV("%s cost [%lld] ms", _name, getTimeMs() - _start);
	}
private:
	long long _start;
	const char *_name;
};

class IMemoryHelper: public RefBase {
public:
	virtual void getMemory(unsigned char ** pp, int size)=0;
	virtual void freeMemory() =0;
	virtual ~IMemoryHelper() {
	}
	//virtual int getPixelFormat()=0;
};

class SimpleMemoryHelper: public IMemoryHelper {
public:
	SimpleMemoryHelper();
	virtual ~SimpleMemoryHelper();
	virtual void getMemory(unsigned char ** pp, int size);
	virtual void freeMemory();

private:
	int mSize;
	unsigned char *mBuffer;
};

}

#endif
