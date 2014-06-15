LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)


LOCAL_PREBUILT_LIBS := \
			LibVfb.a  \
			libFB.a	\
			libFace.a
			

include $(BUILD_MULTI_PREBUILT)
