#/*************************************************************
# * 
# * Copyright (C), 2008-2012, OPPO Mobile Comm Corp., Ltd
# * 
# * VENDOR_EDIT
# *
# * Date Created: 2012-06-16 16:32
# * 
# * Author: Lycan.Wang@Prd.Camera.CameraSrv
# * 
# * Description : NULL
# * 
# * ------------------ Revision History: ---------------------
# *      <author>        <date>          <desc>
# *
# *************************************************************/ 

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

WRP_SRC_PATH := mediatek/platform/mt6589/hardware/camera/hal/adapter/oppo/Scalado/scalado_res/wrp_files
WRP_DST_PATH := system/media/images/scalado_res/wrp_files


PRODUCT_COPY_FILES += $(WRP_SRC_PATH)/fisheye.wrp:$(WRP_DST_PATH)/fisheye.wrp 
PRODUCT_COPY_FILES += $(WRP_SRC_PATH)/eyeballedevy.wrp:$(WRP_DST_PATH)/eyeballedevy.wrp 
PRODUCT_COPY_FILES += $(WRP_SRC_PATH)/speedyweed.wrp:$(WRP_DST_PATH)/speedyweed.wrp 
PRODUCT_COPY_FILES += $(WRP_SRC_PATH)/alienscum.wrp:$(WRP_DST_PATH)/alienscum.wrp 
PRODUCT_COPY_FILES += $(WRP_SRC_PATH)/jellybelly.wrp:$(WRP_DST_PATH)/jellybelly.wrp 
PRODUCT_COPY_FILES += $(WRP_SRC_PATH)/bigbuppa.wrp:$(WRP_DST_PATH)/bigbuppa.wrp 
PRODUCT_COPY_FILES += $(WRP_SRC_PATH)/booziebowl.wrp:$(WRP_DST_PATH)/booziebowl.wrp 
PRODUCT_COPY_FILES += $(WRP_SRC_PATH)/pettypetit.wrp:$(WRP_DST_PATH)/pettypetit.wrp 
PRODUCT_COPY_FILES += $(WRP_SRC_PATH)/funkyfresh.wrp:$(WRP_DST_PATH)/funkyfresh.wrp 
PRODUCT_COPY_FILES += $(WRP_SRC_PATH)/conehead.wrp:$(WRP_DST_PATH)/conehead.wrp 

IMG_SRC_PATH := mediatek/platform/mt6589/hardware/camera/hal/adapter/oppo/Scalado/scalado_res/image
IMG_DST_PATH := system/media/images/scalado_res/image

IMG_DST_PATH2 := system/media/images/camera/
PRODUCT_COPY_FILES += $(IMG_SRC_PATH)/start_front_12039.png:$(IMG_DST_PATH2)/start_front_12039.png
PRODUCT_COPY_FILES += $(IMG_SRC_PATH)/start_front_12061.png:$(IMG_DST_PATH2)/start_front_12061.png
PRODUCT_COPY_FILES += $(IMG_SRC_PATH)/start_back.png:$(IMG_DST_PATH2)/start_back.png
PRODUCT_COPY_FILES += $(IMG_SRC_PATH)/xuJiaoPhoto_400x400.png:$(IMG_DST_PATH)/xuJiaoPhoto_400x400.png
	
	
