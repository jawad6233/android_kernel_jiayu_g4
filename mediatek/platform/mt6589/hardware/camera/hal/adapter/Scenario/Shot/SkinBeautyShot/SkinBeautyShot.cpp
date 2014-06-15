/*************************************************************
 * 
 * Copyright (C), 2008-2013, OPPO Mobile Comm Corp., Ltd
 *  
 * VENDOR_EDIT   CUSTOM_PROCESS
 *
 * Date Created: 2013-02-22
 * 
 * Author: wuhongru@Prd.Camera.CameraSrv
 * 
 * Description : NULL
 * 
 * ------------------ Revision History: ---------------------
 *      <author>        <date>          <desc>
 *
 *************************************************************/ 
#define LOG_TAG "MtkCam/Shot"
//
#include <common/CamLog.h>
#include <common/CamTypes.h>
#include <common/hw/hwstddef.h>
//
#include <common/camutils/CamFormat.h>
#include <common/camutils/CamInfo.h>
#include <common/camutils/CameraProfile.h>

//
#include <drv/sensor_hal.h>
//
#include <common/IParamsManager.h>
#include <camshot/ICamShot.h>
#include <camshot/ISingleShot.h>
//
#include <Shot/IShot.h>
//
#include "doc_hacker.h"


#include "ImpShot.h"
#include "SkinBeautyShot.h"
//
#include "CamUtils.h"

using namespace android;
using namespace NSShot;
using namespace MtkCamUtils;

/******************************************************************************
 *
 ******************************************************************************/
#define MY_LOGV(fmt, arg...)        CAM_LOGV("(%d)(%s)[%s] "fmt, ::gettid(), getShotName(), __FUNCTION__, ##arg)
#define MY_LOGD(fmt, arg...)        CAM_LOGD("(%d)(%s)[%s] "fmt, ::gettid(), getShotName(), __FUNCTION__, ##arg)
#define MY_LOGI(fmt, arg...)        CAM_LOGI("(%d)(%s)[%s] "fmt, ::gettid(), getShotName(), __FUNCTION__, ##arg)
#define MY_LOGW(fmt, arg...)        CAM_LOGW("(%d)(%s)[%s] "fmt, ::gettid(), getShotName(), __FUNCTION__, ##arg)
#define MY_LOGE(fmt, arg...)        CAM_LOGE("(%d)(%s)[%s] "fmt, ::gettid(), getShotName(), __FUNCTION__, ##arg)
#define MY_LOGA(fmt, arg...)        CAM_LOGA("(%d)(%s)[%s] "fmt, ::gettid(), getShotName(), __FUNCTION__, ##arg)
#define MY_LOGF(fmt, arg...)        CAM_LOGF("(%d)(%s)[%s] "fmt, ::gettid(), getShotName(), __FUNCTION__, ##arg)
//
#define MY_LOGV_IF(cond, ...)       do { if ( (cond) ) { MY_LOGV(__VA_ARGS__); } }while(0)
#define MY_LOGD_IF(cond, ...)       do { if ( (cond) ) { MY_LOGD(__VA_ARGS__); } }while(0)
#define MY_LOGI_IF(cond, ...)       do { if ( (cond) ) { MY_LOGI(__VA_ARGS__); } }while(0)
#define MY_LOGW_IF(cond, ...)       do { if ( (cond) ) { MY_LOGW(__VA_ARGS__); } }while(0)
#define MY_LOGE_IF(cond, ...)       do { if ( (cond) ) { MY_LOGE(__VA_ARGS__); } }while(0)
#define MY_LOGA_IF(cond, ...)       do { if ( (cond) ) { MY_LOGA(__VA_ARGS__); } }while(0)
#define MY_LOGF_IF(cond, ...)       do { if ( (cond) ) { MY_LOGF(__VA_ARGS__); } }while(0)


/******************************************************************************
 *
 ******************************************************************************/
extern "C"
sp<IShot>
createInstance_SkinBeautyShot(
    char const*const    pszShotName, 
    uint32_t const      u4ShotMode, 
    int32_t const       i4OpenId,
    sp<IParamsManager>  pParamsMgr
)
{
    sp<IShot>       pShot = NULL;
    sp<SkinBeautyShot>  pImpShot = NULL;
    //
    //  (1.1) new Implementator.
    pImpShot = new SkinBeautyShot(pszShotName, u4ShotMode, i4OpenId, pParamsMgr);
    if  ( pImpShot == 0 ) {
        CAM_LOGE("[%s] new SkinBeautyShot", __FUNCTION__);
        goto lbExit;
    }
    //
    //  (1.2) initialize Implementator if needed.
    if  ( ! pImpShot->onCreate() ) {
        CAM_LOGE("[%s] onCreate()", __FUNCTION__);
        goto lbExit;
    }
    //
    //  (2)   new Interface.
    pShot = new IShot(pImpShot);
    if  ( pShot == 0 ) {
        CAM_LOGE("[%s] new IShot", __FUNCTION__);
        goto lbExit;
    }
    //
lbExit:
    //
    //  Free all resources if this function fails.
    if  ( pShot == 0 && pImpShot != 0 ) {
        pImpShot->onDestroy();
        pImpShot = NULL;
    }
    //
    return  pShot;
}

static bool splitInt(String8 const& s8Input, Vector<int>& rOutput)
{
    rOutput.clear();

    if  ( s8Input.isEmpty() )
    {
        CAM_LOGV("empty string");
        return  false;
    }

    char const*start = s8Input.string();
    char *end = NULL;
    do {
        int value = ::strtol(start, &end, 10);
        if  ( start == end ) {
            CAM_LOGV("no digits in str:%s", s8Input.string());
            return  false;
        }
        rOutput.push_back(value);
        CAM_LOGV("%d", value);
        start = end + 1;
    } while ( end && *end );

    return  (rOutput.size() > 0);
}

/******************************************************************************
 *  This function is invoked when this object is firstly created.
 *  All resources can be allocated here.
 ******************************************************************************/
bool SkinBeautyShot::onCreate()
{
#warning "[TODO] SkinBeautyShot::onCreate()"
    bool ret = true;
    return ret;
}


/******************************************************************************
 *  This function is invoked when this object is ready to destryoed in the
 *  destructor. All resources must be released before this returns.
 ******************************************************************************/
void SkinBeautyShot::onDestroy()
{
#warning "[TODO] SkinBeautyShot::onDestroy()"
}


/******************************************************************************
 *
 ******************************************************************************/
SkinBeautyShot::SkinBeautyShot(
    char const*const pszShotName, 
    uint32_t const u4ShotMode, 
    int32_t const i4OpenId, 
    sp<IParamsManager>  pParamsMgr
)
    : ImpShot(pszShotName, u4ShotMode, i4OpenId)
{
    mpParamsMgr = pParamsMgr;
}


/******************************************************************************
 *
 ******************************************************************************/
SkinBeautyShot::~SkinBeautyShot()
{
    mpParamsMgr = NULL;
}


/******************************************************************************
 *
 ******************************************************************************/
bool SkinBeautyShot::sendCommand(
                                    uint32_t const  cmd, 
                                    uint32_t const  arg1, 
                                    uint32_t const  arg2)
{
    AutoCPTLog cptlog(Event_Shot_sendCmd, cmd, arg1);
    bool ret = true;
    //
    switch  (cmd)
    {
    //  This command is to reset this class. After captures and then reset, 
    //  performing a new capture should work well, no matter whether previous 
    //  captures failed or not.
    //
    //  Arguments:
    //          N/A
    case eCmd_reset:
        ret = onCmd_reset();
        break;

    //  This command is to perform capture.
    //
    //  Arguments:
    //          N/A
    case eCmd_capture:
        ret = onCmd_capture();
        break;

    //  This command is to perform cancel capture.
    //
    //  Arguments:
    //          N/A
    case eCmd_cancel:
        onCmd_cancel();
        break;
    //
    default:
        ret = ImpShot::sendCommand(cmd, arg1, arg2);
    }
    //
    return ret;
}

/******************************************************************************
 *
 ******************************************************************************/
bool SkinBeautyShot::onCmd_reset()
{
#warning "[TODO] SkinBeautyShot::onCmd_reset()"
    bool ret = true;
    return ret;
}

/******************************************************************************
 *
 ******************************************************************************/
bool SkinBeautyShot::onCmd_capture()
{ 
    AutoCPTLog cptlog(Event_Shot_capture);
    MBOOL ret = MTRUE; 
    NSCamShot::ISingleShot *pSingleShot = NSCamShot::ISingleShot::createInstance(static_cast<EShotMode>(mu4ShotMode), "SkinBeautyShot"); 
    // 
    pSingleShot->init(); 
    
    pSingleShot->enableDataMsg(NSCamShot::ECamShot_DATA_MSG_YUV);

    // 
    pSingleShot->enableNotifyMsg(NSCamShot::ECamShot_NOTIFY_MSG_EOF); 
    //
    EImageFormat ePostViewFmt = static_cast<EImageFormat>(android::MtkCamUtils::FmtUtils::queryImageioFormat(mShotParam.ms8PostviewDisplayFormat)); 

    pSingleShot->enableDataMsg(NSCamShot::ECamShot_DATA_MSG_JPEG
                               | ((ePostViewFmt != eImgFmt_UNKNOWN) ? NSCamShot::ECamShot_DATA_MSG_POSTVIEW : NSCamShot::ECamShot_DATA_MSG_NONE)
                               ); 

    
    // shot param 
        NSCamShot::ShotParam rShotParam(eImgFmt_UYVY,         //yuv format 
                             mShotParam.mi4PictureWidth,      //picutre width 
                             mShotParam.mi4PictureHeight,     //picture height
                             mShotParam.mi4Rotation,          //picture rotation 
                             0,                               //picture flip 
                             ePostViewFmt,                    // postview format 
                             mShotParam.mi4PostviewWidth,      //postview width 
                             mShotParam.mi4PostviewHeight,     //postview height 
                             0,                               //postview rotation 
                             0,                               //postview flip 
                             mShotParam.mu4ZoomRatio           //zoom   
                            );                               

    // jpeg param 
    NSCamShot::JpegParam rJpegParam(NSCamShot::ThumbnailParam(mJpegParam.mi4JpegThumbWidth, mJpegParam.mi4JpegThumbHeight, 
                                                                mJpegParam.mu4JpegThumbQuality, MTRUE),
                                                        mJpegParam.mu4JpegQuality,       //Quality 
                                                        MFALSE                            //isSOI 
                         ); 
 
                                                                     
    // sensor param 
    NSCamShot::SensorParam rSensorParam(static_cast<MUINT32>(MtkCamUtils::DevMetaInfo::queryHalSensorDev(getOpenId())),                             //Device ID 
                             ACDK_SCENARIO_ID_CAMERA_CAPTURE_JPEG,         //Scenaio 
                             10,                                       //bit depth 
                             MFALSE,                                   //bypass delay 
                             MFALSE                                   //bypass scenario 
                            );  
    //
    pSingleShot->setCallbacks(fgCamShotNotifyCb, fgCamShotDataCb, this); 
    //     
    ret = pSingleShot->setShotParam(rShotParam); 
    
    //
    ret = pSingleShot->setJpegParam(rJpegParam); 

    // 
    ret = pSingleShot->startOne(rSensorParam); 
   
    //
    ret = pSingleShot->uninit(); 

    //
    pSingleShot->destroyInstance(); 


    return ret;
}

/******************************************************************************
 *
 ******************************************************************************/
void SkinBeautyShot::onCmd_cancel()
{
    AutoCPTLog cptlog(Event_Shot_cancel);
#warning "[TODO] SkinBeautyShot::onCmd_cancel()"
}

/******************************************************************************
 *
 ******************************************************************************/
MBOOL SkinBeautyShot::fgCamShotNotifyCb(MVOID* user, NSCamShot::CamShotNotifyInfo const msg)
{
    AutoCPTLog cptlog(Event_Shot_handleNotifyCb);
    SkinBeautyShot *pSkinBeautyShot = reinterpret_cast <SkinBeautyShot *>(user); 
    if (NULL != pSkinBeautyShot) 
    {
        if (NSCamShot::ECamShot_NOTIFY_MSG_EOF == msg.msgType) 
        {
            pSkinBeautyShot->mpShotCallback->onCB_Shutter(true, 0); 
        }
    }

    return MTRUE; 
}

/******************************************************************************
 *
 ******************************************************************************/
MBOOL SkinBeautyShot::fgCamShotDataCb(MVOID* user, NSCamShot::CamShotDataInfo const msg)
{
    SkinBeautyShot *pSkinBeautyShot = reinterpret_cast<SkinBeautyShot *>(user); 
    if (NULL != pSkinBeautyShot) 
    {
        if (NSCamShot::ECamShot_DATA_MSG_POSTVIEW == msg.msgType) 
        {
            pSkinBeautyShot->handlePostViewData( msg.puData, msg.u4Size);  
        }
        else if (NSCamShot::ECamShot_DATA_MSG_JPEG == msg.msgType)
        {
            pSkinBeautyShot->handleJpegData(msg.puData, msg.u4Size, reinterpret_cast<MUINT8*>(msg.ext1), msg.ext2);
        }
        else if (NSCamShot::ECamShot_DATA_MSG_YUV == msg.msgType)
        {
            pSkinBeautyShot->handleYuvDataCallback(msg.puData, msg.u4Size);
        }
    }

    return MTRUE; 
}

/******************************************************************************
*
*******************************************************************************/
MBOOL SkinBeautyShot::handlePostViewData(MUINT8* const puBuf, MUINT32 const u4Size)
{
    AutoCPTLog cptlog(Event_Shot_handlePVData);
    MY_LOGD("+ (puBuf, size) = (%p, %d)", puBuf, u4Size); 
   // mpShotCallback->onCB_PostviewDisplay(0, 
   //                                      u4Size, 
   //                                      reinterpret_cast<uint8_t const*>(puBuf)
   //                                     ); 
    
#ifdef QUICK_CAPTURE
	  //lanhe@MTKcamera.captrue add for speedup camera captrue
	  mpShotCallback->onCB_PostviewClient(0, 
												  u4Size, 
												  reinterpret_cast<uint8_t const*>(puBuf)
												  ); 
#endif /*QUICK_CAPTURE*/
    MY_LOGD("-"); 
    return  MTRUE;
}

/******************************************************************************
*
*******************************************************************************/
MBOOL SkinBeautyShot::handleJpegData(MUINT8* const puJpegBuf, 
                                        MUINT32 const u4JpegSize, 
                                        MUINT8* const puThumbBuf, 
                                        MUINT32 const u4ThumbSize)
{
    AutoCPTLog cptlog(Event_Shot_handleJpegData);
    MY_LOGD("+ (puJpgBuf, jpgSize, puThumbBuf, thumbSize) = (%p, %d, %p, %d)", puJpegBuf, u4JpegSize, puThumbBuf, u4ThumbSize); 

    MUINT8 *puExifHeaderBuf = new MUINT8[128 * 1024]; 
    MUINT32 u4ExifHeaderSize = 0; 

    CPTLogStr(Event_Shot_handleJpegData, CPTFlagSeparator, "makeExifHeader");
    makeExifHeader(eAppMode_PhotoMode, puThumbBuf, u4ThumbSize, puExifHeaderBuf, u4ExifHeaderSize); 
    MY_LOGD("(thumbbuf, size, exifHeaderBuf, size) = (%p, %d, %p, %d)", 
                      puThumbBuf, u4ThumbSize, puExifHeaderBuf, u4ExifHeaderSize); 

    // dummy raw callback 
    mpShotCallback->onCB_RawImage(0, 0, NULL);   

    // Jpeg callback 
    CPTLogStr(Event_Shot_handleJpegData, CPTFlagSeparator, "onCB_CompressedImage");
    mpShotCallback->onCB_CompressedImage(0,
                                         u4JpegSize, 
                                         reinterpret_cast<uint8_t const*>(puJpegBuf),
                                         u4ExifHeaderSize,                       //header size 
                                         puExifHeaderBuf,                    //header buf
                                         0,                       //callback index 
                                         true                     //final image 
                                         ); 
    MY_LOGD("-"); 

    delete [] puExifHeaderBuf; 

    return MTRUE; 
}

void SkinBeautyShot::getCameraState(ICameraHacker::CamState& camState)
{
    mpParamsMgr->getPreviewSize(&camState.prvW, &camState.prvH);
    mpParamsMgr->getPictureSize(&camState.capH, &camState.capW);
    camState.zoom = mpParamsMgr->getZoomRatio();
   // mpParamsMgr->set(MtkCameraParameters::KEY_CAPS_EFFECT, 1);
    camState.caps_effect = mpParamsMgr->getInt(MtkCameraParameters::KEY_CAPS_EFFECT);
    camState.quadEffect = mpParamsMgr->getInt(MtkCameraParameters::KEY_QUAD_EFFECT);
    camState.blurCircleRatio = mpParamsMgr->getFloat(MtkCameraParameters::KEY_BLUR_CIRCLE_RATIO);
    camState.rotation = mpParamsMgr->getInt(CameraParameters::KEY_ROTATION);
    Vector<int> value;
    if (splitInt(mpParamsMgr->getStr(MtkCameraParameters::KEY_SURFACE_SIZE), value)) {
	    camState.surfaceW = value[0];
	    camState.surfaceH = value[1];
	}
    if (splitInt(mpParamsMgr->getStr(MtkCameraParameters::KEY_BLUR_CENTER_POINT), value)) {
    	camState.centerPointX = value[0];
	    camState.centerPointY = value[1];
	}
}

MBOOL SkinBeautyShot::handleYuvDataCallback(MUINT8* const puBuf, MUINT32 const u4Size)
{
    MY_LOGD("[handleYuvDataCallback] + (puBuf, size) = (%p, %d)", puBuf, u4Size);
    
    //saveBufToFile("mnt/sdcard/yuv.yuv", puBuf, u4Size);
    
    ICameraHacker::CamState camState;
    getCameraState(camState);


    DocHacker * hacker = new DocHacker();


    hacker->onRawCaptured(puBuf, camState);

    //saveBufToFile("mnt/sdcard/yuv2.yuv", puBuf, u4Size);
    delete hacker;
    
    return 0;
}
