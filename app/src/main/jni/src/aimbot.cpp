#include "main.h"
#include "Event.h"
// 遍历自瞄对象
int findminat()
{
    float minScreenDist = NumIo[3];  // Максимальное расстояние на экране для фова
    float minWorldDist = 999999.0f;   // Минимальное мировое расстояние
    int targetByScreen = -1;          // Цель по экранному расстоянию (внутри фова)
    int targetByWorld = -1;           // Цель по мировому расстоянию
    int finalTarget = -1;
    
    // Если включен долговременный аим (защелка), ищем заблокированную цель
    if (DrawIo[32] && IsAimLongAim && AimName[0] != '\0') {
        for (int i = 0; i < MaxPlayerCount; i++) {
            if (strcmp(Aim[i].Name, AimName) == 0) {
                // Проверяем, все еще ли цель валидна
                bool isValid = true;
                
                // Проверка на ботов (если включено игнорирование)
                if (DrawIo[23] && Aim[i].isBot) {
                    isValid = false;
                }
                
                // Проверка на лежачих (если включено игнорирование)
                if (DrawIo[22] && Aim[i].health <= 0) {
                    isValid = false;
                }
                
                if (isValid) {
                    return i; // Возвращаем заблокированную цель
                }
                break;
            }
        }
    }
    
    for (int i = 0; i < MaxPlayerCount; i++)
    {
        // Проверяем условия для всех целей
        bool isValidTarget = true;
        
        // Проверка на ботов (если включено игнорирование)
        if (DrawIo[23] && Aim[i].isBot) {
            continue;
        }
        
        // Проверка на лежачих (если включено игнорирование)
        if (DrawIo[22] && Aim[i].health <= 0) {
            continue;
        }
        
        switch ((int)NumIo[18]) {
        case 0: // Режим: ближайший к центру экрана (в пределах фова)
            if (DrawIo[32] && IsAimLongAim && strcmp(Aim[i].Name, AimName) == 0) {
                return i; // Возвращаем заблокированную цель
            }
            
            // Проверяем, находится ли цель в пределах фова
            if (Aim[i].ScreenDistance < minScreenDist && Aim[i].ScreenDistance < NumIo[3]) {
                minScreenDist = Aim[i].ScreenDistance;
                targetByScreen = i;
            }
            break;
            
        case 1: // Режим: ближайший по мировому расстоянию (в пределах фова)
            if (DrawIo[32] && IsAimLongAim && strcmp(Aim[i].Name, AimName) == 0) {
                return i;
            }
            
            // Сначала проверяем, в фове ли цель
            if (Aim[i].ScreenDistance < NumIo[3]) {
                // Цель в фове, выбираем ближайшую по расстоянию
                if (Aim[i].WodDistance < minWorldDist) {
                    minWorldDist = Aim[i].WodDistance;
                    targetByWorld = i;
                }
            }
            break;
            
        case 2: // Режим: гибридный
            if (DrawIo[32] && IsAimLongAim && strcmp(Aim[i].Name, AimName) == 0) {
                return i;
            }
            
            if (Aiming == 1) { // В прицеле - как режим 0
                if (Aim[i].ScreenDistance < minScreenDist && Aim[i].ScreenDistance < NumIo[3]) {
                    minScreenDist = Aim[i].ScreenDistance;
                    targetByScreen = i;
                }
            } else { // Не в прицеле - как режим 1
                if (Aim[i].ScreenDistance < NumIo[3]) {
                    // Цель в фове, выбираем ближайшую по расстоянию
                    if (Aim[i].WodDistance < minWorldDist) {
                        minWorldDist = Aim[i].WodDistance;
                        targetByWorld = i;
                    }
                }
            }
            break;
        }
    }
    
    // Определяем финальную цель в зависимости от режима
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
    
    // Проверяем, нашли ли цель
    if (finalTarget == -1) {
        Gmin = -1;
        IsAimLongAim = false;
        return -1;
    }
    
    // Устанавливаем долговременную блокировку если включен DrawIo[32]
    if (DrawIo[32]) {
        strcpy(AimName, Aim[finalTarget].Name);
        IsAimLongAim = true;
    }
    
    Gmin = finalTarget;
    return finalTarget;
}
int getWepIdx(int id) {
    switch(id) {
        // Штурмовые винтовки (AR)
        case 101008: return 0;  // M762
        case 101001: return 1;  // AKM
        case 101004: return 2;  // M416
        case 101003: return 3;  // SCAR-L
        case 101002: return 4;  // M16A4
        case 101009: return 5;  // Mk47
        case 101006: return 6;  // AUG
        case 101005: return 7;  // Groza
        case 101010: return 8;  // G36C
        case 101007: return 9;  // QBZ
        case 101011: return 10; // AC-VAL
        case 101012: return 11; // Honey Badger
        // Пулеметы (LMG)
        case 105001: return 12; // M249
        case 105002: return 13; // DP-28
        case 105010: return 14; // MG3
        // Пистолеты-пулеметы (SMG)
        case 102001: return 15; // UZI
        case 102003: return 16; // Vector
        case 100103: return 17; // PP-19 (Bizon)
        case 102007: return 18; // MP5K
        case 102002: return 19; // UMP45
        case 102004: return 20; // Thompson
        case 102105: return 21; // P90
        case 102005: return 22; // Bizon (второй ID)
        default: return -1;
    }
}

float GetPitch(float Fov)
{

    if (Fov > 75 && Fov <= 130) // 不开镜
    {
        return (0.7f / 30) * NumIo[4];
    }
    else if (Fov == 70 || Fov == 75) // 机瞄 ，侧瞄
    {

        return (1.8f / 30) * NumIo[4];
    }
    else if (Fov == 55 || Fov == 60) // 红点 ，全息
    {

        return (1.8f / 30) * NumIo[4];
    }
    else if ((int)Fov == 44) // 2倍
    {

        return (5.3f / 30) * NumIo[4];
    }
    else if ((int)Fov == 26) // 3倍
    {

        return (7.2f / 30) * NumIo[4];
    }
    else if ((int)Fov == 20) // 4倍
    {

        return (8.6f / 30) * NumIo[4];
    }
    else if ((int)Fov == 13) // 6倍
    {

        return (14.1f / 30) * NumIo[4];
    }

    return (2.5f / 30) * NumIo[4]; // 8倍
}
float GetWeaponId(int WeaponId)
{
    switch (WeaponId)
    {
    // 突击步枪
    case 101008:
        return 1.2; // M762
        break;
    case 101001:
        return 1.1; // AKM
        break;
    case 101004:
        return 0.8; // M416
        break;
    case 101003:
        return 0.8; // SCAR-L
        break;
    case 101002:
        return 0.85; // M16A4
        break;
    case 101009:
        return 1.05; // Mk47
        break;
    case 101006:
        return 0.75; // AUG
        break;
    case 101005:
        return 1.05; // Groza
        break;
    case 101010:
        return 1.15; // G36C
        break;
    case 101007:
        return 0.95; // QBZ
        break;
    case 101011:
        return 1.0; // AC-VAL
        break;
    case 101012:
        return 0.90; // 蜜獾突击步枪
        break;
    // 机关枪
    case 105001:
        return 1.3; // M249
        break;
    case 105002:
        return 1.3; // DP-28
    case 105010:
        return 1.05; // MG3
        break;
    // 冲锋枪
    case 102001:
        return 0.6; // UZI
        break;
    case 102003:
        return 0.6; // Vector
        break;
    case 100103:
        return 0.65; // PP-19
        break;
    case 102007:
        return 0.6; // MP5K
        break;
    case 102002:
        return 0.6; // UMP 5
        break;
    case 102004:
        return 0.6; // 汤姆逊
        break;
    case 102105:
        return 0.5; // P90
        break;
    case 102005:
        return 0.6; // 野牛
        break;
    default:
        return 0.95; // 未收录
        break;
    }
    return 1.0; // 未获取
}

float getScopeAcc(int Scope)
{
    switch (Scope)
    {
    case 0:
        return 1.0f;
        break;
    case 1:
        return .98f;
        break;
    case 2:
        return .95f;
        break;
    case 3:
        return .94f;
        break;
    case 4:
        return .92f;
        break;
    case 5:
        return .9f;
        break;
    case 6:
        return .88f;
        break;
    case 7:
        return .86f;
        break;
    case 8:
        return .85f;
        break;
    default:
        return .9f;
        break;
    }
}
static Gyro *gyro = nullptr;
void AimBotAuto(ImDrawList *Draw)
// 自瞄线程
{
    bool isDown = false;
    // 是否按下触摸
    gyro = new Gyro;
    double leenx = 0.0f;
    // x轴速度

    double leeny = 0.0f;
    // y轴速度

    double RandomnumberX = NumIo[5], RandomnumberY = NumIo[6];
    // 随机触摸点

    double tx = NumIo[5], ty = NumIo[6];
    // 触摸点位置

    float halfSize = NumIo[7] / 2;

    float SpeedMin = 2.0f;
    // 临时触摸速度

    double w = 0.0f, h = 0.0f, cmp = 0.0f;
    // 宽度 高度 正切

    double ScreenX = displayInfo.width > displayInfo.height ? displayInfo.height : displayInfo.width;
    double ScreenY = displayInfo.width > displayInfo.height ? displayInfo.width : displayInfo.height;
    // 分辨率(竖屏)PS:滑屏用的坐标是竖屏状态下的

    double ScrXH = ScreenX / 2.0f;
    // 一半屏幕X

    double ScrYH = ScreenY / 2.0f;
    // 一半屏幕X

    static float TargetX = 0;
    static float TargetY = 0;
    // 触摸目标位置
    Vector3A obj;
    float PushX;
    /* 压枪X */
    float PushY;
    /* 压枪Y */

    float NowCoor[3];
    //   Touch_Init();
    while (1)
    {
    
    
        if (!DrawIo[20])
        {
            IsAimLongAim = false;
            usleep(NumIo[9] * 1000);
            continue;
        }
        findminat();
        // 获取目标
        if (Gmin == -1)
        {
            IsAimLongAim = false;
            usleep(NumIo[9] * 1000);
            continue;
        }

        float ToReticleDistance = Aim[Gmin].ScreenDistance;
        // --- ИСПРАВЛЕННЫЙ ПРЕДИКШЕН (БЕЗ БАЛЛИСТИКИ) ---
        float bulletSpeed = (NumIo[11] > 0) ? NumIo[11] : 850.0f; // Скорость пули из меню
        float intensity = (NumIo[22] > 0) ? NumIo[22] : 1.0f;    // Сила упреждения
        float FlyTime = Aim[Gmin].WodDistance / bulletSpeed;
        
        // Линейное предсказание: Позиция + (Скорость * Время * Множитель)
        obj.X = Aim[Gmin].ObjAim.X + (Aim[Gmin].AimMovement.X * FlyTime * intensity);
        obj.Y = Aim[Gmin].ObjAim.Y + (Aim[Gmin].AimMovement.Y * FlyTime * intensity);
        obj.Z = Aim[Gmin].ObjAim.Z + (Aim[Gmin].AimMovement.Z * FlyTime * intensity);
        // ----------------------------------------------
int wIdx = getWepIdx(MyWeapon);
int b = (wIdx != -1) ? (100 + wIdx * 5) : -1;

// РАЗДЕЛЕНИЕ ОТДАЧИ
float activeRecoil;
if (Aiming) {
    // Если мы в прицеле: берем спец. слот для ADS (b+3) или глобальный ADS (NumIo[28])
    activeRecoil = (b != -1 && NumIo[b+3] > 0) ? NumIo[b+3] : NumIo[28];
} else {
    // Если от бедра: берем обычный слот (b) или глобальный Hip (NumIo[4])
    activeRecoil = (b != -1 && NumIo[b] > 0) ? NumIo[b] : NumIo[4];
}

float activeSpeed  = (b != -1 && NumIo[b+1] > 0) ? NumIo[b+1] : NumIo[16];
float activeSmooth = (b != -1 && NumIo[b+2] > 0) ? NumIo[b+2] : NumIo[9];

if (Firing) {
    // Применяем выбранную отдачу
    obj.Z -= Aim[Gmin].WodDistance * activeRecoil * GetWeaponId(MyWeapon);
}

   float cameras = matrix[3] * obj.X + matrix[7] * obj.Y + matrix[11] * obj.Z + matrix[15];

        Vector2A vpvp = WorldToScreen(obj, matrix, cameras);
        瞄准x = vpvp.X;
        瞄准y = vpvp.Y;
        zm_y = vpvp.X;
        zm_x = ScreenX - vpvp.Y;
        float AimDs = sqrt(pow(px - vpvp.X, 2) + pow(py - vpvp.Y, 2));
Aimspeace = activeSpeed; 

        if (zm_x <= 0 || zm_x >= ScreenX || zm_y <= 0 || zm_y >= ScreenY)
        {
            IsAimLongAim = false;
            usleep(NumIo[9] * 1000);
            continue;
        }

        if (ToReticleDistance <= NumIo[3] || AimDs <= NumIo[3])
        {
            bool shouldAim = false;
            
            switch ((int)NumIo[0]) {
                case 0: shouldAim = (Firing == 1); break;
                case 1: shouldAim = (Aiming == 1); break;
                case 2: shouldAim = (Firing == 1 || Aiming == 1); break;
                case 3: shouldAim = (Firing == 1 && Aiming == 1); break;
            }

            
            if (!shouldAim) {
                {
                    IsAimLongAim = false;
                    usleep(NumIo[9] * 1000);
                    continue;
                }
                usleep(5000);
                continue;
            }
            float Acc = getScopeAcc((int)(90 / fov));
            if (zm_x > ScrXH)
            {
                TargetX = -(ScrXH - zm_x) / Aimspeace * Acc;
                if (TargetX + ScrXH > ScrXH * 2)
                {
                    TargetX = 0;
                }
            }
            else if (zm_x < ScrXH)
            {
                TargetX = (zm_x - ScrXH) / Aimspeace * Acc;
                if (TargetX + ScrXH < 0)
                {
                    TargetX = 0;
                }
            }
            if (zm_y > ScrYH)
            {
                TargetY = -(ScrYH - zm_y) / Aimspeace * Acc;
                if (TargetY + ScrYH > ScrYH * 2)
                {
                    TargetY = 0;
                }
            }
            else if (zm_y < ScrYH)
            {
                TargetY = (zm_y - ScrYH) / Aimspeace * Acc;
                if (TargetY + ScrYH < 0)
                {
                    TargetY = 0;
                }
            }
            if (TargetY >= 35 || TargetX >= 35 || TargetY <= -35 || TargetX <= -35)
            {
                usleep(NumIo[9] * 1000);
                continue;
            }

            float xxx = vpvp.X - (ScreenY / 2);
            float yyy = vpvp.Y - (ScreenX / 2);
            xxx *= Acc;
            yyy *= Acc;
            xxx *= NumIo[9];
            yyy *= NumIo[9];
            


    if (displayInfo.orientation == 1) {
        
        NumIo[10] = 0;
    } else {
   
        NumIo[10] = 1;
    }

            if (NumIo[10])
            {
                xxx = -xxx;
                yyy = -yyy;
            }
            if (gyro)
            {
                gyro->update(-(xxx * 0.002), (yyy * 0.002));
            }

            usleep(activeSmooth * 1000);
        }
        else
        {
            IsAimLongAim = false;
        }
    }
}