#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <string>

// Подключаем ваши заголовочные файлы
#include "Draw.h" 

// Ссылаемся на глобальные переменные из Draw.cpp
extern ANativeWindow* native_window;
extern int native_window_screen_x;
extern int native_window_screen_y;
extern bool g_Initialized;

extern "C" {

// Инициализация: Java Surface -> ANativeWindow -> Vulkan Init
JNIEXPORT void JNICALL
Java_android_support_little_GlJniView_initVulkan(JNIEnv *env, jclass clazz, jobject surface) {
    if (surface != nullptr) {
        // Получаем нативное окно из Java Surface
        native_window = ANativeWindow_fromSurface(env, surface);
        
        // Получаем реальные размеры
        native_window_screen_x = ANativeWindow_getWidth(native_window);
        native_window_screen_y = ANativeWindow_getHeight(native_window);
        
        // Запускаем вашу инициализацию Vulkan (функция из Draw.cpp)
        // ВНИМАНИЕ: initGUI_draw больше не должна создавать окно сама!
        initGUI_draw(native_window_screen_x, native_window_screen_y, false);
    }
}

// Изменение размеров (поворот экрана)
JNIEXPORT void JNICALL
Java_android_support_little_GlJniView_resize(JNIEnv *env, jclass clazz, jint width, jint height) {
    native_window_screen_x = width;
    native_window_screen_y = height;
    // Если нужно пересоздать SwapChain при повороте, добавьте логику здесь
}

// Отрисовка кадра (вызывается в цикле из Java потока)
JNIEXPORT void JNICALL
Java_android_support_little_GlJniView_step(JNIEnv *env, jclass clazz) {
    if (g_Initialized) {
        drawBegin();
        // Здесь ваша логика отрисовки меню и ESP
        // ...
        drawEnd();
    }
}

// Обработка нажатий
JNIEXPORT void JNICALL
Java_android_support_little_GlJniView_MotionEventClick(JNIEnv *env, jclass clazz, jboolean down, jfloat PosX, jfloat PosY) {
    // Безопасный доступ к ImGui IO
    if (ImGui::GetCurrentContext() != nullptr) {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(PosX, PosY);
        io.MouseDown[0] = down;
    }
}

// Завершение работы
JNIEXPORT void JNICALL
Java_android_support_little_GlJniView_imgui_Shutdown(JNIEnv *env, jclass clazz) {
    shutdown();
    if (native_window) {
        ANativeWindow_release(native_window);
        native_window = nullptr;
    }
}

}