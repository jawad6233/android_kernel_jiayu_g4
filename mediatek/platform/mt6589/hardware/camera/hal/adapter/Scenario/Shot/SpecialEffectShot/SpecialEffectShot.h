/*************************************************************
 * 
 * Copyright (C), 2008-2013, OPPO Mobile Comm Corp., Ltd
 *  
 * VENDOR_EDIT   CUSTOM_PROCESS
 *
 * Date Created: 2013-02-18
 * 
 * Author: xinlan.he@Prd.Camera.CameraSrv
 * 
 * Description : NULL
 * 
 * ------------------ Revision History: ---------------------
 *      <author>        <date>          <desc>
 *
 *************************************************************/ 
#ifndef _MTK_CAMERA_CAMADAPTER_SCENARIO_SHOT_SPECIALEFFECTSHOT_H_
#define _MTK_CAMERA_CAMADAPTER_SCENARIO_SHOT_SPECIALEFFECTSHOT_H_

namespace android {
namespace NSShot {
/******************************************************************************
 *
 ******************************************************************************/


/******************************************************************************
 *
 ******************************************************************************/
class SpecialEffectShot : public ImpShot
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Interfaces.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:     ////                    Instantiation.
    virtual                         ~SpecialEffectShot();
                                    SpecialEffectShot(
                                                    char const*const pszShotName, 
                                                    uint32_t const u4ShotMode, 
                                                    int32_t const i4OpenId, 
                                                    sp<IParamsManager>  pParamsMgr
                                                );

public:     ////                    Operations.

    //  This function is invoked when this object is firstly created.
    //  All resources can be allocated here.
    virtual bool                    onCreate();

    //  This function is invoked when this object is ready to destryoed in the
    //  destructor. All resources must be released before this returns.
    virtual void                    onDestroy();

    virtual bool                    sendCommand(
                                        uint32_t const  cmd, 
                                        uint32_t const  arg1, 
                                        uint32_t const  arg2
                                    );

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Implementations.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
protected:  ////                    Operations.
    virtual bool                    onCmd_reset();
    virtual bool                    onCmd_capture();
    virtual void                    onCmd_cancel();


protected:  ////                    callbacks 
    static MBOOL fgCamShotNotifyCb(MVOID* user, NSCamShot::CamShotNotifyInfo const msg);
    static MBOOL fgCamShotDataCb(MVOID* user, NSCamShot::CamShotDataInfo const msg); 

protected:
    MBOOL           handlePostViewData(MUINT8* const puBuf, MUINT32 const u4Size);
    MBOOL           handleJpegData(MUINT8* const puJpegBuf, MUINT32 const u4JpegSize, MUINT8* const puThumbBuf, MUINT32 const u4ThumbSize);    
    MBOOL           handleYuvDataCallback(MUINT8* const puBuf, MUINT32 const u4Size);
    void            getCameraState(ICameraHacker::CamState& camState);

private:
    sp<IParamsManager>  mpParamsMgr;

};


/******************************************************************************
 *
 ******************************************************************************/
}; // namespace NSShot
}; // namespace android
#endif  //  _MTK_CAMERA_CAMADAPTER_SCENARIO_SHOT_SPECIALEFFECTSHOT_H_

