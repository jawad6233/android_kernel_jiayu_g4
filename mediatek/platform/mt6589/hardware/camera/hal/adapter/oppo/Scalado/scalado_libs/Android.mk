#/*************************************************************
# * 
# * Copyright (C), 2008-2012, OPPO Mobile Comm Corp., Ltd
# * 
# * VENDOR_EDIT
# *
# * Date Created: 2012-06-16 16:02
# * 
# * Author: Lycan.Wang@Prd.Camera.CameraSrv
# * 
# * Description : NULL
# * 
# * ------------------ Revision History: ---------------------
# *      <author>        <date>          <desc>
# *
# *************************************************************/ 

LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)


LOCAL_PREBUILT_LIBS := \
	baseapi.a \
	capseng.a \
	codecs.a \
	cthdr3.a \
	facedetector.a \
	fmautorama.a \
	fmclearshot.a \
	fmexif.a \
	fmface.a \
	fmhdr2.a \
	fmhdrimage.a \
	fmimageenhance.a \
	fmimagefusion.a \
	fmlocaltimewarp.a \
	fmlowlight.a \
	fmnaturalflash.a \
	fmphotoart.a \
	fmspeedview.a \
	libScaladoFace.a \
	osl_gcc.a \
	excodecs.a
 
include $(BUILD_MULTI_PREBUILT)
   
   
   
   