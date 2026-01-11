#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <string>

// Include headers
#include "../include/ScreenConfig.h"
#include "Draw.h" 

// Global variables from Draw.cpp
extern ANativeWindow* native_window;
extern int native_window_screen_x;
extern int native_window_screen_y;
extern bool g_Initialized;

extern "C" {

// JNI_OnLoad - MUST be called when library is loaded
// This initializes JavaVM for ScreenConfig JNI calls
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    // CRITICAL: Initialize JavaVM for ScreenConfig
    screen_config_init_jvm(vm);
    
    LOGI("JNI_OnLoad: ScreenConfig initialized");
    return JNI_VERSION_1_6;
}

// Vulkan initialization: Java Surface -> ANativeWindow -> Vulkan Init
JNIEXPORT void JNICALL
Java_android_support_little_GlJniView_initVulkan(JNIEnv *env, jclass clazz, jobject surface) {
    if (surface != nullptr) {
        // Get native window from Java Surface
        native_window = ANativeWindow_fromSurface(env, surface);
        
        // Get real screen sizes
        native_window_screen_x = ANativeWindow_getWidth(native_window);
        native_window_screen_y = ANativeWindow_getHeight(native_window);
        
        // Run your Vulkan initialization (function from Draw.cpp)
        initGUI_draw(native_window_screen_x, native_window_screen_y, false);
    }
}

// Resize/rotation handling
JNIEXPORT void JNICALL
Java_android_support_little_GlJniView_resize(JNIEnv *env, jclass clazz, jint width, jint height) {
    native_window_screen_x = width;
    native_window_screen_y = height;
    // Add SwapChain recreation logic here if needed
}

// Frame rendering (called in loop from Java thread)
JNIEXPORT void JNICALL
Java_android_support_little_GlJniView_step(JNIEnv *env, jclass clazz) {
    if (g_Initialized) {
        drawBegin();
        // Your menu and ESP rendering logic here
        // ...
        drawEnd();
    }
}

// Touch event handling
JNIEXPORT void JNICALL
Java_android_support_little_GlJniView_MotionEventClick(JNIEnv *env, jclass clazz, jboolean down, jfloat PosX, jfloat PosY) {
    // Safe access to ImGui IO
    if (ImGui::GetCurrentContext() != nullptr) {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(PosX, PosY);
        io.MouseDown[0] = down;
    }
}

// Shutdown
JNIEXPORT void JNICALL
Java_android_support_little_GlJniView_imgui_Shutdown(JNIEnv *env, jclass clazz) {
    shutdown();
    if (native_window) {
        ANativeWindow_release(native_window);
        native_window = nullptr;
    }
}

}
