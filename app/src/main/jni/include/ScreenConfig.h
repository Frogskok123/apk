#ifndef SCREEN_CONFIG_H
#define SCREEN_CONFIG_H

#include <jni.h>
#include <android/log.h>
#include <string>
#include <cstdlib>

#define LOG_TAG "ScreenConfig"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Глобальные переменные для конфигурации экрана
struct ScreenInfo {
    int width;           // Ширина экрана
    int height;          // Высота экрана
    int orientation;     // 0 = Portrait, 1 = Landscape
    bool initialized;    // Флаг инициализации
};

extern ScreenInfo g_screen_info;
extern JavaVM* g_java_vm;  // Глобальная ссылка на JavaVM

/**
 * Инициализация JVM для использования в JNI вызовах
 * @param vm - указатель на JavaVM из JNI_OnLoad
 */
inline void screen_config_init_jvm(JavaVM* vm) {
    g_java_vm = vm;
}

/**
 * Получить информацию об экране через JNI
 * Вызывает Java метод GlJniView.getScreenConfig()
 */
inline void screen_config_from_java() {
    if (!g_java_vm) {
        LOGE("JavaVM not initialized!");
        return;
    }

    JNIEnv* env = nullptr;
    int getEnvStat = g_java_vm->GetEnv((void**)&env, JNI_VERSION_1_6);
    
    if (getEnvStat == JNI_EDETACHED) {
        // Текущий поток не привязан, прикрепляем его
        if (g_java_vm->AttachCurrentThread(&env, nullptr) != 0) {
            LOGE("Failed to attach current thread to JavaVM");
            return;
        }
    } else if (getEnvStat == JNI_EVERSION) {
        LOGE("JNI version not supported");
        return;
    }

    if (!env) {
        LOGE("JNIEnv is null");
        return;
    }

    try {
        // Получаем класс GlJniView
        jclass jni_class = env->FindClass("android/support/little/GlJniView");
        if (!jni_class) {
            LOGE("Failed to find GlJniView class");
            env->ExceptionClear();
            return;
        }

        // Получаем статический метод getScreenConfig()
        jmethodID method_id = env->GetStaticMethodID(
            jni_class,
            "getScreenConfig",
            "()Ljava/lang/String;"
        );
        
        if (!method_id) {
            LOGE("Failed to find getScreenConfig method");
            env->DeleteLocalRef(jni_class);
            env->ExceptionClear();
            return;
        }

        // Вызываем метод
        jstring result = (jstring)env->CallStaticObjectMethod(jni_class, method_id);
        
        if (!result) {
            LOGE("Failed to call getScreenConfig");
            env->DeleteLocalRef(jni_class);
            env->ExceptionClear();
            return;
        }

        // Преобразуем Java String в C++ string
        const char* result_cstr = env->GetStringUTFChars(result, nullptr);
        std::string config_str(result_cstr);
        env->ReleaseStringUTFChars(result, result_cstr);

        // Парсим строку формата: "width|height|orientation"
        // Например: "1080|2400|0"
        size_t pos1 = config_str.find('|');
        size_t pos2 = config_str.find('|', pos1 + 1);

        if (pos1 != std::string::npos && pos2 != std::string::npos) {
            g_screen_info.width = std::stoi(config_str.substr(0, pos1));
            g_screen_info.height = std::stoi(config_str.substr(pos1 + 1, pos2 - pos1 - 1));
            g_screen_info.orientation = std::stoi(config_str.substr(pos2 + 1));
            g_screen_info.initialized = true;

            LOGI("Screen config: %dx%d, orientation: %d",
                 g_screen_info.width,
                 g_screen_info.height,
                 g_screen_info.orientation);
        } else {
            LOGE("Failed to parse screen config string: %s", config_str.c_str());
        }

        env->DeleteLocalRef(jni_class);
        env->DeleteLocalRef(result);

    } catch (const std::exception& e) {
        LOGE("Exception in screen_config_from_java: %s", e.what());
        env->ExceptionClear();
    }
}

/**
 * Получить только что инициализированную ширину
 */
inline int screen_get_width() {
    return g_screen_info.width;
}

/**
 * Получить только что инициализированную высоту
 */
inline int screen_get_height() {
    return g_screen_info.height;
}

/**
 * Получить ориентацию экрана
 * @return 0 - Portrait, 1 - Landscape
 */
inline int screen_get_orientation() {
    return g_screen_info.orientation;
}

/**
 * Проверить инициализирована ли конфигурация
 */
inline bool screen_is_initialized() {
    return g_screen_info.initialized;
}

/**
 * Сбросить конфигурацию
 */
inline void screen_config_reset() {
    g_screen_info.width = 0;
    g_screen_info.height = 0;
    g_screen_info.orientation = 0;
    g_screen_info.initialized = false;
}

#endif // SCREEN_CONFIG_H
