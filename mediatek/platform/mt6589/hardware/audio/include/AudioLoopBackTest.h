/************************************************************************************
** File: - AudioLoopBackTest.h
** VENDOR_EDIT
** Copyright (C), 2008-2012, OPPO Mobile Comm Corp., Ltd
** 
** Description: 
**      Audio LoopBack Test headfile
** 
** Version: 1.0
** Date created: 15:33:52,25/02/2012
** Author: Xiaojun.Lv@MediaDriver.AudioAFE
** 
** --------------------------- Revision History: --------------------------------
** 	<author>	<data>			<desc>
** 
************************************************************************************/

#ifdef VENDOR_EDIT
#ifndef ANDROID_AUDIO_LOOPBACK_TEST_H
#define ANDROID_AUDIO_LOOPBACK_TEST_H


#include "AudioMTKHardware.h"
#include "AudioFtm.h"
#include "LoopbackManager.h"


namespace android {


class AudioMTKHardware ;


//LeiYu@MultimediaSrv.LoopTest, 2012/01/17, Add for looptest
//mic type for loopback used
/*enum Loopback_MicType
{
    MIC1_OFF=0,
    MIC1_ON,
    MIC2_OFF,
    MIC2_ON
};*/
#ifndef MIC1_OFF
#define MIC1_OFF  0
#endif

#ifndef MIC1_ON
#define MIC1_ON   1
#endif

#ifndef MIC2_OFF
#define MIC2_OFF  2
#endif

#ifndef MIC2_ON
#define MIC2_ON   3
#endif


//loopback types definitaion.
enum LOOPBACK_TYPE
{
    LPTYPE_ALL_OFF=0,
    //Main mic loop
    LPTYPE_MAINMIC_2_RECEIVER,
    LPTYPE_MAINMIC_2_SPK,
    LPTYPE_MAINMIC_2_HEADSET,
    //2nd mic loop
    LPTYPE_REFMIC_2_RECEIVER,
    LPTYPE_REFMIC_2_SPK,
    LPTYPE_REFMIC_2_HEADSET,
    //headset mic loop
    LPTYPE_HMIC_2_RECEIVER,
    LPTYPE_HMIC_2_SPK,
    LPTYPE_HMIC_2_HEADSET,
    LPTYPE_INVALID_OP
};
//end LeiYu@MultimediaSrv.LoopTest, 2012/01/17, Add for looptest

class AudioLoopBackTest
{
public:
    AudioLoopBackTest(AudioMTKHardware *hw);
    ~AudioLoopBackTest();
    bool AudioLoopBackTestMain(AudioParameter         *param); 

private:
    AudioFtm * mAudioFtmInstance;
    AudioMTKHardware *mHw;
    LOOPBACK_TYPE mPreLptype;
    bool mLoopBackTestEn;
    bool mInLoopbackTest;
};
}

#endif //end of ANDROID_AUDIO_LOOPBACK_TEST_H

#endif //end of VENDOR_EDIT





