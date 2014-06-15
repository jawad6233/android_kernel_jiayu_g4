/*************************************************************
 * 
 * Copyright (C), 2008-2013, OPPO Mobile Comm Corp., Ltd
 * 
 * VENDOR_EDIT
 *
 * Date Created: 2013-04-2 
 * 
 * Author: xinlan.he@Prd.Camera.CameraSrv
 * 
 * Description : NULL
 * 
 * ------------------ Revision History: ---------------------
 *      <author>        <date>          <desc>
 *
 *************************************************************/ 

#define LOG_TAG "ScaladoThread"

#include <inc/CamUtils.h>
#include <sys/resource.h>
#include <sys/time.h>

#include <sys/prctl.h>
#include <cutils/properties.h>
#include "utils.h"

#include "PhotoArtThread.h"
#define  show_log  0

namespace android {

ScaladoJob::ScaladoJob(int partMode,
                            int jobId, 
                            void * inBuf, 
                            int buffSize, 
                            int facing, 
                            SCALADO_CAM_MODE camMode,
                            struct ICameraHacker::CamState camState,
                            bool forBlurCaptureInit)
: mPartMode(partMode),
  mJobId(jobId),
  mImageWidth(0),
  mImageHeight(0),
  mInBuffer(inBuf),
  mInBufferSize(buffSize),
  mScaladoCamMode(camMode),
  mForBlurCaptureInit(forBlurCaptureInit),
  mOutBuffer(NULL),  
  mOutBufferSize(0),
  mPhotoArt(new ScaladoPhotoArt()), 
  mMemoryHelper(new SimpleMemoryHelper()) 
{
    mPhotoArt->setFacing(facing);
    mFacing = facing;
    memcpy(&mCamState, &camState, sizeof(struct ICameraHacker::CamState));
}

ScaladoJob::~ScaladoJob()
{
    MY_LOGD_IF(show_log, "+");

    if (mOutBuffer != NULL) {
	    mMemoryHelper->freeMemory();
	    mOutBuffer = NULL;
	}
	
	mPhotoArt = NULL;
	mMemoryHelper = NULL;
	
    MY_LOGD_IF(show_log, "-");
}

bool ScaladoJob::setParamByCamMode(SCALADO_CAM_MODE camMode) 
{
    bool ret = true;
    if (camMode == RAW_CAPTURE_MODE) {
        int surfaceW = 0, surfaceH = 0;
        int pointX = 0, pointY = 0;

        mPhotoArt->setPixelFormat(CM_YUV422_Y1UY2V);
        if((mCamState.rotation/90%2) != 0){
            mImageWidth = mCamState.capH;
            mImageHeight = mCamState.capW;        
            if ((mCamState.rotation/90%3) == 0) {
                pointX = mCamState.centerPointY;
                pointY = mCamState.surfaceW - mCamState.centerPointX;
            } else {
                pointX = mCamState.surfaceH - mCamState.centerPointY;
                pointY = mCamState.centerPointX;
            }
            surfaceW = mCamState.surfaceH;
            surfaceH = mCamState.surfaceW;
            if (mFacing == 1)
            {
                pointY = surfaceH - pointY;
            }
        } else {
            mImageWidth = mCamState.capW;
            mImageHeight = mCamState.capH;
            if ((mCamState.rotation/90%4) == 0) {                
                pointX = mCamState.centerPointX;
                pointY = mCamState.centerPointY;              
            } else {
                pointX = mCamState.surfaceW - mCamState.centerPointX;
                pointY = mCamState.surfaceH - mCamState.centerPointY;
            }
            surfaceW = mCamState.surfaceW;
            surfaceH = mCamState.surfaceH;                
            if (mFacing == 1)
            {
                pointX = surfaceW - pointX;
            }
        }
        switch(mPartMode)
        {
        case TwoPartMode:
            mOutBufferSize = mImageWidth * mImageHeight;
            break;
        case FourPartMode:
            mOutBufferSize = mImageWidth * mImageHeight/2;
            break;
        default:
            ret = false;
            MY_LOGE("Wrong param !!");
            break;
        }

        mPhotoArt->setBlurCntPoint(pointX,
                                   pointY,
                                   surfaceW,
                                   surfaceH,
                                   mCamState.blurCircleRatio);
    } else {
        mPhotoArt->setPixelFormat(CM_YUV420P);
        mImageWidth = mCamState.prvW;
        mImageHeight = mCamState.prvH;
        switch(mPartMode)
        {
        case TwoPartMode:
            mOutBufferSize = mImageWidth * mImageHeight * 3/4;
            break;
        case FourPartMode:
            mOutBufferSize = mImageWidth * mImageHeight * 3/8;
            break;
        default:
            ret = false;
            MY_LOGE("Wrong param !!");
            break;
        }
    	mPhotoArt->setBlurCntPoint(mCamState.centerPointX,
                           mCamState.centerPointY,
                           mCamState.surfaceW,
                           mCamState.surfaceH,
                           mCamState.blurCircleRatio);
    }
	mPhotoArt->setZoomRatio(100);
	return ret;
}

bool ScaladoJob::createSessionByPartMode()
{
    Ipp8u *src = (Ipp8u *)mInBuffer;
    bool ret = true;
    
    switch(mPartMode)
    {
    case TwoPartMode:
        mPhotoArt->createSession(src, (Ipp8u *)mOutBuffer, mImageWidth, mImageHeight, mImageWidth, mImageHeight/2);
        break;
    case FourPartMode:
        mPhotoArt->createSession(src, (Ipp8u *)mOutBuffer, mImageWidth, mImageHeight, mImageWidth, mImageHeight/4);
        break;
    default:
        ret = false;
        MY_LOGE("Wrong param !!");
        break;	        
    }
    return ret;
}

bool ScaladoJob::process()
{	      
    CRESULT res = CERR_OK;
    
    if ( !setParamByCamMode(mScaladoCamMode) ) {
        MY_LOGE("CamMode(%d) is wrong!", mScaladoCamMode);
        return false;
    }

    if (mForBlurCaptureInit) {        
        mPhotoArt->capture_blur_region_init(mImageWidth, mImageHeight);
        return true;
    }
    
	mMemoryHelper->getMemory((unsigned char **)&mOutBuffer, mOutBufferSize);
	
    mPhotoArt->setPartModeAndId((PartModeEnum)mPartMode, (PartEnum)mJobId);

	if ( !createSessionByPartMode() ) {
        MY_LOGE("something is wrong!");
        return false;	    
	}

    res = mPhotoArt->applyEffect(mCamState.caps_effect, mScaladoCamMode, mCamState.rotation);

    MY_LOGD_IF((res != CERR_OK), "applyEffect res:%s", scberror_cresultToString(res));

	mPhotoArt->renderToOutBuffer(mCamState.caps_effect);

	mPhotoArt->destroySession(); 

	return true;
}

void * ScaladoJob::getOutBuffer()
{
    return mOutBuffer;
}

int ScaladoJob::getOutBufferSize()
{
    return mOutBufferSize;
}


/******************************************************************************
 *
 ******************************************************************************/
ScaladoJobThread::ScaladoJobThread(sp<ScaladoJob> job):mJob(job)
{}

/******************************************************************************
 *
 ******************************************************************************/
ScaladoJobThread::~ScaladoJobThread()
{
    mJob = NULL;
}

/******************************************************************************
 *
 ******************************************************************************/
// Ask this object's thread to exit. This function is asynchronous, when the
// function returns the thread might still be running. Of course, this
// function can be called from a different thread.
void ScaladoJobThread::requestExit()
{
    MY_LOGD("+");
    
    Thread::requestExit();
    
    MY_LOGD("-");
}

/******************************************************************************
 *
 ******************************************************************************/
status_t ScaladoJobThread::readyToRun()
{
    ::prctl(PR_SET_NAME,"ScaladoJobThread", 0, 0, 0);

    //mi4ThreadId = ::gettid();

    //  thread policy & priority
    //  Notes:
    //      Even if pthread_create() with SCHED_OTHER policy, a newly-created thread 
    //      may inherit the non-SCHED_OTHER policy & priority of the thread creator.
    //      And thus, we must set the expected policy & priority after a thread creation.
    int const policy    = SCHED_OTHER;
    int const priority  = 0;

    struct sched_param sched_p;
    ::sched_getparam(0, &sched_p);
    sched_p.sched_priority = priority;  //  Note: "priority" is nice value
    sched_setscheduler(0, policy, &sched_p);    
    setpriority(PRIO_PROCESS, 0, priority); 

    MY_LOGD(
        "policy:(expect, result)=(%d, %d), priority:(expect, result)=(%d, %d)"
        , policy, ::sched_getscheduler(0)
        , priority, getpriority(PRIO_PROCESS, 0)
    );
    
    return NO_ERROR;
}

bool ScaladoJobThread::threadLoop(){
    if (mJob.get() != NULL) {
        bool reslut = mJob->process();
        MY_LOGD("scalado thread process result=%d", reslut);
    }
    return false;
}

}
