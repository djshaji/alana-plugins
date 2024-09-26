LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS) 
LOCAL_MODULE := looper 
LOCAL_SRC_FILES := looper.c 
LOCAL_C_INCLUDES := dsp/ plugin/ 
LOCAL_LDFLAGS := -llog
include $(BUILD_SHARED_LIBRARY) 
