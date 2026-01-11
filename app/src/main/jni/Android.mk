LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Compiler settings
LOCAL_CFLAGS := -std=c++17
LOCAL_CFLAGS += -Wno-error=format-security -w
LOCAL_CFLAGS += -fno-rtti -fno-exceptions -fpermissive -fvisibility=hidden
LOCAL_CFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR

LOCAL_CPPFLAGS := -std=c++17
LOCAL_CPPFLAGS += -Wno-error=format-security -fpermissive -w -s
LOCAL_CPPFLAGS += -fno-rtti -fno-exceptions -fms-extensions -Wno-error=c++11-narrowing -fvisibility=hidden
LOCAL_CPPFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR

# Header files
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Encrypt
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Http
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/Vulkan
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/ImGui
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/ImGui
LOCAL_C_INCLUDES += $(LOCAL_PATH)/src/Vulkan

# Source files - Main
LOCAL_SRC_FILES := \
    src/Main.cpp \
    src/JNI_Bridge.cpp \
    src/ScreenConfig.cpp \
    src/draw.cpp \
    src/aimbot.cpp

# Source files - ImGui
LOCAL_SRC_FILES += \
    src/ImGui/Draw.cpp \
    src/ImGui/TouchHelperA.cpp \
    src/ImGui/imgui.cpp \
    src/ImGui/imgui_demo.cpp \
    src/ImGui/imgui_draw.cpp \
    src/ImGui/imgui_tables.cpp \
    src/ImGui/imgui_widgets.cpp \
    src/ImGui/imgui_impl_android.cpp \
    src/ImGui/imgui_impl_vulkan.cpp \
    src/ImGui/stb_image.cpp

# Source files - Vulkan
LOCAL_SRC_FILES += \
    src/Vulkan/vulkan_wrapper.cpp \
    src/Vulkan/VulkanUtils.cpp \
    src/Vulkan/VulkanMain.cpp \
    src/Vulkan/CreateShaderModule.cpp

# Library name
LOCAL_MODULE := littleHack

# Android libraries
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv1_CM -lGLESv2 -lGLESv3 -lvulkan

# Build as shared library
include $(BUILD_SHARED_LIBRARY)