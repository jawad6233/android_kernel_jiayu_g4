/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/
#define LOG_TAG "aaa_hal_yuv"

#ifndef ENABLE_MY_LOG
    #define ENABLE_MY_LOG       (1)
#endif

#include <stdlib.h>
#include <stdio.h>
#include <aaa_types.h>
#include <aaa_error_code.h>
#include <aaa_log.h>
//#include <dbg_aaa_param.h>
#include <dbg_isp_param.h>
#include "aaa_hal_yuv.h"
//#include <aaa_state.h>   //by jmac
//#include <camera_custom_nvram.h>
//#include <awb_param.h>
//#include <awb_mgr.h>
#include <af_param.h>
#include <mcu_drv.h>
#include <isp_reg.h>
//#include <af_mgr.h>
#include <flash_param.h>
//#include <isp_tuning_mgr.h>
#include <isp_tuning.h>
#include <IBaseCamExif.h>
#include <sensor_hal.h>
#include <faces.h>
//#include <ae_param.h>
#include <CamDefs.h>
//#include <ae_mgr.h>
#include <kd_camera_feature.h>
#include "kd_imgsensor_define.h"


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// AF thread
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <config/PriorityDefs.h>
#include <sys/prctl.h>
MINT32        g_bAFThreadLoop_yuv;
pthread_t     g_AFThread_yuv;
sem_t         g_semAFThreadEnd_yuv;    
IspDrv*       g_pIspDrv_yuv;

using namespace NS3A;
using namespace NSIspTuning;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define ERROR_CHECK(API)\
   {\
   MRESULT err = API;\
   if (FAILED(err))\
   {\
       setErrorCode(err);\
       return MFALSE;\
   }}\

#ifdef VENDOR_EDIT
//ZhangZhengrong@MtkCamera, 2013/03/19, Add for scene detect
#include <time.h>
//#define SCENE_DETECT(fmt, arg...) XLOGD(fmt, ##arg)
#define SCENE_DETECT(fmt, arg...) do{}while(0)

//#define SET_SCENE_IN_PREVIEW

extern MINT32 portrait_detect_flag;

typedef struct {
    MUINT32 indoor_min_br;
    MUINT32 indoor_max_br;
    MUINT32 outdoor_min_br;
    MUINT32 outdoor_min_wb;
    MUINT32 night_max_wb;
    MUINT32 night_max_br;
    MUINT32 mix_max_wb;
    MUINT32 mix_min_wb;
} scene_detect_para;

scene_detect_para scene_para;
#endif /* VENDOR_EDIT */



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Hal3AYuv*
Hal3AYuv::
createInstance(MINT32 const i4SensorDevId)
{
    Hal3AYuv *pHal3AYuv  = Hal3AYuv::getInstance();

    switch (i4SensorDevId)
    {
        case SENSOR_DEV_MAIN:
            pHal3AYuv->init(ESensorDev_Main);
        break;
        case SENSOR_DEV_SUB:
            pHal3AYuv->init(ESensorDev_Sub);
        break;
        case SENSOR_DEV_MAIN_2:
            pHal3AYuv->init(ESensorDev_MainSecond);
        break;
        case SENSOR_DEV_MAIN_3D:
            pHal3AYuv->init(ESensorDev_Main3D);
        break;
        case SENSOR_DEV_ATV:
            pHal3AYuv->init(ESensorDev_Atv);
        break;
        default:
            MY_ERR("Unsupport sensor device: %d\n", i4SensorDevId);
            return MNULL;
        break;
    }

    return pHal3AYuv;
}

Hal3AYuv*
Hal3AYuv::
getInstance()
{
    static Hal3AYuv singleton;
    return &singleton;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID
Hal3AYuv::
destroyInstance()
{
    uninit();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Hal3AYuv::Hal3AYuv()
    : Hal3ABase()
    , m_Users(0)
    , m_Lock()
    , m_errorCode(S_3A_OK)
    , m_rParam()
    , m_bReadyToCapture(MFALSE)
    , m_i4SensorDev(0)
    , bAELockSupp(0)
    , bAWBLockSupp(0)
{

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Hal3AYuv::~Hal3AYuv()
{

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MRESULT
Hal3AYuv::
init(MINT32 i4SensorDev)
{
    MY_LOG("[%s()] m_Users: %d \n", __FUNCTION__, m_Users);

    MRESULT ret = S_3A_OK;

   	Mutex::Autolock lock(m_Lock);
   
   	if (m_Users > 0){
      		MY_LOG("%d has created \n", m_Users);
      		android_atomic_inc(&m_Users);
      		return S_3A_OK;
   	}
    //SensorHal init
    if (!m_pSensorHal)   {
        m_pSensorHal = SensorHal::createInstance();
        MY_LOG("[m_pSensorHal]:0x%08x \n",m_pSensorHal);
        if (!m_pSensorHal) {
            MY_ERR("SensorHal::createInstance() fail \n");
            return ret;
        }
    }
    m_i4SensorDev = i4SensorDev;
    // init
    sendCommand(ECmd_Init, 0);
    //ERROR_CHECK(IspTuningMgr::getInstance().init(getSensorDev()))
    
	#ifdef VENDOR_EDIT
	//LiuBin@MtkCamera, 2013/03/05, Add for yuv sensor AF
    m_i4ZoomX = 0;
	m_i4ZoomY = 0;
	m_i4ZoomWidth = 256;
	m_i4ZoomHeight = 256;
	memset(m_sAFArea, 0, sizeof(m_sAFArea));
#ifdef VENDOR_EDIT
//LiPeng.Bai, 2013/08/06, modify for linecamera  AE problem, bug id:308712

	memset(m_AFzone, 0, sizeof(m_AFzone));

#else

	m_AFzone[0]=108;
	m_AFzone[1]=101;
	m_AFzone[2]=147;
	m_AFzone[3]=131;
	m_AFzone[4]=256;
	m_AFzone[5]=256;
		
#endif
	
	memset(&m_sFDArea, 0, sizeof(m_sFDArea));
	memset(m_FDZone, 0, sizeof(m_FDZone));
	m_i4AFAreaCnt = 0;
	m_eCmdState = ECmd_Init;
	
	#ifdef VENDOR_EDIT
	//feng.hu@mtk camera team modified for ms2r continuous af
	void sendAfcCommand(void);
	af_status_pre = 0;
	m_bCAFTriggered = MFALSE;
	mFrameCnt = 0;
	#endif
	m_i4AFErrorCount = 30;
	m_bAFDone = MFALSE;
	m_bIsFocused = MFALSE;
	m_i4AFMode = AF_MODE_INFINITY;
	#endif /* VENDOR_EDIT */

    android_atomic_inc(&m_Users);

    return S_3A_OK;

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MRESULT
Hal3AYuv::
uninit()
{
    MRESULT ret = S_3A_OK;

    MY_LOG("[%s()] m_Users: %d \n", __FUNCTION__, m_Users);
    
    Mutex::Autolock lock(m_Lock);
    
    // If no more users, return directly and do nothing.
    if (m_Users <= 0){
    	   return S_3A_OK;
    }

    // More than one user, so decrease one User.
    android_atomic_dec(&m_Users);

    // There is no more User after decrease one User
    if (m_Users == 0) {
        //Reset Parameter
        Param_T npara;
        m_rParam = npara;
        
        sendCommand(ECmd_Uninit, 0);
        //ERROR_CHECK(IspTuningMgr::getInstance().uninit())
        //SensorHal uninit
        if (m_pSensorHal){
            m_pSensorHal->destroyInstance();
            m_pSensorHal = NULL;
        }
    }
    // There are still some users
    else{
    	   MY_LOG("Still %d users \n", m_Users);
    }
    
    return S_3A_OK;
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef VENDOR_EDIT
//feng.hu@mtk camera team modified for ms2r continuous af
void Hal3AYuv::sendAfcCommand(void)
{
	MINT32 af_status;

	if (mFrameCnt <20)
	{
		mFrameCnt++;
		return;
	}
	
	m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_GET_YUV_AF_STATUS, (int)&af_status, 0, 0);
	MY_LOG("[sendAfcCommand] af_status = %x hufeng debug\n", af_status);
	if (af_status != af_status_pre)
	{
		if (af_status == SENSOR_AF_FOCUSING)
		{
			m_pAFYuvCallBack->doNotifyCb(I3ACallBack::eID_NOTIFY_AF_MOVING, 1, 0, 0);
			m_pAFYuvCallBack->doNotifyCb(I3ACallBack::eID_NOTIFY_AF_FOCUSED, 0, 0, 0);
		}
		else if (af_status == SENSOR_AF_FOCUSED)
		{
			m_pAFYuvCallBack->doNotifyCb(I3ACallBack::eID_NOTIFY_AF_MOVING, 0, 0, 0);
			m_pAFYuvCallBack->doNotifyCb(I3ACallBack::eID_NOTIFY_AF_FOCUSED, 1, 0, 0);

			#ifdef OPPO_EXT_ISP_MS2R
			//ZhangZhengrong@MtkCamera, 2013/06/22, Add for scene detect
            af_position = getAFposition();
            #endif /* OPPO_EXT_ISP_MS2R */
		}
		else if (af_status == SENSOR_AF_ERROR)
		{
			m_pAFYuvCallBack->doNotifyCb(I3ACallBack::eID_NOTIFY_AF_MOVING, 0, 0, 0);
			m_pAFYuvCallBack->doNotifyCb(I3ACallBack::eID_NOTIFY_AF_FOCUSED, 0, 0, 0);
		}
		
		af_status_pre =af_status; 
	}
	
	if(mdelayFrameCnt > 0)
	{
		mdelayFrameCnt--;
	}
	else
	{
		mdelayFrameCnt = 1;
		m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_DO_YUV_CAF, (int)&m_bCAFTriggered, 0, 0);
	}
	
}
#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL Hal3AYuv::sendCommand(ECmd_T const eCmd, MINT32 const i4Arg)
{
    if(eCmd != ECmd_Update)
	      MY_LOG("[%s()],%d\n", __FUNCTION__,eCmd);

    if (eCmd == ECmd_Init){        
        #ifdef OPPO_EXT_ISP_MS2R
		//ZhangZhengrong@MtkCamera, 2013/03/19, Add for scene detect
        CamSceneDetectInit();
        #endif /* OPPO_EXT_ISP_MS2R */
        //EnableAFThread(1);
        return MTRUE;
    }
    else if  (eCmd == ECmd_CameraPreviewStart || eCmd == ECmd_RecordingStart){
         //Force reset Parameter
         Param_T old_para,rst_para;
         old_para = m_rParam;
         m_rParam = rst_para;
         setParams(old_para);
		 m_bReadyToCapture = MFALSE;

        #ifdef OPPO_EXT_ISP_MS2R
        //ZhangZhengrong@MtkCamera, 2013/03/19, Add for scene detect
        if (m_rParam.u4AsdMode == 1) {
            #ifndef SET_SCENE_IN_PREVIEW
            setScene(PreScene_to_Auto);
            #endif
            delay_frame = 30;
            scene_pre = 0;
            scene_cur = 0;
            sporting_cnt = 0;
            sporting_stop_cnt = 0;
        }
        #endif /* OPPO_EXT_ISP_MS2R */
        return MTRUE;
    }
    else if  (eCmd == ECmd_Uninit){
        #ifdef OPPO_EXT_ISP_MS2R
		//ZhangZhengrong@MtkCamera, 2013/03/19, Add for scene detect
        CamSceneDetectUninit();
        #endif /* OPPO_EXT_ISP_MS2R */
        //EnableAFThread(0);
        return MTRUE;
    }
    else if (eCmd == ECmd_PrecaptureStart){
        #ifdef OPPO_EXT_ISP_MS2R
	    //ZhangZhengrong@MtkCamera, 2013/03/19, Add for scene detect
	    if (m_rParam.u4AsdMode == 1) {
            if(PreScene_to_Auto != m_rParam.u4SceneMode)
                PreScene_to_Auto = m_rParam.u4SceneMode;
            #ifndef SET_SCENE_IN_PREVIEW
            setScene(scene_cur);
            usleep(50000);
            #endif
        }

		//LiuBin@MtkCamera, 2013/04/23, Add for set isp to capture
		setISPCapture();
        #endif /* OPPO_EXT_ISP_MS2R */	
		
		if ((FLASHLIGHT_FORCE_ON == m_rParam.u4StrobeMode)
		|| (FLASHLIGHT_AUTO == m_rParam.u4StrobeMode))
		{
			m_bReadyToCapture = MFALSE;
		}
		else
		{
			m_bReadyToCapture = MTRUE;
		}

        
        return MTRUE;
    }
    else if (eCmd == ECmd_Update){
		#ifdef OPPO_EXT_ISP_MS2R
		#ifdef VENDOR_EDIT
		//feng.hu@mtk camera team modified for ms2r continuous af
		if (AF_MODE_AFC == m_i4AFMode)
		{
			sendAfcCommand();
		}
		#endif
		
		//if (isFocusFinish())
		//modify by liubin for video AE shake when facing light
		if (ECmd_AFStart == m_eCmdState && isFocusFinish())
		{
			setState(ECmd_AFEnd);
			#ifdef VENDOR_EDIT
			//ZhangZhengrong@MtkCamera, 2013/03/19, Add for scene detect
            af_position = getAFposition();
            #endif /* VENDOR_EDIT */
		}
		
		#ifdef VENDOR_EDIT
		//ZhangZhengrong@MtkCamera, 2013/03/19, Add for scene detect    
        if (m_rParam.u4AsdMode == 1) {
            CamSceneDetect();
        }
        else {
            delay_frame = 30;
            scene_pre = 0;
            scene_cur = 0;
            sporting_cnt = 0;
            sporting_stop_cnt = 0;
        }
		#endif /* VENDOR_EDIT */
		#endif /* OPPO_EXT_ISP_MS2R */
		return MTRUE;
    }
    else{
        MY_LOG("undefine \n");    
        return MTRUE;
    }
}

#ifdef VENDOR_EDIT
//LiuBin@MtkCamera, 2013/03/05, Add for yuv sensor AF
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MRESULT Hal3AYuv::setAFMode(MINT32 a_eAFMode)
{
    if (m_i4AFMode == a_eAFMode)   {
        return S_AF_OK;
    }

    m_i4AFMode = a_eAFMode;
	
    MY_LOG("[setAFMode] af mode = 0x%x \n", m_i4AFMode);
	
    return S_AF_OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Hal3AYuv::mapAreaToZone(
		CameraArea_T *psArea, 
		MINT32 u4AreaW, 
		MINT32 u4AreaH,
		MINT32 *psZone,
		MINT32 u4ZoneW,
		MINT32 u4ZoneH)
{
	*psZone = (psArea->i4Left + u4AreaW/2) * u4ZoneW / u4AreaW;
	*(psZone+1) = (psArea->i4Top + u4AreaH/2) * u4ZoneH / u4AreaH;
	*(psZone+2) = (psArea->i4Right + u4AreaW/2) * u4ZoneW / u4AreaW;
	*(psZone+3) = (psArea->i4Bottom + u4AreaH/2) * u4ZoneH / u4AreaH;
	*(psZone+4) = u4ZoneW;
	*(psZone+5) = u4ZoneH;
	MY_LOG("Origin----L[%d], T[%d], R[%d], B[%d] \r\n", 
		psArea->i4Left,
		psArea->i4Top,
		psArea->i4Right,
		psArea->i4Bottom);

	MY_LOG("Map to Zone----L[%d], T[%d], R[%d], B[%d] \r\n", 
		*psZone,
		*(psZone+1),
		*(psZone+2),
		*(psZone+3));
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Hal3AYuv::updateAFAreaAndZone(
	CameraFocusArea_T *psFocusArea, 
	MINT32 u4AreaW, 
	MINT32 u4AreaH)
{
	if (psFocusArea->u4Count > MAX_FOCUS_AREAS)
	{
		psFocusArea->u4Count = MAX_FOCUS_AREAS;
	}
	m_i4AFAreaCnt = psFocusArea->u4Count;
	MY_LOG("m_i4AFAreaCnt---------[%d] \r\n", m_i4AFAreaCnt);

	if ((m_sAFArea[0].i4Left == psFocusArea->rAreas[0].i4Left)
		&& (m_sAFArea[0].i4Top == psFocusArea->rAreas[0].i4Top))
	{
		return;
	}
	
	for (MINT32 i=0; i<m_i4AFAreaCnt; i++)
	{
		m_sAFArea[i].i4Left = psFocusArea->rAreas[i].i4Left;
		m_sAFArea[i].i4Top = psFocusArea->rAreas[i].i4Top;
		m_sAFArea[i].i4Right = psFocusArea->rAreas[i].i4Right;
		m_sAFArea[i].i4Bottom = psFocusArea->rAreas[i].i4Bottom;
	}

	mapAreaToZone(&m_sAFArea[0], u4AreaW, u4AreaH, m_AFzone, m_i4ZoomWidth, m_i4ZoomHeight);
	
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL Hal3AYuv::isFocusFinish()
{
	MINT32 err = 0;
    MINT32 focus_status = 0xffffffff;
	MBOOL bAFDone = MFALSE;

#ifdef VENDOR_EDIT
//LiPeng.Bai , 2013-08-09, add for solve Instagram camera capture problem, bug id:323021

	if ((m_rParam.u4AfMode == AF_MODE_AFC) ||(m_rParam.u4AfMode== AF_MODE_AFC_VIDEO))
	{
		MY_LOG("[QQbai] m_rParam.u4AfMode=AF_MODE_AFC or AF_MODE_AFC_VIDEO, return focus finish directly");
		m_i4AFErrorCount = 30;
		m_bIsFocused = MTRUE;
		m_bAFDone = MTRUE;
		return m_bAFDone;
	}
	   
#endif

	m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_GET_YUV_AF_STATUS, (int)&focus_status, 0, 0);

    if(err < 0){
        MY_ERR("sendCommand() error: SENSOR_CMD_GET_YUV_AF_STATUS\n");
        return 1;
    }
    MY_LOG("focus_status = 0x%x\n",focus_status);

	if (SENSOR_AF_FOCUSED == focus_status)
	{
		m_i4AFErrorCount = 30;
		m_bIsFocused = MTRUE;
		m_bAFDone = MTRUE;
	}
	else if (SENSOR_AF_ERROR == focus_status)
	{
		if (m_i4AFErrorCount > 0)
		{
			m_i4AFErrorCount--;
			m_bAFDone = MFALSE;
		}
		else
		{
			m_i4AFErrorCount = 30;
			m_bIsFocused = MFALSE;
			m_bAFDone = MTRUE;
		}
	}
	else
	{
		m_bAFDone = MFALSE;
	}
	
	return m_bAFDone;
	
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL Hal3AYuv::isReadyToCapture() const
{
	MINT32 err = 0;
	MBOOL bIsCapture = MFALSE;

	if ((FLASHLIGHT_FORCE_ON == m_rParam.u4StrobeMode)
		|| (FLASHLIGHT_AUTO == m_rParam.u4StrobeMode))
	{
		err = m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev), SENSOR_CMD_GET_ISP_STATUS,(int)&bIsCapture, 0, 0);
		if (err < 0)
		{
			MY_ERR("sendCommand() error: SENSOR_CMD_GET_YUV_AF_STATUS\n");
	        return MTRUE;
		}
		if (MTRUE == bIsCapture)
		{
			MY_LOG("ISP is in capture state now , ready for capture \r\n");
			return MTRUE;
		}
		else
		{
			MY_LOG("ISP is not ready for capture now , please wait on \r\n");
			return MFALSE;
		}
	}
	
	return m_bReadyToCapture;
	
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID Hal3AYuv::setISPCapture()
{
	m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev), SENSOR_CMD_SET_ISP_CAPTURE, 0, 0, 0);
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MVOID Hal3AYuv::setState(ECmd_T eCmdState)
{
	if (eCmdState == m_eCmdState)
	{
		return;
	}
	switch (eCmdState)
	{
		case ECmd_AFStart:
		{
			if ((m_rParam.u4AfMode != AF_MODE_AFC) && 
				(m_rParam.u4AfMode != AF_MODE_AFC_VIDEO))
			{
				//set focus area
				m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_YUV_AF_WINDOW, (int)m_AFzone, 0, 0);
				//single focus
				m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_YUV_SINGLE_FOCUS_MODE, 0, 0, 0);
			#ifndef VENDOR_EDIT
			//LiPeng.Bai, 2013/08/16,Modify for focus fail, bug id:319069, 322120, 322356, 319506(13013)
			
				//af callback
				m_pAFYuvCallBack->doNotifyCb(I3ACallBack::eID_NOTIFY_AF_MOVING, !isFocusFinish(), 0, 0);
				m_pAFYuvCallBack->doNotifyCb(I3ACallBack::eID_NOTIFY_AF_FOCUSED, m_bIsFocused, 0, 0);
				
			#endif	/*VENDOR_EDIT*/
			
			}
			break;
		}
		case ECmd_AFEnd:
		{
			MY_LOG("ECmd_AFEnd-------callback \r\n");
			
		#ifndef VENDOR_EDIT
		//LiPeng.Bai, 2013/08/16, Modify for focus fail, bug id:319069, 322120, 322356, 319506(13013)
		
			m_pAFYuvCallBack->doNotifyCb(I3ACallBack::eID_NOTIFY_AF_MOVING, !isFocusFinish(), 0, 0);
			m_pAFYuvCallBack->doNotifyCb(I3ACallBack::eID_NOTIFY_AF_FOCUSED, m_bIsFocused, 0, 0);
		#else
			//eID_NOTIFY_AF_MOVING  1: doing focus		eID_NOTIFY_AF_FOCUSED	1:focus succeed
			m_pAFYuvCallBack->doNotifyCb(I3ACallBack::eID_NOTIFY_AF_MOVING, 0, 0, 0);
			m_pAFYuvCallBack->doNotifyCb(I3ACallBack::eID_NOTIFY_AF_FOCUSED, 1, 0, 0);
		#endif	/*VENDOR_EDIT*/

			break;
		}
		
		default:
			break;
	}
	m_eCmdState = eCmdState;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL Hal3AYuv::setFDInfo(MVOID* a_sFaces)
{
	MtkCameraFaceMetadata *pFaces = (MtkCameraFaceMetadata *)a_sFaces;

	if (pFaces->number_of_faces == 0)
	{
		memset(&m_sFDArea, 0, sizeof(CameraArea_T));
		return MFALSE;
	}

	if ((pFaces->faces->rect[0] != m_sFDArea.i4Left)
		|| (pFaces->faces->rect[1] != m_sFDArea.i4Top)
		|| (pFaces->faces->rect[2] != m_sFDArea.i4Right)
		|| (pFaces->faces->rect[3] != m_sFDArea.i4Bottom))
	{
		m_sFDArea.i4Left = pFaces->faces->rect[0];
		m_sFDArea.i4Top = pFaces->faces->rect[1];
		m_sFDArea.i4Right = pFaces->faces->rect[2];
		m_sFDArea.i4Bottom = pFaces->faces->rect[3];
	}

	mapAreaToZone(&m_sFDArea, 2000, 2000, m_FDZone, m_i4ZoomWidth, m_i4ZoomHeight);

	m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_YUV_AE_WINDOW, (int)m_FDZone, 0, 0);
	
	return MTRUE;	
}
#endif /* VENDOR_EDIT */

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL Hal3AYuv::setParams(Param_T const &rNewParam)
{
    MINT32 yuvCmd = 0;
    MINT32 yuvParam = 0; 

    MY_LOG("[%s()] \n", __FUNCTION__);

    if (m_rParam.u4EffectMode != rNewParam.u4EffectMode){
        MY_LOG("[FID_COLOR_EFFECT],(%d)->(%d) \n",m_rParam.u4EffectMode,rNewParam.u4EffectMode);
        yuvCmd = FID_COLOR_EFFECT;
        yuvParam = rNewParam.u4EffectMode;        
        m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_YUV_FEATURE_CMD,(int)&yuvCmd,(int)&yuvParam,0);
    }
    if (m_rParam.i4ExpIndex != rNewParam.i4ExpIndex){
        MY_LOG("[FID_AE_EV],Idx:(%d)->(%d),Step:(%d)->(%d) \n",m_rParam.i4ExpIndex,rNewParam.i4ExpIndex,m_rParam.fExpCompStep, rNewParam.fExpCompStep);
        yuvCmd = FID_AE_EV;
        yuvParam = mapAEToEnum(rNewParam.i4ExpIndex,rNewParam.fExpCompStep);        
        m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_YUV_FEATURE_CMD,(int)&yuvCmd,(int)&yuvParam,0);
    }
    if (m_rParam.u4SceneMode != rNewParam.u4SceneMode){
        MY_LOG("[FID_SCENE_MODE],(%d)->(%d) \n",m_rParam.u4SceneMode,rNewParam.u4SceneMode);
        yuvCmd = FID_SCENE_MODE;
        yuvParam = rNewParam.u4SceneMode;        
        m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_YUV_FEATURE_CMD,(int)&yuvCmd,(int)&yuvParam,0);
    }
   // if (m_rParam.u4AwbMode != rNewParam.u4AwbMode){
        MY_LOG("[FID_AWB_MODE],(%d)->(%d) \n",m_rParam.u4AwbMode,rNewParam.u4AwbMode);
        yuvCmd = FID_AWB_MODE;
        yuvParam = rNewParam.u4AwbMode;        
        m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_YUV_FEATURE_CMD,(int)&yuvCmd,(int)&yuvParam,0);
   // }
    if (m_rParam.u4AntiBandingMode != rNewParam.u4AntiBandingMode){
        MY_LOG("[FID_AE_FLICKER],(%d)->(%d) \n",m_rParam.u4AntiBandingMode,rNewParam.u4AntiBandingMode);
        yuvCmd = FID_AE_FLICKER;
        yuvParam = rNewParam.u4AntiBandingMode;        
        m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_YUV_FEATURE_CMD,(int)&yuvCmd,(int)&yuvParam,0);
    }    
    if (m_rParam.u4SaturationMode != rNewParam.u4SaturationMode){
        MY_LOG("[FID_ISP_SAT],(%d)->(%d) \n",m_rParam.u4SaturationMode,rNewParam.u4SaturationMode);
        yuvCmd = FID_ISP_SAT;
        yuvParam = rNewParam.u4SaturationMode;        
        m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_YUV_FEATURE_CMD,(int)&yuvCmd,(int)&yuvParam,0);
    }
    if (m_rParam.u4ContrastMode != rNewParam.u4ContrastMode){
        MY_LOG("[FID_ISP_CONTRAST],(%d)->(%d) \n",m_rParam.u4ContrastMode,rNewParam.u4ContrastMode);
        yuvCmd = FID_ISP_CONTRAST;
        yuvParam = rNewParam.u4ContrastMode;        
        m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_YUV_FEATURE_CMD,(int)&yuvCmd,(int)&yuvParam,0);
    }
    if (m_rParam.u4IsoSpeedMode != rNewParam.u4IsoSpeedMode){
        MY_LOG("[FID_AE_ISO],(%d)->(%d) \n",m_rParam.u4IsoSpeedMode,rNewParam.u4IsoSpeedMode);
        yuvCmd = FID_AE_ISO;
        yuvParam = mapISOToEnum(rNewParam.u4IsoSpeedMode);
        m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_YUV_FEATURE_CMD,(int)&yuvCmd,(int)&yuvParam,0);
    }
    if (m_rParam.bIsAELock != rNewParam.bIsAELock && bAELockSupp==1){
         MY_LOG("[FID_AE_LOCK],(%d)->(%d) \n",m_rParam.bIsAELock,rNewParam.bIsAELock);
         yuvCmd = FID_AE_SCENE_MODE;
         yuvParam=rNewParam.bIsAELock==1?AE_MODE_OFF:AE_MODE_AUTO;
         m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_YUV_FEATURE_CMD,(int)&yuvCmd,(int)&yuvParam,0);             
    }
    if (m_rParam.bIsAWBLock != rNewParam.bIsAWBLock && bAWBLockSupp==1){
         MY_LOG("[FID_AWB_LOCK],(%d)->(%d) \n",m_rParam.bIsAWBLock,rNewParam.bIsAWBLock);
         yuvCmd = FID_AWB_MODE;
         yuvParam=(rNewParam.bIsAWBLock==1)?AWB_MODE_OFF:AWB_MODE_AUTO;
         m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_YUV_FEATURE_CMD,(int)&yuvCmd,(int)&yuvParam,0);             
    }
    //for cam-mode
    if (m_rParam.u4CamMode != rNewParam.u4CamMode){
    	   if (rNewParam.u4CamMode == eAppMode_VideoMode||rNewParam.u4CamMode == eAppMode_VtMode){
             MY_LOG("[FID_CAM_MODE],(%d)->(%d),fps(%d) \n",m_rParam.u4CamMode, rNewParam.u4CamMode,rNewParam.i4MaxFps);
             yuvParam=(rNewParam .i4MaxFps<=20000)?15:30;
             m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_VIDEO_FRAME_RATE,(int)&yuvParam,0,0);             
    	   }
    }
    //for frame rate
    if (m_rParam.i4MaxFps!=rNewParam.i4MaxFps||m_rParam.i4MinFps!=rNewParam.i4MinFps){
        if(rNewParam.i4MinFps==rNewParam.i4MaxFps&&rNewParam.i4MaxFps>0){
             MY_LOG("[FID_FIX_FRAMERATE],Max(%d)->(%d) \n",m_rParam.i4MaxFps,rNewParam.i4MaxFps);
             yuvParam=(rNewParam.i4MaxFps<=20000)?15:30;
             m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_VIDEO_FRAME_RATE,(int)&yuvParam,0,0);             
        }
    }
	
	#ifdef VENDOR_EDIT
	//LiuBin@MtkCamera, 2013/02/26, Add for feature set
	//for focus mode
	if (rNewParam.u4AfMode != m_rParam.u4AfMode)
	{
		setAFMode(rNewParam.u4AfMode);
	}
	//for focus area
	if (rNewParam.rFocusAreas.u4Count > 0)
	{
		updateAFAreaAndZone((CameraFocusArea_T *)&(rNewParam.rFocusAreas), 2000, 2000);
	}
	//for flashlight
	if (rNewParam.u4StrobeMode != m_rParam.u4StrobeMode)
	{
		MY_LOG("[FID_AE_STROBE], (%d)->(%d) \n",m_rParam.u4StrobeMode,rNewParam.u4StrobeMode);
        yuvCmd = FID_AE_STROBE;
        yuvParam = rNewParam.u4StrobeMode;        
        m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_YUV_FEATURE_CMD,(int)&yuvCmd,(int)&yuvParam,0);
	}
	#endif /* VENDOR_EDIT */

    m_rParam = rNewParam;

    return MTRUE;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL Hal3AYuv::getSupportedParams(FeatureParam_T &rFeatureParam) 
{	
    MINT32 ae_lock=0,awb_lock=0;
    MINT32 max_focus=0,max_meter=0;

    MY_LOG("[%s()] \n", __FUNCTION__);

    m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_GET_YUV_AE_AWB_LOCK,(int)&ae_lock,(int)&awb_lock,0);
    bAELockSupp = ae_lock==1?1:0;
    bAWBLockSupp = awb_lock==1?1:0;
    rFeatureParam.bExposureLockSupported = bAELockSupp;
    rFeatureParam.bAutoWhiteBalanceLockSupported = bAWBLockSupp;
    MY_LOG("AE_sup(%d),AWB_sub(%d) \n",bAELockSupp,bAWBLockSupp);

    m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_GET_YUV_AF_MAX_NUM_FOCUS_AREAS,(int)&max_focus,0,0);
    m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_GET_YUV_AE_MAX_NUM_METERING_AREAS,(int)&max_meter,0,0);
    rFeatureParam.u4MaxMeterAreaNum = max_meter>=1?1:0;
    rFeatureParam.u4MaxFocusAreaNum = max_focus>=1?1:0;    
    MY_LOG("FOCUS_max(%d),METER_max(%d) \n",max_focus,max_meter);

    //rFeatureParam.i4MaxLensPos = AfMgr::getInstance().getMaxLensPos();
    //rFeatureParam.i4MinLensPos = AfMgr::getInstance().getMinLensPos();
    rFeatureParam.i4AFBestPos = 0;
    rFeatureParam.i8BSSVlu = 0;
    
    return MTRUE;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL Hal3AYuv::autoFocus()
{
	#ifdef OPPO_EXT_ISP_MS2R
	setState(ECmd_AFStart);
	#else
	m_pAFYuvCallBack->doNotifyCb(I3ACallBack::eID_NOTIFY_AF_FOCUSED, 1, 0, 0);
	#endif

    return MTRUE;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL Hal3AYuv::cancelAutoFocus()
{
//    ERROR_CHECK(StateMgr::getInstance().sendCmd(ECmd_AFEnd))  // by jmac

    return MTRUE;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL Hal3AYuv::setZoom(MUINT32 u4ZoomRatio_x100, MUINT32 u4XOffset, MUINT32 u4YOffset, MUINT32 u4Width, MUINT32 u4Height)
{
//    ERROR_CHECK(AeMgr::getInstance().setZoomWinInfo(u4XOffset, u4YOffset, u4Width, u4Height))

    return MTRUE;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL Hal3AYuv::set3AEXIFInfo(IBaseCamExif *pIBaseCamExif) const
{
	   MY_LOG("[%s()] \n", __FUNCTION__);
	   
    SENSOR_EXIF_INFO_STRUCT mSensorInfo;
    EXIF_INFO_T rEXIFInfo;
    memset(&rEXIFInfo, 0, sizeof(EXIF_INFO_T));
    memset(&mSensorInfo, 0, sizeof(SENSOR_EXIF_INFO_STRUCT));

    m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_GET_YUV_EXIF_INFO,(int)&mSensorInfo,0,0);             

    MY_LOG("FNumber=%d, AEISOSpeed=%d, AWBMode=%d, CapExposureTime=%d, FlashLightTimeus=%d, RealISOValue=%d\n", 
           mSensorInfo.FNumber, mSensorInfo.AEISOSpeed, mSensorInfo.AWBMode, 
           mSensorInfo.CapExposureTime, mSensorInfo.FlashLightTimeus, mSensorInfo.RealISOValue);

    rEXIFInfo.u4FNumber = mSensorInfo.FNumber>0 ? mSensorInfo.FNumber : 28;
    rEXIFInfo.u4FocalLength = 350;
    rEXIFInfo.u4SceneMode = m_rParam.u4SceneMode>0 ? m_rParam.u4SceneMode : 0;
    rEXIFInfo.u4AWBMode = mSensorInfo.AWBMode>0 ? mSensorInfo.AWBMode : 0;
    rEXIFInfo.u4CapExposureTime = mSensorInfo.CapExposureTime>0? mSensorInfo.CapExposureTime : 0;
    rEXIFInfo.u4FlashLightTimeus = mSensorInfo.FlashLightTimeus>0? mSensorInfo.FlashLightTimeus : 0;
    rEXIFInfo.u4AEISOSpeed =mapEnumToISO(mSensorInfo.AEISOSpeed);
    rEXIFInfo.u4AEISOSpeed = mapEnumToISO(mSensorInfo.AEISOSpeed);
    rEXIFInfo.i4AEExpBias = 0;
    	
    pIBaseCamExif->set3AEXIFInfo(&rEXIFInfo);

    return MTRUE;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL Hal3AYuv::setDebugInfo(IBaseCamExif *pIBaseCamExif) const
{
    return MTRUE;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MINT32 Hal3AYuv::getDelayFrame(EQueryType_T const eQueryType) const
{
    MUINT32 ret = 0;
    SENSOR_DELAY_INFO_STRUCT pDelay;
	   
    MY_LOG("[%s()] \n", __FUNCTION__);

    memset(&pDelay,0x0,sizeof(SENSOR_DELAY_INFO_STRUCT));
    m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_GET_YUV_DELAY_INFO,(int)&pDelay,0,0);
    MY_LOG("Init:%d,effect:%d,awb:%d \n",pDelay.InitDelay,pDelay.EffectDelay,pDelay.AwbDelay);

    switch (eQueryType)
    {
        case EQueryType_Init:
        {
		    #ifndef VENDOR_EDIT
		    //LiuBin@MtkCamera, 2013/03/19, Modify for speed up the camera start
			ret = (pDelay.InitDelay>0 && pDelay.InitDelay<5)?pDelay.InitDelay:3;
		    #else /* VENDOR_EDIT */
		    ret = pDelay.InitDelay; 
		    #endif /* VENDOR_EDIT */
            return ret;
        }
        case EQueryType_Effect:
        {
             ret = (pDelay.EffectDelay>0 && pDelay.EffectDelay<5)?pDelay.EffectDelay:0;
             return ret;
        }
        case EQueryType_AWB:
        {
            ret = (pDelay.AwbDelay>0 && pDelay.AwbDelay<5)?pDelay.AwbDelay:0;
            return ret;
        }
        default:
            return 0;
    }
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL Hal3AYuv::setIspProfile(EIspProfile_T const eIspProfile)
{
//    ERROR_CHECK(IspTuningMgr::getInstance().setIspProfile(eIspProfile))
 //   ERROR_CHECK(IspTuningMgr::getInstance().validate())

    return MTRUE;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// AF thread
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MRESULT Hal3AYuv::EnableAFThread(MINT32 a_bEnable)
{
    MRESULT ret = S_3A_OK;
#if 0
    if (a_bEnable)  {

        if (g_bAFThreadLoop_yuv == 0)
        {  
            ret = AfMgr::getInstance().init();
            if (FAILED(ret)) {
                MY_ERR("AfMgr::getInstance().init() fail\n");
                return ret;
            }
            
            // create AF thread
            MY_LOG("[AFThread] Create");    
            g_bAFThreadLoop_yuv = 1;
            sem_init(&g_semAFThreadEnd_yuv, 0, 0);
            ::prctl(PR_SET_NAME,"AFthread", 0, 0, 0);
            pthread_attr_t const attr = {0, NULL, 1024 * 1024, 4096, SCHED_RR, PRIO_RT_AF_THREAD};
            pthread_create(&g_AFThread_yuv, &attr, AFThreadFunc, NULL);
        }
    }
    else   {

        if (g_bAFThreadLoop_yuv == 1)
        {
            ret = AfMgr::getInstance().uninit();
            if (FAILED(ret)) {
                MY_ERR("AfMgr::getInstance().init() fail\n");
                return ret;
            }
        
            g_bAFThreadLoop_yuv = 0;
            ::sem_wait(&g_semAFThreadEnd_yuv);
            MY_LOG("[AFThread] Delete");
        }
    }
#endif
    return ret;
}

MVOID * Hal3AYuv::AFThreadFunc(void *arg)
{
#if 0
    MY_LOG("[AFThread] tid: %d \n", gettid());

    if (!g_pIspDrv_yuv) {
        MY_LOG("[AFThread] m_pIspDrv null\n");
        return NULL;
    }

    // wait AFO done
    ISP_DRV_WAIT_IRQ_STRUCT WaitIrq;
    WaitIrq.Clear = ISP_DRV_IRQ_CLEAR_WAIT;
    WaitIrq.Type = ISP_DRV_IRQ_TYPE_INT;
    WaitIrq.Status = ISP_DRV_IRQ_INT_STATUS_AF_DON_ST;
    WaitIrq.Timeout = 200; // 200 msec

    while (g_bAFThreadLoop_yuv) {
       
        if (g_pIspDrv_yuv->waitIrq(WaitIrq) >= 0) // success
        {  
//            StateMgr::getInstance().sendCmd(ECmd_AFUpdate);  // by jmac
        }
        else
        {
            MY_LOG("[AFThread] AF irq timeout\n");
        }
    }

    ::sem_post(&g_semAFThreadEnd_yuv);

    MY_LOG("[AFThread] End \n");

#endif
    return NULL;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// setCallbacks
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MBOOL Hal3AYuv::setCallbacks(I3ACallBack* cb)
{
	MY_LOG("[%s()][p]%d\n", __FUNCTION__, cb);

    m_pAFYuvCallBack=cb;


    return MTRUE;

   // return AfMgr::getInstance().setCallbacks(cb);
}

//******************************************************************************
// Map AE exposure to Enum
//******************************************************************************
MINT32 Hal3AYuv::mapAEToEnum(MINT32 mval,MFLOAT mstep)
{
    MINT32 pEv,ret;  

    pEv = 100 * mval * mstep;
    
    if     (pEv <-250) { ret = AE_EV_COMP_n30;}  // EV compensate -3.0
    else if(pEv <-200) { ret = AE_EV_COMP_n25;}  // EV compensate -2.5
    else if(pEv <-170) { ret = AE_EV_COMP_n20;}  // EV compensate -2.0
    else if(pEv <-160) { ret = AE_EV_COMP_n17;}  // EV compensate -1.7
    else if(pEv <-140) { ret = AE_EV_COMP_n15;}  // EV compensate -1.5
    else if(pEv <-130) { ret = AE_EV_COMP_n13;}  // EV compensate -1.3    
    else if(pEv < -90) { ret = AE_EV_COMP_n10;}  // EV compensate -1.0
    else if(pEv < -60) { ret = AE_EV_COMP_n07;}  // EV compensate -0.7
    else if(pEv < -40) { ret = AE_EV_COMP_n05;}  // EV compensate -0.5
    else if(pEv < -10) { ret = AE_EV_COMP_n03;}  // EV compensate -0.3    
    else if(pEv ==  0) { ret = AE_EV_COMP_00; }  // EV compensate -2.5
    else if(pEv <  40) { ret = AE_EV_COMP_03; }  // EV compensate  0.3
    else if(pEv <  60) { ret = AE_EV_COMP_05; }  // EV compensate  0.5
    else if(pEv <  90) { ret = AE_EV_COMP_07; }  // EV compensate  0.7
    else if(pEv < 110) { ret = AE_EV_COMP_10; }  // EV compensate  1.0
    else if(pEv < 140) { ret = AE_EV_COMP_13; }  // EV compensate  1.3
    else if(pEv < 160) { ret = AE_EV_COMP_15; }  // EV compensate  1.5
    else if(pEv < 180) { ret = AE_EV_COMP_17; }  // EV compensate  1.7    
    else if(pEv < 210) { ret = AE_EV_COMP_20; }  // EV compensate  2.0
    else if(pEv < 260) { ret = AE_EV_COMP_25; }  // EV compensate  2.5
    else if(pEv < 310) { ret = AE_EV_COMP_30; }  // EV compensate  3.0
    else               { ret = AE_EV_COMP_00;}
    
    MY_LOG("[%s()]EV:(%d),Ret:(%d)\n", __FUNCTION__, pEv,ret);

    return ret;
}

//******************************************************************************
// Map AE ISO to Enum
//******************************************************************************
MINT32 Hal3AYuv::mapISOToEnum(MUINT32 u4NewAEISOSpeed)
{
    MINT32 ret;  
    
    switch(u4NewAEISOSpeed){
        case 0:
            ret = AE_ISO_AUTO;
            break;
        case 100:
            ret = AE_ISO_100;
            break;
        case 200:
            ret = AE_ISO_200;
            break;
        case 400:
            ret = AE_ISO_400;
            break;
        case 800:
             ret = AE_ISO_800;
           break;
        case 1600:
            ret = AE_ISO_1600;
           break;
        default:
            MY_LOG("The iso enum value is incorrectly:%d\n", u4NewAEISOSpeed);            
            ret = AE_ISO_AUTO;
            break;
    }
    MY_LOG("[%s()]ISOVal:(%d),Ret:(%d)\n", __FUNCTION__, u4NewAEISOSpeed, ret);

    return ret;
}

//******************************************************************************
// Map AE ISO to Enum
//******************************************************************************
MINT32 Hal3AYuv::mapEnumToISO(MUINT32 u4NewAEIsoEnum) const
{
    MINT32 ret;  
    
    switch(u4NewAEIsoEnum){
        case AE_ISO_AUTO:
            ret = 100;
            break;
        case AE_ISO_100:
            ret = 100;
            break;
        case AE_ISO_200:
            ret = 200;
            break;
        case AE_ISO_400:
            ret = 400;
            break;
        case AE_ISO_800:
             ret = 800;
           break;
        case AE_ISO_1600:
            ret = 1600;
           break;
        default:
            ret = 100;
            break;
    }
    MY_LOG("[%s()]ISOEnum:(%d),Ret:(%d)\n", __FUNCTION__, u4NewAEIsoEnum, ret);

    return ret;
}

#ifdef VENDOR_EDIT
//ZhangZhengrong@MtkCamera, 2013/03/19, Add for scene detect
MUINT32
Hal3AYuv::getWaveDetectValue(void)
{
	MINT32 err = 0;
	MUINT32 wdv = 0;

    //MY_LOG("#####getWaveDetectValue enter#####");
    
    err = m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_GET_WDV, (int)&wdv,0,0);
    
	if(err < 0){
            MY_LOG("sendCommand() error: SENSOR_CMD_GET_WDV\n");
            return 0;
        }

	return wdv;
	
}

MINT32
Hal3AYuv::getBrightness(void)
{
	MINT32 err = 0;
	MUINT32 brightness = 0;

    //MY_LOG("#####getBrightness enter#####");
    
    err = m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_GET_BRIGHTNESS, (int)&brightness);

	if(err < 0){
        MY_LOG("sendCommand() error: SENSOR_CMD_GET_BRIGHTNESS\n");
        return 0;
    }

	return brightness;
	
}

MINT32
Hal3AYuv::getWhiteBalance(void)
{
	MINT32 err = 0;
	MUINT32 whitebalance =0;

    //MY_LOG("#####getWhiteBalance enter#####");

    err = m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_GET_WB, (int)&whitebalance);

	if(err < 0){
        MY_LOG("sendCommand() error: SENSOR_CMD_GET_WB\n");
        return 0;
    }

	return whitebalance;
	
}

/*******************************************************************************
*
********************************************************************************/
MUINT32
Hal3AYuv::getAeParameter(void)
{
	MINT32 err = 0;
	MUINT32 AeParameter = 0;

    //MY_LOG("#####getAeParameter enter#####");

    err = m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_GET_AE_GAIN_SHUTTER, (int)&AeParameter);
    
	if(err < 0){
            MY_LOG("sendCommand() error: SENSOR_CMD_GET_AE_GAIN_SHUTTER\n");
            return 0;
    }

    //MY_LOG("getAeParameter end:gain=%d shutter=%d",AeParameter>>16&0xFFFF,AeParameter&0xFFFF);
	return AeParameter;
	
}

/*******************************************************************************
*
********************************************************************************/
MINT32
Hal3AYuv::getAFposition(void)
{
	MINT32 err = 0;
	MINT32 AFposition = 0;

    //MY_LOG("getAFposition enter#####");

    err = m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_GET_AF_POSITION, (int)&AFposition);

	if(err < 0){
            MY_LOG("sendCommand() error: SENSOR_CMD_GET_AF_POSITION\n");
            return 0;
    }

    //MY_LOG("getAFposition end:AFposition=%d",AFposition);
	return AFposition;
	
}

/*******************************************************************************
*
********************************************************************************/
MINT32
Hal3AYuv::setScene(MUINT32 u4Scene)
{
    //MY_LOG("setScene enter:u4Scene=%d\n",u4Scene);

    MINT32 err = 0;

    MINT32 yuvCmd = FID_SCENE_MODE;
    MINT32 yuvParam = u4Scene;
    
    err = m_pSensorHal->sendCommand(static_cast<halSensorDev_e>(m_i4SensorDev),SENSOR_CMD_SET_YUV_FEATURE_CMD,(int)&yuvCmd,(int)&yuvParam,0);

    if(err < 0){
        MY_LOG("setScene error: SENSOR_CMD_SET_YUV_FEATURE_CMD\n");
        return err;
    }

    return err;
}

typedef enum {
	    SCENE_BEGIN    =   0, 
        SCENE_OFF      =   SCENE_BEGIN,   // Disable scene mode equal Auto mode
        SCENE_NORMAL,                          // Normal mode
        SCENE_ACTION,                          // Action mode
        SCENE_PORTRAIT, /*3*/                       // Portrait mode
        SCENE_LANDSCAPE,                       // Landscape
        SCENE_NIGHTSCENE,                      // Night Scene
        SCENE_NIGHTPORTRAIT,                   // Night Portrait
        SCENE_THEATRE,                         // Theatre mode
        SCENE_BEACH,                           // Beach mode
        SCENE_SNOW,                            // Snow mode
        SCENE_SUNSET,                          // Sunset mode
        SCENE_STEADYPHOTO,                     // Steady photo mode
        SCENE_FIREWORKS,                       // Fireworks mode
        SCENE_SPORTS, /*13*/                   // Sports mode
        SCENE_PARTY,                           // Party mode
        SCENE_CANDLELIGHT,                     // Candle light mode
        //  (Unsupported legacy symbol; don't use this symbol if possible)
        SCENE_ISO_ANTI_SHAKE,                  // ISO Anti Shake mode
        //  (Unsupported legacy symbol; don't use this symbol if possible)
        SCENE_BRACKET_AE,                      // Bracket AE
        SCENE_INDOOR,  /*18*/                  //new add for indoor
        SCENE_MIX_ILLUMINANT,                  //new add for mix
        SCENE_MACRO,                           //new add for macro
        SCENE_HDR,
        SCENE_NUM,
	}SCENE;

typedef struct {
    SCENE scene;
    char  scene_name[16];
} scene_mode;
scene_mode scene_mode_define[8] = {
        {SCENE_PORTRAIT, "portrait"},
        {SCENE_LANDSCAPE, "landscape"},
        {SCENE_INDOOR, "indoor"},
        {SCENE_MIX_ILLUMINANT, "mix_illuminant"},
        {SCENE_NIGHTSCENE, "nightscene"},
        {SCENE_SPORTS, "sports"},
        {SCENE_MACRO, "macro"},
        {SCENE_NORMAL, "normal/auto"}
    };
    
#define COUNT_AVER_CNT 5
//-------------------------------------------------------------------------------------------------------------------------
MINT32 max(MINT32 a, MINT32 b)
{
    if (a >= b)
    {
        return a ;
    }
    else
    {
        return b ;
    }
}
//-------------------------------------------------------------------------------------------------------------------------
MINT32 min(MINT32 a, MINT32 b)
{
    if (a <= b)
    {
        return a ;
    }
    else
    {
        return b ;
    }
}

#define COUNT_W_RATIO_NUM 128
#define COUNT_H_RATIO_NUM 72

#define FULL_THRESHOLD    COUNT_W_RATIO_NUM*COUNT_H_RATIO_NUM*0.14
#define CENTER_THRESHOLD  (COUNT_W_RATIO_NUM*COUNT_H_RATIO_NUM*0.14)*1/4
#define BORDER_THRESHOLD  (COUNT_W_RATIO_NUM*COUNT_H_RATIO_NUM*0.14)*1/16

typedef struct{
	MINT32 mR;
	MINT32 mG;
	MINT32 mB;
}pic_rgb_stat;
pic_rgb_stat *p_rgb_stat = NULL;

typedef struct{
	MINT32 y;
	MINT32 cb;
	MINT32 cr;
}pic_yuv_stat;
pic_yuv_stat *p_yuv_stat = NULL;
pic_yuv_stat *p_pre_yuv_stat = NULL;

MUINT32
Hal3AYuv::PictureProcessing(MUINT8 *buf, MUINT32 width, MUINT32 high)
{
    MINT32 count_y_diff_cnt = 0;
    MINT32 count_u_diff_cnt = 0;
    MINT32 count_v_diff_cnt = 0;

    MINT32 left_up_corner_diff_cnt = 0;
    MINT32 right_up_corner_diff_cnt = 0;
    MINT32 left_down_corner_diff_cnt = 0;
    MINT32 right_down_corner_diff_cnt = 0;
    MINT32 left_center_diff_cnt = 0;
    MINT32 right_center_diff_cnt = 0;
    MINT32 center_diff_cnt = 0;

    /************h***************
    *****13  9   5  1****  *
    *******19     17*****  *
    *****14  10  6  2****  w
    ************************
    *****15  11  7  3****  *
    *******20     18*****  *
    *****16  12  8  4****  ******/
    MINT32 block_1_diff_cnt = 0,block_2_diff_cnt = 0,block_3_diff_cnt = 0,block_4_diff_cnt = 0;
    MINT32 block_5_diff_cnt = 0,block_6_diff_cnt = 0,block_7_diff_cnt = 0,block_8_diff_cnt = 0;
    MINT32 block_9_diff_cnt = 0,block_10_diff_cnt = 0,block_11_diff_cnt = 0,block_12_diff_cnt = 0;
    MINT32 block_13_diff_cnt = 0,block_14_diff_cnt = 0,block_15_diff_cnt = 0,block_16_diff_cnt = 0;

    MINT32 block_17_diff_cnt = 0,block_18_diff_cnt = 0,block_19_diff_cnt = 0,block_20_diff_cnt = 0;

    MINT32 count_green_cnt = 0;
    
    MINT32 w,h;
    MINT32 i,j,k;
    MINT32 num_w = COUNT_W_RATIO_NUM, num_h = COUNT_H_RATIO_NUM;
    unsigned char *p_src, *bufY, *bufU, *bufV;

    SCENE_DETECT("PictureProcessing enter\n");

    if(m_rParam.u4AsdMode != 1)
        return 1;

    SCENE_DETECT("delay_frame=%d",delay_frame);
    if(delay_frame > 0) {
        if(scene_indoor == 1 || scene_landscape == 1 || scene_mix_illuminant == 1 ||
            scene_night == 1 || scene_sports == 1 || scene_portrait == 1 || scene_macro == 1) {
            count_change_cnt = 0;
        }
        
        delay_to_stable = 1;    
        delay_frame--;
        return 1;
    }

    if(portrait_detect_flag != 0) {
        SCENE_DETECT("####scene is portrait\n");
        scene_portrait = 1;
        delay_frame = 20;
    }

    if(scene_portrait == 1 || scene_macro == 1 || scene_night == 1) {
        CamSceneDetectPost();
        return 1;
    }
    

    p_src=(unsigned char *)(buf);
    w = width/num_w;
    h =  high/num_h;

    bufY = p_src;
    bufV = p_src + width * high;
    bufU = p_src + width * high* 5/4;
    
    SCENE_DETECT("PictureProcessing start:w=%d h=%d af_focused=%d\n",width,high,af_focused);  

    for(i = 0;i<num_w*num_h;)
  	{
  		for(j = 0;j < h;j++)
		{
	        for(k = 0;k < w;k++)
    		{
                p_yuv_stat[i].y += *(bufY+width*j+k);
    			//p_yuv_stat[i].cr +=*(bufV + (k>>1) +( j>>1)*width/2 );
                //p_yuv_stat[i].cb +=*(bufU + (k>>1) +( j>>1)*width/2 );
			}
		}
		bufY+=w;
		//bufV+=w/2;
		//bufU+=w/2;
		
		i++;
		
		if(i%num_w==0) {
			bufY+=width *( h-1);
			//bufV+=width/2*(h/2-1);
			//bufU+=width/2*(h/2-1);
		}

		//MY_LOG("awb_prase_color adr:i=%d y=0x%08x v=0x%08x u=0x%08x",i,bufY,bufV,bufU);
		
	}

    for(i = 0;i<num_h;i++)
    {
        for(j = 0;j<num_w;j++)
        {
            p_yuv_stat[num_w*i+j].y = p_yuv_stat[num_w*i+j].y/(w*h);
            //p_yuv_stat[num_w*i+j].cr = p_yuv_stat[num_w*i+j].cr/(w*h);
            //p_yuv_stat[num_w*i+j].cb = p_yuv_stat[num_w*i+j].cb/(w*h);
            
            if(abs(p_yuv_stat[num_w*i+j].y - p_pre_yuv_stat[num_w*i+j].y) > 20) {
                count_y_diff_cnt++;

                if(i<num_h/4 && j<num_w/4)
                    block_1_diff_cnt++;
                if(i<num_h/4 && j>=num_w/4 && j<num_w/2)
                    block_2_diff_cnt++;
                if(i<num_h/4 && j>=num_w/2 && j<num_w*3/4)
                    block_3_diff_cnt++;
                if(i<num_h/4 && j>=num_w*3/4)
                    block_4_diff_cnt++;

                if(i>=num_h/4 && i<num_h/2 && j<num_w/4)
                    block_5_diff_cnt++;
                if(i>=num_h/4 && i<num_h/2 && j>=num_w/4 && j<num_w/2)
                    block_6_diff_cnt++;
                if(i>=num_h/4 && i<num_h/2 && j>=num_w/2 && j<num_w*3/4)
                    block_7_diff_cnt++;
                if(i>=num_h/4 && i<num_h/2 && j>=num_w*3/4)
                    block_8_diff_cnt++;

                if(i>=num_h/2 && i<num_h*3/4 && j<num_w/4)
                    block_9_diff_cnt++;
                if(i>=num_h/2 && i<num_h*3/4 && j>=num_w/4 && j<num_w/2)
                    block_10_diff_cnt++;
                if(i>=num_h/2 && i<num_h*3/4 && j>=num_w/2 && j<num_w*3/4)
                    block_11_diff_cnt++;
                if(i>=num_h/2 && i<num_h*3/4 && j>=num_w*3/4)
                    block_12_diff_cnt++;

                if(i>=num_h*3/4 && j<num_w/4)
                    block_13_diff_cnt++;
                if(i>=num_h*3/4 && j>=num_w/4 && j<num_w/2)
                    block_14_diff_cnt++;
                if(i>=num_h*3/4 && j>=num_w/2 && j<num_w*3/4)
                    block_15_diff_cnt++;
                if(i>=num_h*3/4 && j>=num_w*3/4)
                    block_16_diff_cnt++;

                if(i>=num_h/8 && i<num_h*3/8 && j>=num_w/8 && j<num_w*3/8)
                    block_17_diff_cnt++;
                if(i>=num_h/8 && i<num_h*3/8 && j>=num_w*5/8 && j<num_w*7/8)
                    block_18_diff_cnt++;
                if(i>=num_h*5/8 && i<num_h*7/8 && j>=num_w/8 && j<num_w*3/8)
                    block_19_diff_cnt++;
                if(i>=num_h*5/8 && i<num_h*7/8 && j>=num_w*5/8 && j<num_w*7/8)
                    block_20_diff_cnt++;

                if(i>=num_h/4 && i<num_h*3/4 && j>=num_w/4 && j<num_w*3/4)
                    center_diff_cnt++;
            }
            p_pre_yuv_stat[num_w*i+j].y = p_yuv_stat[num_w*i+j].y;

            /* YUV to RGB */
            /*p_rgb_stat[num_w*i+j].mR=1.164*(p_yuv_stat[num_w*i+j].y-16) + 1.596 * (p_yuv_stat[num_w*i+j].cr-128);
            p_rgb_stat[num_w*i+j].mG=1.164*(p_yuv_stat[num_w*i+j].y-16) - 0.813*(p_yuv_stat[num_w*i+j].cr-128) - 0.392*(p_yuv_stat[num_w*i+j].cb-128);
            p_rgb_stat[num_w*i+j].mB=1.164*(p_yuv_stat[num_w*i+j].y-16) + 2.017 * (p_yuv_stat[num_w*i+j].cb-128);

            p_rgb_stat[num_w*i+j].mR = max( 0, min( p_rgb_stat[num_w*i+j].mR, 255 ) );
            p_rgb_stat[num_w*i+j].mG = max( 0, min( p_rgb_stat[num_w*i+j].mG, 255 ) );
            p_rgb_stat[num_w*i+j].mB = max( 0, min( p_rgb_stat[num_w*i+j].mB, 255 ) );

            if(p_rgb_stat[num_w*i+j].mG > 120) {
                count_green_cnt++;
            }*/
        }
    }

    if(delay_to_stable == 1) {
        delay_to_stable = 0;
        SCENE_DETECT("####delay 1 frame to pic-data stable after delay_frame\n");
        
        return 1;
    }


    if(count_y_diff_cnt > FULL_THRESHOLD
        || block_1_diff_cnt>BORDER_THRESHOLD || block_2_diff_cnt>BORDER_THRESHOLD
        || block_3_diff_cnt>BORDER_THRESHOLD || block_4_diff_cnt>BORDER_THRESHOLD
        || block_5_diff_cnt>BORDER_THRESHOLD || block_6_diff_cnt>BORDER_THRESHOLD
        || block_7_diff_cnt>BORDER_THRESHOLD || block_8_diff_cnt>BORDER_THRESHOLD
        || block_9_diff_cnt>BORDER_THRESHOLD || block_10_diff_cnt>BORDER_THRESHOLD
        || block_11_diff_cnt>BORDER_THRESHOLD || block_12_diff_cnt>BORDER_THRESHOLD
        || block_13_diff_cnt>BORDER_THRESHOLD || block_14_diff_cnt>BORDER_THRESHOLD
        || block_15_diff_cnt>BORDER_THRESHOLD || block_16_diff_cnt>BORDER_THRESHOLD
        || block_17_diff_cnt>BORDER_THRESHOLD || block_18_diff_cnt>BORDER_THRESHOLD
        || block_19_diff_cnt>BORDER_THRESHOLD || block_20_diff_cnt>BORDER_THRESHOLD
        || center_diff_cnt>CENTER_THRESHOLD){
		sporting_cnt++;
		sporting_stop_cnt=0;
		
		SCENE_DETECT("####scene is sport\n");
    }
    else {
        sporting_stop_cnt++;
        sporting_cnt=0;
    }

    if(sporting_cnt > 3) {
        SCENE_DETECT("####sporting_cnt=%d\n",sporting_cnt);
        delay_frame = 3;

        scene_sports = 1;
    }

    if(sporting_stop_cnt > 3) {
        SCENE_DETECT("####sporting_stop_cnt=%d\n",sporting_stop_cnt);
        delay_frame = 3;
    }

    CamSceneDetectPost();
    
    return 1;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
Hal3AYuv::CamSceneDetectInit()
{
    SCENE_DETECT("aaa_hal_yuv:CamSceneDetectInit enter\n");

    pre_fv = 0;
    pre_br = 0;
    pre_wb = 0;

    af_position = 0;
    af_focused = 1;
    portrait_detect_flag = 0;     

    count_change_cnt = 0;
    delay_to_stable = 0;
    delay_frame = 30;
    sporting_cnt = 0;
    sporting_stop_cnt = 0;
    PreScene_to_Auto = 0;

    count_flag = 0;
    stable_cnt = 0;
    change_cnt_wb = 0;
    change_cnt_br = 0;
    averWB_10frame = 0;
    averBr_10frame = 0;
	averWB_10frame_cnt = 0;

    scene_indoor = 0;
    scene_landscape = 0;
    scene_mix_illuminant = 0;
    scene_night = 0;
    scene_sports = 0;
    scene_portrait = 0;
    scene_macro = 0;

    scene_cur = 0;
    scene_pre = 0;

    //get system time
    int i = 0, j = 0, z = 0;
    time_t start = time(NULL);
    char *get_time;
    char get_hour[10] = {0};
    get_time=ctime(&start);
    for(i=0,j=0,z=0;i<48;i++) {
        if(get_time[i] != '\0') {
            if(get_time[i] == ' ') {
                j++;
                if(j==2 && get_time[i+1]==' ')
                    j--;
                z=0;
                continue;
            }
            else {
                if(j==3) {
                    get_hour[z++]=get_time[i];
                } if(j==4) {
                    break;
                }
            }
        }
        else
            break;
    }
    //SCENE_DETECT("#####time:%s\n",get_time);
    //SCENE_DETECT("#####hour time:%s\n",get_hour);
    cur_time=atoi(get_hour);
    MY_LOG("CamSceneDetectInit:cur_time=%d\n",cur_time);

    scene_para.indoor_min_br = 300;
    scene_para.indoor_max_br = 1200;
    scene_para.outdoor_min_br = 2200;
    scene_para.outdoor_min_wb = 4800;    
    scene_para.night_max_wb = 5000;
    scene_para.night_max_br = 100;
    scene_para.mix_max_wb = 4100/*4500*/;
    scene_para.mix_min_wb = 3300/*3200*/;
	
    MY_LOG("indoor(%d %d)outdoor(%d %d)night(%d %d)mix(%d %d)",scene_para.indoor_min_br,
        scene_para.indoor_max_br,scene_para.outdoor_min_br,scene_para.outdoor_min_wb,
        scene_para.night_max_wb,scene_para.night_max_br,scene_para.mix_max_wb,scene_para.mix_min_wb);

    //rgb memory malloc
    /*if(!p_rgb_stat)
        p_rgb_stat = (pic_rgb_stat*)malloc(COUNT_W_RATIO_NUM * COUNT_H_RATIO_NUM * sizeof(pic_rgb_stat));

    if(p_rgb_stat == NULL)
    {
        SCENE_DETECT("CamSceneDetectInit:rgb_stat memory malloc fail\n"); 
        return 0;
    } else {
        SCENE_DETECT("CamSceneDetectInit:rgb_stat memory malloc success\n");
        memset(p_rgb_stat, 0, (COUNT_W_RATIO_NUM * COUNT_H_RATIO_NUM * sizeof(pic_rgb_stat)));
    }*/

    //yuv memory malloc
    if(!p_yuv_stat)
        p_yuv_stat = (pic_yuv_stat*)malloc(COUNT_W_RATIO_NUM * COUNT_H_RATIO_NUM * sizeof(pic_yuv_stat));

    if(p_yuv_stat == NULL)
    {
        SCENE_DETECT("CamSceneDetectInit:yuv_stat memory malloc fail\n"); 
        return 0;
    } else {
        SCENE_DETECT("CamSceneDetectInit:yuv_stat memory malloc success\n");
        memset(p_yuv_stat, 0, (COUNT_W_RATIO_NUM * COUNT_H_RATIO_NUM * sizeof(pic_yuv_stat)));
    }

    if(!p_pre_yuv_stat)
        p_pre_yuv_stat = (pic_yuv_stat*)malloc(COUNT_W_RATIO_NUM * COUNT_H_RATIO_NUM * sizeof(pic_yuv_stat));

    if(p_pre_yuv_stat == NULL)
    {
        SCENE_DETECT("CamSceneDetectInit:pre_yuv_stat memory malloc fail\n"); 
        return 0;
    } else {
        SCENE_DETECT("CamSceneDetectInit:pre_yuv_stat memory malloc success\n");
        memset(p_pre_yuv_stat, 0, (COUNT_W_RATIO_NUM * COUNT_H_RATIO_NUM * sizeof(pic_yuv_stat)));
    }
    
    SCENE_DETECT("aaa_hal_yuv:CamSceneDetectInit end\n");

    return 1;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
Hal3AYuv::CamSceneDetectUninit()
{
    SCENE_DETECT("aaa_hal_yuv:CamSceneDetectUninit enter\n");

    //free rgb memory
    if(p_rgb_stat) {
		free(p_rgb_stat);
		SCENE_DETECT("CamSceneDetectUninit:free rgb_stat memory\n");
	}
	p_rgb_stat = NULL;

	//free yuv memory
    if(p_yuv_stat) {
		free(p_yuv_stat);
		SCENE_DETECT("CamSceneDetectUninit:free yuv_stat memory\n");
	}
	p_yuv_stat = NULL;

	if(p_pre_yuv_stat) {
		free(p_pre_yuv_stat);
		SCENE_DETECT("CamSceneDetectUninit:free pre_yuv_stat memory\n");
	}
	p_pre_yuv_stat = NULL;

	SCENE_DETECT("aaa_hal_yuv:CamSceneDetectUninit end\n");

    return 1;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
Hal3AYuv::CamSceneDetect()
{
    SCENE_DETECT("aaa_hal_yuv:CamSceneDetect enter\n");
    
    MUINT32 cur_br = getBrightness();
    //MUINT32 cur_fv = getWaveDetectValue();
    MUINT32 cur_fv = 0;
    MUINT32 cur_wb = getWhiteBalance();
    MUINT32 cur_ae = getAeParameter();
    MUINT32 cur_ae_gain = cur_ae>>16&0xFFFF;
    MUINT32 cur_ae_shutter = cur_ae&0xFFFF;
    
    /*SCENE_DETECT("####test:br=%d gain=%d shutter=%d",cur_br,cur_ae_gain,cur_ae_shutter);
    SCENE_DETECT("####test:indoor(%d %d)outdoor(%d %d)night(%d %d)mix(%d %d)",scene_para.indoor_min_br,
        scene_para.indoor_max_br,scene_para.outdoor_min_br,scene_para.outdoor_min_wb,
        scene_para.night_max_wb,scene_para.night_max_br,scene_para.mix_max_wb,scene_para.mix_min_wb);
    SCENE_DETECT("#####test:count_flag=%d af_position=%d count_change_cnt=%d\n",
        count_flag,af_position,count_change_cnt);
    SCENE_DETECT("#####test:br=%d pre_br=%d ***wb=%d pre_wb=%d",cur_br,pre_br,cur_wb,pre_wb);
    SCENE_DETECT("#####test:wbcnt=%d brcnt=%d stable_cnt=%d\n",change_cnt_wb,change_cnt_br,stable_cnt);
    SCENE_DETECT("#####test:averwb=%d averbr=%d avercnt=%d",averWB_10frame,averBr_10frame, averWB_10frame_cnt);*/
    
    if(cur_wb == 0) {
        count_flag = 0;
        stable_cnt=0;
        averWB_10frame = 0;
        averBr_10frame = 0;
        averWB_10frame_cnt = 0;
    } else {           
        if(count_flag) {
            SCENE_DETECT("#####test:detect change\n");
            if(abs(averWB_10frame - cur_wb) > 200) {
                change_cnt_wb++;
                SCENE_DETECT("#####test:scene change for wb_cnt=%d\n",change_cnt_wb);
            } else {
                change_cnt_wb = 0;
            }
            
            if(abs(averBr_10frame - cur_br) > 100) {
                change_cnt_br++;
                SCENE_DETECT("#####test:scene change for br_cnt=%d\n",change_cnt_br);
            } else {
                change_cnt_br = 0;
            }

            if(change_cnt_wb >= 6 || change_cnt_br >= 8) {
                count_flag = 0;
                stable_cnt=0;
                count_change_cnt = 0;
                averWB_10frame = 0;
                averBr_10frame = 0;
                averWB_10frame_cnt = 0;
                
                scene_cur = scene_pre;
            }
    
        }

        /*if(!count_flag) {
            if(abs(pre_br-cur_br)<100 && abs(pre_wb-cur_wb)<150)
                stable_cnt++;
            else {
                stable_cnt = 0;

                averWB_10frame = 0;
                averBr_10frame = 0;
                averWB_10frame_cnt = 0;
            }
        }*/            

        if(!count_flag/* && stable_cnt>2*/) {
            if(++averWB_10frame_cnt <= COUNT_AVER_CNT) {
                averWB_10frame += cur_wb;
                averBr_10frame += cur_br;
            }
            else {
                averWB_10frame = averWB_10frame/COUNT_AVER_CNT;
                averBr_10frame = averBr_10frame/COUNT_AVER_CNT;
                //SCENE_DETECT("#####averWB_10frame=%d averBr_10frame=%d\n",averWB_10frame,averBr_10frame);
                MY_LOG("#####averWB_10frame=%d averBr_10frame=%d\n",averWB_10frame,averBr_10frame);
                
                count_flag = 1;
            }
        }

        if(count_flag == 1) {
            SCENE_DETECT("####test:count_flag = 1\n");
            count_change_cnt++;


            if(averBr_10frame == 0 && cur_ae_gain >= 300) {
                scene_night = 1;
                SCENE_DETECT("####scene is night at br==0\n");
            } else if(averBr_10frame < scene_para.night_max_br) {
                if(averWB_10frame < scene_para.night_max_wb && (cur_time > 18 && cur_time < 7)) {
                    scene_night = 1;
                    SCENE_DETECT("####scene is night when night\n");
                }
            }

            if(averBr_10frame > scene_para.indoor_min_br && averBr_10frame <= 500 && af_position != 1
                && averWB_10frame < 4600) {
                scene_indoor = 1;
                SCENE_DETECT("####scene is indoor mid br\n");
            }

            if(averBr_10frame > 500 && averBr_10frame <= scene_para.indoor_max_br && af_position != 1
                /*&& averWB_10frame < 6200*/) {
                if(averWB_10frame > 5200 && averWB_10frame < 5600
                    && averBr_10frame < 800) {
                    /* maybe is facing toward grassland at dusk*/
                } else {
                    scene_indoor = 1;
                    SCENE_DETECT("####scene is indoor normal br\n");
                }
            }

            if(averWB_10frame > 6000 && averBr_10frame > 1800) {
                SCENE_DETECT("####scene is landscape low br\n");
                scene_landscape = 1;
            }
            
            
            if(averBr_10frame >= scene_para.outdoor_min_br &&
                averWB_10frame >= scene_para.outdoor_min_wb &&
                af_position == 3) {
                SCENE_DETECT("####scene is landscape\n");
                scene_landscape = 1;
            }
        }
    }

    if(af_position == 1) {
        scene_macro = 1;
        SCENE_DETECT("####scene is macro\n");
    }

    pre_fv = cur_fv;
    pre_br = cur_br;
    pre_wb = cur_wb;

    return scene_cur;
}

/*******************************************************************************
*
********************************************************************************/
MINT32
Hal3AYuv::CamSceneDetectPost()
{
    MINT32 i;
    
    SCENE_DETECT("aaa_hal_yuv:CamSceneDetectPost enter\n");

    if(scene_indoor == 1 || scene_landscape == 1 || scene_mix_illuminant == 1 ||
        scene_night == 1 || scene_sports == 1 || scene_portrait == 1 || scene_macro == 1) {
        count_change_cnt = 0;
    }

    if (scene_portrait == 1){
		scene_cur = SCENE_PORTRAIT;
		goto out ;
	}
	else if(scene_macro == 1 ){
		scene_cur = SCENE_MACRO;
	 	goto out ;
	}
	else if(scene_night == 1 ){
		scene_cur = SCENE_NIGHTSCENE;
	 	goto out ;
	}
	else if(scene_sports == 1 ){
		scene_cur = SCENE_SPORTS;
	 	goto out ;	
	}	
	else if(scene_landscape == 1){
		scene_cur = SCENE_LANDSCAPE;
		goto out ;
	}
	else if(scene_indoor == 1 ){
	    if(averWB_10frame > scene_para.mix_min_wb &&
	        averWB_10frame < scene_para.mix_max_wb) {
            scene_cur = SCENE_MIX_ILLUMINANT;
	    } else {
		    scene_cur = SCENE_INDOOR;
	    }
		goto out ;
	}
	else if(scene_mix_illuminant == 1 ){
		scene_cur = SCENE_MIX_ILLUMINANT;
		goto out ;
	}
	else if(count_change_cnt >= 10) {
	    scene_cur = SCENE_NORMAL;
    }

    out:
        SCENE_DETECT("####scene cur value=%d scene pre value=%d\n",scene_cur,scene_pre);

    SCENE_DETECT("ptrait=%d lscape=%d indoor=%d mix=%d night=%d sports=%d macro=%d\n",
        scene_portrait,scene_landscape,scene_indoor,scene_mix_illuminant,scene_night,
        scene_sports,scene_macro);

    /*for(i=0;i<8;i++) {
        if(scene_mode_define[i].scene == scene_cur) {
            //SCENE_DETECT("####scene_cur=%s\n",scene_mode_define[i].scene_name);
            MY_LOG("####scene_cur=%s\n",scene_mode_define[i].scene_name);
            break;
        }
    }*/

    if(scene_pre != scene_cur) {
        SCENE_DETECT("####scene change\n");
        MY_LOG("####scene_cur=%d\n",scene_cur);

        #ifdef SET_SCENE_IN_PREVIEW
        setScene(scene_cur);
        #endif

        m_pAFYuvCallBack->doNotifyCb(0x40000000, 0x00000002, scene_cur, 0);
    }
    //m_pAFYuvCallBack->doNotifyCb(0x40000000, 0x00000002, scene_cur, 0);

    scene_indoor = 0;
    scene_landscape = 0;
    scene_mix_illuminant = 0;
    scene_night = 0;
    scene_sports = 0;
    scene_portrait = 0;
    scene_macro = 0;

    scene_pre = scene_cur;

    return 1;    
}
#endif /* VENDOR_EDIT */
