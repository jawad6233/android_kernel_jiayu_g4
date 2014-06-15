/*************************************************************
 * 
 * Copyright (C), 2012-2102, OPPO Mobile Comm Corp., Ltd
 * 
 * VENDOR_EDIT  USE_DOC_SOFTWARE
 *
 * File : doc.h
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
#ifndef __DOC_H__
#define  __DOC_H__
//
#include <cutils/xlog.h>
#include <cutils/logd.h>

#include "face.h"
#include "gtypes.h"
#include "LibFB.h"
#include "LibVfb.h"	
#include <utils/RefBase.h> 
//#include <MemoryBase.h>
//#include <MemoryHeapBase.h>
//#include <IMemory.h>
#include "doc_base.h"
//#include "../../../client/CamClient/FD/FDClient.h"
#include <inc/CamUtils.h>


#define MY_LOGV(fmt, arg...)        CAM_LOGV("(%d)[%s] "fmt, ::gettid(), __FUNCTION__, ##arg)
#define MY_LOGD(fmt, arg...)        CAM_LOGD("(%d)[%s] "fmt, ::gettid(), __FUNCTION__, ##arg)
#define MY_LOGI(fmt, arg...)        CAM_LOGI("(%d)[%s] "fmt, ::gettid(), __FUNCTION__, ##arg)
#define MY_LOGW(fmt, arg...)        CAM_LOGW("(%d)[%s] "fmt, ::gettid(), __FUNCTION__, ##arg)
#define MY_LOGE(fmt, arg...)        CAM_LOGE("(%d)[%s] "fmt, ::gettid(), __FUNCTION__, ##arg)
//
#define MY_LOGV_IF(cond, arg...)    if (cond) { MY_LOGV(arg); }
#define MY_LOGD_IF(cond, arg...)    if (cond) { MY_LOGD(arg); }
#define MY_LOGI_IF(cond, arg...)    if (cond) { MY_LOGI(arg); }
#define MY_LOGW_IF(cond, arg...)    if (cond) { MY_LOGW(arg); }
#define MY_LOGE_IF(cond, arg...)    if (cond) { MY_LOGE(arg); }

#define FUNCTION_IN               MY_LOGD("+")
#define FUNCTION_OUT              MY_LOGD("-")


namespace android {

enum{
	FB_MODE = 1,
	VFB_MODE = 2,
};

typedef struct{
    VFB_CONFIG cfg;
    int needupdate;
}DoC_VFB_CFG;

class FDClient;

class facedetect: public virtual RefBase
{
public:	
	facedetect(int32_t DOC_MODE);
	~facedetect();
	int DoFaceDetection(void* inmem, VFB_FACE* vfbfaces,FB_FACE* fbfaces, int fbvsvfb, int sizewidth, int sizeheight);
private:
	void initfd(int32_t FD_INIT);
	FDSTATUS CreateFaceDetector(int32_t FD_MODE);
	FD_DETECTOR     *pFD;          /* The face detector object */
	FD_DETECTOR_CFG fdCfg;         /* The face configuration object */
	FD_HEAP fd_heap;               /* The face detection heap */
	FD_IMAGE fim;
		
};

class facebeautifier: public virtual RefBase
{
public:
	facebeautifier();
	~facebeautifier();
	void fbmain(void* inmem,void* fb_FullImg, int width, int height);

private:
	void init_fb();
	void init_fbimage( GT_IMAGE *fim, UINT32 width, UINT32 height, UINT8 *data );
	GTSTATUS DoFaceBeautification(FB_FACE *faces, INT32 noFaces);
	
	FB_BEAUTIFIER *btfr;    /* The face beautification object */
	FB_BEAUTIFIER_CFG fbcfg;
	GT_HEAP fb_heap;
	sp<facedetect> fd_det1;
	GT_IMAGE 	smallImg;             /* Image structures for face beautification*/
    GT_IMAGE 	fullImg;		
};

class videofacebeautifier: public virtual RefBase
{
public:
	videofacebeautifier();
	 ~videofacebeautifier();
	void vfbmain(void*fdmem,void* inmem, int prvWidth, int prvHeight);
	void faceDetect(void * inMem, int * outArray, int outArraySize);

private:
	int init_vfb();
	DoC_VFB_CFG mvfb_cfg;
	GT_HEAP vfb_heap;
	VFB_LIB* pLib;
	VFB_ENGINE* pEng;
	sp<facedetect> fd_det;
	int DpreW,DpreH;
	
};

class DoC: public virtual DoCBase 
{
public:
	DoC();
	void vfbdatacallback(void * inMem, int memSize, int prvW, int prvH);
	void fbdatacallback(void* _FullImg, int fullWidth, int fullHeight);
	void fddatacallback(void * inMem, int memSize);
	int  DoC_setting(void* parm1, void* parm2, int mode);
	void faceDetect(int * outArray, int outArraySize);
	virtual ~DoC();
	
	//friend class FDClient;
private:
	void* FDimg;
	sp<videofacebeautifier> vfb_det;	
	sp<facebeautifier> fb_det;
};


};
#endif

