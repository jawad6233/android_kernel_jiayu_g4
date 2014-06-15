/*************************************************************
 *
 * Copyright (C), 2008-2012, OPPO Mobile Comm Corp., Ltd
 *
 * VENDOR_EDIT
 *
 * Date Created: 2012-06-12 11:09
 *
 * Author: Lycan.Wang@Prd.Camera.CameraSrv
 *
 * Description : NULL
 *
 * ------------------ Revision History: ---------------------
 *      <author>        <date>          <desc>
 *
 *************************************************************/
#define LOG_TAG "Scalado_photoart"

#include <inc/CamUtils.h>

#include <IParamsManager.h>
#include <fcntl.h>
#include "utils.h"
#include "caps_uth.h"
#include "scalado_photoart.h"

#define  show_log  0

namespace android {

static const char IMAGE_DIR[] = "/system/media/images/scalado_res";
static const char* gBlurFileName = "/system/media/images/scalado_res/image/xuJiaoPhoto_400x400.png";
//char* gBlurFileName = "/mnt/sdcard/xuJiaoPhoto_400x400.png";

CTImage * ScaladoPhotoArt::mCaptureMaskimage = NULL;

ScaladoPhotoArt::ScaladoPhotoArt() :
    mSession(NULL),
    mPixelFormat(CM_YUV420P)
{
    resetParams();

    mWrpBuffer.data = NULL;
    mWrpBuffer.size = 0;
    mWrpFileIndex = FUN_WARP_EFFECT_START;
}

void ScaladoPhotoArt::resetParams()
{
    mResult = CERR_OK;
    mInDims = {0,0};
    mPInImage = NULL;
    mPOutImage = NULL;
    mAYUVInImage = NULL;

    for (int i = 0; i < 4; i++) {
        mBlockImage[i] = NULL;
    }

    mInputblurimage = NULL;
    mCircleImage = NULL;
    mSessionBlur = NULL;
    mSessionCircle = NULL;
    mSessionMask = NULL;
    mMaskimage = NULL;
    mCntPoint = {0, 0};
    mSurSize = {0, 0};
    mIsPointChange = false;
    mXPoint = 0;
    mYPoint = 0;
    mCirMul = 1.0f;
    isneedinit = true;
    mlastRatio = 1;
    mPartMode = NonePartMode; 
    // 不能加下面这句，否则会黑屏
    //	destroySession();
}

CRESULT ScaladoPhotoArt::createSession(Ipp8u *pSrc, Ipp8u *pDes,
        int inWidth, int inHeight) {

    if (pSrc == NULL || pDes == NULL || inWidth < 0 || inHeight < 0) {
        MY_LOGE("error in parameters");
        MY_LOGE("pSrc 0x%x, pDes 0x%x, inWidth %d, inHeight %d", pSrc ,pDes, inWidth, inHeight);
        return CERR_OK;
    }
    mInDims = {inWidth, inHeight};
    mOutDims = {inWidth, inHeight};

    float dim2Size = (float) caps_pixelBitSize((CTColormode) mPixelFormat) / 8;

    mInBuffer.size = inWidth * inHeight * dim2Size;
    mInBuffer.data = (CTuint8*) pSrc;
    mResult = caps_createImage(&mInBuffer, mInDims, mInDims.w * dim2Size,
            (CTColormode) mPixelFormat, &mPInImage);
    mResult = caps_newSessionWithRaw(mPInImage, &mSession);
    mOutBuffer.size = inWidth * inHeight * dim2Size;
    mOutBuffer.data = (CTuint8*) pDes;

    mResult = caps_createImage(&mOutBuffer, mInDims, mInDims.w * dim2Size,
            (CTColormode) mPixelFormat, &mPOutImage);

    return CERR_OK;
}

CRESULT ScaladoPhotoArt::createSession(Ipp8u *pSrc, Ipp8u *pDes,
        int inWidth, int inHeight, int outWidth, int outHeight) {

    if (pSrc == NULL || pDes == NULL || inWidth < 0 || inHeight < 0) {
        MY_LOGE("error in parameters");
        MY_LOGE("pSrc 0x%x, pDes 0x%x, inWidth %d, inHeight %d", pSrc ,pDes, inWidth, inHeight);
        return CERR_OK;
    }
    MY_LOGD("pSrc 0x%x, pDes 0x%x, inWidth %d, inHeight %d, outWidth %d, outHeight %d", 
        pSrc ,pDes, inWidth, inHeight, outWidth, outHeight);
        
    mInDims = {inWidth, inHeight};
    mOutDims = {outWidth, outHeight};         
    
    float dim2Size = (float) caps_pixelBitSize((CTColormode) mPixelFormat) / 8;

    mInBuffer.size = inWidth * inHeight * dim2Size;
    mInBuffer.data = (CTuint8*) pSrc;
    mResult = caps_createImage(&mInBuffer, mInDims, mInDims.w * dim2Size,
            (CTColormode) mPixelFormat, &mPInImage);
    mResult = caps_newSessionWithRaw(mPInImage, &mSession);
    mOutBuffer.size = outWidth * outHeight * dim2Size;
    mOutBuffer.data = (CTuint8*) pDes;

    mResult = caps_createImage(&mOutBuffer, mOutDims, mOutDims.w * dim2Size,
            (CTColormode) mPixelFormat, &mPOutImage);

    return CERR_OK;
}


CRESULT ScaladoPhotoArt::applyEffect(int effectIdx, SCALADO_CAM_MODE scaladoCamMode, int rotation){
    LOMO_EFFECT_TYPE type = (LOMO_EFFECT_TYPE)effectIdx;

    MY_LOGD_IF(show_log, "lomo type = %d", type);

    if (mSession == NULL) {
        MY_LOGE("mSession is null");
        return CERR_INVALID_PARAMETER;
    }

    //scalado zoom
    if (mZoomRatio != 100) {
        CTRect cropRect;
        cropRect.w = mInDims.w * 100 / mZoomRatio;
        cropRect.h = mInDims.h * 100 / mZoomRatio;
        cropRect.x = (mInDims.w - cropRect.w) / 2;
        cropRect.y = (mInDims.h - cropRect.h) / 2;
        mResult = caps_doCrop(mSession, cropRect);
    }

    switch (type) {
        case LOMO_MIRROR_EFFECT:
            mResult = applyMirrorEffect(scaladoCamMode, rotation);
            break;
        case LOMO_TUNNEL_EFFECT:
            mResult = applyTunnelEffect();
            break;
        case LOMO_XUJIAO:
            mResult = applyBlur(mInDims.w, mInDims.h, scaladoCamMode);
            break;
        case LOMO_THEME_COLOR_GREEN_EFFECT:
            mResult = applyColorAdjust(lomo_color_theme_green_para_default);
            break;
        case LOMO_THEME_COLOR_BLUE_EFFECT:
            mResult = applyColorAdjust(lomo_color_theme_blue_para_default);
            break;
        case LOMO_THEME_COLOR_YELLOW_EFFECT:
            mResult = applyColorAdjust(lomo_color_theme_yellow_para_default);
            break;
        case LOMO_SOFTNESS_EFFECT:
            mResult = caps_applyMilky(mSession);
            break;
        case LOMO_SKETCH_EFFECT_GRAY:
            mResult = caps_applySketch(mSession, SKETCH_GRAY);
            break;
        case LOMO_2X2_LATTICE_EFFECT:
            mResult = applyQuadEffect();
            break;
        case LOMO_SKETCH_EFFECT_COLOR:
            mResult = caps_applySketch(mSession, SKETCH_COLOR);
            break;
        case FUN_FLIP_EFFECT:
            mResult = caps_beginFlip(mSession);
            mResult = caps_setFlip(mSession, FLIP_VERTICAL);
            caps_endFlip(mSession);
            return mResult;
            break;
        case LOMO_FISHEYE_EFFECT:
        case FUN_CONEHEAD_WARP_EFFECT:
        case FUN_PETTYPETIT_WARP_EFFECT:
        case FUN_ALIENSCUM_WARP_EFFECT:
        case FUN_BIGBUPPA_WARP_EFFECT:
            return applyWrapFromBufferEffect( type, scaladoCamMode, rotation);
            break;
        case FUN_BOOZIEBOWL_WARP_EFFECT:
        case FUN_EYEBALLEDEVY_WARP_EFFECT:
        case FUN_FUNKYFRESH_WARP_EFFECT:
        case FUN_JELLYBELLY_WARP_EFFECT:
        case FUN_SPEEDYWEED_WARP_EFFECT:
            if (updateWarpBuffer(type)) {
               return caps_applyWarpFromBuffer(mSession, &mWrpBuffer);
            } else {
                MY_LOGE("unable to get valid wrp data");
                return CERR_FILE_NOT_FOUND;
            }
            break;
        default:
            MY_LOGE("applyEffect %d", type);
            break;
    }
    return mResult;
}

//change this method to map more wrp files
bool ScaladoPhotoArt::updateWarpBuffer(LOMO_EFFECT_TYPE type) {
    MY_LOGD_IF(show_log, "updateWarpBufferByType E");
    const char* wrpFileName;

    switch (type) {
        case FUN_ALIENSCUM_WARP_EFFECT:
            wrpFileName = "alienscum.wrp";
            break;
        case LOMO_FISHEYE_EFFECT:
            wrpFileName = "fisheye.wrp";
            break;
        case FUN_BIGBUPPA_WARP_EFFECT:
            wrpFileName = "bigbuppa.wrp";
            break;
        case FUN_BOOZIEBOWL_WARP_EFFECT:
            wrpFileName = "booziebowl.wrp";
            break;
        case FUN_CONEHEAD_WARP_EFFECT:
            wrpFileName = "conehead.wrp";
            break;
        case FUN_EYEBALLEDEVY_WARP_EFFECT:
            wrpFileName = "eyeballedevy.wrp";
            break;
        case FUN_FUNKYFRESH_WARP_EFFECT:
            wrpFileName = "funkyfresh.wrp";
            break;
        case FUN_JELLYBELLY_WARP_EFFECT:
            wrpFileName = "jellybelly.wrp";
            break;
        case FUN_PETTYPETIT_WARP_EFFECT:
            wrpFileName = "pettypetit.wrp";
            break;
        case FUN_SPEEDYWEED_WARP_EFFECT:
            wrpFileName = "speedyweed.wrp";
            break;
        default:
            MY_LOGE("unknown wrp type");
            return false;
    }

    char path[100];
    memset(path, sizeof(path) / sizeof(char), 0);
    sprintf(path, "%s/wrp_files/%s", IMAGE_DIR, wrpFileName);
    int fd = open(path, 0);
    close(fd);

    if (fd  == -1) {
        MY_LOGW("File [%s] not found ,current wrp file not updated", path);
        return false;
    } else {
        if (mWrpFileIndex == type){
            MY_LOGD("no need to update wrp file");
            return true;
        }
        if (mWrpBuffer.data != NULL) {
            //release last CTBuffer
            delete[] mWrpBuffer.data;
        }
        CTFile *file;
        oslfile_open(path, FOF_ACCESS_READ | FOF_MODE_BINARY, &file);
        uint32_t size = oslfile_size(file);
        mWrpBuffer.data = new uint8_t[size];
        mWrpBuffer.size = size;
        oslfile_read(file, mWrpBuffer.data, size);
        oslfile_close(file);
        return true;
    }
    MY_LOGD_IF(show_log, "updateWarpBufferByType X");
}

//calculate the actual center point and circle 
void ScaladoPhotoArt::calcPointAndModulus(CTSize imageSize, int facing) {
    float a = 0;
    float b = 0;
    int surfaceW = mSurSize.w;
    int surfaceH = mSurSize.h;
    
    if (mSurSize.w > 0 && mSurSize.h > 0) {
        a = (float)imageSize.w / (float)surfaceW;
        b = (float)imageSize.h / (float)surfaceH;

        if (a > b) {
            mCirMul = mCirMul*a;
        } else {
            mCirMul = mCirMul*b;
        }

        if (mCirMul < 0.1){ //this value must be larger than 0.1
            mCirMul = 0.1;
        }

        mCntPoint.x = a * mXPoint;
        mCntPoint.y = b * mYPoint;      

        //back camera need mirror the position
        if (facing == 1) {
            mCntPoint.x = mInDims.w - mCntPoint.x;
        }

        MY_LOGD_IF(show_log, "mCntPoint(%d,%d)  surSize(%d,%d) mCirMul(%f)",
            mCntPoint.x, mCntPoint.y, surfaceW, surfaceH, mCirMul);
    }
}

//do the init job before capture proc 
void ScaladoPhotoArt::capture_blur_region_init(int w, int h) {
    CRESULT res;
    CTuint32 scanline = 0;
    CTColor color = { 255, 255, 255, 255 };
    CTSize imgsize = { w, h };
    CTSize blur_imagesize = { w, h };
    CTSize dims =  { 0, 0 };
    CAPS_CLIPART clipart;
    
    CTImage * circleImage;
    CTSession sessionCircle;
    CTSession sessionMask;

    mInDims = {w, h};

    res = caps_newSessionWithFile(gBlurFileName, IMAGE_FORMAT_PNG, &sessionCircle);
    MY_LOGE_IF((CERR_OK != res), "res:%s", scberror_cresultToString(mResult));
    
    res = caps_getCurrentImageSize(sessionCircle, &dims);
    scanline = caps_pixelByteSize(CM_ARGB8888) * dims.w;
    res = caps_createImage(0, dims, scanline, CM_ARGB8888, &circleImage);
    res = caps_renderAsRawToBuffer(sessionCircle, circleImage,CAPS_OUTPUT_PRESERVE_AR, 0);

    MY_LOGE_IF((CERR_OK != res), "res:%s", scberror_cresultToString(mResult));
    
    if (mCaptureMaskimage != NULL) {
        caps_destroyImage(mCaptureMaskimage);
        mCaptureMaskimage = NULL;
    }

    scanline = caps_pixelBitSize(CM_GRAY8) * imgsize.w;
    res = caps_createImage(0, imgsize, scanline, CM_GRAY8, &mCaptureMaskimage);
    MY_LOGD("mCaptureMaskimage imgsize(%dx%d) scanline(%d) size(%d)",
        imgsize.w, imgsize.h, scanline, mCaptureMaskimage->pixels->size);

    res = caps_newSessionWithColor(color, imgsize, &sessionMask);

    //the mirror position have caculate outside, here we donot care and just set 0
    calcPointAndModulus(imgsize, 0);   
    
    MY_LOGD("imgSize(%d,%d), mCntPoint(%d,%d) surSize(%d,%d) mCirMul(%f) mFacing(%d)",
            w, h, mCntPoint.x, mCntPoint.y, mSurSize.w, mSurSize.h, mCirMul, mFacing);
            
    res = caps_beginClipartFromRaw(sessionMask, circleImage,
            OWNERSHIP_EXTERNAL, mCntPoint, mCirMul, 0.0f, &clipart);

    res = caps_endClipart(clipart);
            
    res = caps_renderAsRawToBuffer(sessionMask, mCaptureMaskimage,
            CAPS_OUTPUT_PRESERVE_AR, 0);   
            
    MY_LOGE_IF((CERR_OK != res), "res:%s", scberror_cresultToString(mResult));
    
    caps_destroySession(sessionCircle);
    caps_destroySession(sessionMask);
    caps_destroyImage(circleImage);
#if 0
    android::MtkCamUtils::saveBufToFile("/mnt/sdcard/captureMask",
                  mCaptureMaskimage->pixels->data,
                  mCaptureMaskimage->pixels->size);
#endif
}

CTImage * ScaladoPhotoArt::blur_region_init(int w, int h) {

    CRESULT res;
    CTuint32 scanline = 0;
    CTColor color = { 255, 255, 255, 255 };
    CTSize imgsize = { w, h };
    CTSize dims = { 0, 0 };
    CAPS_CLIPART clipart;
    
    if (isneedinit) {
        isneedinit = false;
        if (mInputblurimage != NULL) {
            caps_destroyImage(mInputblurimage);
            mInputblurimage = NULL;
        }
        if (mCircleImage != NULL) {
            caps_destroyImage(mCircleImage);
            mCircleImage = NULL;
        }
        if (mMaskimage != NULL) {
            caps_destroyImage(mMaskimage);
            mMaskimage = NULL;
        }
        if (mSessionMask != NULL) {
            caps_destroySession(mSessionMask);
            mSessionMask = NULL;
        }
    }

    //  blur_imagesize.w = blur_imagesize.w >> 2;   //scalado vincent 20110917
    // blur_imagesize.h = blur_imagesize.h >> 2;    //scalado vincent 20110917

    if (mInputblurimage == NULL) {
        scanline = caps_pixelBitSize(CM_AYUV4444) * imgsize.w / 8;
        res = caps_createImage(0, imgsize, scanline, CM_AYUV4444,
                &mInputblurimage);
    }

    if (mSessionCircle == NULL) {
        res = caps_newSessionWithFile(gBlurFileName, IMAGE_FORMAT_PNG,
                &mSessionCircle);
    }

    if (mCircleImage == NULL) {
        res = caps_getCurrentImageSize(mSessionCircle, &dims);
        scanline = caps_pixelByteSize(CM_ARGB8888) * dims.w;
        res = caps_createImage(0, dims, scanline, CM_ARGB8888, &mCircleImage);
        res = caps_renderAsRawToBuffer(mSessionCircle, mCircleImage,
                CAPS_OUTPUT_PRESERVE_AR, 0);
    }

    if (mMaskimage == NULL) {
        scanline = caps_pixelBitSize(CM_GRAY8) * imgsize.w;
        res = caps_createImage(0, imgsize, scanline, CM_GRAY8, &mMaskimage);
    }    

    if ((mIsPointChange == true) || (mSessionMask == NULL)){
        if (mSessionMask != NULL) {
            caps_destroySession(mSessionMask);
            mSessionMask = NULL;
        }
        res = caps_newSessionWithColor(color, imgsize, &mSessionMask);

        calcPointAndModulus(mInDims, mFacing);
        res = caps_beginClipartFromRaw(mSessionMask, mCircleImage,
                OWNERSHIP_EXTERNAL, mCntPoint, mCirMul, 0.0f, &clipart);

        res = caps_endClipart(clipart);
                
        res = caps_renderAsRawToBuffer(mSessionMask, mMaskimage,
                CAPS_OUTPUT_PRESERVE_AR, 0);
                
        //usleep(10000);
    }

    return mMaskimage;
}

CRESULT  ScaladoPhotoArt::applyBlur(int w, int h, SCALADO_CAM_MODE scaladoCamMode) {
    CRESULT res;

    /*oppo xinlan.he 这里为了使预览和拍照照片的模糊程度相符合
     *数字(480 * 2)可能需要调整,
     */
    CAPS_BLUR_LEVEL blvl = (CAPS_BLUR_LEVEL)(BLUR_HIGH + w / (480 * 2));//BLUR_MAX;

    float ratio = mlastRatio;
    CTSession currSession = mSession;
    CTSession tmpSession = NULL;
    CTImage * maskimage = NULL;    

    MY_LOGD_IF(show_log, "+");

    if (mInDims.h > 0) {
        ratio = (float)mInDims.w / (float)mInDims.h;
    }

    if (ABS(mlastRatio - ratio) > 0.01) {
        MY_LOGD("isneedinit = true  mlastRatio = %f  ratio = %f", mlastRatio, ratio);
        mlastRatio = ratio;
        //长宽比有变化,需重新初始化虚焦相关值
        isneedinit = true;
    }

    if (mZoomRatio != 100) {
        res = caps_newSessionWithRaw(mPOutImage, &tmpSession);
        res = caps_renderAsRawToBuffer(mSession, mPOutImage,
                CAPS_OUTPUT_PRESERVE_AR, 0);
        res = caps_signalSessionEvent(tmpSession, EVENT_SOURCEDATA_CHANGE);
        if (mSessionBlur == NULL) {
            res = caps_newSessionWithRaw(mPOutImage, &mSessionBlur);
        }
        currSession = tmpSession;
    }
    
    if (scaladoCamMode == RAW_CAPTURE_MODE && mCaptureMaskimage != NULL) {
        MY_LOGD("use prepared mask image");
        maskimage = mCaptureMaskimage;
        
        if (mInputblurimage == NULL) {
            CTuint32 scanline = 0;
            scanline = caps_pixelBitSize(CM_AYUV4444) * mInDims.w / 8;
            res = caps_createImage(0, mInDims, scanline, CM_AYUV4444,
                    &mInputblurimage);
        }
    } else {    
        maskimage = blur_region_init(w, h);
    }
    
    if (tmpSession != NULL) {
        caps_destroySession(tmpSession);
    }
    
    if (mSessionBlur == NULL) {
        res = caps_newSessionWithRaw(mPInImage, &mSessionBlur);
    }

    res = caps_beginBlur(mSessionBlur);
    
    if (scaladoCamMode == RAW_CAPTURE_MODE && mCaptureMaskimage != NULL) {
        CTRect region;
        getValidRectByPartModeAndId(mInDims, region);
        res = caps_setBlur(mSessionBlur, blvl, &region);
    } else {
        res = caps_setBlur(mSessionBlur, blvl, 0);
    }
    
    caps_endBlur(mSessionBlur);

    res = caps_renderAsRawToBuffer(mSessionBlur, mInputblurimage,
            CAPS_OUTPUT_PRESERVE_AR, 0);

    res = caps_insertImageFromRawMask(mSession, mInputblurimage,
            OWNERSHIP_EXTERNAL, maskimage, OWNERSHIP_EXTERNAL, 0);
    //usleep(10000);  

    mIsPointChange = false;
    MY_LOGD_IF(show_log, "-");
    
    return res;
}


CRESULT ScaladoPhotoArt::getRotationStatus(int rotation, CAPS_ROTATION *pPreRotation, CAPS_ROTATION *pSubRotation){
    switch ( rotation ) {
        case 0:
            *pPreRotation = ROTATE_TO_0;
            *pSubRotation = ROTATE_TO_0;
            break;
        case 90:
            *pPreRotation = ROTATE_TO_270;
            *pSubRotation = ROTATE_TO_90;
            break;
        case 180:
            *pPreRotation = ROTATE_TO_180;
            *pSubRotation = ROTATE_TO_180;
            break;
        case 270:
            *pPreRotation = ROTATE_TO_90;
            *pSubRotation = ROTATE_TO_270;
            break;
        default:
            MY_LOGE("invalid parameters rotation = %d", rotation);
            return CERR_INVALID_PARAMETER;
            break;
    }
    return CERR_OK;
}

CRESULT ScaladoPhotoArt::applyRotateEffect(CAPS_ROTATION rotation){
    mResult = CERR_OK;
    if ( rotation != ROTATE_TO_0 ) {
        mResult = caps_beginRotate(mSession);
        MY_LOGE_IF((CERR_OK != mResult), "-1-result:%s", scberror_cresultToString(mResult));
        mResult = caps_setRotate(mSession, rotation);
        MY_LOGE_IF((CERR_OK != mResult), "-2-result:%s", scberror_cresultToString(mResult));
        caps_endRotate(mSession);
    }
    return mResult;
}

CRESULT ScaladoPhotoArt::applyMirrorEffect(SCALADO_CAM_MODE scaladoCamMode, int rotation){
    CAPS_ROTATION preRotation = ROTATE_TO_0;
    CAPS_ROTATION subRotation = ROTATE_TO_0;
    bool bRotate = false;

    MY_LOGD_IF(show_log, "scaladoCamMode = %d, rotation = %d",scaladoCamMode, rotation);
    MY_LOGD_IF(show_log, "mInDims.w = %d, mInDims.h = %d", mInDims.w, mInDims.h);

    if ( scaladoCamMode == RAW_CAPTURE_MODE) {
        getRotationStatus(rotation, &preRotation, &subRotation);
    }

    mResult = applyRotateEffect(preRotation);
    MY_LOGE_IF((CERR_OK != mResult), "-1-result:%s", scberror_cresultToString(mResult));

    mResult = caps_applyMirror(mSession);
    MY_LOGE_IF((CERR_OK != mResult), "-2-result:%s", scberror_cresultToString(mResult));

    mResult = applyRotateEffect(subRotation);
    MY_LOGE_IF((CERR_OK != mResult), "-3-result:%s", scberror_cresultToString(mResult));

    return mResult;

}

CRESULT ScaladoPhotoArt::applyWrapFromBufferEffect(LOMO_EFFECT_TYPE type, SCALADO_CAM_MODE scaladoCamMode, int rotation){
    CAPS_ROTATION preRotation = ROTATE_TO_0;
    CAPS_ROTATION subRotation = ROTATE_TO_0;

    MY_LOGD_IF(show_log, "scaladoCamMode = %d, rotation = %d",scaladoCamMode, rotation);
    MY_LOGD_IF(show_log, "mInDims.w = %d, mInDims.h = %d", mInDims.w, mInDims.h);

    if ( scaladoCamMode == RAW_CAPTURE_MODE) {
        getRotationStatus(rotation, &preRotation, &subRotation);
    }

    mResult = applyRotateEffect(preRotation);
    MY_LOGE_IF((CERR_OK != mResult), "result:%s", scberror_cresultToString(mResult));

    if (updateWarpBuffer(type)) {
        mResult = caps_applyWarpFromBuffer(mSession, &mWrpBuffer);
        MY_LOGE_IF((CERR_OK != mResult), "result:%s", scberror_cresultToString(mResult));
    }

    mResult = applyRotateEffect(subRotation);
    MY_LOGE_IF((CERR_OK != mResult), "result:%s", scberror_cresultToString(mResult));

    return mResult;
}

CRESULT ScaladoPhotoArt::applyTunnelEffect()
{
    CTPoint pos;
    CTuint32 radius;
    CAPS_SPOTLIGHT spotlight;
    CTfloat transitionSize = lomo_tunnel_para_default.transitionSize;

    //mInDims.w wlz modify for scalado zoom
    pos.x = (CTint32) (lomo_tunnel_para_default.origin_x
            * (CTfloat) (mInDims.w * 100 / mZoomRatio));
    pos.y = (CTint32) (lomo_tunnel_para_default.origin_y
            * (CTfloat) (mInDims.h * 100 / mZoomRatio));

    if (mInDims.w > mInDims.h) {
        radius = (CTuint32) (lomo_tunnel_para_default.radius
                * (CTfloat) mInDims.w * 100 / mZoomRatio);
    } else {
        radius = (CTuint32) (lomo_tunnel_para_default.radius
                * (CTfloat) mInDims.h * 100 / mZoomRatio);
    }

    #warning "[TODO] lycan why?"
    if (mInDims.w == 3264) {
        transitionSize = 0.9 * transitionSize;
    }

    mResult = caps_beginSpotlight(mSession, pos, radius, transitionSize,
            &spotlight);
    mResult = caps_endSpotlight(spotlight);
    return CERR_OK;

    
}


CRESULT ScaladoPhotoArt::getActualViewSize(
        CTSize surSize, CTSize sizeIn, CTSize & sizeOut) {
    sizeOut.w = sizeIn.w;
    sizeOut.h = sizeIn.h;

    if (surSize.w > 0 && surSize.h > 0) {
        float a, b;
        a = (float)sizeIn.w / (float)surSize.w;
        b = (float)sizeIn.h / (float)surSize.h;
        if (a < b) {
            sizeOut.h = surSize.h * sizeIn.w / surSize.w;
        } else {
            sizeOut.w = surSize.w * sizeIn.h / surSize.h;
        }
        MY_LOGD_IF(show_log, "a(%f) b(%f) surSize(%d %d) sizeIn(%d %d), sizeOut(%d, %d)\n",
            a, b, surSize.w, surSize.h, sizeIn.w, sizeIn.h, sizeOut.w, sizeOut.h);
        return CERR_OK;
    }
    return CERR_INVALID_PARAMETER;
}

CRESULT ScaladoPhotoArt::applyQuadEffect() {

    CTRect cropRect;
    CTSize blockSize = { mInDims.w / 2, mInDims.h / 2};

    MY_LOGD_IF(show_log, "blockSize(%dx%d) mSurSize(%dx%d)",
            blockSize.w, blockSize.h, mSurSize.w, mSurSize.h);
    if (mBlockImage == NULL || mBlockImage[0] == NULL) {
        MY_LOGD("first init mBlockImage\n");
        for (int i = 0; i < 4; i++) {
            mResult = caps_createImage(NULL, blockSize,
                    blockSize.w * caps_pixelByteSize(CM_AYUV4444), CM_AYUV4444,
                    &mBlockImage[i]);
        }
    }
    if (mAYUVInImage == NULL) {
        caps_createImage(NULL, mInDims, mInDims.w * caps_pixelByteSize(
                    CM_AYUV4444), CM_AYUV4444, &mAYUVInImage);
    }

    for (int i = 0; i < 4; i++) {
        if (mEffect[i] != LOMO_XUJIAO) {
            mResult = applyEffect(mEffect[i], RECEIVE_PREVIEW_MODE, 0);
        }
        mResult = caps_renderAsRawToBuffer(mSession, mBlockImage[i],
                CAPS_OUTPUT_PRESERVE_AR,  0);
        caps_undoAll(mSession);
        if (mEffect[i] == LOMO_XUJIAO) {
            CTSize blockBlurSize = {0, 0};
            getActualViewSize(mSurSize, blockSize, blockBlurSize);
            imageBlur(mBlockImage[i]->pixels->data, 3, blockBlurSize.w, blockBlurSize.h);
        }
    }
    destroySession();


    mergeImage(mBlockImage[0]->pixels->data,
            mBlockImage[1]->pixels->data,
            mBlockImage[2]->pixels->data,
            mBlockImage[3]->pixels->data,
            mAYUVInImage->pixels->data,
            blockSize.w,
            blockSize.h);

    caps_newSessionWithRaw(mAYUVInImage, &mSession);

    return CERR_OK;
}

CRESULT ScaladoPhotoArt::applyColorAdjust(const ColorThemeParams param) 
{
    mResult = caps_beginRGBColorAdjust(mSession);
    mResult = caps_setRGBColorAdjust(mSession, param.r, param.g, param.b);
    caps_endRGBColorAdjust(mSession);
    return CERR_OK;
}

void ScaladoPhotoArt::renderToOutBuffer() 
{
    MY_LOGD_IF(show_log, "+");
    mResult = caps_renderAsRawToBuffer(mSession, mPOutImage,
            CAPS_OUTPUT_PRESERVE_AR, 0);
    MY_LOGE_IF((CERR_OK != mResult), "- result:%s", scberror_cresultToString(mResult));
}

void ScaladoPhotoArt::getValidRectByPartModeAndId(CTSize inputSize, CTRect & validRect) 
{
    switch(mPartMode) {      
    case TwoPartMode: {
        switch (mPartId) {
        case PART_UP:        
            validRect.x = 0;
            validRect.y = 0;
            break;
        case PART_DOWN:
            validRect.x = 0;
            validRect.y = inputSize.h / 2;
            break;
        default:
            MY_LOGE("wrong param !!");
            return;
        }            
        validRect.w = inputSize.w;
        validRect.h = inputSize.h / 2;
        break;
    }
    case FourPartMode: {
        switch (mPartId) {
        case PART_1:        
            validRect.x = 0;
            validRect.y = 0;
            break;
        case PART_2:        
            validRect.x = 0;
            validRect.y = inputSize.h / 4;
            break;
        case PART_3:
            validRect.x = 0;
            validRect.y = inputSize.h / 2;
            break;
        case PART_4:
            validRect.x = 0;
            validRect.y = inputSize.h * 3 / 4;
            break;
        default:
            MY_LOGE("wrong param !!");
            return;
        }
        validRect.w = inputSize.w;
        validRect.h = inputSize.h / 4;
        break;
    }
    default :
        MY_LOGE("wrong param !!");
        return;
    }
}

void ScaladoPhotoArt::renderToOutBuffer(int effect_id) 
{
    CTRect cropRect;
    
    MY_LOGD_IF(1, "effect_id=%d partMode=%d part=%d", effect_id, mPartMode, mPartId);
    
    getValidRectByPartModeAndId(mInDims, cropRect);
    MY_LOGD_IF(show_log, "caps_doCrop(x,y,w,h)(%d,%d,%d,%d)", cropRect.x, cropRect.y, cropRect.w,cropRect.h);
    
    mResult = caps_doCrop(mSession, cropRect);
    
    mResult = caps_renderAsRawToBuffer(mSession, mPOutImage,CAPS_OUTPUT_PRESERVE_AR, 0);
    
    MY_LOGE_IF((CERR_OK != mResult), "result:%s", scberror_cresultToString(mResult));
}


CRESULT ScaladoPhotoArt::destroySession() {
    MY_LOGD_IF(show_log, "+");
    if (mSession != NULL) {
        caps_destroySession(mSession);
        mSession = NULL;
    }
    if (mSessionBlur != NULL) {
        caps_destroySession(mSessionBlur);
        mSessionBlur = NULL;
    }
    MY_LOGD_IF(show_log, "-");
    return CERR_OK;
}

void ScaladoPhotoArt::release2X2Buffer() {
    if (mBlockImage[0] == NULL) {
        return;
    }

    for (int i = 0; i < 4; i++){
        if (mBlockImage[i] != NULL){
            caps_destroyImage(mBlockImage[i]);
            mBlockImage[i] = NULL;
        }
    }
}

ScaladoPhotoArt::~ScaladoPhotoArt() {
    MY_LOGD("+");

    release2X2Buffer();
    if (mWrpBuffer.data != NULL) {
        delete[] mWrpBuffer.data;
        mWrpBuffer.data = NULL;
        mWrpFileIndex = FUN_WARP_EFFECT_START;
    }
    
    if (mInputblurimage != NULL){
        caps_destroyImage(mInputblurimage);
    }
    if (mCircleImage != NULL){
        caps_destroyImage(mCircleImage);
    }
    if (mMaskimage != NULL){
        caps_destroyImage(mMaskimage);
    }

    if (mAYUVInImage != NULL) {
        caps_destroyImage(mAYUVInImage);
        mAYUVInImage = NULL;
    } 

    if (mSessionCircle != NULL) {
        caps_destroySession(mSessionCircle);
        mSessionCircle = NULL;
    }
    if (mSessionMask != NULL) {
        caps_destroySession(mSessionMask);
        mSessionMask = NULL;
    }
    destroySession();
    MY_LOGD("-");
}

void ScaladoPhotoArt::setPixelFormat(int pixelFormat) {
    mPixelFormat = pixelFormat;
}

void ScaladoPhotoArt::setFacing(int facing){
    mFacing = facing;
}
void ScaladoPhotoArt::setZoomRatio(int zoomRatio){
    mZoomRatio = zoomRatio;
    MY_LOGD_IF(show_log, "set mZoomRatio=%d", mZoomRatio);
}

void ScaladoPhotoArt::setBlurCntPoint(int x,int y,int w,int h,float f) {

    mSurSize.w = w;
    mSurSize.h = h;

    if ((x == mXPoint) && (y == mYPoint) && (f == mCirMul)){
        mIsPointChange = false;
    } else {
        mXPoint = x;
        mYPoint = y;
        mCirMul = f;
        mCntPoint.x = mXPoint;
        mCntPoint.y = mYPoint;

        mIsPointChange = true;
    }
    MY_LOGD_IF(show_log, "mXPoint=%d  mYPoint=%d mSurSize.w=%d mSurSize.h=%d mCirMul=%f",
            mXPoint, mYPoint, mSurSize.w, mSurSize.h, mCirMul);
}

void ScaladoPhotoArt::setQuadEffect(int index){
    if (mFacing) {
        mEffect[1] = index & 0xFF;
        mEffect[0] = (index >> 8) & 0xFF;
        mEffect[3] = (index >> 16) & 0xFF;
        mEffect[2] = (index >> 24) & 0xFF;

    } else {
        mEffect[0] = index & 0xFF;
        mEffect[1] = (index >> 8) & 0xFF;
        mEffect[2] = (index >> 16) & 0xFF;
        mEffect[3] = (index >> 24) & 0xFF;
    }
}

void ScaladoPhotoArt::setPartModeAndId(PartModeEnum partMode, PartEnum id){
    mPartMode = partMode;
    mPartId = id;
}

void ScaladoPhotoArt::onDestory() {
    MY_LOGD("mCaptureMaskimage(0x%x)", mCaptureMaskimage);
    if (mCaptureMaskimage != NULL) {
        caps_destroyImage(mCaptureMaskimage);
        mCaptureMaskimage = NULL;
    }
}

}
