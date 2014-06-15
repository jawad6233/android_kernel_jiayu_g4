/*************************************************************
 * 
 * Copyright (C), 2012-2102, OPPO Mobile Comm Corp., Ltd
 * 
 * VENDOR_EDIT  USE_DOC_SOFTWARE
 *
 * File : doc_hacker.cpp
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
#define LOG_TAG "DocHacker"

//#include <utils/Log.h>


#include <inc/CamUtils.h>
#include <IParamsManager.h>
#include <sys/resource.h>
#include <sys/time.h>

#include "doc_hacker.h"
#include "doc.h"

#define  show_log  0

namespace android {

DocHacker::DocHacker(int facing) :
    mDocEnabled(true)
{
    mDoC = new DoC();
    mDocResizeMem = malloc(DOC_SMALL_IMG_SIZE_W*DOC_SMALL_IMG_SIZE_H*2);
    if(mDocResizeMem) {
	  memset(mDocResizeMem,0, DOC_SMALL_IMG_SIZE_W*DOC_SMALL_IMG_SIZE_H*2);
    }
    mConvertBuffer = NULL;
    mDocDstMemSize = 0;
}

DocHacker::DocHacker() :
    mDocEnabled(true)
{
    mDoC = new DoC();
    mDocResizeMem = malloc(DOC_SMALL_IMG_SIZE_W*DOC_SMALL_IMG_SIZE_H*2);
    if(mDocResizeMem) {
	  memset(mDocResizeMem,0, DOC_SMALL_IMG_SIZE_W*DOC_SMALL_IMG_SIZE_H*2);
    }
    mConvertBuffer = NULL;
    mDocDstMemSize = 0;
}
DocHacker::~DocHacker() {
	//MY_LOGD("DocHacker::~DocHacker E");
	mDoC = NULL;
    //mDocResizeMem = NULL;
    if (mDocResizeMem) {
        free(mDocResizeMem);
        mDocResizeMem = NULL;
    }
    if (mConvertBuffer) {
        free(mConvertBuffer);
        mConvertBuffer = NULL;
    }
    mDocDstMemSize = 0;
}

void DocHacker::onReceivePreviewFrame(void* mem, CamState camState) {
    if (!mDocEnabled) {
        return;
    }
    MY_LOGD_IF(show_log,"[DocHacker]onReceivePreviewFrame E");
    memcpy(&mCamState, &camState, sizeof(CamState));
    if(NULL == mConvertBuffer) {
        MY_LOGD("DocHacker::onReceivePreviewFrame buffer alloc");
        mConvertBuffer = malloc(mCamState.prvW*mCamState.prvH*2);
    }
    convertYuvYV12AndYuvI420(mem, mConvertBuffer, mCamState.prvW*mCamState.prvH*3/2);
    resize(mConvertBuffer,DOC_RESIZE_PREV, camState);
    onCustomCallback(ICameraHacker::ICAMERA_MSG_RECEIVE_SMALL_FD,
                                NULL, 
                                (void *)(mCamState.prvW*mCamState.prvH*2));                   

    //MY_LOGD("DocHacker::prvW = %d,prevH = %d",mCamState.prvW, mCamState.prvH);
    mDoC->vfbdatacallback(mConvertBuffer, 0, mCamState.prvW, mCamState.prvH);
   // if(mDocDstMemSize&&mem) {
   // mDocDstMem = mConvertBuffer;
      //memcpy(mDocDstMem, mem, mDocDstMemSize);
  //  }

    MY_LOGD_IF(show_log,"[DocHacker]onReceivePreviewFrame X");
    

}

void DocHacker::onRawCaptured(void* mem, CamState camState) {	
	CTint32 fullW,fullH;

    if (!mDocEnabled) {
        return;
    }
	
    memcpy(&mCamState, &camState, sizeof(CamState));
    
    MY_LOGD("DocHacker::onrawcaptured ratition = %d",mCamState.rotation);

    resize(mem,DOC_RESIZE_CAPS, camState);                   
    onCustomCallback(ICameraHacker::ICAMERA_MSG_RECEIVE_SMALL_FD,
                                NULL, 
                               (void *)(mCamState.capW*mCamState.capH*2));
    if(mCamState.rotation == 90 || mCamState.rotation == 270) {
        fullW = mCamState.capW;
        fullH = mCamState.capH;
    }
    else if(mCamState.rotation == 0 || mCamState.rotation == 180) {
        fullW = mCamState.capH;
        fullH = mCamState.capW;
    }
	MY_LOGD("onRawCaptured::fullW = %d,fullH = %d",fullW,fullH);
    mDoC->fbdatacallback(mem, fullW, fullH);
	
    MY_LOGD("DocHacker restore Priority");
}

void DocHacker::resize(void *mem ,int PrvorCap,CamState camState) {
    MY_LOGD_IF(show_log,"[DocHacker] resize start"); 

    memcpy(&mCamState, &camState, sizeof(CamState));
    
   CTSize inDims, outDims;
   CTBuffer inBuffer,outBuffer;
   CTint32 fullW,fullH;
   CTImage *pInImage;
   CTImage *pOutImage;
   CTSession session;
   
   if(PrvorCap == DOC_RESIZE_PREV) {
       fullW = mCamState.prvW;
       fullH = mCamState.prvH;
   } 
   else if(PrvorCap == DOC_RESIZE_CAPS) {
       if(mCamState.rotation == 90 || mCamState.rotation == 270) {
           fullW = mCamState.capW;
           fullH = mCamState.capH;
       }
       else if (mCamState.rotation == 0 || mCamState.rotation == 180) {
           fullW = mCamState.capH;
           fullH = mCamState.capW;
       }
    }
   inDims.w = fullW;
   inDims.h = fullH;
    MY_LOGD("[DocHacker] fullw = %d,h = %d",fullW,fullH);
   if(fullW*3 == fullH*4) {
       outDims.w = DOC_SMALL_IMG_SIZE_W;
       outDims.h = DOC_SMALL_IMG_SIZE_H;
   }
   else if(fullW*4 == fullH*3) {
       outDims.w = DOC_SMALL_IMG_SIZE_H;
       outDims.h = DOC_SMALL_IMG_SIZE_W;
   } 
   else if(fullW*9 == fullH*16) {
       outDims.w = DOC_SMALL_IMG_SIZE_W;
       outDims.h = DOC_SMALL_IMG_SIXTEENINE_SIZE_H;
   }
   else if(fullW*16 == fullH*9) {
       outDims.w = DOC_SMALL_IMG_SIXTEENINE_SIZE_H;
       outDims.h = DOC_SMALL_IMG_SIZE_W;
   }
   inBuffer.size = inDims.w*inDims.h*2;
   inBuffer.data = (CTuint8*)(mem);
   outBuffer.size = outDims.w*outDims.h*2;
   outBuffer.data = (CTuint8*)mDocResizeMem;
   //MY_LOGD("docskinbeauty mode prvw = %d ,prvh = %d",fullW,fullH);
  
    if(PrvorCap == DOC_RESIZE_PREV) {
        //MY_LOGD("[DocHacker]resize prev resize");
        caps_createImage(&inBuffer, inDims, inDims.w*2,CM_YUV420P, &pInImage);
    }
    else if(PrvorCap == DOC_RESIZE_CAPS) {
         //MY_LOGD("[DocHacker]resize cap resize");
        caps_createImage(&inBuffer, inDims, inDims.w*2,CM_YUV422, &pInImage);
    }
    caps_newSessionWithRaw(pInImage, &session);
    caps_createImage(&outBuffer, outDims, outDims.w*2, CM_YUV422, &pOutImage);
    caps_renderAsRawToBuffer(session, pOutImage, CAPS_OUTPUT_PRESERVE_AR, 0);
    caps_destroySession(session);	
    MY_LOGD_IF(show_log,"[DocHacker] resize end");

}

void DocHacker::onCustomCallback(int msgId, void * param1, void * param2) {
    switch (msgId) {
    case ICAMERA_MSG_GET_SMALL_FACE_INFO:
        mDoC->faceDetect((int *)param1, (int)param2);
        break;
        
    case ICAMERA_MSG_RECEIVE_SMALL_FD:
        if (!mDocEnabled) {
            return;
        }

        MY_LOGD("DocHacker::ICAMERA_MSG_RECEIVE_SMALL_FD");
        mDoC->fddatacallback(mDocResizeMem, (int)param2);
        break;
        
    default:
        break;
    }
}

//NOTE: This method must be called after onReceivePreviewFrame() or onRawCaptured() immediately
bool DocHacker::getProcessedMem(void * mem, int memSize) {
    if (mConvertBuffer == NULL || mem == NULL ) {
        MY_LOGW("getProcessedMem: mDstMem(0x%x) mem(0x%x)", 
            mConvertBuffer, mem); 
        return false;
    }
    MY_LOGD("DocHacker::getProcessMem");
    convertYuvYV12AndYuvI420(mConvertBuffer, mem, memSize);
    return true;
}


/*
* convert  format I420(YYYYYYYY UU VV) to format YV12(YYYYYYYY VV UU)
* or convert YV12(YYYYYYYY VV UU) to format format I420(YYYYYYYY UU VV) 
*/ 
bool DocHacker::convertYuvYV12AndYuvI420(void * memIn, void * memOut, int memSize) {
    //copy Y data
    memcpy(memOut, memIn, 2*memSize/3);

    //copy v data
    memcpy(((unsigned char *)memOut + 2*memSize/3), memIn + 5*memSize/6, memSize/6);

    //copy u data
    memcpy(((unsigned char *)memOut + 5*memSize/6), memIn + 2*memSize/3, memSize/6);

    return true;
}

#if 0
// NV21: YYYYYYYY VUVU   convert to   YV12: YYYYYYYY VV UU 
bool DocHacker::YuvNV21ToYuvYV12(void * memIn, void * memOut, int memSize) {

    MY_LOGD("+");
    if (memIn == NULL || memOut == NULL || memSize <= 0) {
        return false;
    }

    unsigned char * pIn = (unsigned char *)memIn;
    unsigned char * pOut = (unsigned char *)memOut;
    unsigned char * pUdata, * pUdataStart;

    int YdataSize = memSize * 2 / 3;
    int VUdataSize = memSize / 6;

    pUdata = new unsigned char[VUdataSize];

    //copy Y data
    memcpy(pOut, pIn, YdataSize);
    
    pIn += YdataSize;
    pOut += YdataSize;
    pUdataStart = pUdata;

    //copy V data
    for (int i=0; i<VUdataSize; i++) {
        *pOut = *pIn;
        pOut++;
        pIn ++;
        
        *pUdata = *pIn;
        pIn++;
    }

    pOut = (unsigned char *)((unsigned char *)memIn + YdataSize + VUdataSize);
    //copy U data
    memcpy(pOut, pUdata, VUdataSize);

    delete pUdata;
    
    MY_LOGD("-");
    return true;
}
#endif

}
