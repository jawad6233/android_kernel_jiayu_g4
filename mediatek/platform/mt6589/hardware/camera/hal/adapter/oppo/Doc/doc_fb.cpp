/*************************************************************
 * 
 * Copyright (C), 2012-2102, OPPO Mobile Comm Corp., Ltd
 * 
 * VENDOR_EDIT  USE_DOC_SOFTWARE
 *
 * File : doc_fb.cpp
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


#include "doc.h"
#include "LibFB.h"
#include "face.h"
#include "gtypes.h"

namespace android {

/**
 Face beautification heap management 
 */

extern "C"  void GTCALL fb_heap_destroy( GT_HEAP* heap )
{
    //LOGD("debug fb_heap_destroy");
}

extern "C"  void* GTCALL fb_heap_alloc( GT_HEAP* heap, UINT32 size )
{
	int * addr = NULL;
	//if (size < 3000000)
    addr = (int *)malloc(size);
	//else
    //LOGD("debug fb_heap_alloc addr %x  size %x", (int)addr, size);
    return ((void *)(addr));
}
  
extern "C"  void GTCALL fb_heap_free( GT_HEAP* heap, void* mem )
{ 
	//LOGD("debug fb_heap_free addr %x", (int)mem);
    free(mem);
}

struct GT_HEAP_VFT fb_vft = {
  fb_heap_destroy,
  fb_heap_alloc,
  fb_heap_free
};


void facebeautifier::init_fb(){

MY_LOGD("enter init fb");

  fb_heap.vft = &fb_vft;

/* Initialize configuration structure */
  memset( &fbcfg, 0, sizeof(fbcfg));
  fbcfg.structSize = sizeof(fbcfg);
 // memset(&fb_heap, 0, sizeof(fb_heap));

  /* Indicate the types of processing to be applied
     Any number of processing types can be combined. 
     In these example all available types are applied.
  */
  //fbcfg.processingType = FBB_FEAT_CORR_SKIN_SMOOTHING
                       		//|FBB_FEAT_CORR_EYES_ENHANCEMENT
                       		//|FBB_FEAT_CORR_SKIN_TONING
  //                     		|FBB_FEAT_CORR_TEETH_WHITENING
                       		//|FBB_FEAT_CORR_EYES_ENLARGEMENT
 //                      		;

  /* Perform correction even in the absence of faces and process all faces regardless of size */
  fbcfg.flags = FBB_CORRECT_ALL_FACES | FBB_CORRECT_WHEN_NO_FACES;
  
  /* Use best quality mode (recommended) */
  fbcfg.optimizationLevel = FBB_OPT_QUALITY_OVER_SPEED;

  /* Configure each type of processing */
  fbcfg.skinSmoothingCfg.structSize = sizeof ( FB_SKIN_SMOOTHING_CFG );
  fbcfg.skinSmoothingCfg.mode = FBB_MODE_FULL_BODY;
  fbcfg.skinSmoothingCfg.type = /*1FBB_CORR_TYPE_SUBTLE;*/FBB_CORR_TYPE_SKINSOFTENING;
  fbcfg.skinSmoothingCfg.level = 90; 

  fbcfg.skinToningCfg.structSize = sizeof ( FB_SKIN_TONING_CFG );     
  fbcfg.skinToningCfg.mode = FBB_MODE_FULL_BODY;
  fbcfg.skinToningCfg.type = FBB_ST_TYPE_WHITE;
  fbcfg.skinToningCfg.level = 90;  

  fbcfg.teethWhiteningCfg.structSize = sizeof ( FB_TEETH_WHITENING_CFG );
  fbcfg.teethWhiteningCfg.level = 90;  

  fbcfg.eyesEnhancementCfg.structSize = sizeof ( FB_EYES_ENHANCEMENT_CFG );
  fbcfg.eyesEnhancementCfg.level = 97; 

  fbcfg.eyesEnlargementCfg.structSize = sizeof ( FB_EYES_ENLARGEMENT_CFG );
  fbcfg.eyesEnlargementCfg.level = 90; 

//wuhongru@MMApp.Camera, 2012-12-04 Add for Diff Project
#if 1
  fbcfg.faceSlimmingCfg.structSize = sizeof(FB_FACE_SLIMMING_CFG); 
  fbcfg.faceSlimmingCfg.level = 86;
  MY_LOGD("whr debug diff project");
#endif
  MY_LOGD("exit init fb");
}

facebeautifier::facebeautifier(){
	MY_LOGD("enter into fb");
	GTSTATUS fbstatus;
    btfr = NULL;
	init_fb();
	fd_det1 = new facedetect(FB_MODE);
	fbstatus = FbBeautifierCreate(&fb_heap, &fbcfg, &btfr);
	MY_LOGD("exit fb,fbstatus = %d",fbstatus);
	
}
	
facebeautifier::~facebeautifier(){
	if(btfr != NULL)
	{
		btfr->vft->Destroy( btfr );
	}
	fd_det1 = NULL;
}

void facebeautifier::init_fbimage( GT_IMAGE *fim, UINT32 width, UINT32 height, UINT8 *data )
{
  MY_LOGD("going to init_fbimage");
  memset( fim, 0, sizeof(GT_IMAGE) );

  fim->width  = width;
  fim->height = height;
  fim->format = GT_FMT_UYVY_422;
  fim->planes[0].stride = width*2;
  fim->planes[0].data = data;
}

/** 
  Perform face beautification on the list of faces
 */
GTSTATUS facebeautifier::DoFaceBeautification(FB_FACE *faces, INT32 noFaces)
{  
  MY_LOGD("enter do face beauty");
  int i;  
  GTSTATUS status = GTS_FAIL;
  FB_STATUS_INFO *statusInfo = NULL;      

  /*
    At this point face detection data (from a face detector)
    should be available. Make sure the rectangles are scaled
    accordingly for the size of prvImage.
  */

  for (i = 0; i < noFaces; i++)
  {
	faces[i].id = i;/* set unique face identifier  for the current face */
	/* Initialize face and eyes data, scale positions from QVGA to XGA */
    faces[i].rcFace.top = ( faces[i].rcFace.top * fullImg.width) / smallImg.width;
    faces[i].rcFace.height = ( faces[i].rcFace.height * fullImg.width) / smallImg.width;
    faces[i].rcFace.left = ( faces[i].rcFace.left * fullImg.width) / smallImg.width;
    faces[i].rcFace.width = ( faces[i].rcFace.width * fullImg.width) / smallImg.width;

    faces[i].angle = faces[i].angle;
    faces[i].yawAngle = faces[i].yawAngle;

    faces[i].rcLeftEye.top = ( faces[i].rcLeftEye.top * fullImg.width) / smallImg.width;
    faces[i].rcLeftEye.height = ( faces[i].rcLeftEye.height * fullImg.width) / smallImg.width;
    faces[i].rcLeftEye.left = ( faces[i].rcLeftEye.left * fullImg.width) / smallImg.width;
    faces[i].rcLeftEye.width = ( faces[i].rcLeftEye.width * fullImg.width) / smallImg.width;
   
    faces[i].rcRightEye.top = ( faces[i].rcRightEye.top * fullImg.width) / smallImg.width;
    faces[i].rcRightEye.height = ( faces[i].rcRightEye.height * fullImg.width) / smallImg.width;
    faces[i].rcRightEye.left = ( faces[i].rcRightEye.left * fullImg.width) / smallImg.width;
    faces[i].rcRightEye.width = ( faces[i].rcRightEye.width * fullImg.width) / smallImg.width;
    
    faces[i].rcLips.top = ( faces[i].rcLips.top * fullImg.width) / smallImg.width;
    faces[i].rcLips.height = ( faces[i].rcLips.height * fullImg.width) / smallImg.width;
    faces[i].rcLips.left = ( faces[i].rcLips.left * fullImg.width) / smallImg.width;
    faces[i].rcLips.width = ( faces[i].rcLips.width * fullImg.width) / smallImg.width;
  }

  status = btfr->vft->Process(btfr,NULL,NULL,&fullImg, faces, noFaces, &statusInfo, NULL, NULL);

  MY_LOGD("\n Apply beautification status=%d \n", status);

  /* Check status to see if processing was successful */
  if (GTS_OK != status )
  {
    INT32 featureStatus;

    MY_LOGD("\n Status for each of the features:\n");

    /* Check each feature to see what went wrong */
    featureStatus = statusInfo->vft->GetStatusForFeature( statusInfo, FBB_FEAT_CORR_SKIN_SMOOTHING );
    if ( GTS_OK != featureStatus )
    {
      printf ("\n\tSkin smoothing status: %x", status );
    }
	
    featureStatus = statusInfo->vft->GetStatusForFeature( statusInfo, FBB_FEAT_CORR_SKIN_TONING );
    if ( GTS_OK != featureStatus )
    {
      printf ("n\tSkin toning status: %x", status );
    }

    featureStatus = statusInfo->vft->GetStatusForFeature( statusInfo, FBB_FEAT_CORR_TEETH_WHITENING );
    if ( GTS_OK != featureStatus )
    {
      printf ("n\tTeeth whitening status: %x", status );
	}

    featureStatus = statusInfo->vft->GetStatusForFeature( statusInfo, FBB_FEAT_CORR_EYES_ENHANCEMENT );
    if ( GTS_OK != featureStatus )
    {
      printf ("n\tEye enhancement status: %x", status );
    }

    featureStatus = statusInfo->vft->GetStatusForFeature( statusInfo, FBB_FEAT_CORR_EYES_ENLARGEMENT );
    if ( GTS_OK != featureStatus )
    {
      printf ("\n\Eye enlargement status: %x", status );
    }

	featureStatus = statusInfo->vft->GetStatusForFeature( statusInfo, FBB_FEAT_CORR_FACE_SLIMMING );
    if ( GTS_OK != featureStatus )
    {
      printf ("\n\tFace slimming status: %x", status );
    }

  }

  //Destroy the status info object
  if (statusInfo)
  {
    statusInfo->vft->Destroy ( statusInfo );
  }
  
  return  status;
}

void facebeautifier::fbmain(void* inmem, void* fb_FullImg, int width, int height)
{
   MY_LOGD("enter fbmain");
    FDSTATUS st;    
    GTSTATUS fb_status;    
    int ret;
    GTSTATUS fbstatus;

    MY_LOGD("fbmain  width = %d, height = %d",width,height);
    FB_FACE  fbfaces[MAX_NUM_FACES];
    int faceCnt = 0;
    int selfb = 1;
    int getcap4W = width;
    int getcap4H = height;
    int picSize = width * height;
 
    MY_LOGD("[fbmain]getfullsizeW = %d,fullsizeH = %d",width,height);

    /* Read the input images */  
    //LOGD("fbmain_fbfullbuf = %x",(int)fb_FullImg);
    //LOGD("fbmain_small buf = %d",(int)inmem);
    if(width*3 == height*4) {
      init_fbimage(&smallImg, DOC_SMALL_IMG_SIZE_W, DOC_SMALL_IMG_SIZE_H, (UINT8 *)inmem);
    }
    else if(width*4 == height*3) {
        init_fbimage(&smallImg, DOC_SMALL_IMG_SIZE_H, DOC_SMALL_IMG_SIZE_W, (UINT8 *)inmem);
    }
    else if(width*9 == height*16) {
        init_fbimage(&smallImg, DOC_SMALL_IMG_SIZE_W, DOC_SMALL_IMG_SIXTEENINE_SIZE_H, (UINT8 *)inmem);
    }
    else if(width*16 == height*9) {
        init_fbimage(&smallImg, DOC_SMALL_IMG_SIXTEENINE_SIZE_H, DOC_SMALL_IMG_SIZE_W, (UINT8 *)inmem);
    }
	MY_LOGD("fbmain   width = %d , height =%d ",width,height);
    init_fbimage(&fullImg, width, height,(UINT8 *)fb_FullImg);

    MY_LOGD("\tDetecting faces...");
    /* Detect faces */
    faceCnt = fd_det1->DoFaceDetection(inmem, NULL, fbfaces, selfb, width, height);

    MY_LOGD("facecnt_fb= %d",faceCnt);

    /* Do Face Beautification on the list of detected faces */
    MY_LOGD("fbbbb_facesssss = %x",(int)fbfaces);
    if(faceCnt != 0)
	{
        MY_LOGD("go in to fb");
        fb_status = DoFaceBeautification(fbfaces, faceCnt);
    }
    MY_LOGD("exit fbmain"); 

    btfr->vft->Destroy( btfr );
    btfr = NULL;
    fbstatus = FbBeautifierCreate(&fb_heap, &fbcfg, &btfr);
}

};
