/*************************************************************
 * 
 * Copyright (C), 2012-2102, OPPO Mobile Comm Corp., Ltd
 * 
 * VENDOR_EDIT  USE_DOC_SOFTWARE
 *
 * File : doc.cpp
 *
 * Date Created: 2012-03-14
 * 
 * Author: Wuhongru @MultimediaDrv
 * 
 * Description : doc face beauty 
 * 
 * ------------------ Revision History: ---------------------
 *      <author>        <date>          <desc>
 *      Wuhongru        2012-03-14       init
 *      xinlan.he       2012-06-20       do some Optimization
 *      xiaoyunhe    2012-11-21        do some Optimization
 *************************************************************/ 
#define LOG_TAG "DOC.cpp"

#include "face.h"
#include "gtypes.h"
#include "LibFB.h"
#include "LibVfb.h"
#include "doc.h"
#include <utils/RefBase.h> 



namespace android {

DoC::DoC() {
	MY_LOGD("doc()");
	FDimg = malloc(DOC_SMALL_IMG_SIZE_W*DOC_SMALL_IMG_SIZE_H*2);
	if(FDimg) {
		memset(FDimg,0, DOC_SMALL_IMG_SIZE_W*DOC_SMALL_IMG_SIZE_H*2);
	}
	vfb_det = new videofacebeautifier();
	fb_det = new facebeautifier();
	//LOGD("Doc_init() E");
}

DoC::~DoC() {
	if(FDimg) {
		free(FDimg);
	}

	FDimg = NULL;
	vfb_det = NULL;
	fb_det = NULL;
}

int DoC::DoC_setting(void* parm1, void* parm2, int mode) {
	 return 0;
}

void DoC::fddatacallback(void * inMem, int memSize) {
	MY_LOGD("fddatacallback()");
	if(FDimg) {			
		memcpy(FDimg, inMem,(DOC_SMALL_IMG_SIZE_W*DOC_SMALL_IMG_SIZE_H*2));
	} else {
		/*do fd right now*/
		  //facedetect::DoFaceDetection(data,face->vfb,face->fb,(face->faceCnt),3);
			//face->available = 1;
	    //LOGD("fddatacallback() FDimg is NULL !!\n");
	}
	return;
}

void DoC::vfbdatacallback(void * inMem, int memSize, int prvW, int prvH) {
	MY_LOGD("DoC::vfbdatacallback() E lycan test"); 
	if(FDimg != NULL) {
        MY_LOGD("vfbdatacallback::prvW = %d,prevH = %d",prvW, prvH);
    	  vfb_det->vfbmain(FDimg, inMem, prvW, prvH);
	} else {
	    MY_LOGD("vfbdatacallback() FDimg is NULL !!\n");
    }
}

void DoC::fbdatacallback(void* _FullImg, int fullWidth, int fullHeight) {
	//LOGD("go into doc fbcb@@@@@@@@@@@@@@@@");
	fb_det->fbmain(FDimg,_FullImg, fullWidth, fullHeight);
}

void DoC::faceDetect(int * outArray, int outArraySize) {
    vfb_det->faceDetect(FDimg, outArray, outArraySize);
}

};
