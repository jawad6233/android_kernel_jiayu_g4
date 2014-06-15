LOCAL_PATH := $(call my-dir)

################################################################################
#
################################################################################
include $(CLEAR_VARS)


LOCAL_WHOLE_STATIC_LIBRARIES := \
			LibVfb  \
			libFB	\
			libFace
#-----------------------------------------------------------
LOCAL_SRC_FILES += doc.cpp
LOCAL_SRC_FILES += doc_facedetect.cpp
LOCAL_SRC_FILES += doc_fb.cpp
LOCAL_SRC_FILES += doc_vfb.cpp
#-----------------------------------------------------------
LOCAL_C_INCLUDES += $(TOP)/$(MTK_ROOT)/hardware/camera/inc
LOCAL_C_INCLUDES += $(TOP)/$(MTK_ROOT)/hardware/camera/inc/common
LOCAL_C_INCLUDES += $(TOP)/$(MTK_ROOT)/hardware/camera/inc/common/camutils
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/inc
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/inc/common
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/hal/adapter/
LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/hal/adapter/oppo/Doc/doc_include
#ifdef VENDOR_EDIT
#wuhongru@CamSrv
#LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_PLATFORM)/hardware/camera/hal/client/CamClient/FD
#endif
#-----------------------------------------------------------
#LOCAL_CFLAGS += 

#-----------------------------------------------------------

#-----------------------------------------------------------	
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libcam.camadapter.docFace

#-----------------------------------------------------------

LOCAL_SHARED_LIBRARIES := \
    libutils \
    liblog \
    libbinder \
    libcutils \
	libstlport	\
	libdl
	
include $(BUILD_STATIC_LIBRARY)


################################################################################
#
################################################################################
include $(LOCAL_PATH)/doc_libs/Android.mk

################################################################################
#
################################################################################
include $(CLEAR_VARS)
include $(call all-makefiles-under,$(LOCAL_PATH))

