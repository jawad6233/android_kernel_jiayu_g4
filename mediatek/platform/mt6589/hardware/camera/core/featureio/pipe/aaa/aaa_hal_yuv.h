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

#ifndef _AAA_HAL_YUV_H_
#define _AAA_HAL_YUV_H_

//------------Thread-------------
#include <linux/rtpm_prio.h>
#include <pthread.h>
#include <semaphore.h>
//-------------------------------
#include <isp_drv.h>

#include <aaa_hal_base.h>
#include <utils/threads.h>
#include <sensor_hal.h>

using namespace android;

namespace NS3A
{


/*******************************************************************************
*
********************************************************************************/
class Hal3AYuv : public Hal3ABase
{

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
protected:
    Hal3AYuv();
    virtual ~Hal3AYuv();

#ifdef VENDOR_EDIT
//ZhangZhengrong@MtkCamera, 2013/03/19, Add for scene detect
	MUINT32 pre_fv;
	MUINT32 pre_br;
	MUINT32 pre_wb;
	
	MINT32 count_flag;
	MINT32 stable_cnt;
	MINT32 change_cnt_wb;
	MINT32 change_cnt_br;
	MUINT32 averWB_10frame;
	MUINT32 averBr_10frame;
	MINT32 averWB_10frame_cnt;

	MINT32 af_position;
	MINT32 af_focused;
	MUINT32 cur_time;
	
	MINT32 count_change_cnt;
	MINT32 delay_to_stable;
	MINT32 delay_frame;
	MINT32 sporting_cnt;
	MINT32 sporting_stop_cnt;	

	MINT32 scene_indoor;
	MINT32 scene_landscape;
	MINT32 scene_mix_illuminant;
	MINT32 scene_night;
	MINT32 scene_sports;
	MINT32 scene_portrait;
	MINT32 scene_macro;

	MINT32 scene_cur;
	MINT32 scene_pre;
	MUINT32 PreScene_to_Auto;
#endif /* VENDOR_EDIT */


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Interfaces.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:
    static Hal3AYuv* createInstance(MINT32 const i4SensorDevId);
    static Hal3AYuv* getInstance();
    virtual MVOID destroyInstance();
    virtual MBOOL sendCommand(ECmd_T const eCmd, MINT32 const i4Arg = 0);

    virtual inline MINT32 getErrorCode() const
    {
        return m_errorCode;
    }

    virtual inline MBOOL getParams(Param_T &rParam) const
    {
        rParam = m_rParam;
        return MTRUE;
    }

    virtual MBOOL setParams(Param_T const &rNewParam);
    virtual MBOOL getSupportedParams(FeatureParam_T &rFeatureParam);

    virtual MBOOL isReadyToCapture() const;
	#if 0
    {
        return m_bReadyToCapture;
    }
	#endif

    virtual MBOOL autoFocus();
    virtual MBOOL cancelAutoFocus();
    virtual MBOOL setZoom(MUINT32 u4ZoomRatio_x100, MUINT32 u4XOffset, MUINT32 u4YOffset, MUINT32 u4Width, MUINT32 u4Height);
    virtual MBOOL set3AEXIFInfo(IBaseCamExif *pIBaseCamExif) const;
    virtual MBOOL setDebugInfo(IBaseCamExif *pIBaseCamExif) const;
    virtual MINT32 getDelayFrame(EQueryType_T const eQueryType) const;
    virtual MBOOL setIspProfile(EIspProfile_T const eIspProfile);
    virtual MRESULT EnableAFThread(MINT32 a_bEnable);
    virtual MBOOL setCallbacks(I3ACallBack* cb);
	virtual MBOOL setFDInfo(MVOID* a_sFaces);
	
	#ifdef VENDOR_EDIT
	//ZhangZhengrong@MtkCamera, 2013/03/19, Add for scene detect
	virtual MINT32 CamSceneDetect();
	virtual MINT32 CamSceneDetectPost();
	virtual MINT32 CamSceneDetectInit();
	virtual MINT32 CamSceneDetectUninit();
    virtual MUINT32 getWaveDetectValue(void);
	virtual MINT32 getBrightness(void);
	virtual MINT32 getWhiteBalance(void);
	virtual MUINT32 getAeParameter(void);
	virtual MINT32 getAFposition(void);
	virtual MINT32 setScene(MUINT32 a_u4Scene);
	virtual MUINT32 PictureProcessing(MUINT8 *frm_Addr, MUINT32 w, MUINT32 h);
	#endif /* VENDOR_EDIT */

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Implementation
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
public:
    MRESULT init(MINT32 i4SensorDev);
    MRESULT uninit();

    inline MVOID setErrorCode(MRESULT errorCode)
    {
        m_errorCode = errorCode;
    }

    inline MVOID resetReadyToCapture()
    {
        m_bReadyToCapture = MFALSE;
    }

    inline MVOID notifyReadyToCapture()
    {
        m_bReadyToCapture = MTRUE;
    }

    inline MINT32 getSensorDev()
    {
        return m_i4SensorDev;
    }

    private:
    static MVOID* AFThreadFunc(void *arg);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Data member
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
private:
    volatile int  m_Users;
    mutable Mutex m_Lock;
    MRESULT       m_errorCode;
    Param_T       m_rParam;
    MBOOL         m_bReadyToCapture;
    MINT32        m_i4SensorDev;
	I3ACallBack*  m_pAFYuvCallBack;
    SensorHal*    m_pSensorHal;           

	//for sensor af error state timing 900ms
    MINT32 m_i4AFErrorCount;
	MBOOL m_bIsFocused;
	MBOOL m_bAFDone;
	MINT32 m_i4AFMode;
	MINT32 m_i4StrobeMode;

    MBOOL bAELockSupp;
    MBOOL bAWBLockSupp;

	#ifdef VENDOR_EDIT
	//LiuBin@MtkCamera, 2013/03/05, Add for yuv AF
	MINT32			m_i4ZoomX;
	MINT32			m_i4ZoomY;
	MINT32			m_i4ZoomWidth;
	MINT32			m_i4ZoomHeight;
	CameraArea_T 	m_sAFArea[MAX_FOCUS_AREAS];
	MINT32 			m_AFzone[6];
	MINT32			m_i4AFAreaCnt;
	CameraArea_T	m_sFDArea;
	MINT32			m_FDZone[6];

	//state
	ECmd_T		m_eCmdState;
	#endif /* VENDOR_EDIT */

private:
	MINT32 mapAEToEnum(MINT32 mval,MFLOAT mstep);
	MINT32 mapISOToEnum(MUINT32 u4NewAEISOSpeed);
	MINT32 mapEnumToISO(MUINT32 u4NewAEIsoEnum) const;

	void mapAreaToZone(
		CameraArea_T *psArea, 
		MINT32 u4AreaW, 
		MINT32 u4AreaH,
		MINT32 *psZone,
		MINT32 u4ZoneW,
		MINT32 u4ZoneH);
	MBOOL isFocusFinish();
	MRESULT setAFMode(MINT32 a_eAFMode);
	MVOID setISPCapture();
	MVOID setState(ECmd_T eCmdState);
	MVOID updateAFAreaAndZone(CameraFocusArea_T *psFocusArea, MINT32 u4AreaW, MINT32 u4AreaH);
	#ifdef VENDOR_EDIT
	//feng.hu@mtk camera team modified for ms2r continuous af
	void sendAfcCommand(void);
	MINT32 af_status_pre;
	MINT32 mdelayFrameCnt;
	MBOOL  m_bCAFTriggered;
	MINT32 mFrameCnt;
	#endif

};

}; // namespace NS3A

#endif

