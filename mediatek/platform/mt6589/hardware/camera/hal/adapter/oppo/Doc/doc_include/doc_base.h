/*************************************************************
 * 
 * Copyright (C), 2012-2102, OPPO Mobile Comm Corp., Ltd
 * 
 * VENDOR_EDIT  USE_DOC_SOFTWARE
 *
 * File : doc.h
 *
 * Date Created: 2012-07-3
 * 
 * Author: xinlan.he @MultimediaDrv
 * 
 * Description : doc face beauty 
 * 
 * ------------------ Revision History: ---------------------
 *      <author>        <date>          <desc>
 *      xiaoyunhe    2012-11-21        do some Optimization
 *************************************************************/
#ifndef __DOC_BASE_H__
#define  __DOC_BASE_H__

#include <cutils/xlog.h>
#include <cutils/logd.h>
#include <utils/RefBase.h> 

namespace android {

#define DOC_SMALL_IMG_SIZE_W 320
#define DOC_SMALL_IMG_SIZE_H 240
#define DOC_SMALL_IMG_SIXTEENINE_SIZE_H 180 // 16/9 ratio size
#define DOC_AVG_IMG_SIZE_W 1024
#define DOC_AVG_IMG_SIZE_H 768

#define DOC_PIC_4M_LIMITS  (4 *1024 *1024 + 300)
#define DOC_AVG_IMG_4M_SIZE_H 614

#define MAX_NUM_FACES 10

#define DOC_RESIZE_PREV 0
#define DOC_RESIZE_CAPS 1

class DoCBase: public virtual RefBase
{
public:
	virtual void vfbdatacallback(void * inMem, int memSize, int prvW, int prvH) = 0;
	virtual void fbdatacallback(void* _FullImg, int fullWidth, int fullHeight) = 0;
	virtual void fddatacallback(void * inMem, int memSize) = 0;
	virtual void faceDetect(int * outArray, int outArraySize) = 0;
};


};
#endif

