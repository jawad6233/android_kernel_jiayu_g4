/*************************************************************
 * 
 * Copyright (C), 2008-2012, OPPO Mobile Comm Corp., Ltd
 * 
 * VENDOR_EDIT
 *
 * Date Created: 2012-06-12 11:10
 * 
 * Author: Lycan.Wang@Prd.Camera.CameraSrv
 * 
 * Description : NULL
 * 
 * ------------------ Revision History: ---------------------
 *      <author>        <date>          <desc>
 *
 *************************************************************/ 

#ifndef __SCALADO_PHOTOART_H__
#define __SCALADO_PHOTOART_H__

//scalado header
#include <utils/RefBase.h>
#include "oslfile.h"
#include "oslmem.h"
#include "caps_base.h"
#include "fm_speedview.h"
#include "fm_clearshot.h"
#include "fm_photoart.h"
#include "caps_urext.h"
#include "ctstream_file.h"
#include "fm_imagefusion.h"

#define SHOW_ERROR(errorCode)	\
	if(CERR_OK != errorCode)\
	{\
		LOGE("In file: (%s) , line: (%d) , return (%s)", __FILE__, __LINE__, scberror_cresultToString(errorCode));\
	}

#define ABS(x)    ((x) > 0 ? (x) : (-1*(x)))
typedef unsigned char Ipp8u;

namespace android {

typedef enum {
    RAW_CAPTURE_MODE,
    POSTVIEW_CAPTURE_MODE,
    RECEIVE_PREVIEW_MODE
}SCALADO_CAM_MODE;

typedef enum {
	LOMO_TUNNEL_EFFECT = 1, 
	LOMO_FISHEYE_EFFECT, 
	LOMO_2X2_LATTICE_EFFECT, 
	LOMO_THEME_COLOR_GREEN_EFFECT, 
	LOMO_THEME_COLOR_BLUE_EFFECT, 
	LOMO_THEME_COLOR_YELLOW_EFFECT, 
	LOMO_SOFTNESS_EFFECT, 
	LOMO_SKETCH_EFFECT_GRAY,
	LOMO_SKETCH_EFFECT_COLOR,
	LOMO_MIRROR_EFFECT,
	LOMO_XUJIAO,
	FUN_WARP_EFFECT_START = 20,
	FUN_FLIP_EFFECT, 
	FUN_ALIENSCUM_WARP_EFFECT,//jian xia ba
	FUN_BIGBUPPA_WARP_EFFECT, //zuozhongliang
	FUN_BOOZIEBOWL_WARP_EFFECT,
	FUN_CONEHEAD_WARP_EFFECT, //dabizi
	FUN_EYEBALLEDEVY_WARP_EFFECT,
	FUN_FUNKYFRESH_WARP_EFFECT,
	FUN_JELLYBELLY_WARP_EFFECT,
	FUN_PETTYPETIT_WARP_EFFECT, //maolianpang
	FUN_SPEEDYWEED_WARP_EFFECT,
} LOMO_EFFECT_TYPE;

typedef enum {
	SET_DEFAULT = 0,
	SET_RADIUS_UP,
	SET_RADIUS_DOWN,
	SET_ORIGIN_UP,
	SET_ORIGIN_DOWN,
	SET_ORIGIN_LEFT,
	SET_ORIGIN_RIGHT,
	SET_TRANSITION_SIZE_UP,
	SET_TRANSITION_SIZE_DOWN,
	MAX_TUNNEL_PARA_SETTING
} TUNNEL_PARA_SETTING;

enum PartEnum
{
    PART_NONE,
    
    PART_UP,
    PART_DOWN, 
    
    PART_1,
    PART_2,
    PART_3, 
    PART_4,
    
    PART_ALL, 
};
enum PartModeEnum
{
    TwoPartMode,
    FourPartMode,
    NonePartMode,
};

typedef struct {
	CTfloat origin_x; /*0 to 1, 相对WIDTH的比例*/
	CTfloat origin_y; /*0 to 1, 相对HEIGHT的比例*/
	CTfloat radius; /*0 to 1, 相对最长边的比例*/
	CTfloat transitionSize; /*0 to 1，过渡区域相对半径的比例*/
} TunnerParams;

typedef struct {
	CTfloat r; //[-1,1]
	CTfloat g; //[-1,1]
	CTfloat b; //[-1,1]
} ColorThemeParams;

const TunnerParams lomo_tunnel_para_default = { 0.5f, 0.5f, 0.25f, 0.5f };

const ColorThemeParams lomo_color_theme_red_para_default = { 0.4f, -0.08f,
		0.25f };
const ColorThemeParams lomo_color_theme_green_para_default = { -0.11f, 0.45f,
		0.12f };
const ColorThemeParams lomo_color_theme_blue_para_default = { 0.02f, 0.1f,
		0.81f };
const ColorThemeParams lomo_color_theme_yellow_para_default = { 0.15f, -0.02f,
		-0.30f };

class ScaladoPhotoArt: public virtual RefBase {
public:
	ScaladoPhotoArt();
	virtual ~ScaladoPhotoArt();
    CRESULT createSession(Ipp8u *pSrc, Ipp8u *pDes, 
            int inWidth, int inHeight);
	CRESULT createSession(Ipp8u *pSrc, Ipp8u *pDes,
        int inWidth, int inHeight, int outWidth, int outHeight);
	
    CRESULT applyEffect(int effectIdx, SCALADO_CAM_MODE scaladoCamMode, int rotation);
    CRESULT destroySession();

	void renderToOutBuffer();
	void renderToOutBuffer(int effect_id);

    void setPixelFormat(int mPixelFormat);
    void setFacing(int facing);
 
    void setBlurCntPoint(int x,int y,int w,int h,float f);
    void setZoomRatio(int zoomRatio);
    void setQuadEffect(int index);
    void setPartModeAndId(PartModeEnum partMode, PartEnum id);    
    void getValidRectByPartModeAndId(CTSize inputSize, CTRect & cropRect);
    CRESULT getActualViewSize(CTSize surSize, CTSize sizeIn, CTSize & sizeOut);
            
    void capture_blur_region_init(int w, int h);        
    static void onDestory();        
			
private:
    CTSession mSession;
    CRESULT mResult;
    CTBuffer mInBuffer;
    CTBuffer mOutBuffer;
    CTSize mInDims;
	CTSize mOutDims;
    CTImage* mPInImage;
    CTImage* mPOutImage;
    CTImage* mAYUVInImage;
    CTImage* mBlockImage[4];

    CTImage *mInputblurimage;
    CTImage *mCircleImage;
    CTImage *mMaskimage;    
    CTSession mSessionBlur;
    CTSession mSessionCircle;
    CTSession mSessionMask;
    bool isneedinit;
    CTPoint  mCntPoint;
    static CTImage *mCaptureMaskimage;

    bool mIsPointChange;
    int mXPoint;
    int mYPoint;
    CTSize   mSurSize;
    float    mCirMul; 
    float    mlastRatio;/*图片尺寸长宽比*/
    int mEffect[4];
    
	CRESULT applyQuadEffect();
    CRESULT applyTunnelEffect();
    CRESULT applyMirrorEffect(SCALADO_CAM_MODE mScaladoCamMode, int rotation);
    CRESULT getRotationStatus(int rotation, CAPS_ROTATION *pPreRotation, CAPS_ROTATION *pSubRotation);
    CRESULT applyRotateEffect(CAPS_ROTATION rotation);
    CRESULT applyWrapFromBufferEffect(LOMO_EFFECT_TYPE type, SCALADO_CAM_MODE scaladoCamMode, int rotation);

    CRESULT applyColorAdjust(const ColorThemeParams param);

    void calcPointAndModulus(CTSize imageSize, int facing);    
    CTImage * blur_region_init(int w, int h);
    CRESULT applyBlur(int w, int h, SCALADO_CAM_MODE scaladoCamMode);
    void release2X2Buffer();
	void resetParams();

    bool updateWarpBuffer(LOMO_EFFECT_TYPE type);
    CTBuffer mWrpBuffer;

    int mWrpFileIndex; 
    int mPixelFormat;
    int mZoomRatio;
    int mFacing;

    PartModeEnum    mPartMode;
    PartEnum        mPartId;
};

}
#endif
