#pragma once

#include <dirent.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring> // добавим для memset
#include <cerrno>  // для errno

class Gyro {
public:
    int fd = -1;
    
    Gyro() {
        struct sockaddr_in addr;
        fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            perror("socket");
            return;
        }
        
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(12345);
        
        // Пробуем подключиться к localhost
        if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {
            perror("inet_pton");
            close(fd);
            fd = -1;
            return;
        }
        
        if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            printf("Gyro connect error: %s\n", strerror(errno));
            close(fd);
            fd = -1;
            return;
        }
        
        printf("Gyro connected successfully on port 12345\n");
    }
    
    ~Gyro() {
        if (fd > 0) {
            close(fd);
        }
    }
    
    
    bool bGyroConnect() {
        return fd > 0;
    }
    
    void update(float x, float y) {
        if (!bGyroConnect()) {
            return;
        }
        
        float temp[2] = {x, y};
        int written = write(fd, temp, sizeof(temp));
        if (written != sizeof(temp)) {
            printf("Gyro write error: %s\n", strerror(errno));
            close(fd);
            fd = -1;
        }
    }
};