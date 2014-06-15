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
#define LOG_TAG "PhotoArtHacker"

#include <inc/CamUtils.h>
#include <IParamsManager.h>
#include <sys/resource.h>
#include <sys/time.h>

#include <sys/prctl.h>
#include <cutils/properties.h>
#include "utils.h"

#include "PhotoArtThread.h"

#include "PhotoArtHacker.h"
#define  show_log  0

namespace android {

sp<Thread>       PhotoArtHacker::mThreadForBlurInit = NULL;
sp<ScaladoJob>   PhotoArtHacker::mScaladoJobForBlurInit = NULL;

PhotoArtHacker::PhotoArtHacker(int facing) :
    mPhotoArt(new ScaladoPhotoArt()), 
    mMemoryHelper(new SimpleMemoryHelper()),
    mFacing(facing)
{
    mDstMem = NULL;
    mDstMemSize = 0;
    
    mPhotoArt->setPixelFormat(CM_YUV420P);
    mPhotoArt->setFacing(facing);

    for (int i=0; i<MAX_THREAD_NUM; i++) {
        mThread[i] = NULL;
        mScaladoJob[i] = NULL;
    }
    mPartMode = NonePartMode;
}

PhotoArtHacker::~PhotoArtHacker() {
	MY_LOGD_IF(show_log, "+");
	mPhotoArt = NULL;
	
	if (mDstMem != NULL) {
	    mMemoryHelper->freeMemory();
	    mDstMem = NULL;
	}
	mMemoryHelper = NULL;
	
    for (int i=0; i<MAX_THREAD_NUM; i++) {
        mThread[i] = NULL;
        mScaladoJob[i] = NULL;
    }
    
	MY_LOGD_IF(show_log, "-");
}

void PhotoArtHacker::onReceivePostview(void* mem, CamState camState) {
    MY_LOGD_IF(show_log, "+");

    memcpy(&mCamState, &camState, sizeof(CamState));
	mPhotoArt->setPixelFormat(CM_BGR565);
	mPhotoArt->setBlurCntPoint(mCamState.centerPointX,
                           mCamState.centerPointY,
                           mCamState.surfaceW,
                           mCamState.surfaceH,
                           mCamState.blurCircleRatio);
                           
	mScaladoCamMode = POSTVIEW_CAPTURE_MODE; 

	scaladoProc(mem, mCamState.prvW, mCamState.prvH);
	
    MY_LOGD_IF(show_log, "-");
}

void PhotoArtHacker::onReceivePreviewFrame(void* mem, CamState camState) {
    MY_LOGD_IF(show_log, "+");

    memcpy(&mCamState, &camState, sizeof(CamState));
	mPhotoArt->setPixelFormat(CM_YUV420P);
	mPhotoArt->setQuadEffect(camState.quadEffect);
	mScaladoCamMode = RECEIVE_PREVIEW_MODE; 
	
	mPhotoArt->setBlurCntPoint(mCamState.centerPointX,
	                           mCamState.centerPointY,
	                           mCamState.surfaceW,
	                           mCamState.surfaceH,
	                           mCamState.blurCircleRatio);
	scaladoProc(mem, mCamState.prvW, mCamState.prvH);
	
    MY_LOGD_IF(show_log, "-");
}

void PhotoArtHacker::onRawCaptured(void* mem, CamState camState) {		
    MY_LOGD("+");
    memcpy(&mCamState, &camState, sizeof(CamState));
    mPhotoArt->setPixelFormat(CM_YUV422_Y1UY2V);
    mScaladoCamMode = RAW_CAPTURE_MODE; 

	mPhotoArt->setBlurCntPoint(mCamState.centerPointX,
	                           mCamState.centerPointY,
	                           mCamState.surfaceW,
	                           mCamState.surfaceH,
	                           mCamState.blurCircleRatio);

    MY_LOGD("onRawCaptured camState.rotation = %d,mCamState.capH = %d, mCamState.capW = %d ",camState.rotation, mCamState.capH, mCamState.capW);
	
	if (mCamState.caps_effect == LOMO_XUJIAO && mThreadForBlurInit != NULL) {
            mThreadForBlurInit->join();
    }
	
#if 0
    if((camState.rotation/90%2) != 0){
        scaladoProc(mem, mCamState.capH, mCamState.capW);
    }else{
        scaladoProc(mem, mCamState.capW, mCamState.capH);
    }
    
    if (mCamState.caps_effect == LOMO_XUJIAO) {
        onCustomCallback(ICAMERA_MSG_DO_ON_DESTORY, 0, 0);
    }
#else    

    multiThreadProc(FourPartMode, RAW_CAPTURE_MODE, mem, CM_YUV422_Y1UY2V);
#endif

    MY_LOGD("-");
}

//note: this method can just do capture job so far.
void PhotoArtHacker::multiThreadProc(PartModeEnum partMode, 
                                          SCALADO_CAM_MODE camMode, 
                                          void * inMem, 
                                          CTColormode format) {
    status_t status;
    int partCnt = 0;
    int partStartIdx = 0;
    
    mPartMode = partMode;

    if (partMode == TwoPartMode) {
        partCnt = 2;
        partStartIdx = PART_UP;
    } else if (partMode == FourPartMode) {
        partCnt = 4;
        partStartIdx = PART_1;
    } else {
        return;
    }
    
    for (int i=0; i<partCnt; i++) {
        mScaladoJob[i] = new ScaladoJob(partMode, 
                                        partStartIdx + i, 
                                        inMem, 
                                        mCamState.capW*mCamState.capH*caps_pixelBitSize(format), 
                                        mFacing, 
                                        camMode, 
                                        mCamState);
        mThread[i] = new ScaladoJobThread(mScaladoJob[i]);
        
        status = mThread[i]->run();
        if ( INVALID_OPERATION == status) {
            MY_LOGW("ScaladoJobThread %d is running", i);
        } else if ( OK != status ) {
            MY_LOGE("Fail to run ScaladoJobThread %d, status[%s(%d)]", i, ::strerror(-status), -status);
        }
    }  
}


void PhotoArtHacker::scaladoProc(const void* inMem, int width, int height) {

	size_t size; 
    Ipp8u *src = (Ipp8u *)inMem;

	switch(mScaladoCamMode){
		case RAW_CAPTURE_MODE:
			size = width * height * 2;
			break;
		case POSTVIEW_CAPTURE_MODE:
			size = width * height * 2;
			break;
		case RECEIVE_PREVIEW_MODE:
			size = width * height * 3/2;
			break;
	}
    mPhotoArt->setZoomRatio(100);

	if (mDstMem != NULL) {
	    mMemoryHelper->freeMemory();
	    mDstMem = NULL;
	}
	mMemoryHelper->getMemory(&mDstMem, size);
	mDstMemSize = size;

	mPhotoArt->createSession(src, mDstMem, width, height);

    mPhotoArt->applyEffect(mCamState.caps_effect, mScaladoCamMode, mCamState.rotation);

	mPhotoArt->renderToOutBuffer();

	mPhotoArt->destroySession(); 
}


//NOTE: This method must be called after onReceivePreviewFrame() or onRawCaptured() immediately
bool PhotoArtHacker::getProcessedMem(void * mem,  int memSize) {

    if (mPartMode != NonePartMode && mScaladoCamMode == RAW_CAPTURE_MODE) {
        int i = 0;
        for (i=0; i<MAX_THREAD_NUM; i++) {
            if (mThread[i] != NULL) {
                mThread[i]->join();
            }
        }        

        bool status = mergeImageYuv422ByPartMode(mPartMode, mem, memSize);
        MY_LOGD("getProcessedMem: outBuffer status=%d", status);
        
        for (i=0; i<MAX_THREAD_NUM; i++) {
            mThread[i] = NULL;
            mScaladoJob[i] = NULL;
        }
        
        if (mCamState.caps_effect == LOMO_XUJIAO) {
            onCustomCallback(ICAMERA_MSG_DO_ON_DESTORY, 0, 0);
        }
    } else {  		
        if (mDstMem == NULL || mem == NULL || mDstMemSize == 0) {
            MY_LOGE("getProcessedMem: mDstMem(0x%x) mem(0x%x) mDstMemSize(%d)", 
                mDstMem, mem, mDstMemSize); 
            return false;
        }
    	if (memSize != mDstMemSize) {
    		MY_LOGW("getProcessedMem: memSize(%d) != mDstMemSize(%d)", 
                memSize, mDstMemSize); 
    	}
        memcpy(mem, mDstMem, memSize);
    }
    return true;
}

bool PhotoArtHacker::mergeImageYuv420pByPartMode(PartModeEnum partMode, 
                                                             void* outMem, 
                                                             int   outMemSize){
    int bufSize = 0;
    int bufCnt = 0;
    void * currAddr = outMem;
    int i = 0;
    
    if (partMode == TwoPartMode) {
        bufCnt = 2;
    } else if (partMode == FourPartMode){
        bufCnt = 4;
    } else {
        MY_LOGE("wrong params!!"); 
        return false;
    }

    bufSize = mScaladoJob[0]->getOutBufferSize();
    
    //todo: here merge two YV12 image
#if 0    
    //Y data
    for (i=0; i<bufCnt; i++) {
        currAddr = outMem + bufSize * i * 2/3;
        memcpy(currAddr, mScaladoJob[i]->getOutBuffer(), bufSize * 2/3);
    }

    //u data
    for (i=0; i<bufCnt; i++) {
        currAddr = outMem + bufSize * bufCnt * 2/3 + bufSize * i * 1/6;
        memcpy(currAddr, mScaladoJob[i]->getOutBuffer(), bufSize * 1/6);
    }
    

    //v data
    for (i=0; i<bufCnt; i++) {
        currAddr = outMem + bufSize * bufCnt * 5/6 + bufSize * i * 1/6;
        memcpy(currAddr, mScaladoJob[i]->getOutBuffer(), bufSize * 1/6);
    }
#endif
    return true;
}

bool PhotoArtHacker::mergeImageYuv422ByPartMode(PartModeEnum partMode, 
                                                             void* outMem, 
                                                             int   outMemSize){
    int bufSize = 0;
    int bufCnt = 0;
    void * currAddr = outMem;
    
    if (partMode == TwoPartMode) {
        bufCnt = 2;
    } else if (partMode == FourPartMode){
        bufCnt = 4;
    } else {
        MY_LOGE("wrong params!!"); 
        return false;
    }
    
    bufSize = mScaladoJob[0]->getOutBufferSize();
    
    for (int i=0; i<bufCnt; i++) {
        currAddr = outMem + bufSize*i;
        memcpy(currAddr, mScaladoJob[i]->getOutBuffer(), bufSize);
    }
    
    return true;
}


void PhotoArtHacker::onCustomCallback(int msgId, void * param1, void * param2) {
    MY_LOGD("msgId=%d", msgId);
    if (msgId == ICAMERA_MSG_DO_ON_CAPTURE) {    
        if (mCamState.caps_effect == LOMO_XUJIAO && mScaladoJobForBlurInit == NULL && mThreadForBlurInit == NULL) {
            MY_LOGD("run init blur thread");
            mScaladoJobForBlurInit = new ScaladoJob(0, 0, 0, 0, mFacing, RAW_CAPTURE_MODE, mCamState, true);
            mThreadForBlurInit = new ScaladoJobThread(mScaladoJobForBlurInit); 
            status_t status = mThreadForBlurInit->run();
            if ( INVALID_OPERATION == status) {
                MY_LOGW("ScaladoJobThread init blur is running");
            } else if ( OK != status ) {
                MY_LOGE("Fail to run ScaladoJobThread init blur, status[%s(%d)]", ::strerror(-status), -status);
            }
        }        
    } else if (msgId == ICAMERA_MSG_DO_ON_DESTORY) {
        if (mThreadForBlurInit != NULL) {
            mThreadForBlurInit->join();
            mThreadForBlurInit = NULL;
        }
        mScaladoJobForBlurInit = NULL;
        ScaladoPhotoArt::onDestory();
    }
}

}
