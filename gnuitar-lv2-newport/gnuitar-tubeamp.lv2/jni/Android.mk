LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS) 
LOCAL_MODULE := gnuitar-tubeamp.lv2 
LOCAL_SRC_FILES := tubeamp.c 
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY) 
LOCAL_C_INCLUDES := dsp/ plugin/
LOCAL_LDFLAGS += -O3
