// ModuleInstaller.h
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "gyro_module.h"

// Цвета для терминала
#define C_RED     "\u001B[0;31m"
#define C_GREEN   "\u001B[0;32m"
#define C_YELLOW  "\u001B[1;33m"
#define C_BLUE    "\u001B[0;34m"
#define C_CYAN    "\u001B[0;36m"
#define C_RESET   "\u001B[0m"

enum RootType {
    ROOT_NONE = 0,
    ROOT_MAGISK,
    ROOT_KERNELSU,
    ROOT_APATCH
};

class GyroModuleInstaller {
private:
    static RootType detectRootType() {
        if (system("su -c 'ksud -V' > /dev/null 2>&1") == 0) {
            return ROOT_KERNELSU;
        }
        if (system("su -c 'magisk -V' > /dev/null 2>&1") == 0) {
            return ROOT_MAGISK;
        }
        if (system("su -c 'apd -V' > /dev/null 2>&1") == 0) {
            return ROOT_APATCH;
        }
        return ROOT_NONE;
    }
    
    static const char* getRootName(RootType type) {
        switch(type) {
            case ROOT_MAGISK: return "Magisk";
            case ROOT_KERNELSU: return "KernelSU";
            case ROOT_APATCH: return "APatch";
            default: return "None";
        }
    }
    
    static const char* getInstallCommand(RootType type) {
        switch(type) {
            case ROOT_MAGISK: return "magisk --install-module";
            case ROOT_KERNELSU: return "ksud module install";
            case ROOT_APATCH: return "apd module install";
            default: return nullptr;
        }
    }

public:
    // Проверка установки модуля
    static bool isModuleInstalled() {
        struct stat st;
        return (stat("/data/adb/modules/gyro_daemon", &st) == 0);
    }
    
    // Проверка работы демона
    static bool isDaemonRunning() {
        return (system("su -c 'netstat -tulpn | grep :12345' > /dev/null 2>&1") == 0);
    }
    
    // Установка модуля из встроенного массива
    static bool installModule() {
        RootType rootType = detectRootType();
        
        if (rootType == ROOT_NONE) {
            printf(C_RED "[✗] No root manager found!" C_RESET);
            return false;
        }
        
        printf(C_CYAN "[*] Root Manager: %s" C_RESET, getRootName(rootType));
        printf(C_YELLOW "[*] Extracting embedded module (%u bytes)..." C_RESET, gyro_module_zip_len);
        fflush(stdout);
        
        const char* tmpPath = "/data/local/tmp/gyro_mod_tmp.zip";
        
        // Записываем встроенный массив в файл
        FILE* f = fopen(tmpPath, "wb");
        if (!f) {
            printf(C_RED " FAILED" C_RESET);
            printf(C_RED "    Unable to create temp file" C_RESET);
            return false;
        }
        
        size_t written = fwrite(gyro_module_zip, 1, gyro_module_zip_len, f);
        fclose(f);
        
        if (written != gyro_module_zip_len) {
            printf(C_RED " FAILED" C_RESET);
            printf(C_RED "    Write error (%zu/%u bytes)" C_RESET, written, gyro_module_zip_len);
            remove(tmpPath);
            return false;
        }
        
        printf(C_GREEN " OK" C_RESET);
        
        // Установка через root менеджер
        const char* installCmd = getInstallCommand(rootType);
        if (!installCmd) {
            printf(C_RED "[✗] Unknown root type" C_RESET);
            remove(tmpPath);
            return false;
        }
        
        printf(C_YELLOW "[*] Installing via %s..." C_RESET, getRootName(rootType));
        fflush(stdout);
        
        char cmd[512];
        snprintf(cmd, sizeof(cmd), "su -c '%s %s' > /dev/null 2>&1", installCmd, tmpPath);
        
        int result = system(cmd);
        remove(tmpPath); // Удаляем временный файл
        
        if (result == 0) {
            printf(C_GREEN " OK" C_RESET);
            return true;
        } else {
            printf(C_RED " FAILED (exit code: %d)" C_RESET, result);
            return false;
        }
    }
    
    // Автоматическая проверка и установка (с вопросами)
    static int autoCheck() {
        printf(C_CYAN "╔════════════════════════════════════════╗" C_RESET);
        printf(C_CYAN "║      Gyro Module Status Check          ║" C_RESET);
        printf(C_CYAN "╚════════════════════════════════════════╝" C_RESET);
        printf("");
        
        printf(C_CYAN "[*] Checking module status..." C_RESET);
        
        if (isModuleInstalled()) {
            printf(C_GREEN "[✓] Module installed" C_RESET);
            
            if (isDaemonRunning()) {
                printf(C_GREEN "[✓] Daemon running on port 12345" C_RESET);
                printf(C_GREEN "[✓] Gyro aimbot ready!" C_RESET);
                return 0; // Всё готово
            } else {
                printf(C_YELLOW "[!] Daemon not running" C_RESET);
                printf(C_YELLOW "    Module installed but not activated" C_RESET);
                printf(C_YELLOW "    Reboot required to activate daemon" C_RESET);
                
                printf("Reboot device now? (y/n): ");
                char answer;
                scanf(" %c", &answer);
                
                if (answer == 'y' || answer == 'Y') {
                    printf(C_BLUE "[*] Rebooting device..." C_RESET);
                    system("su -c reboot");
                    exit(0);
                } else {
                    printf(C_YELLOW "[!] Please reboot manually and restart cheat" C_RESET);
                    return 2; // Требуется перезагрузка
                }
            }
        }
        
        // Модуль не установлен
        printf(C_YELLOW "[!] Module not installed" C_RESET);
        printf(C_YELLOW "    Gyro aimbot will not work without module" C_RESET);
        
        printf("Install gyro module now? (y/n): ");
        char answer;
        scanf(" %c", &answer);
        
        if (answer != 'y' && answer != 'Y') {
            printf(C_YELLOW "[!] Skipping installation" C_RESET);
            printf(C_YELLOW "[!] Continuing without gyro aimbot" C_RESET);
            return 1; // Пропустить гиро
        }
        
        printf("");
        if (installModule()) {
            printf("");
            printf(C_GREEN "╔════════════════════════════════════════╗" C_RESET);
            printf(C_GREEN "║   MODULE INSTALLED SUCCESSFULLY!      ║" C_RESET);
            printf(C_GREEN "╚════════════════════════════════════════╝" C_RESET);
            printf("");
            printf(C_YELLOW "⚠  REBOOT REQUIRED TO ACTIVATE MODULE" C_RESET);
            
            printf("Reboot device now? (y/n): ");
            scanf(" %c", &answer);
            
            if (answer == 'y' || answer == 'Y') {
                printf(C_BLUE "[*] Rebooting device..." C_RESET);
                system("su -c reboot");
                exit(0);
            } else {
                printf(C_YELLOW "[!] Please reboot manually:" C_RESET);
                printf(C_YELLOW "    su -c reboot" C_RESET);
                printf(C_YELLOW "[!] After reboot, restart this cheat" C_RESET);
                return 2; // Требуется перезагрузка
            }
        } else {
            printf("");
            printf(C_RED "[✗] Installation failed!" C_RESET);
            printf(C_YELLOW "    Check root permissions and try again" C_RESET);
            
            printf("Continue without gyro aimbot? (y/n): ");
            scanf(" %c", &answer);
            
            if (answer != 'y' && answer != 'Y') {
                printf(C_RED "[✗] Exiting..." C_RESET);
                exit(1);
            }
            
            printf(C_YELLOW "[!] Continuing without gyro support" C_RESET);
            return 1; // Продолжить без гиро
        }
    }
    
    // Автоматическая установка БЕЗ вопросов (тихий режим)
    static int autoCheckSilent() {
        printf(C_CYAN "[*] Checking gyro module..." C_RESET);
        fflush(stdout);
        
        if (isModuleInstalled() && isDaemonRunning()) {
            printf(C_GREEN " OK" C_RESET);
            printf(C_GREEN "[✓] Gyro aimbot available" C_RESET);
            return 0;
        }
        
        if (isModuleInstalled() && !isDaemonRunning()) {
            printf(C_YELLOW " NOT RUNNING" C_RESET);
            printf(C_YELLOW "[!] Module installed but needs reboot" C_RESET);
            return 2;
        }
        
        printf(C_YELLOW " NOT INSTALLED" C_RESET);
        printf(C_YELLOW "[*] Auto-installing gyro module..." C_RESET);
        
        if (installModule()) {
            printf(C_GREEN "[✓] Installation complete" C_RESET);
            printf(C_YELLOW "[!] Reboot required (reboot manually)" C_RESET);
            return 2;
        }
        
        printf(C_YELLOW "[!] Installation failed, continuing without gyro" C_RESET);
        return 1;
    }
};