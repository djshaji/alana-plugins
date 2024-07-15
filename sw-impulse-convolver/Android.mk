LOCAL_PATH := $(call my-dir) 
include $(CLEAR_VARS) 
LOCAL_MODULE := impulse_1885
LOCAL_SRC_FILES := imp_1199.c 
LOCAL_C_INCLUDES := dsp/ plugin/ 
LOCAL_LDLIBS := -llog -lsndfile -lfftw3 -lzita-convolver -lsamplerate -lfftw3f
include $(BUILD_SHARED_LIBRARY) 
