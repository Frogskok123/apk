# Application build settings
APP_BUILD_SCRIPT := Android.mk

# Target architectures
APP_ABI := arm64-v8a armeabi-v7a

# Target Android API level
# Must match or be lower than compileSdkVersion in build.gradle
APP_PLATFORM := android-34

# C++ runtime library
# Using static linking to avoid dependency issues
APP_STL := c++_static

# Optimization level
APP_OPTIM := release

# Native debugging support (disable for production)
# APP_DEBUG := false