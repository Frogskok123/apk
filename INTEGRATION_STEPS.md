# Шаги интеграции ScreenConfig в ваш проект

## ✅ Уже сделано:

1. ✅ Создан `app/src/main/jni/include/ScreenConfig.h` - header с функциями
2. ✅ Создан `app/src/main/jni/src/ScreenConfig.cpp` - реализация JNI
3. ✅ Обновлен `app/src/main/java/android/support/little/GlJniView.java` - добавлен метод getScreenConfig()
4. ✅ Обновлен `app/src/main/jni/Android.mk` - добавлен ScreenConfig.cpp в сборку
5. ✅ Создан `.github/workflows/android-build.yml` - CI/CD pipeline

---

## 🔧 Что вам нужно сделать:

### 1️⃣ Обновить JNI_Bridge.cpp

**Добавить в начало файла:**
```cpp
#include "../include/ScreenConfig.h"
```

**Обновить JNI_OnLoad:**
```cpp
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    // ДОБАВИТЬ ЭТО:
    screen_config_init_jvm(vm);
    
    // ... остальная инициализация ...
    return JNI_VERSION_1_6;
}
```

### 2️⃣ Обновить Main.cpp

**Добавить в начало main():**
```cpp
void main(int argc, char *argv[]) {
    // ДОБАВИТЬ ЭТО:
    screen_config_from_java();
    
    if (!screen_is_initialized()) {
        LOGE("Failed to initialize screen config");
        return -1;
    }
    
    // Использовать вместо ANativeWindowCreator:
    int screen_width = screen_get_width();
    int screen_height = screen_get_height();
    int orientation = screen_get_orientation();
    
    // Остальной код...
}
```

### 3️⃣ Заменить старые вызовы

**Найти и заменить:**
```cpp
// ДО (старые вызовы):
// abs_ScreenX = (displayInfo.height > displayInfo.width ? displayInfo.height : displayInfo.width);
// abs_ScreenY = (displayInfo.height < displayInfo.width ? displayInfo.height : displayInfo.width);

// ПОСЛЕ (новые вызовы):
abs_ScreenX = screen_get_width();
abs_ScreenY = screen_get_height();
```

### 4️⃣ Обновить tool.h (опционально)

Если используете глобальные переменные, обновите:
```cpp
// Добавить в top-level
extern int screen_get_width();
extern int screen_get_height();
extern int screen_get_orientation();
```

---

## 🔍 Проверка

### Локально скомпилировать:
```bash
cd app/src/main/jni
ndk-build clean
ndk-build -j$(nproc)

# Проверить результат
ls -la ../jniLibs/arm64-v8a/liblittleHack.so
ls -la ../jniLibs/armeabi-v7a/liblittleHack.so
```

### Через Gradle:
```bash
./gradlew assembleDebug
```

### Через GitHub Actions:
1. Сделайте push в main
2. Зайдите на https://github.com/Frogskok123/apk/actions
3. Посмотрите статус workflow
4. Скачайте артефакты

---

## 📋 Контрольный список

- [ ] Скопировать `include/ScreenConfig.h`
- [ ] Скопировать `src/ScreenConfig.cpp`
- [ ] Обновить Android.mk (добавить ScreenConfig.cpp)
- [ ] Добавить `#include "ScreenConfig.h"` в JNI_Bridge.cpp
- [ ] Вызвать `screen_config_init_jvm(vm)` в JNI_OnLoad
- [ ] Вызвать `screen_config_from_java()` в main()
- [ ] Заменить старые вызовы getScreenConfig()
- [ ] Скомпилировать локально
- [ ] Протестировать на устройстве
- [ ] Сделать push на GitHub

---

## 🚨 Типичные ошибки

### ❌ Ошибка: "undefined reference to 'screen_config_init_jvm'"
**Решение:** Убедитесь, что ScreenConfig.cpp добавлен в Android.mk

### ❌ Ошибка: "JNI exception in getScreenConfig"
**Решение:** Проверьте, что `screen_config_init_jvm()` вызван в JNI_OnLoad

### ❌ Ошибка: "Failed to find GlJniView class"
**Решение:** Проверьте пакет класса. Должен быть `android/support/little/GlJniView`

### ❌ Ошибка: "Failed to parse screen config string"
**Решение:** getScreenConfig() может вернуть неправильный формат. Проверьте логи.

---

## 📞 Отладка через логи

```bash
# Просмотр логов ScreenConfig
adb logcat | grep "ScreenConfig"

# Просмотр всех логов
adb logcat | grep "littleHack"

# Просмотр логов при сбое
adb logcat | grep -E "ERROR|FATAL|CRASH"

# Сохранить в файл
adb logcat > logfile.txt
```

---

## ✨ Преимущества решения

✅ Полностью Java-based (работает со всеми Android версиями)  
✅ Безопасно (нет прямого доступа к нативным API)  
✅ Надежно (обработка ошибок на уровне JNI)  
✅ Быстро (одноразовый вызов при инициализации)  
✅ Тестируемо (легко мокировать на тестах)  
✅ Расширяемо (можно добавить другие параметры)  

---

## 📚 Дополнительная информация

- Полное руководство: `SCREEN_CONFIG_GUIDE.md`
- Исходный анализ: `repository_analysis.md`
- GitHub Actions: `.github/workflows/android-build.yml`

---

**Статус:** ✅ Готово к интеграции  
**Версия:** 1.0  
**Дата:** 11 января 2026  
**Автор:** Frogskok123
