/*************************************************************
 * 
 * Copyright (C), 2008-2012, OPPO Mobile Comm Corp., Ltd
 * 
 * VENDOR_EDIT
 *
 * Date Created: 2012-06-12 11:06
 * 
 * Author: Lycan.Wang@Prd.Camera.CameraSrv
 * 
 * Description : NULL
 * 
 * ------------------ Revision History: ---------------------
 *      <author>        <date>          <desc>
 *
 *************************************************************/ 

#ifndef ANDROID_HARDWARE_CAMERAHACKER_H
#define ANDROID_HARDWARE_CAMERAHACKER_H

#include "Scalado/scalado_include/caps_types.h"
#include "Scalado/scalado_include/caps_base.h"


namespace android {

class ICameraHacker: public RefBase {

public:

    enum ICameraMsg {
        ICAMERA_MSG_RECEIVE_SMALL_FD,
        ICAMERA_MSG_GET_SMALL_FACE_INFO,
        ICAMERA_MSG_DO_ON_CAPTURE,
        ICAMERA_MSG_DO_ON_DESTORY,
    };

    struct CamState{
        int pixelFormat;
		int capW;
		int capH;
		int prvW;
		int prvH;
		int zoom;
		int caps_effect;
		int quadEffect;
		int surfaceW;
		int surfaceH;
		int centerPointX;
		int centerPointY;
		int rotation;
		float blurCircleRatio;
	};

	virtual void onReceivePreviewFrame(void* mem, CamState camState) = 0;
	virtual void onRawCaptured(void* mem, CamState camState) = 0;
	virtual void resize(void *mem ,int PrvorCap,CamState camState) {
	}
    virtual bool getProcessedMem(void * mem,  int memSize) {
        return false;
    }
    virtual void onCustomCallback(int msgId, void * param1, void * param2) {
    }
	virtual ~ICameraHacker() {
	}

};

};

#endif
