#include "main.h"
#include "Event.h"

int findminat()
{
    float minScreenDist = NumIo[3];
    float minWorldDist = 999999.0f;
    int targetByScreen = -1;
    int targetByWorld = -1;
    int finalTarget = -1;

    if (DrawIo[32] && IsAimLongAim && AimName[0] != '\0') {
        for (int i = 0; i < MaxPlayerCount; i++) {
            if (strcmp(Aim[i].Name, AimName) == 0) {
                bool isValid = true;
                if (DrawIo[23] && Aim[i].isBot) {
                    isValid = false;
                }
                if (DrawIo[22] && Aim[i].health <= 0) {
                    isValid = false;
                }
                if (isValid) {
                    return i;
                }
                break;
            }
        }
    }

    for (int i = 0; i < MaxPlayerCount; i++) {
        bool isValidTarget = true;
        if (DrawIo[23] && Aim[i].isBot) {
            continue;
        }
        if (DrawIo[22] && Aim[i].health <= 0) {
            continue;
        }

        switch ((int)NumIo[18]) {
            case 0:
                if (DrawIo[32] && IsAimLongAim && strcmp(Aim[i].Name, AimName) == 0) {
                    return i;
                }
                if (Aim[i].ScreenDistance < minScreenDist && Aim[i].ScreenDistance < NumIo[3]) {
                    minScreenDist = Aim[i].ScreenDistance;
                    targetByScreen = i;
                }
                break;
            case 1:
                if (DrawIo[32] && IsAimLongAim && strcmp(Aim[i].Name, AimName) == 0) {
                    return i;
                }
                if (Aim[i].ScreenDistance < NumIo[3]) {
                    if (Aim[i].WodDistance < minWorldDist) {
                        minWorldDist = Aim[i].WodDistance;
                        targetByWorld = i;
                    }
                }
                break;
            case 2:
                if (DrawIo[32] && IsAimLongAim && strcmp(Aim[i].Name, AimName) == 0) {
                    return i;
                }
                if (Aiming == 1) {
                    if (Aim[i].ScreenDistance < minScreenDist && Aim[i].ScreenDistance < NumIo[3]) {
                        minScreenDist = Aim[i].ScreenDistance;
                        targetByScreen = i;
                    }
                } else {
                    if (Aim[i].ScreenDistance < NumIo[3]) {
                        if (Aim[i].WodDistance < minWorldDist) {
                            minWorldDist = Aim[i].WodDistance;
                            targetByWorld = i;
                        }
                    }
                }
                break;
        }
    }

    switch ((int)NumIo[18]) {
        case 0:
            finalTarget = targetByScreen;
            break;
        case 1:
            finalTarget = targetByWorld;
            break;
        case 2:
            if (Aiming == 1) {
                finalTarget = targetByScreen;
            } else {
                finalTarget = targetByWorld;
            }
            break;
    }

    if (finalTarget == -1) {
        Gmin = -1;
        IsAimLongAim = false;
        return -1;
    }

    if (DrawIo[32]) {
        strcpy(AimName, Aim[finalTarget].Name);
        IsAimLongAim = true;
    }

    Gmin = finalTarget;
    return finalTarget;
}

float GetPitch(float Fov)
{
    if (Fov > 75 && Fov <= 130)
    {
        return (0.7f / 30) * NumIo[4];
    }
    else if (Fov == 70 || Fov == 75)
    {
        return (1.8f / 30) * NumIo[4];
    }
    else if (Fov == 55 || Fov == 60)
    {
        return (1.8f / 30) * NumIo[4];
    }
    else if ((int)Fov == 44)
    {
        return (5.3f / 30) * NumIo[4];
    }
    else if ((int)Fov == 26)
    {
        return (7.2f / 30) * NumIo[4];
    }
    else if ((int)Fov == 20)
    {
        return (8.6f / 30) * NumIo[4];
    }
    else if ((int)Fov == 13)
    {
        return (14.1f / 30) * NumIo[4];
    }
    return (2.5f / 30) * NumIo[4];
}

float GetWeaponId(int WeaponId)
{
    switch (WeaponId)
    {
        case 101008: return 1.2;   // M762
        case 101001: return 1.1;   // AKM
        case 101004: return 0.8;   // M416
        case 101003: return 0.8;   // SCAR-L
        case 101002: return 0.85;  // M16A4
        case 101009: return 1.05;  // Mk47
        case 101006: return 0.75;  // AUG
        case 101005: return 1.05;  // Groza
        case 101010: return 1.15;  // G36C
        case 101007: return 0.95;  // QBZ
        case 101011: return 1.0;   // AC-VAL
        case 101012: return 0.90;
        case 105001: return 1.3;   // M249
        case 105002: return 1.3;   // DP-28
        case 105010: return 1.05;  // MG3
        case 102001: return 0.6;   // UZI
        case 102003: return 0.6;   // Vector
        case 100103: return 0.65;  // PP-19
        case 102007: return 0.6;   // MP5K
        case 102002: return 0.6;   // UMP 5
        case 102004: return 0.6;
        case 102105: return 0.5;   // P90
        case 102005: return 0.6;
        default:     return 0.95;
    }
    return 1.0;
}

float getScopeAcc(int Scope)
{
    switch (Scope)
    {
        case 0: return 1.0f;
        case 1: return .98f;
        case 2: return .95f;
        case 3: return .94f;
        case 4: return .92f;
        case 5: return .9f;
        case 6: return .88f;
        case 7: return .86f;
        case 8: return .85f;
        default: return .9f;
    }
}

static Gyro *gyro = nullptr;
void AimBotAuto(ImDrawList *Draw)
{
    bool isDown = false;
    gyro = new Gyro;
    double leenx = 0.0f;
    double leeny = 0.0f;
    double RandomnumberX = NumIo[5], RandomnumberY = NumIo[6];
    double tx = NumIo[5], ty = NumIo[6];
    float halfSize = NumIo[7] / 2;
    float SpeedMin = 2.0f;
    double w = 0.0f, h = 0.0f, cmp = 0.0f;
    double ScreenX = displayInfo.width > displayInfo.height ? displayInfo.height : displayInfo.width;
    double ScreenY = displayInfo.width > displayInfo.height ? displayInfo.width : displayInfo.height;
    double ScrXH = ScreenX / 2.0f;
    double ScrYH = ScreenY / 2.0f;
    static float TargetX = 0;
    static float TargetY = 0;
    Vector3A obj;
    float PushX;
    float PushY;
    float NowCoor[3];

    while (1)
    {
        if (!DrawIo[20])
        {
            IsAimLongAim = false;
            usleep(NumIo[9] * 1000);
            continue;
        }

        findminat();
        if (Gmin == -1)
        {
            IsAimLongAim = false;
            usleep(NumIo[9] * 1000);
            continue;
        }

        float ToReticleDistance = Aim[Gmin].ScreenDistance;
        float FlyTime = Aim[Gmin].WodDistance / NumIo[11];
        float DropM = 540.0f * FlyTime * FlyTime;
        
        // Чтение данных локального игрока
        int MyState = 0;
        float currentFov = 90.0f;
        if (oneself != 0) {
            MyState = driver->read<int>(oneself + 0xfb8);
            if (fov > 1.0f) currentFov = fov;
        }

        NowCoor[0] = Aim[Gmin].ObjAim.X;
        NowCoor[1] = Aim[Gmin].ObjAim.Y;
        NowCoor[2] = Aim[Gmin].ObjAim.Z;
        
        obj.X = NowCoor[0] + (Aim[Gmin].AimMovement.X * FlyTime * NumIo[22]);
        obj.Y = NowCoor[1] + (Aim[Gmin].AimMovement.Y * FlyTime * NumIo[22]);
        obj.Z = NowCoor[2] + (Aim[Gmin].AimMovement.Z * FlyTime * NumIo[22]) + DropM;

        // ====== УСИЛЕННАЯ КОМПЕНСАЦИЯ ОТДАЧИ ======
        if (Firing) {
            // МНОЖИТЕЛЬ СИЛЫ: Увеличен до 60.0 (было 15.0)
            // Если все равно слабо, увеличьте это число до 80.0 или 100.0
            float STRENGTH_MULTIPLIER = 60.0f; 
            
            float baseRecoil = GetPitch(currentFov) * STRENGTH_MULTIPLIER;
            float recoilOffset = baseRecoil * Aim[Gmin].WodDistance;

            // Бонусы для поз
            if (MyState == 656 || MyState == 657 || MyState == 1680 || MyState == 1681) {
                recoilOffset *= (1.0f + NumIo[26]); // Crouch
            } else if (MyState == 672) {
                recoilOffset *= (1.0f + NumIo[27]); // Prone
            }
            
            // Смещаем цель вниз
            obj.Z -= recoilOffset;
        }
        // ============================================

        float cameras = matrix[3] * obj.X + matrix[7] * obj.Y + matrix[11] * obj.Z + matrix[15];
        Vector2A vpvp = WorldToScreen(obj, matrix, cameras);
        
        瞄准x = vpvp.X;
        瞄准y = vpvp.Y;
        zm_y = vpvp.X;
        zm_x = ScreenX - vpvp.Y;
        
        float AimDs = sqrt(pow(px - vpvp.X, 2) + pow(py - vpvp.Y, 2));
        Aimspeace = NumIo[16];

        if (zm_x <= 0 || zm_x >= ScreenX || zm_y <= 0 || zm_y >= ScreenY)
        {
            IsAimLongAim = false;
            usleep(NumIo[9] * 1000);
            continue;
        }

        if (ToReticleDistance > NumIo[3] && AimDs > NumIo[3])
        {
            IsAimLongAim = false;
            usleep(NumIo[9] * 1000);
            continue;
        }

        bool shouldAim = false;
        switch ((int)NumIo[0]) {
            case 0: shouldAim = (Firing == 1); break;
            case 1: shouldAim = (Aiming == 1); break;
            case 2: shouldAim = (Firing == 1 || Aiming == 1); break;
            case 3: shouldAim = (Firing == 1 && Aiming == 1); break;
        }

        if (!shouldAim) {
            IsAimLongAim = false;
            usleep(NumIo[9] * 1000);
            continue;
        }

        float Acc = getScopeAcc((int)(90 / currentFov));
        
        if (zm_x > ScrXH) {
            TargetX = -(ScrXH - zm_x) / Aimspeace * Acc;
            if (TargetX + ScrXH > ScrXH * 2) TargetX = 0;
        } else if (zm_x < ScrXH) {
            TargetX = (zm_x - ScrXH) / Aimspeace * Acc;
            if (TargetX + ScrXH < 0) TargetX = 0;
        }

        if (zm_y > ScrYH) {
            TargetY = -(ScrYH - zm_y) / Aimspeace * Acc;
            if (TargetY + ScrYH > ScrYH * 2) TargetY = 0;
        } else if (zm_y < ScrYH) {
            TargetY = (zm_y - ScrYH) / Aimspeace * Acc;
            if (TargetY + ScrYH < 0) TargetY = 0;
        }

        if (TargetY >= 35 || TargetX >= 35 || TargetY <= -35 || TargetX <= -35) {
            usleep(NumIo[9] * 1000);
            continue;
        }

        float xxx = vpvp.X - (ScreenY / 2);
        float yyy = vpvp.Y - (ScreenX / 2);
        xxx *= Acc * NumIo[9];
        yyy *= Acc * NumIo[9];

        if (displayInfo.orientation == 1) NumIo[10] = 0;
        else NumIo[10] = 1;

        if (NumIo[10]) {
            xxx = -xxx;
            yyy = -yyy;
        }

        if (gyro) {
            gyro->update(-(xxx * 0.002), (yyy * 0.002));
        }

        usleep(NumIo[9] * 1000);
    }
}