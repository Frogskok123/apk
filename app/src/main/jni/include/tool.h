#ifndef NATIVESURFACE_MEMREAD_H
#define NATIVESURFACE_MEMREAD_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/socket.h>
#include <malloc.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <locale>
#include <string>
#include <codecvt>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/uio.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include "wen所有整合版.h"
using namespace std;
std::vector<std::string> lines;
	// 定义圆周率
#define PI 3.141592653589793238
typedef unsigned short UTF16;
typedef char UTF8;


float px,py;

inline string exec_native_surface(string command) {
    char buffer[128];
    string result = "";
    // Open pipe to file
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "popen failed!";
    }
    // read till end of process:
    while (!feof(pipe)) {
        // use buffer to read and add to result
        if (fgets(buffer, 128, pipe) != nullptr) {
            result += buffer;
        }
    }
    pclose(pipe);
    return result;
}
// 获取进程


// 算法 结构体 //
struct Vector2A
{
	float X;
	float Y;

	  Vector2A()
	{
		this->X = 0;
		this->Y = 0;
	}

	Vector2A(float x, float y)
	{
		this->X = x;
		this->Y = y;
	}
};
struct Vector3A
{
	float X;
	float Y;
	float Z;

	  Vector3A()
	{
		this->X = 0;
		this->Y = 0;
		this->Z = 0;
	}

	Vector3A(float x, float y, float z)
	{
		this->X = x;
		this->Y = y;
		this->Z = z;
	}

};
struct D2DVector
{
	float X;
	float Y;
};

struct D3DVector
{
	float X;
	float Y;
	float Z;
};
struct D4DVector
{
	float X;
	float Y;
	float Z;
	float W;
};


struct FMatrix
{
	float M[4][4];
};





struct FTransform
{
	D4DVector Rotation;
	Vector3A Translation;
	float chunk;
	Vector3A Scale3D;
};






Vector3A MarixToVector(FMatrix matrix)//
{
	return Vector3A(matrix.M[3][0], matrix.M[3][1], matrix.M[3][2]);
}

FMatrix MatrixMulti(FMatrix m1, FMatrix m2)//
{
	FMatrix matrix = FMatrix();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				matrix.M[i][j] += m1.M[i][k] * m2.M[k][j];
			}
		}
	}
	return matrix;
}

FMatrix TransformToMatrix(FTransform transform)//
{
	FMatrix matrix;
	matrix.M[3][0] = transform.Translation.X;
	matrix.M[3][1] = transform.Translation.Y;
	matrix.M[3][2] = transform.Translation.Z;
	float x2 = transform.Rotation.X + transform.Rotation.X;
	float y2 = transform.Rotation.Y + transform.Rotation.Y;
	float z2 = transform.Rotation.Z + transform.Rotation.Z;
	float xx2 = transform.Rotation.X * x2;
	float yy2 = transform.Rotation.Y * y2;
	float zz2 = transform.Rotation.Z * z2;
	matrix.M[0][0] = (1 - (yy2 + zz2)) * transform.Scale3D.X;
	matrix.M[1][1] = (1 - (xx2 + zz2)) * transform.Scale3D.Y;
	matrix.M[2][2] = (1 - (xx2 + yy2)) * transform.Scale3D.Z;
	float yz2 = transform.Rotation.Y * z2;
	float wx2 = transform.Rotation.W * x2;
	matrix.M[2][1] = (yz2 - wx2) * transform.Scale3D.Z;
	matrix.M[1][2] = (yz2 + wx2) * transform.Scale3D.Y;
	float xy2 = transform.Rotation.X * y2;
	float wz2 = transform.Rotation.W * z2;
	matrix.M[1][0] = (xy2 - wz2) * transform.Scale3D.Y;
	matrix.M[0][1] = (xy2 + wz2) * transform.Scale3D.X;
	float xz2 = transform.Rotation.X * z2;
	float wy2 = transform.Rotation.W * y2;
	matrix.M[2][0] = (xz2 + wy2) * transform.Scale3D.Z;
	matrix.M[0][2] = (xz2 - wy2) * transform.Scale3D.X;
	matrix.M[0][3] = 0;
	matrix.M[1][3] = 0;
	matrix.M[2][3] = 0;
	matrix.M[3][3] = 1;
	return matrix;
}

FTransform getBone(unsigned long addr)
{
    FTransform transform;
    driver->read(addr, &transform, 4 * 11);
    return transform;
}

// 读取字符信息
void getUTF8(char *buf, unsigned long namepy)
{
	unsigned short buf16[32] = {0};
	driver->read(namepy, buf16, 28);
	unsigned short *pTempUTF16 = buf16;
	char *pTempUTF8 = buf;
	char *pUTF8End = pTempUTF8 + 32;
	while (pTempUTF16 < pTempUTF16 + 28)
	{
		if (*pTempUTF16 <= 0x007F && pTempUTF8 + 1 < pUTF8End)
		{
			*pTempUTF8++ = (char)*pTempUTF16;
		}
		else if (*pTempUTF16 >= 0x0080 && *pTempUTF16 <= 0x07FF && pTempUTF8 + 2 < pUTF8End)
		{
			*pTempUTF8++ = (*pTempUTF16 >> 6) | 0xC0;
			*pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
		}
		else if (*pTempUTF16 >= 0x0800 && *pTempUTF16 <= 0xFFFF && pTempUTF8 + 3 < pUTF8End)
		{
			*pTempUTF8++ = (*pTempUTF16 >> 12) | 0xE0;
			*pTempUTF8++ = ((*pTempUTF16 >> 6) & 0x3F) | 0x80;
			*pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
		}
		else
		{
			break;
		}
		pTempUTF16++;
	}
}



struct Vec2 {
    float x;
    float y;
    Vec2() {
        this->x = 0;
        this->y = 0;
    }
    Vec2(float x, float y) {
        this->x = x;
        this->y = y;
    }
    Vec2 operator+(float v) const {
        return Vec2(x + v, y + v);
    }
    Vec2 operator-(float v) const {
        return Vec2(x - v, y - v);
    }
    Vec2 operator*(float v) const {
        return Vec2(x * v, y * v);
    }
    Vec2 operator/(float v) const {
        return Vec2(x / v, y / v);
    }
    Vec2& operator+=(float v) {
        x += v; y += v; return *this;
    }
    Vec2& operator-=(float v) {
        x -= v; y -= v; return *this;
    }
    Vec2& operator*=(float v) {
        x *= v; y *= v; return *this;
    }
    Vec2& operator/=(float v) {
        x /= v; y /= v; return *this;
    }
    Vec2 operator+(const Vec2& v) const {
        return Vec2(x + v.x, y + v.y);
    }
    Vec2 operator-(const Vec2& v) const {
        return Vec2(x - v.x, y - v.y);
    }
    Vec2 operator*(const Vec2& v) const {
        return Vec2(x * v.x, y * v.y);
    }
    Vec2 operator/(const Vec2& v) const {
        return Vec2(x / v.x, y / v.y);
    }
    Vec2& operator+=(const Vec2& v) {
        x += v.x; y += v.y; return *this;
    }
    Vec2& operator-=(const Vec2& v) {
        x -= v.x; y -= v.y; return *this;
    }
    Vec2& operator*=(const Vec2& v) {
        x *= v.x; y *= v.y; return *this;
    }
    Vec2& operator/=(const Vec2& v) {
        x /= v.x; y /= v.y; return *this;
    }
};




struct Vec3 {
    float x;
    float y;
    float z;
    Vec3() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
    Vec3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vec3 operator+(float v) const {
        return Vec3(x + v, y + v, z + v);
    }
    Vec3 operator-(float v) const {
        return Vec3(x - v, y - v, z - v);
    }
    Vec3 operator*(float v) const {
        return Vec3(x * v, y * v, z * v);
    }
    Vec3 operator/(float v) const {
        return Vec3(x / v, y / v, z / v);
    }
    Vec3& operator+=(float v) {
        x += v; y += v; z += v; return *this;
    }
    Vec3& operator-=(float v) {
        x -= v; y -= v; z -= v; return *this;
    }
    Vec3& operator*=(float v) {
        x *= v; y *= v; z *= v; return *this;
    }
    Vec3& operator/=(float v) {
        x /= v; y /= v; z /= v; return *this;
    }
    Vec3 operator+(const Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }
    Vec3 operator-(const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }
    Vec3 operator*(const Vec3& v) const {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }
    Vec3 operator/(const Vec3& v) const {
        return Vec3(x / v.x, y / v.y, z / v.z);
    }
    Vec3& operator+=(const Vec3& v) {
        x += v.x; y += v.y; z += v.z; return *this;
    }
    Vec3& operator-=(const Vec3& v) {
        x -= v.x; y -= v.y; z -= v.z; return *this;
    }
    Vec3& operator*=(const Vec3& v) {
        x *= v.x; y *= v.y; z *= v.z; return *this;
    }
    Vec3& operator/=(const Vec3& v) {
        x /= v.x; y /= v.y; z /= v.z; return *this;
    }
};



// 计算旋转坐标
Vector2A rotateCoord(float angle, float objRadar_x, float objRadar_y)
{
	Vector2A radarCoordinate;
	float s = sin(angle * PI / 180);
	float c = cos(angle * PI / 180);
	radarCoordinate.X = objRadar_x * c + objRadar_y * s;
	radarCoordinate.Y = -objRadar_x * s + objRadar_y * c;
	return radarCoordinate;
}
//世界坐标转屏幕坐标
Vector2A WorldToScreen(Vector3A obj, float matrix[16], float ViewW)//
{
	float x =
		px + (matrix[0] * obj.X + matrix[4] * obj.Y + matrix[8] * obj.Z + matrix[12]) / ViewW * px;
	float y =
		py - (matrix[1] * obj.X + matrix[5] * obj.Y + matrix[9] * obj.Z + matrix[13]) / ViewW * py;

	return Vector2A(x, y);
}

Vector2A WorldToScreen2(Vector3A obj, float matrix[16]) // Убрали ViewW из аргументов
{
    // 1. Рассчитываем W (глубину) для текущей точки (кости)
    // В UE4 матрицы обычно Row-Major, и W находится в 4-м столбце (индексы 3, 7, 11, 15)
    float w = matrix[3] * obj.X + matrix[7] * obj.Y + matrix[11] * obj.Z + matrix[15];

    // 2. Проверка: если точка позади камеры (W < 0.001), возвращаем {0,0} или индикатор ошибки
    
    // 3. Рассчитываем X и Y (Clip Space)
    float x = matrix[0] * obj.X + matrix[4] * obj.Y + matrix[8] * obj.Z + matrix[12];
    float y = matrix[1] * obj.X + matrix[5] * obj.Y + matrix[9] * obj.Z + matrix[13];

    // 4. Перспективное деление (Perspective Divide) и перевод в координаты экрана
    // px и py - это координаты центра экрана (Width/2, Height/2)
    float ScreenX = px + (x / w) * px;
    float ScreenY = py - (y / w) * py;

    return Vector2A(ScreenX, ScreenY);
}



static float tm=127/255.f;
static ImVec4 arr[] = {{144/255.f,238/255.f,144/255.f,tm},{135/255.f,206/255.f,255/255.f,tm},{255/255.f,0/255.f,0/255.f,tm},{0/255.f,255/255.f,0/255.f,tm},{0 /255.f,255/255.f, 127/255.f,tm},{255/255.f,182/255.f,193/255.f,tm},{218/255.f,112/255.f,214/255.f,tm},{248/255.f,248/255.f,255/255.f,tm},{0/255.f,255/255.f,255/255.f,tm},{255/255.f,165/255.f,0/255.f,tm},{153/255.f,204/255.f,255/255.f,tm},{204/255.f,255/255.f,153/255.f,tm},{255/255.f,255/255.f,153/255.f,tm},{255/255.f,153/255.f,153/255.f,tm},{153/255.f,153/255.f,204/255.f,tm},{204/255.f,204/255.f,204/255.f,tm},{102/255.f,204/255.f,153/255.f,tm},{255/255.f,102/255.f,0/255.f,tm},{102/255.f,204/255.f,204/255.f,tm},{153/255.f,204/255.f,255/255.f,tm}};
static int length=sizeof(arr)/16;
float get_3D_Distance(float Self_x, float Self_y, float Self_z, float Object_x, float Object_y,
					  float Object_z)
{
	float x, y, z;
	x = Self_x - Object_x;
	y = Self_y - Object_y;
	z = Self_z - Object_z;
	// 求平方根
	return (float)(sqrt(x * x + y * y + z * z));
}


bool 获取载具(int id, char* name)
{
	//;
	// memset(name, '\0', sizeof(name));
	
	if (id >= 3306001 && id<= 336010)//不知道什么b原因读取不到
	{
		strcpy(name, "三轮摩托");
		return true;
	}
	else if (id >= 3306001 && id <= 3306010)//另写一行
	{
		strcpy(name, "轿车");
		return true;
	}
	else if (id >= 3301001 && id <= 3303010)
	{
		strcpy(name, "蹦蹦");
		return true;
    }
	else if (id >= 3334001 && id <= 3334010)
	{
		strcpy(name, "滑翔机");
		return true;
	}
	
	else if (id >= 3309001 && id <= 3309001)
	{
		strcpy(name, "三轮摩托");
		return true;
	}
	else if (id >= 3325001 && id <= 3325010)
	{
		strcpy(name, "快艇");
		return true;
    }
	else if (id >= 3313001 && id <= 3313010)
	{
		strcpy(name, "敞篷跑车");
		return true;
    }
	else if (id >= 3336001 && id <= 3336001)
	{
		strcpy(name, "带蓬跑车");
		return true;
	}
    else if (id >= 3349001 && id <= 3349001)
	{
		strcpy(name, "带蓬跑车");
		return true;
	}
	else if (id >= 3307001 && id <= 3307010)
	{
		strcpy(name,"巴士");
		return true;
    }
	else if (id >= 3302001 && id <= 3302010)
	{
		strcpy(name,"有篷吉普");
		return true;
    }
	else if (id >= 3303001 && id <= 3303010)
	{
		strcpy(name,"无蓬吉普");
		return true;
	}
	else if (id >= 3304001 && id <= 3304010)
	{
		strcpy(name,"吉普车");
		return true;
	}	
	else if (id >= 3314001 && id <= 3314010)
	{
		strcpy(name,"小型皮车");
		return true;
	}	
	else if (id >= 3322001 && id <= 3322010)
	{
		strcpy(name,"雪地吉普");
		return true;
	}	
	else if (id >= 3324001 && id <= 3324010)
	{
	
		strcpy(name,"雪地摩托");
		return true;
	}	
	else if (id >= 3321001 && id <= 3321010)
	{
		strcpy(name,"雪地摩托车");
		return true;
	}
	else if (id >= 3308001 && id <= 3308010)
	{
		strcpy(name,"双轮摩托车");
		return true;
	}
	else if (id >= 3310001 && id <= 3310010)
	{
		strcpy(name,"大型皮卡");
		return true;
	}
	else if (id >= 3315001 && id <= 3315010)
	{
		strcpy(name,"小绵羊");
		return true;
	}
	else if (id >= 3326001 && id <= 3326010)
	{
		strcpy(name,"摩托艇");
		return true;
	}
	else if (id >= 3329001 && id <= 3329010)
	{
		strcpy(name,"装甲车");
		return true;
	}
	else if (id >= 3312001 && id <= 3312010)
	{
		strcpy(name,"有蓬跑车");
		return true;
	}
	else if (id >= 3335001 && id <= 3335010)
	{
		strcpy(name,"大脚车");
		return true;
	}
	else if (id >= 3323001 && id <= 3323010)
	{
		strcpy(name,"三轮车");
		return true;
	}
	else if (id >= 3348001 && id <= 3348010)
	{
		strcpy(name,"姥爷车");
	    return true;
	}
	
	    return false;
}
bool 获取投掷物(int id, char* name)
{
if (id >= 602004 && id<= 602004)
	{
		
    strcpy(name, "手雷");
		
      return true;
      
	 }
	 else if (id >= 602003 && id <= 602003)//另写一行
	{
		strcpy(name, "燃烧瓶");
		return true;
	}
	else if (id >= 602002 && id <= 602002)//另写一行
	{
		strcpy(name, "烟雾弹");
		return true;
	}
	else if (id >= 602001 && id <= 602001)//另写一行
	{
		strcpy(name, "铝热弹");
		return true;
	}
        return false;
}
 char *状态(int dz)
{
   // int dz;
	static char name[30];
memset(name, '\0', sizeof(name));
      if(dz == 2097168)   
        strcpy(name, "开车");
 else if (dz== 16777216)
	    strcpy(name, "游泳"); 	
 else if (dz== 1040)
	    strcpy(name, "开镜"); 	
 else if (dz == 272)
        strcpy(name, "开枪");
 else if (dz== 4112)
	    strcpy(name, "歪头");
 else if (dz == 19)
	    strcpy(name, "奔跑");
 else if (dz == 144)
	    strcpy(name, "跳跃");	
 else if (dz == 147)
	    strcpy(name, "跳跃");	
 else if (dz == 6552)
		strcpy(name, "掐雷了快跑");
 else if (dz == 64)
		strcpy(name, "趴着");		
 else if (dz == 32)
		strcpy(name, "蹲着");		
 else if (dz == 144)
		strcpy(name, "跳跃");		
 else if (dz == 4128)
		strcpy(name, "蹲着歪头");		
 else if (dz == 4384)
		strcpy(name, "蹲着开火");		
 else if (dz == 528)
		strcpy(name, "换弹中");		
 else if (dz == 320 )
		strcpy(name, "趴着开火");		
 else if (dz == 288)
		strcpy(name, "蹲着开火");		
 else if (dz == 576)
		strcpy(name, "趴着换弹");				
 else if (dz == 544)
		strcpy(name, "蹲着换弹");			
 else if (dz == 67108880)
		strcpy(name, "翻墙中");		
 else if (dz == 273)
		strcpy(name, "走着开火");			
 else if (dz == 4194320)
		strcpy(name, "乘坐");	
 else if (dz == 17)
		strcpy(name, "行走中");			
  else if (dz == 288)
		strcpy(name, "蹲着开火");		
return name;

}
//获取手持
std::string GetHol(int wuqi) {
    switch (wuqi) {
    
    
        
case 604109:
return "Flying Armor";

        
case 601095:
return "Magnetic gun";
    
case 3000341:
return "Grenade robot";

    
    
    
case 101008:
return "M762";

case 1010081:
return "M762 broken";

case 1010082:
return "M762 broken";

case 1010083:
return "M762 Normal";

case 1010084:
return "M762 Improve";

case 1010085:
return "M762 HQ";

case 1010086:
return "M762 Kobra";

case 1010087:
return "M762 Steel";


case 101001:
return "AKM";

case 1010011:
return "AKM broken";

case 1010012:
return "AKM broken";

case 1010013:
return "AKM Normal";

case 1010014:
return "AKM Improve";

case 1010015:
return "AKM HQ";

case 1010016:
return "AKM Kobra";

case 1010017:
return "AKM Steel";


case 101004:
return "M416";

case 1010041:
return "M416 broken";

case 1010042:
return "M416 broken";

case 1010043:
return "M416 Normal";

case 1010044:
return "M416 Improve";

case 1010045:
return "M416 HQ";

case 1010046:
return "M416 Kobra";

case 1010047:
return "M416 Steel";


case 101003:
return "SCAR-L";

case 1010031:
return "SCAR-L broken";

case 1010032:
return "SCAR-L broken";

case 1010033:
return "SCAR-L Normal";

case 1010034:
return "SCAR-L Improve";

case 1010035:
return "SCAR-L HQ";

case 1010036:
return "SCAR-L Kobra";

case 1010037:
return "SCAR-L Steel";


case 101002:
return "M16A4";

case 1010021:
return "M16A4 broken";

case 1010022:
return "M16A4 broken";

case 1010023:
return "M16A4 Normal";

case 1010024:
return "M16A4 Improve";

case 1010025:
return "M16A4 HQ";

case 1010026:
return "M16A4 Kobra";

case 1010027:
return "M16A4 Steel";


case 101009:
return "Mk47";

case 1010091:
return "Mk47 broken";

case 1010092:
return "Mk47 broken";

case 1010093:
return "Mk47 Normal";

case 1010094:
return "Mk47 Improve";

case 1010095:
return "Mk47 HQ";

case 1010096:
return "Mk47 Kobra";

case 1010097:
return "Mk47 Steel";


case 101006:
return "AUG";

case 1010061:
return "AUG broken";

case 1010062:
return "AUG broken";

case 1010063:
return "AUG Normal";

case 1010064:
return "AUG Improve";

case 1010065:
return "AUG HQ";

case 1010066:
return "AUG Kobra";

case 1010067:
return "AUG Steel";


case 101005:
return "Groza";

case 1010051:
return "Groza broken";

case 1010052:
return "Groza broken";

case 1010053:
return "Groza Normal";

case 1010054:
return "Groza Improve";

case 1010055:
return "Groza HQ";

case 1010056:
return "Groza Kobra";

case 1010057:
return "Groza Steel";


case 101010:
return "G36C";

case 1010101:
return "G36C broken";

case 1010102:
return "G36C broken";

case 1010103:
return "G36C Normal";

case 1010104:
return "G36C Improve";

case 1010105:
return "G36C HQ";

case 1010106:
return "G36C Kobra";

case 1010107:
return "G36C Steel";


case 101007:
return "QBZ";

case 1010071:
return "QBZ broken";

case 1010072:
return "QBZ broken";

case 1010073:
return "QBZ Normal";

case 1010074:
return "QBZ Improve";

case 1010075:
return "QBZ HQ";

case 1010076:
return "QBZ Kobra";

case 1010077:
return "QBZ Steel";


case 101011:
return "AC-VAL";

case 1010111:
return "AC-VAL broken";

case 1010112:
return "AC-VAL broken";

case 1010113:
return "AC-VAL Normal";

case 1010114:
return "AC-VAL Improve";

case 1010115:
return "AC-VAL HQ";

case 1010116:
return "AC-VAL Kobra";

case 1010117:
return "AC-VAL Steel";


case 101012:
return "Badger";

case 1010121:
return "Badger broken";

case 1010122:
return "Badger broken";

case 1010123:
return "Badger Normal";

case 1010124:
return "Badger Improve";

case 1010125:
return "Badger HQ";

case 1010126:
return "Badger Kobra";

case 1010127:
return "Badger Steel";

//连狙
case 103009:
return "SLR";

case 1030091:
return "SLR broken";

case 1030092:
return "SLR broken";

case 1030093:
return "SLR Normal";

case 1030094:
return "SLR Improve";

case 1030095:
return "SLR HQ";

case 1030096:
return "SLR Kobra";

case 1030097:
return "SLR Steel";


case 103005:
return "VSS";

case 1030051:
return "VSS broken";

case 1030052:
return "VSS broken";

case 1030053:
return "VSS Normal";

case 1030054:
return "VSS Improve";

case 1030055:
return "VSS HQ";

case 1030056:
return "VSS Kobra";

case 1030057:
return "VSS Steel";


case 103006:
return "Mini14";

case 1030061:
return "Mini14 broken";

case 1030062:
return "Mini14 broken";

case 1030063:
return "Mini14 Normal";

case 1030064:
return "Mini14 Improve";

case 1030065:
return "Mini14 HQ";

case 1030066:
return "Mini14 Kobra";

case 1030067:
return "Mini14 Steel";


case 103010:
return "QBU";

case 1030101:
return "QBU broken";

case 1030102:
return "QBU broken";

case 1030103:
return "QBU Normal";

case 1030104:
return "QBU Improve";

case 1030105:
return "QBU HQ";

case 1030106:
return "QBU Kobra";

case 1030107:
return "QBU Steel";


case 103004:
return "SKS";

case 1030041:
return "SKS broken";

case 1030042:
return "SKS broken";

case 1030043:
return "SKS Normal";

case 1030044:
return "SKS Improve";

case 1030045:
return "SKS HQ";

case 1030046:
return "SKS Kobra";

case 1030047:
return "SKS Steel";


case 103007:
return "MK14";

case 1030071:
return "MK14 broken";

case 1030072:
return "MK14 broken";

case 1030073:
return "MK14 Normal";

case 1030074:
return "MK14 Improve";

case 1030075:
return "MK14 HQ";

case 1030076:
return "MK14 Kobra";

case 1030077:
return "MK14 Steel";


case 103014:
return "MK20-H";

case 1030141:
return "MK20-H broken";

case 1030142:
return "MK20-H broken";

case 1030143:
return "MK20-H Normal";

case 1030144:
return "MK20-H Improve";

case 1030145:
return "MK20-H HQ";

case 1030146:
return "MK20-H Kobra";

case 1030147:
return "MK20-H Steel";


case 103013:
return "M4117";

case 1030131:
return "M4117 broken";

case 1030132:
return "M4117 broken";

case 1030133:
return "M4117 Normal";

case 1030134:
return "M4117 Improve";

case 1030135:
return "M4117 HQ";

case 1030136:
return "M4117 Kobra";

case 1030137:
return "M4117 Steel";

//连狙
case 103012:
return "ARM";

case 1030121:
return "ARM broken";

case 1030122:
return "ARM broken";

case 1030123:
return "ARM Normal";

case 1030124:
return "ARM Improve";

case 1030125:
return "ARM HQ";

case 1030126:
return "ARM Kobra";

case 1030127:
return "ARM Steel";


case 103003:
return "AWM";

case 1030031:
return "AWM broken";

case 1030032:
return "AWM broken";

case 1030033:
return "AWM Normal";

case 1030034:
return "AWM Improve";

case 1030035:
return "AWM HQ";

case 1030036:
return "AWM Kobra";

case 1030037:
return "AWM Steel";


case 103002:
return "M24";

case 1030021:
return "M24 broken";

case 1030022:
return "M24 broken";

case 1030023:
return "M24 Normal";

case 1030024:
return "M24 Improve";

case 1030025:
return "M24 HQ";

case 1030026:
return "M24 Kobra";

case 1030027:
return "M24 Steel";


case 103011:
return "Mosin";

case 1030111:
return "Mosin broken";

case 1030112:
return "Mosin broken";

case 1030113:
return "Mosin Normal";

case 1030114:
return "Mosin Improve";

case 1030115:
return "Mosin HQ";

case 1030116:
return "Mosin Kobra";

case 1030117:
return "Mosin Steel";


case 103001:
return "Kar98K";

case 1030011:
return "Kar98K broken";

case 1030012:
return "Kar98K broken";

case 1030013:
return "Kar98K Normal";

case 1030014:
return "Kar98K Improve";

case 1030015:
return "Kar98K HQ";

case 1030016:
return "Kar98K Kobra";

case 1030017:
return "Kar98K Steel";


case 103008:
return "Win94";

case 1030081:
return "Win94 broken";

case 1030082:
return "Win94 broken";

case 1030083:
return "Win94 Normal";

case 1030084:
return "Win94 Improve";

case 1030085:
return "Win94 HQ";

case 1030086:
return "Win94 Kobra";

case 1030087:
return "Win94 Steel";

//机关枪
case 105001:
return "M249";

case 1050011:
return "M249 broken";

case 1050012:
return "M249 broken";

case 1050013:
return "M249 Normal";

case 1050014:
return "M249 Improve";

case 1050015:
return "M249 HQ";

case 1050016:
return "M249 Kobra";

case 1050017:
return "M249 Steel";


case 105002:
return "DP-28";

case 1050021:
return "DP-28 broken";

case 1050022:
return "DP-28 broken";

case 1050023:
return "DP-28 Normal";

case 1050024:
return "DP-28 Improve";

case 1050025:
return "DP-28 HQ";

case 1050026:
return "DP-28 Kobra";

case 1050027:
return "DP-28 Steel";


case 105010:
return "MG3";

case 1050101:
return "MG3 broken";

case 1050102:
return "MG3 broken";

case 1050103:
return "MG3 Normal";

case 1050104:
return "MG3 Improve";

case 1050105:
return "MG3 HQ";

case 1050106:
return "MG3 Kobra";

case 1050107:
return "MG3 Steel";


case 107001:
return "CrossBow";

case 1070011:
return "CrossBow broken";

case 1070012:
return "CrossBow broken";

case 1070013:
return "CrossBow Normal";

case 1070014:
return "CrossBow Improve";

case 1070015:
return "CrossBow HQ";

case 1070016:
return "CrossBow Kobra";

case 1070017:
return "CrossBow Steel";


case 107007:
return "Bow";

case 1070071:
return "Bow broken";

case 1070072:
return "Bow broken";

case 1070073:
return "Bow Normal";

case 1070074:
return "Bow Improve";

case 1070075:
return "Bow HQ";

case 1070076:
return "Bow Kobra";

case 1070077:
return "Bow Steel";

//冲锋枪
case 102001:
return "UZI";

case 1020011:
return "UZI broken";

case 1020012:
return "UZI broken";

case 1020013:
return "UZI Normal";

case 1020014:
return "UZI Improve";

case 1020015:
return "UZI HQ";

case 1020016:
return "UZI Kobra";

case 1020017:
return "UZI Steel";


case 102003:
return "Vector";

case 1020031:
return "Vector broken";

case 1020032:
return "Vector broken";

case 1020033:
return "Vector Normal";

case 1020034:
return "Vector Improve";

case 1020035:
return "Vector HQ";

case 1020036:
return "Vector Kobra";

case 1020037:
return "Vector Steel";


case 100103:
return "PP-19";

case 1001031:
return "PP-19 broken";

case 1001032:
return "PP-19 broken";

case 1001033:
return "PP-19 Normal";

case 1001034:
return "PP-19 Improve";

case 1001035:
return "PP-19 HQ";

case 1001036:
return "PP-19 Kobra";

case 1001037:
return "PP-19 Steel";


case 102007:
return "MP5K";

case 1020071:
return "MP5K broken";

case 1020072:
return "MP5K broken";

case 1020073:
return "MP5K Normal";

case 1020074:
return "MP5K Improve";

case 1020075:
return "MP5K HQ";

case 1020076:
return "MP5K Kobra";

case 1020077:
return "MP5K Steel";


case 102002:
return "UMP-45";

case 1020021:
return "UMP-45 broken";

case 1020022:
return "UMP-45 broken";

case 1020023:
return "UMP-45 Normal";

case 1020024:
return "UMP-45 Improve";

case 1020025:
return "UMP-45 HQ";

case 1020026:
return "UMP-45 Kobra";

case 1020027:
return "UMP-45 Steel";


case 102004:
return "Thomson";

case 1020041:
return "Thomson broken";

case 1020042:
return "Thomson broken";

case 1020043:
return "Thomson Normal";

case 1020044:
return "Thomson Improve";

case 1020045:
return "Thomson HQ";

case 1020046:
return "Thomson Kobra";

case 1020047:
return "Thomson Steel";


case 102105:
return "P90";

case 1021051:
return "P90 broken";

case 1021052:
return "P90 broken";

case 1021053:
return "P90 Normal";

case 1021054:
return "P90 Improve";

case 1021055:
return "P90 HQ";

case 1021056:
return "P90 Kobra";

case 1021057:
return "P90 Steel";


case 102005:
return "Bison";

case 1020051:
return "Bison broken";

case 1020052:
return "Bison broken";

case 1020053:
return "Bison Normal";

case 1020054:
return "Bison Improve";

case 1020055:
return "Bison HQ";

case 1020056:
return "Bison Kobra";

case 1020057:
return "Bison Steel";

//霰弹枪
case 104001:
return "S686";

case 1040011:
return "S686 broken";

case 1040012:
return "S686 broken";

case 1040013:
return "S686 Normal";

case 1040014:
return "S686 Improve";

case 1040015:
return "S686 HQ";

case 1040016:
return "S686 Kobra";

case 1040017:
return "S686 Steel";


case 104002:
return "S1897";

case 1040021:
return "S1897 broken";

case 1040022:
return "S1897 broken";

case 1040023:
return "S1897 Normal";

case 1040024:
return "S1897 Improve";

case 1040025:
return "S1897 HQ";

case 1040026:
return "S1897 Kobra";

case 1040027:
return "S1897 Steel";


case 104003:
return "S12K";

case 1040031:
return "S12K broken";

case 1040032:
return "S12K broken";

case 1040033:
return "S12K Normal";

case 1040034:
return "S12K Improve";

case 1040035:
return "S12K HQ";

case 1040036:
return "S12K Kobra";

case 1040037:
return "S12K Steel";


case 104004:
return "DBS";

case 1040041:
return "DBS broken";

case 1040042:
return "DBS broken";

case 1040043:
return "DBS Normal";

case 1040044:
return "DBS Improve";

case 1040045:
return "DBS HQ";

case 1040046:
return "DBS Kobra";

case 1040047:
return "DBS Steel";


case 104100:
return "SPAS-12";

case 1041001:
return "SPAS-12 broken";

case 1041002:
return "SPAS-12 broken";

case 1041003:
return "SPAS-12 Normal";

case 1041004:
return "SPAS-12 Improve";

case 1041005:
return "SPAS-12 HQ";

case 1041006:
return "SPAS-12 Kobra";

case 1041007:
return "SPAS-12 Steel";

case 1000082:
return "M134";

case 602004:
return "GRANAGE";

case 602003:
return "MOLOTOV";

case 602002:
return "smoke";

case 602001:
return "Flash";

//近战武器
case 108003:
return "Hand";

case 108002:
return "Hui";

case 108001:
return "hand";

case 108004:
return "chlen";

case 0:
return "hand";

default:
return "idk";

    }
}

struct VehicleConfig {
    const char* id;       // Часть имени в памяти (например "VH_Motorcycle")
    const char* label;    // Что писать на экране (например "Motorcycle")
    ImColor color; // Было ImU32, стало ImColor
};
const VehicleConfig vehicles[] = {
    {"VH_Motorcycle", "Motorcycle", IM_COL32(0,255,127,255)},
    {"_Mountainbike_Training_C", "Bike", IM_COL32(0,255,127,255)},
    {"VH_Scooter_C", "Scooter", IM_COL32(0,255,127,255)},
    {"VH_BRDM_C", "BRDM", IM_COL32(0,255,127,255)},
    {"licedTrain_C", "Train", IM_COL32(0,255,127,255)},
    {"Buggy", "Buggy", IM_COL32(0,255,127,255)},
    {"VH_Tuk", "Tuk", IM_COL32(0,255,127,255)},
    {"VH_Snowmobile_C", "SnowMobile", IM_COL32(0,255,127,255)},
    {"MechaMissile_C", "MechaMissile", IM_COL32(0,255,127,255)},
    {"BP_MechaVehicle_C", "MechaVehicle", IM_COL32(0,255,127,255)},
    {"MiniBus", "MiniBus", IM_COL32(0,255,127,255)},
    {"Mirado", "Mirado", IM_COL32(0,255,127,255)},
    {"Rony", "Rony", IM_COL32(0,255,127,255)},
    {"PickUp_07_C", "Jeep", IM_COL32(0,255,127,255)},
    {"UAZ", "UAZ", IM_COL32(0,255,127,255)},
    {"Dacia", "Dacia", IM_COL32(0,255,127,255)},
    {"VH_ATV", "ATV", IM_COL32(0,255,127,255)},
    {"Bigfoot", "Bigfoot", IM_COL32(0,255,127,255)},
    {"UTV", "UTV", IM_COL32(0,255,127,255)},
    {"Monster", "Monster", IM_COL32(0,255,127,255)},
    {"Motorglider", "Motorglider", IM_COL32(0,255,127,255)},
    {"CoupeRB", "CoupeRB", IM_COL32(0,255,127,255)},
    {"Snowbike", "Snowbike", IM_COL32(0,255,127,255)},
    {"AquaRail", "AquaRail", IM_COL32(0,255,127,255)},
    {"PG117", "PG117", IM_COL32(0,255,127,255)},
    {"BP_CombinedMecha", "CombinedMecha", IM_COL32(0,255,127,255)}
};


ImColor ColorArr[100];
ImColor RandomColor()
{
    int R, G, B, A = 150;
    R = (random() % 255);
    G = (random() % 255);
    B = (random() % 255);
    return ImColor(R, G, B, A);
}
void ColorInitialization()
{
    for(int i = 0; i < 100; i++) {
        ColorArr[i] = RandomColor();
    }
}

ImColor TeamColor(int Num)
{
    if(Num < 99 && Num > 0) {
        return ColorArr[Num];
    } else {
        return ImColor(67, 205, 128, 150);
    }
}
#endif
