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
#include "scalado_photoart.h"
#include "ICameraHacker.h"

namespace android {
class ScaladoJob: public RefBase
{
public:
    ScaladoJob(int partMode,
                    int jobId, 
                    void * inBuf, 
                    int buffSize, 
                    int facing, 
                    SCALADO_CAM_MODE camMode,
                    struct ICameraHacker::CamState camState,
                    bool forBlurCaptureInit = false);
    ~ScaladoJob();
public:
    bool  process();    
    void * getOutBuffer();
    
    int  getOutBufferSize();
    
    bool setParamByCamMode(SCALADO_CAM_MODE camMode);
    
    bool createSessionByPartMode();
private:
    int                             mPartMode;
    int                             mJobId;
    int                             mImageWidth;
    int                             mImageHeight;
    
    void *                          mInBuffer;
    int                             mInBufferSize;
    void *                          mOutBuffer;
    int                             mOutBufferSize; 

    struct ICameraHacker::CamState  mCamState;    

    sp<ScaladoPhotoArt>             mPhotoArt;
    sp<IMemoryHelper>               mMemoryHelper;
    SCALADO_CAM_MODE                mScaladoCamMode;

    int                             mFacing;
    bool                            mForBlurCaptureInit;
};


class ScaladoJobThread : public Thread
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Operations in base class Thread
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:     ////
    // Ask this object's thread to exit. This function is asynchronous, when the
    // function returns the thread might still be running. Of course, this
    // function can be called from a different thread.
    virtual void                    requestExit();

    // Good place to do one-time initializations
    virtual status_t                readyToRun();

private:
    // Derived class must implement threadLoop(). The thread starts its life
    // here. There are two ways of using the Thread object:
    // 1) loop: if threadLoop() returns true, it will be called again if
    //          requestExit() wasn't called.
    // 2) once: if threadLoop() returns false, the thread will exit upon return.
    virtual bool                    threadLoop();
    
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Operations.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++    
public:
    ScaladoJobThread(sp<ScaladoJob> job);
    virtual ~ScaladoJobThread();
        
private:
    sp<ScaladoJob>   mJob;
};

}
