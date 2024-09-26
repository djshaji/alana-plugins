LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS) 
LOCAL_MODULE := gnuitar-distort2-np.lv2 
LOCAL_SRC_FILES := distort2.c 
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY) 
LOCAL_C_INCLUDES := dsp/ plugin/
LOCAL_LDFLAGS += -O3
