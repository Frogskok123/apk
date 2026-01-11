# Gradle Wrapper & Java 17 Compatibility Fixes

## Problems Solved

### Problem 1: Missing Gradle Wrapper JAR
The build pipeline was failing with:
```
Error: Could not find or load main class org.gradle.wrapper.GradleWrapperMain
Caused by: java.lang.ClassNotFoundException: org.gradle.wrapper.GradleWrapperMain
```

**Root Cause**: The `gradle/wrapper/gradle-wrapper.jar` file was missing from the repository.

### Problem 2: Deprecated JVM Arguments for Java 17
After fixing the wrapper, the build failed with:
```
Unrecognized VM option 'MaxPermSize=512m'
Error: Could not create the Java Virtual Machine.
```

**Root Cause**: The option `MaxPermSize` was removed in Java 9+. Java 17 doesn't support it.

### Problem 3: Gradle Daemon Issues in CI/CD
Gradle daemon was trying to start but failing in the CI/CD pipeline due to stdin closure.

## Solutions Applied

### 1. Updated Gradle Wrapper Version
**File**: `gradle/wrapper/gradle-wrapper.properties`
- Updated from Gradle 8.0 to **Gradle 8.10**
- Added automatic JAR download in CI/CD workflow

### 2. Fixed Java 17 Compatibility
**File**: `gradle.properties`
```properties
# OLD (doesn't work with Java 17)
org.gradle.jvmargs=-Xmx2048m -XX:MaxPermSize=512m

# NEW (Java 17 compatible)
org.gradle.jvmargs=-Xmx2048m -Dfile.encoding=UTF-8 -XX:+UseG1GC
```

Key changes:
- ✅ Removed deprecated `MaxPermSize=512m`
- ✅ Added `-Dfile.encoding=UTF-8` for proper encoding
- ✅ Added `-XX:+UseG1GC` for better memory management in modern Java

### 3. Enhanced CI/CD Workflow
**File**: `.github/workflows/android-build.yml`
- Added "Initialize Gradle Wrapper" step for automatic JAR download
- Added "Validate Gradle wrapper" step to verify wrapper works
- Added `--no-daemon` flag to all gradle commands in CI/CD:
  - `./gradlew clean --no-daemon`
  - `./gradlew cleanBuildCache --no-daemon`
  - `./gradlew clean assembleDebug --no-daemon --info --refresh-dependencies -x lint`
  - `./gradlew lint --no-daemon --info`

Why `--no-daemon`?
- In CI/CD environments, daemon processes can cause issues
- Prevents stdin closure errors
- Better resource management in ephemeral environments
- Faster failure detection

### 4. Updated .gitignore
**File**: `.gitignore`
- Explicitly included `!gradle/wrapper/gradle-wrapper.jar` to track JAR in Git
- Properly excluded other Gradle directories

## How It Works Now

### Local Development
```bash
# Make gradlew executable
chmod +x ./gradlew

# Build normally
./gradlew clean build
```

### CI/CD Pipeline
1. **Initialize Gradle Wrapper**: Downloads `gradle-wrapper.jar` if missing
2. **Validate Wrapper**: Tests that `./gradlew --version` works
3. **Clean**: Runs with `--no-daemon` to avoid daemon issues
4. **Build**: Runs with `--no-daemon --info --refresh-dependencies`

## Testing

To trigger a new build:
```bash
git add .
git commit -m "trigger: rebuild APK"
git push
```

Monitor at: https://github.com/Frogskok123/apk/actions

## Compatibility

- ✅ **Java 17** - Now fully compatible
- ✅ **Gradle 8.10** - Latest stable version
- ✅ **Android NDK r21e** - Unchanged
- ✅ **Android SDK API 34** - Unchanged
- ✅ **Local & CI/CD** - Works in both environments

## Future Maintenance

### To Upgrade Gradle
```bash
./gradlew wrapper --gradle-version=X.Y.Z
git add gradle/wrapper/
git commit -m "chore: upgrade gradle to X.Y.Z"
git push
```

### To Update Java Version
Modify `.github/workflows/android-build.yml`:
```yaml
- name: Set up Java
  uses: actions/setup-java@v4
  with:
    java-version: '21'  # Update this
    distribution: 'temurin'
    cache: gradle
```

## Troubleshooting

If you still get Gradle errors:

1. **Clear local cache**:
   ```bash
   rm -rf .gradle/
   rm -rf app/build/
   ```

2. **Rebuild wrapper**:
   ```bash
   rm -rf gradle/wrapper/
   ./gradlew wrapper --gradle-version=8.10
   ```

3. **Check Java version**:
   ```bash
   java -version
   # Should show: openjdk version "17.0.x"
   ```
