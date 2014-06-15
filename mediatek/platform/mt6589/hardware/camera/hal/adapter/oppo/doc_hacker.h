/*************************************************************
 * 
 * Copyright (C), 2012-2102, OPPO Mobile Comm Corp., Ltd
 * 
 * VENDOR_EDIT  USE_DOC_SOFTWARE
 *
 * File : doc_hacker.h
 *
 * Date Created: 2012-06-20
 * 
 * Author: xinlan.he@Prd.Camera.CameraSrv
 * 
 * Description : for doc face beauty adaption
 * 
 * ------------------ Revision History: ---------------------
 *      <author>        <date>          <desc>
 *    wuhongru        2013-02-18        do some Optimization
 *************************************************************/ 

#ifndef ANDROID_HARDWARE_DOC_HACKER_H
#define ANDROID_HARDWARE_DOC_HACKER_H


#include "ICameraHacker.h"
#include <stdlib.h>
#include "doc_base.h"
#include "Scalado/scalado_include/caps_types.h"
#include "Scalado/scalado_include/caps_base.h"
#include "utils.h"

namespace android
{
class DocHacker: public ICameraHacker {
public:
	DocHacker(int facing);
	DocHacker();
    virtual ~DocHacker();
    
    virtual void onReceivePreviewFrame(void* mem, CamState camState);
    virtual void onRawCaptured(void* mem, CamState camState);
    virtual bool getProcessedMem(void * mem, int memSize);

    virtual void onCustomCallback(int msgId, void * param1, void * param2);
	
	bool convertYuvYV12AndYuvI420(void * memIn, void * memOut, int memSize);

	//bool YuvNV21ToYuvYV12(void * memIn, void * memOut, int memSize);
	
    void resize(void *mem ,int PrvorCap,CamState camState);
private:
    
    
    struct CamState mCamState;
    bool   mDocEnabled;
    void   *mDocResizeMem;
    void   *mConvertBuffer;
    int    mDocDstMemSize;

    sp<DoCBase> mDoC;
};

};

#endif
