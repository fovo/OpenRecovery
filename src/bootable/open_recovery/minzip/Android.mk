LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	Hash.c \
	SysUtil.c \
	DirUtil.c \
	Inlines.c \
	Zip.c

LOCAL_C_INCLUDES += \
	external/zlib \
	external/safe-iop/include
	
LOCAL_MODULE := libminzip_orcvr
LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS += -Wall

include $(BUILD_STATIC_LIBRARY)
