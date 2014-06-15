#
# libcam.camshot
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#
LOCAL_SRC_FILES := \
    CamShotImp.cpp \
    $(call all-c-cpp-files-under, SingleShot) \
    $(call all-c-cpp-files-under, MultiShot) \
    $(call all-c-cpp-files-under, SampleSingleShot) \

#    $(call all-c-cpp-files-under, SImager) \
#    $(call all-c-cpp-files-under, SImager/ImageTransform) \
#    $(call all-c-cpp-files-under, SImager/JpegCodec) \

#
# Note: "/bionic" and "/external/stlport/stlport" is for stlport.
LOCAL_C_INCLUDES += $(TOP)/bionic
LOCAL_C_INCLUDES += $(TOP)/external/stlport/stlport
# 
# camera Hardware 
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/frameworks/base/include
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/camera/inc
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/camera/inc/common
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/hardware/camera/inc/common/camutils
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/inc
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/inc/common
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/inc/common/camutils
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/kernel/include
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/kernel/core/include/mach
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/hal/adapter/inc
LOCAL_C_INCLUDES += $(TOP)/bionic
LOCAL_C_INCLUDES += $(TOP)/external/stlport/stlport
LOCAL_C_INCLUDES += $(TOP)/$(MTK_ROOT)/hardware/dpframework/inc
LOCAL_C_INCLUDES += $(TOP)/external/jpeg

# jpeg encoder 
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/jpeg/inc \
# m4u 
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/m4u \

#ifdef VENDOR_EDIT
#LiuBin@MtkCamera, 2013/04/02, Add for 12083 ms2r yuv sensor capture raw data
#ifeq ($(strip $(TARGET_PRODUCT)),OPPO89T_12083)
#	LOCAL_CPPFLAGS += -DOPPO_12083_MS2R_ISP
#endif
#LiPeng.Bai@MtkCamera, 2013/04/16, Add for 12093 scene detect
ifeq ($(strip $(TARGET_PRODUCT)),OPPO89W_12093)
	LOCAL_CPPFLAGS += -DOPPO_12083_MS2R_ISP
else ifeq ($(strip $(TARGET_PRODUCT)),OPPO89W_13025)
	LOCAL_CPPFLAGS += -DOPPO_12083_MS2R_ISP
else ifeq ($(strip $(TARGET_PRODUCT)),OPPO89W_13013)
	LOCAL_CPPFLAGS += -DOPPO_12083_MS2R_ISP	
endif
#endif /* VENDOR_EDIT */

#
LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libstlport \
    libcam.campipe \
    libcamdrv \
    libutils \
    libdpframework \
    libjpeg \
	  
# for jpeg enc use 
LOCAL_SHARED_LIBRARIES += \
    libm4u \
    libJpgEncPipe \

# for 3A 
LOCAL_SHARED_LIBRARIES +=\
    libfeatureio \

# camUtils 
LOCAL_SHARED_LIBRARIES +=\
    libcam.utils \
    

LOCAL_STATIC_LIBRARIES := \
 
#
LOCAL_WHOLE_STATIC_LIBRARIES := \
     libcam.camshot.simager \
     libcam.camshot.utils \
     
#
LOCAL_MODULE := libcam.camshot

#
LOCAL_MODULE_TAGS := optional

#
include $(BUILD_SHARED_LIBRARY)

#
include $(call all-makefiles-under,$(LOCAL_PATH))
