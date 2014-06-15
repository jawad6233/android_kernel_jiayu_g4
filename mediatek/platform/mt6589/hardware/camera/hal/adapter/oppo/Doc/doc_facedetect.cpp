/*************************************************************
 * 
 * Copyright (C), 2012-2102, OPPO Mobile Comm Corp., Ltd
 * 
 * VENDOR_EDIT  USE_DOC_SOFTWARE
 *
 * File : doc_facedetect.cpp
 *
 * Date Created: 2012-03-14
 * 
 * Author: Wuhongru @MultimediaDrv
 * 
 * Description : NULL
 * 
 * ------------------ Revision History: ---------------------
 *      <author>        <date>          <desc>
 *      Wuhongru        2012-03-14        init
 *      xinlan.he       2012-06-20        do some Optimization
 *      xiaoyunhe       2012-11-21        do some Optimization
 *      wuhongru        2013-02-21        do some Optimization
 *************************************************************/

#define LOG_TAG "DOCfd"
#include <cutils/log.h>	
#include "doc.h"

#define  show_log  0

namespace android {
extern "C"  void FDCALL fd_heap_destroy( FD_HEAP* heap ) {

}

extern "C"  void* FDCALL fd_heap_alloc( FD_HEAP* heap, UINT32 size ) {
    return (malloc(size));
}

extern "C"  void FDCALL fd_heap_free( FD_HEAP* heap, void* mem ) {
    free(mem);
}

struct FD_HEAP_VFT fd_vft = {
  fd_heap_destroy,
  fd_heap_alloc,
  fd_heap_free
};

facedetect::facedetect(int32_t DOC_MODE) {
	CreateFaceDetector(DOC_MODE);
}

FDSTATUS facedetect::CreateFaceDetector(int32_t FD_MODE) {  
	MY_LOGD("createfd.start");
	initfd(FD_MODE);
	FDSTATUS FDstatus;
	FDstatus = FdDetectorCreate(&fd_heap, &fdCfg, &pFD);
	MY_LOGD("createfd.end,%d",FDstatus);
	return FDstatus;
}

void facedetect:: initfd(int32_t FD_INIT) { 
    pFD = NULL;
   MY_LOGD("initfd.start");
    memset(&fdCfg, 0, sizeof(fdCfg));
    //memset(&fd_heap, 0, sizeof(fd_heap));
    fdCfg.structSize = sizeof(fdCfg);
		
	static const INT16 lockAngles[] = {90, 0, 90, 180, 90, 45, 90, 135, 90, 60, 90, 120, -135, -90, -45, FD_ANGLE_END};
	
    if (FD_INIT == VFB_MODE) {
        MY_LOGD("fdenter vfbmode");	
        fdCfg.flags	= FDD_TRACKING_MODE | FDD_COLOR_FILTER | FDD_FAST_LOCK | FDD_DISABLE_FACE_CONFIRMATION;
        fdCfg.framesPerLock = 3;
		fdCfg.minFace = 30;
        fdCfg.centralLockAreaPercentageW = 85; 
        fdCfg.centralLockAreaPercentageH = 80; 
        fdCfg.maxFaceCount = 5;               // Maximum number of faces that are processed by VFB
	    fdCfg.lockAngles = (INT16*)lockAngles;
    } else if (FD_INIT == FB_MODE) {
        MY_LOGD("fdenter fbmode");
        fdCfg.flags   = FDD_COLOR_FILTER  | FDD_DETECT_EYES;
	    fdCfg.lockAngles = (INT16*)lockAngles;  
		fdCfg.minFace = 60;
    }
    fd_heap.vft = &fd_vft;
    memset(&fim, 0, sizeof(FD_IMAGE));
    fim.structSize = sizeof(FD_IMAGE);
    //fim.width      = 180;
    //fim.height     = DOC_SMALL_IMG_SIZE_W;    
    fim.format     = FD_FMT_UYVY_422;
    //im.stride     =  fim.width*2;
    	
   
    //LOGD("initfd.end");
}

facedetect::~facedetect() {
    pFD->vft->Destroy( pFD ); 
}

int facedetect::DoFaceDetection(void* inmem, VFB_FACE* vfbfaces, FB_FACE* fbfaces, int fbvsvfb, int sizewidth, int sizeheight) {
	FD_FACE  *face = NULL; 
	int idxFace = 0;
       //fim.planes[0].data = inmem;

    if(sizewidth*3 == sizeheight*4) {
        fim.width = DOC_SMALL_IMG_SIZE_W;
        fim.height = DOC_SMALL_IMG_SIZE_H;
    }
    else if(sizewidth*4 == sizeheight*3) {
        fim.width = DOC_SMALL_IMG_SIZE_H;
        fim.height = DOC_SMALL_IMG_SIZE_W;
    } 
    else if(sizewidth*9 == sizeheight*16) {
        fim.width = DOC_SMALL_IMG_SIZE_W;
        fim.height = DOC_SMALL_IMG_SIXTEENINE_SIZE_H;
    }
    else if(sizewidth*16 == sizeheight*9) {
        fim.width = DOC_SMALL_IMG_SIXTEENINE_SIZE_H;
        fim.height = DOC_SMALL_IMG_SIZE_W;
    }
    fim.stride     =  ((fim.width)*2);
  	fim.data  = inmem;

    MY_LOGD_IF(show_log,"DoFaceDetection s");
	pFD->vft->DetectFacesEx(pFD, &fim, 0, 0,0);
	
	MY_LOGD_IF(show_log,"EnumFaces s");
	while (NULL != (face = pFD->vft->EnumFaces( pFD, face ))) 
	{
  	   MY_LOGD_IF(show_log,"EnumFaces idxFace:%d\n", idxFace);	
		if(fbvsvfb == 1) 
		{
            /* Save face to the list */
            fbfaces[idxFace].structSize = sizeof(FB_FACE);

            fbfaces[idxFace].angle = face->angle;
            fbfaces[idxFace].yawAngle = 0;

            fbfaces[idxFace].rcFace.top = face->boundRect.top;
            fbfaces[idxFace].rcFace.left = face->boundRect.left;
            fbfaces[idxFace].rcFace.height = face->boundRect.height;
            fbfaces[idxFace].rcFace.width = face->boundRect.width;

            fbfaces[idxFace].rcLeftEye.top = face->eyeL.top;
            fbfaces[idxFace].rcLeftEye.left = face->eyeL.left;
            fbfaces[idxFace].rcLeftEye.height = face->eyeL.height;
            fbfaces[idxFace].rcLeftEye.width = face->eyeL.width;

            fbfaces[idxFace].rcRightEye.top = face->eyeR.top;
            fbfaces[idxFace].rcRightEye.left = face->eyeR.left;
            fbfaces[idxFace].rcRightEye.height = face->eyeR.height;
            fbfaces[idxFace].rcRightEye.width = face->eyeR.width;

            fbfaces[idxFace].rcLips.top = face->mouth.top;
            fbfaces[idxFace].rcLips.left = face->mouth.left;
            fbfaces[idxFace].rcLips.height = face->mouth.height;
            fbfaces[idxFace].rcLips.width = face->mouth.width;
 		} 
		else if (fbvsvfb == 2) 
		{
            //LOGD("whr debug go in vfb step");
            vfbfaces[idxFace].structSize = sizeof(VFB_FACE);
            vfbfaces[idxFace].boundRect.top  = face->boundRect.top;       //face rectangle   
            vfbfaces[idxFace].boundRect.left = face->boundRect.left;
            vfbfaces[idxFace].boundRect.height = face->boundRect.height;
            vfbfaces[idxFace].boundRect.width = face->boundRect.width;

            vfbfaces[idxFace].trackingID = face->nTrackedFaceID; // NOTE*** if not using DOC FD set to false
            vfbfaces[idxFace].angle = face->angle;          // please consult the API reference for possible values; if using the DOC Face Tracker, it has the same meaning as in the Face Tracker API
            vfbfaces[idxFace].isTracked = face->isTracked;  // please consult the API reference for more detailed meaning if using the DOC Face Tracker, it has the same meaning as in the Face Tracker API
		}
	    idxFace++;
    }
    MY_LOGD_IF(show_log,"DoFaceDetection e");
    free(face);
    return  idxFace;
}

};
