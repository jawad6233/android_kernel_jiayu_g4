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
#ifndef _MTK_HAL_INC_IBUFFCUSTOMPROCESS_H_
#define _MTK_HAL_INC_IBUFFCUSTOMPROCESS_H_
//

#include <IParamsManager.h>
#include "IPreviewBufMgr.h"
using namespace NSMtkPhotoCamAdapter;

namespace android {
/******************************************************************************
 *
 ******************************************************************************/
namespace NSCustomProcess {
/******************************************************************************
*   
*******************************************************************************/
class ICustomProcess : public virtual RefBase
{
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:     ////                    Instantiation.
    static sp<ICustomProcess> createInstance(sp<IParamsManager> pParamsMgr, 
                                             sp<IPreviewBufMgrHandler> pPreviewBufHandler, 
                                             int32_t openId, 
                                             int32_t queueId, 
                                             char* queueName,
                                             int32_t nodeCnt );

    virtual                         ~ICustomProcess() {}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Interfaces.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:     ////

    virtual bool                    init()                                  = 0;
    virtual bool                    uninit()                                = 0;

    virtual bool                    setImgBufProviderClient(
                                        sp<IImgBufProviderClient>const& rpClient
                                    )                                       = 0;

    /**
     *
     */
    virtual void                    setCallbacks(
                                        sp<CamMsgCbInfo> const& rpCamMsgCbInfo
                                    )                                       = 0;

    /**
     *
     */
    virtual status_t                sendCommand(int32_t cmd, int32_t arg1, int32_t arg2) = 0;
    virtual bool                    stopPreview()                           = 0;
    virtual bool                    startPreview()                          = 0;    
    virtual bool                    takePicture()                           = 0;

};

};
}; // namespace android
#endif  //_MTK_HAL_INC_IBUFFCUSTOMPROCESS_H_

