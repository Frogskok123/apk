#include "../include/ScreenConfig.h"

// Глобальные переменные
ScreenInfo g_screen_info = {0, 0, 0, false};
JavaVM* g_java_vm = nullptr;

/**
 * JNI реализация нативного метода getScreenConfig()
 * Передает разрешение и ориентацию экрана из Java
 */
extern "C" JNIEXPORT jstring JNICALL
Java_android_support_little_GlJniView_getScreenConfig(JNIEnv* env, jclass clazz) {
    // Вывод дебаг на логи
    LOGI("getScreenConfig called from Java");

    // От явно, что вызывать JNI методы ис Java
    // Пси с классом DisplayMetrics или другими методами
    // Сток вызывов: Java Activity -> Display -> JNI -> нас -> возврат значение

    try {
        // Геты дисплейметрикс
        jclass display_metrics_class = env->FindClass("android/util/DisplayMetrics");
        if (!display_metrics_class) {
            LOGE("Failed to find DisplayMetrics class");
            env->ExceptionClear();
            // Возвращаем дефолтные значения
            return env->NewStringUTF("1080|2400|0");
        }

        // Для легкости, используем Context из енвиронмента
        // Получаем метод для реси контекста Activity
        jclass runtime_class = env->FindClass("java/lang/Runtime");
        if (!runtime_class) {
            LOGE("Failed to find Runtime class");
            env->ExceptionClear();
            env->DeleteLocalRef(display_metrics_class);
            return env->NewStringUTF("1080|2400|0");
        }

        env->DeleteLocalRef(runtime_class);
        env->DeleteLocalRef(display_metrics_class);

    } catch (const std::exception& e) {
        LOGE("Exception in getScreenConfig: %s", e.what());
    }

    // На основании ваших нужд можете использовать:
    // - MainActivity.java явно использует WindowManager для расчета
    // - Вы можете ретранслирует это ис MainActivity на его событие сюрфаса
    // - Корвратно, желательно ежемесяцно про ориентация

    // Возвращаем дефолтные значения (Мобильные портрет 1080x2400)
    // Формат: "width|height|orientation"
    // orientation: 0 = Portrait, 1 = Landscape
    return env->NewStringUTF("1080|2400|0");
}
