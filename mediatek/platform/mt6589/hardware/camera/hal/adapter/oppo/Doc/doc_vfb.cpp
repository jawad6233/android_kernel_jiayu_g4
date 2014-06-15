/*************************************************************
 * 
 * Copyright (C), 2012-2102, OPPO Mobile Comm Corp., Ltd
 * 
 * VENDOR_EDIT  USE_DOC_SOFTWARE
 *
 * File : doc_vfb.cpp
 *
 * Date Created: 2012-03-14
 * 
 * Author: Wuhongru @MultimediaDrv
 * 
 * Description : NULL
 * 
 * ------------------ Revision History: ---------------------
 *      <author>        <date>          <desc>
 *      Wuhongru        2012-03-14       init
 *      xinlan.he       2012-06-20       do some Optimization
 *      xiaoyunhe    2012-11-21        do some Optimization
 *************************************************************/

#define LOG_TAG "DOCvfb"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include<cutils/log.h>	
#include "face.h"
#include "gtypes.h"
#include "LibFB.h"
#include "LibVfb.h"
#include "doc.h"

#include <dlfcn.h>

#define  show_log  0

namespace android {

#define frameWidth 640
#define frameHeight 480
#define GT_CUSTOM_BOOL

typedef unsigned char BOOLEAN;

extern "C" void GTCALL vfb_heap_destroy( GT_HEAP* heap ) {
}

extern "C" void* GTCALL vfb_heap_alloc( GT_HEAP* heap, UINT32 size ) {
  return (malloc(size));
}

extern "C" void GTCALL vfb_heap_free( GT_HEAP* heap, void* mem ) {
  free(mem);
}

static struct GT_HEAP_VFT vfb_vft = {
  vfb_heap_destroy,
  vfb_heap_alloc,
  vfb_heap_free
};

videofacebeautifier::videofacebeautifier() {
	//LOGD("Doc_VFB::VFB() S");
	DpreW = 0;
	DpreH = 0;
	memset(&mvfb_cfg, 0, sizeof(DoC_VFB_CFG));
	fd_det = new facedetect(VFB_MODE);
	init_vfb();
	//LOGD("Doc_VFB::VFB() E");
}

void videofacebeautifier::vfbmain(void*fdmem,void* inmem, int prvWidth, int prvHeight) {
	DpreW = prvWidth;
	DpreH = prvHeight;
	MY_LOGD_IF(show_log,"[vfbmain]getpresizeW = %d,H = %d",DpreW,DpreH);
	MY_LOGD_IF(show_log,"Doc_VFB::vfbmain() S");
	GTSTATUS iStatus = GTS_FAIL;
	GT_IMAGE frame;
	//xiaoyh VFB_FACE arrVfbFaces[MAX_NUM_FACES];	
	int faceCnt = 0;
	int selvfb = 2;
	MY_LOGD_IF(show_log,"DoFaceDetection facecntstart");
	MY_LOGD_IF(show_log,"vfb_fdimg = %d",(int)fdmem);

	VFB_FACE* arrVfbFaces = NULL;
	if(MAX_NUM_FACES)
	{
		arrVfbFaces =(VFB_FACE*)malloc(MAX_NUM_FACES * sizeof(VFB_FACE) );
		if( arrVfbFaces == NULL )
		{
			// Insufficient memory
			//return -1;
			
			//LOGD("memory allocate fail!");
		}
	}

    faceCnt = fd_det->DoFaceDetection(fdmem, arrVfbFaces, NULL, selvfb, prvWidth, prvHeight);
	MY_LOGD("facecnt = %d",faceCnt);

	const INT32 size = DpreW * DpreH;
	memset(&frame, 0, sizeof(GT_IMAGE));
	frame.width = DpreW;
	frame.height = DpreH;
	frame.format = GT_FMT_YUV_420_P3;

	frame.planes[0].stride = frame.width;
	frame.planes[0].data = inmem;	

	frame.planes[1].stride = frame.width >>1;
	frame.planes[1].data = (void *)((INT32)inmem + size);

    frame.planes[2].stride = frame.width >>1;
    frame.planes[2].data = (UINT8*)frame.planes[1].data + (frame.width/2) * (frame.height/2);

    MY_LOGD_IF(show_log,"runsys.s");   
    //if (faceCnt != 0) {
        iStatus = pEng->vft->Process( pEng, &frame, arrVfbFaces, faceCnt);
        MY_LOGD("whr debugvfb process%d\n", iStatus);
    //}
    
    if (VFB_FAILED(iStatus)) {
        // Handle error
       MY_LOGD( "error: VFB driver [%d]\n", iStatus);
    }

	if (arrVfbFaces)
	{
		free(arrVfbFaces);
	}
	
    //LOGD("runsys.e");
    MY_LOGD_IF(show_log,"Doc_VFB::vfbmain() E");
}

videofacebeautifier::~videofacebeautifier() {
	if(pEng != NULL) {
		pEng->vft->Destroy( pEng );
		//LOGD("whr debug ~vfb");
	}
	fd_det = NULL;
}

int videofacebeautifier::init_vfb() 
{
	GTSTATUS initStatus = GTS_FAIL;
	pEng = NULL;
	memset(&vfb_heap, 0, sizeof(vfb_heap));
	vfb_heap.vft = &vfb_vft;
	
	mvfb_cfg.cfg.structSize = sizeof(VFB_CONFIG);
	mvfb_cfg.cfg.correctionStrength = VFB_CORR_LVL5;
	mvfb_cfg.cfg.framesInertia = 30;
	mvfb_cfg.cfg.correctionAccuracy = 128 ;
	mvfb_cfg.cfg.options = VFB_CORRECT_WHEN_NO_FACES;
	mvfb_cfg.needupdate = 1;
	
	MY_LOGD("videofacebeautifier::init_vfb vfb plib create\n");
	pLib = VfbLibGet();
	MY_LOGD("videofacebeautifier::init_vfb vfb plib created\n");
	initStatus = pLib->vft->CreateVfbEngine( pLib, &vfb_heap, &mvfb_cfg.cfg, &pEng );
	MY_LOGD("whr debugvfb create%d\n", initStatus);
	if( initStatus != GTS_OK ) 
	{
        // Handle error
        MY_LOGD("videofacebeautifier::init_vfb  Error %d\n", initStatus);
        return initStatus; //Failed to create Engine
	}
	return 0;
}

void videofacebeautifier::faceDetect(void * inMem, int * outArray, int outArraySize) {
    VFB_FACE  faces[MAX_NUM_FACES];
    int faceCnt = 0;
    int selfb = 2;
    int i, outArryIdx = 0;
    
    if (inMem == NULL || outArray == NULL) {
       // LOGD("facebeautifier::faceDetect()  inMem(0x%x), outArray(0x%x)", inMem, outArray);
        return;
    }
    
    //faceCnt = fd_det->DoFaceDetection(inMem, faces, NULL, selfb);
    if (outArraySize < faceCnt * 4) {
       // LOGD("facebeautifier::faceDetect()  outArraySize(%d) < faceCnt(%dX4)", outArraySize, faceCnt*4);
        return;
    }
    if (faceCnt > 0) 
	{
        for (i=0; i<outArraySize/4 && i<faceCnt; i++)
		{
            outArray[outArryIdx] = faces[i].boundRect.left;
            outArray[outArryIdx + 1] = faces[i].boundRect.top;
            outArray[outArryIdx + 2] = faces[i].boundRect.width;
            outArray[outArryIdx + 3] = faces[i].boundRect.height;
            outArryIdx += 4;
           // LOGD("facebeautifier::faceDetect() face[%d](%d, %d, %d, %d)", i, faces[i].boundRect.left, 
               // faces[i].boundRect.top, faces[i].boundRect.width, faces[i].boundRect.height);
        }
    } 
	else
	{
        //LOGD("facebeautifier::faceDetect() facecnt = 0!");
    }
}

};

