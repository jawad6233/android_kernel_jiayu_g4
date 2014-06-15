/*************************************************************
 *
 * Copyright (C), 2008-2012, OPPO Mobile Comm Corp., Ltd
 *
 * Date Created: 2012-06-07 14:44
 *
 * Author: Lycan Wang @MultimediaDrv
 *
 * Description : NULL
 *
 * ------------------ Revision History: ---------------------
 *      <author>        <date>          <desc>
 *
 *************************************************************/

#ifndef ANDROID_HARDWARE_PhotoArtHacker_H
#define ANDROID_HARDWARE_PhotoArtHacker_H

using namespace android;

#include "utils.h"
#include "ICameraHacker.h"
#include "scalado_photoart.h"
#include <stdlib.h>

namespace android
{
class ScaladoJob;

class PhotoArtHacker: public ICameraHacker {
public:
	PhotoArtHacker(int facing);
    virtual ~PhotoArtHacker();

    virtual void onReceivePreviewFrame(void* mem, CamState camState);
	virtual void onReceivePostview(void* mem, CamState camState);
    virtual void onRawCaptured(void* mem, CamState camState);
    virtual bool getProcessedMem(void * mem,  int memSize);
    virtual void onCustomCallback(int msgId, void * param1, void * param2);
private:
    sp<ScaladoPhotoArt> mPhotoArt;
    sp<IMemoryHelper> mMemoryHelper;
    SCALADO_CAM_MODE mScaladoCamMode;

	Ipp8u   *mDstMem;
	int      mDstMemSize;
	int 	 mFacing;

    struct CamState mCamState;
    
    enum {MAX_THREAD_NUM = 4};
	sp<Thread> 		mThread[MAX_THREAD_NUM];
	sp<ScaladoJob> 	mScaladoJob[MAX_THREAD_NUM];
	PartModeEnum    mPartMode;
	
	static sp<Thread> 		mThreadForBlurInit;
	static sp<ScaladoJob> 	mScaladoJobForBlurInit;
	
    void scaladoProc(const void* inMem, int width, int height);
    
    void multiThreadProc(PartModeEnum partMode, 
                               SCALADO_CAM_MODE camMode, 
                               void * inMem, 
                               CTColormode format);
                               
    bool mergeImageYuv422ByPartMode(PartModeEnum partMode, 
                                                 void* outMem, 
                                                 int   outMemSize);
                                                 
    bool mergeImageYuv420pByPartMode(PartModeEnum partMode, 
                                                   void* outMem, 
                                                   int   outMemSize);
	
};

};

#endif
