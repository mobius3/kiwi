LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

SDL_PATH := ../SDL
SDL_TTF_PATH := ../SDL_ttf

LOCAL_MODULE := KiWi_static
LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include $(LOCAL_PATH)/$(SDL_TTF_PATH)
  
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/src

LOCAL_SRC_FILES := \
  $(subst $(LOCAL_PATH)/,, \
  $(wildcard $(LOCAL_PATH)/src/*.c))

LOCAL_MODULE_FILENAME := libKiWi

LOCAL_STATIC_LIBRARIES := SDL2_static SDL2_ttf_static

include $(BUILD_STATIC_LIBRARY)
