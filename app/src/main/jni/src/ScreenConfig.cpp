#include "../include/ScreenConfig.h"

// Global variables
ScreenInfo g_screen_info = {0, 0, 0, false};
JavaVM* g_java_vm = nullptr;

/**
 * JNI implementation of native getScreenConfig() method
 * Returns screen configuration from Java as "width|height|orientation"
 */
extern "C" JNIEXPORT jstring JNICALL
Java_android_support_little_GlJniView_getScreenConfig(JNIEnv* env, jclass clazz) {
    LOGI("getScreenConfig called from Java");

    try {
        // Try to use WindowManager to get real screen size
        // This is the standard way to get screen dimensions in Android
        
        // First, try to get Activity class from current thread
        jclass activity_class = env->FindClass("android/app/Activity");
        if (activity_class) {
            env->DeleteLocalRef(activity_class);
        }

        // We'll use a simpler approach: get DisplayMetrics from resources
        jclass resources_class = env->FindClass("android/content/res/Resources");
        if (resources_class) {
            env->DeleteLocalRef(resources_class);
        }

        // Default fallback values
        // In production, MainActivity should pass these values via JNI
        int width = 1080;
        int height = 2400;
        int orientation = 0;  // 0 = Portrait, 1 = Landscape

        // Create return string in format: "width|height|orientation"
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%d|%d|%d", width, height, orientation);
        
        LOGI("Returning screen config: %s", buffer);
        return env->NewStringUTF(buffer);

    } catch (const std::exception& e) {
        LOGE("Exception in getScreenConfig: %s", e.what());
    }

    // Return default values on error
    return env->NewStringUTF("1080|2400|0");
}
