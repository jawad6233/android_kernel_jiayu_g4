/************************************************************************************
** File: - AudioLoopBackTest.cpp
** VENDOR_EDIT
** Copyright (C), 2008-2012, OPPO Mobile Comm Corp., Ltd
** 
** Description: 
**      Audio LoopBack Test
** 
** Version: 1.0
** Date created: 15:31:51,25/02/2012
** Author: Xiaojun.Lv@MediaDriver.AudioAFE
** 
** --------------------------- Revision History: --------------------------------
** 	<author>	<data>			<desc>
** 
************************************************************************************/
#ifdef VENDOR_EDIT
#include <hardware_legacy/AudioHardwareBase.h>
#include <hardware_legacy/AudioSystemLegacy.h>
#include <hardware_legacy/AudioPolicyInterface.h>
#include <utils/String8.h>

#include "AudioLoopBackTest.h"

#undef LOG_TAG
#define LOG_TAG "AudioLoopBackTest"
#include <utils/Log.h>

namespace android
{
static String8 keyLBT_PHmic_PHReciver = String8("AudioLBT_PHmic_PHReciver");
static String8 keyLBT_PHmic_PHSpeaker = String8("AudioLBT_PHmic_PHSpeaker");
static String8 keyLBT_PHmic_HSReciver = String8("AudioLBT_PHmic_HSReciver");
static String8 keyLBT_HSmic_PHReciver = String8("AudioLBT_HSmic_PHReciver");
static String8 keyLBT_HSmic_PHSpeaker = String8("AudioLBT_HSmic_PHSpeaker");
static String8 keyLBT_HSmic_HSReciver = String8("AudioLBT_HSmic_HSReciver");
static String8 keyReceiverTest = String8("receiverTest");
static String8 keyReceiverLoopbackTest = String8("receiverLoopbackTest");
static String8 keyEarphoneLoopbackTest = String8("earphoneLoopbackTest");
//lvxj@MediaDriver.AudioAFE 2012/08/02, add for Ref Mic LoopBackTest
static String8 keyRefMic2RcvLoopbackTest = String8("RefMic2RcvLoopbackTest");
static String8 keyLAD_SetReceiverTest = String8("LAD_SetReceiverTest");

AudioLoopBackTest::AudioLoopBackTest(AudioMTKHardware *hw)
{
    mHw = hw;
    mInLoopbackTest = false;
    mAudioFtmInstance = AudioFtm::getInstance();
}

AudioLoopBackTest::~AudioLoopBackTest()
{
}

bool AudioLoopBackTest::AudioLoopBackTestMain(AudioParameter *param)
{
    ALOGD("<<<<<AudioLoopBackTestMain");
    int value = 0;
    if(param->getInt(keyLBT_HSmic_HSReciver, value) == NO_ERROR)
    {
        param->remove(keyLBT_HSmic_HSReciver);
        if(value )
        {
            LoopbackManager::GetInstance()->SetLoopbackOn(AP_HEADSET_MIC_AFE_LOOPBACK, LOOPBACK_OUTPUT_EARPHONE);
        }
        else
        {
            LoopbackManager::GetInstance()->SetLoopbackOff();
        }
    	goto EXIT_SETPARAMETERS;
    }
    if(param->getInt(keyLBT_HSmic_PHReciver, value) == NO_ERROR)
    {
        param->remove(keyLBT_HSmic_PHReciver);
        if(value )
        {
            LoopbackManager::GetInstance()->SetLoopbackOn(AP_HEADSET_MIC_AFE_LOOPBACK, LOOPBACK_OUTPUT_RECEIVER);
        }
        else
        {
            LoopbackManager::GetInstance()->SetLoopbackOff();
        }
    	goto EXIT_SETPARAMETERS;
    }
    if(param->getInt(keyLBT_HSmic_PHSpeaker, value) == NO_ERROR)
    {
        param->remove(keyLBT_HSmic_PHSpeaker);
        if(value )
        {
            LoopbackManager::GetInstance()->SetLoopbackOn(AP_HEADSET_MIC_AFE_LOOPBACK, LOOPBACK_OUTPUT_SPEAKER);
        }
        else
        {
            LoopbackManager::GetInstance()->SetLoopbackOff();
        }
    	goto EXIT_SETPARAMETERS;
    }
    if(param->getInt(keyLBT_PHmic_HSReciver, value) == NO_ERROR)
    {
        param->remove(keyLBT_PHmic_HSReciver);
        if(value)
        {
            LoopbackManager::GetInstance()->SetLoopbackOn(AP_MAIN_MIC_AFE_LOOPBACK, LOOPBACK_OUTPUT_EARPHONE);
        }
        else
        {
            LoopbackManager::GetInstance()->SetLoopbackOff();
        }
    	goto EXIT_SETPARAMETERS;
    }
    if(param->getInt(keyLBT_PHmic_PHReciver, value) == NO_ERROR)
    {
        param->remove(keyLBT_PHmic_PHReciver);
        if(value)
        {
            LoopbackManager::GetInstance()->SetLoopbackOn(AP_MAIN_MIC_AFE_LOOPBACK, LOOPBACK_OUTPUT_RECEIVER);
        }
        else
        {
            LoopbackManager::GetInstance()->SetLoopbackOff();
        }
    	goto EXIT_SETPARAMETERS;
    }
    if(param->getInt(keyLBT_PHmic_PHSpeaker, value) == NO_ERROR)
    {
        param->remove(keyLBT_PHmic_PHSpeaker);
        if(value)
        {
            LoopbackManager::GetInstance()->SetLoopbackOn(AP_MAIN_MIC_AFE_LOOPBACK, LOOPBACK_OUTPUT_SPEAKER);
        }
        else
        {
            LoopbackManager::GetInstance()->SetLoopbackOff();
        }
    	goto EXIT_SETPARAMETERS;
    }
//LeiYu@MultimediaSrv.LoopTest, 2012/01/16, Add for looptest
	if (param->getInt(keyReceiverTest, value) == NO_ERROR) 
	{
		ALOGD("LY__**receiverTest = %d", value);
		if(value){
			ALOGD("open receiver test!");
    		mAudioFtmInstance->RecieverTest(true);
		}
		else
		{
			ALOGD("close receiver test!");
    		mAudioFtmInstance->RecieverTest(false);
		}
		param->remove(keyReceiverTest);
		goto EXIT_SETPARAMETERS;
	}
	
	if (param->getInt(keyReceiverLoopbackTest, value) == NO_ERROR) 
	{
	    ALOGD("**receiverLoopbackTest = %d", value);
		if(value)
		{
			ALOGD("open receiverLoopbackTest test!");
			LoopbackManager::GetInstance()->SetLoopbackOn(AP_MAIN_MIC_AFE_LOOPBACK, LOOPBACK_OUTPUT_RECEIVER);
		}
		else
		{
			ALOGD("close receiverLoopbackTest test!");
			LoopbackManager::GetInstance()->SetLoopbackOff();
		}
		param->remove(keyReceiverLoopbackTest);
		goto EXIT_SETPARAMETERS;
	}
	if (param->getInt(keyEarphoneLoopbackTest, value) == NO_ERROR) 
	{
        ALOGD("earphoneLoopbackTest = %d", value);
        if(value)
        {
            mInLoopbackTest = true;
        #if 0 //Jirui.kang@BasicDrv.Audio, 2013/03/09, Modify for receiverLoopbackTest problem 
            LoopbackManager::GetInstance()->SetLoopbackOn(AP_MAIN_MIC_AFE_LOOPBACK, LOOPBACK_OUTPUT_EARPHONE);
        #else
            LoopbackManager::GetInstance()->SetLoopbackOn(AP_MAIN_MIC_AFE_LOOPBACK, LOOPBACK_OUTPUT_RECEIVER);
        #endif
        }
        else
        {
            if( mInLoopbackTest == true )
            {
                mInLoopbackTest = false;
                LoopbackManager::GetInstance()->SetLoopbackOff();
            }
            else
            {
                ALOGD("zhli do nothing for earphoneLoopbackTest!!");
            }
        }

        param->remove(keyEarphoneLoopbackTest);
        goto EXIT_SETPARAMETERS;
    }
//end LeiYu@MultimediaSrv.LoopTest, 2012/01/16, Add for looptest
    //lvxj@MediaDriver.AudioAFE 2012/08/02, add for Ref Mic LoopBackTest
    if (param->getInt(keyRefMic2RcvLoopbackTest, value) == NO_ERROR) 
    {
        ALOGD("RefMic2RcvLoopbackTest = %d", value);
        if(value)
        {
            mInLoopbackTest = true;
            LoopbackManager::GetInstance()->SetLoopbackOn(AP_REF_MIC_AFE_LOOPBACK, LOOPBACK_OUTPUT_SPEAKER);
        }
        else
        {
            if( mInLoopbackTest == true )
            {
                mInLoopbackTest = false;
                ALOGD("RefMic2RcvLoopbackTest DEVICE_IN_REF_MIC");
                LoopbackManager::GetInstance()->SetLoopbackOff();
            }
            else
            {
                ALOGD("zhli do nothing for RefMic2RcvLoopbackTest!!");
            }
        }
        param->remove(keyRefMic2RcvLoopbackTest);
        goto EXIT_SETPARAMETERS;
    }
    //end lvxj@MediaDriver.AudioAFE 2012/08/02, add for Ref Mic LoopBackTest
EXIT_SETPARAMETERS:
    return mInLoopbackTest;

}

}
#endif /*VENDOR_EDIT*/







