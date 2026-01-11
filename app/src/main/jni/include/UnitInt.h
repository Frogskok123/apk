/* 颜色定义开始 */
ImColor 红色=ImColor(255,0,0,255);
ImColor 白色=ImColor(255,255,255,255);
ImColor 绿色=ImColor(0,255,0,255);
ImColor 粉色=ImColor(226,145,163,255);
ImColor 紫色=ImColor(15,67,255,255);
ImColor 黄色=ImColor(200,255,0,255);
ImColor 黑色=ImColor(0,0,0,255);
ImColor 蓝色=ImColor(0,167,241,255);
ImColor 棕色=ImColor(144,144,144,255);
ImColor 灰色=ImColor(10,40,40,205);
ImColor 空白=ImColor(0,0,0,0);

ImColor TouchingColor = ImColor(255, 0, 0, 150);
ImColor BoxColor = {1.0f,0.0f,0.0f,1.0f};
ImColor BotBoxColor = ImColor(255,255,255,255);
ImColor BoxblackColor = ImColor(255,0,0,25);
ImColor BotBoxblackColor = ImColor(255,0,0,25);
ImColor LineColor = ImColor(255,0,0,255);
ImColor BotLineColor = ImColor(255,255,255,255);
ImColor BoneColor = ImColor(255,0,0,255);
ImColor BotBoneColor = ImColor(255,255,255,255);
ImColor RightColor = ImColor(255,200,0,255);
ImColor BotRightColor = ImColor(255,255,255,255);    
ImColor WarningColor = ImColor(255,0,0,255);
ImColor BotWarningColor = ImColor(255,255,255,255);
ImColor 骨骼颜色 = 红色;
ImColor 人机骨骼颜色 = 绿色;
ImColor 真人射线 = 红色;
ImColor 人机射线 = 绿色;
/* 颜色定义结束 */
pid_t pid;
/* 绘制定义开始 */
char extra[100];
float matrix[16] = { 0 }, angle, camera, r_x, r_y, r_w,ToReticleDistance,瞄准x,瞄准y;;
long int libbase, Gname, Uworld, Arrayaddr, Matrix, oneself, Objaddr, Object, weapon,handheld, Mesh, human, Bone, Uleve;
int Myteam,countddr,PlayerCount,RobotCount;
char PlayerName[100]; 
char PlayerUID[100];
int zdmax,scwq,dqzd;
void ExplosionRange(Vec3 Obj, ImColor color, float Range, float thickn);
void ExplosionRangeFilled(Vec3 Obj, ImColor color, float Range, int num_segments);
/* 自瞄定义开始 */
float zm_x,zm_y;
int WorldDistance;
int AimCount = 0;
int MaxPlayerCount = 0;
int Gmin=-1;
float Aimspeace;
bool IsAimLongAim = false;
char AimName[32];
float fov;// 自身FOV
bool Firing;
int Aiming;
int MyWeapon;
extern ImVec2 Pos;

struct AimStruct
{
    Vector3A ObjAim;  
    Vector3A AimMovement;  
    float ScreenDistance = 0;
    float WodDistance = 0; 
    bool isBot;
    float health;
	char Name[32];
}
Aim[100];
/* 自瞄定义结束 */
// 绘制四角方框
void DrawPlayerBox(ImDrawList *Draw, float left,float right, float bottom, float top, float x, float y, ImColor color)
{
	float LineSize = 1.0f;
	// x距离，y距离
	float xd = x - left; float yd = y - top;
	
	// 左上角
	Draw->AddLine({left,top},{left - xd*2,top}, color, {LineSize});
	Draw->AddLine({left,top},{left,top + yd/3}, color, {LineSize});
	
	// 右上角
	Draw->AddLine({right,top},{right + xd*2,top}, color, {LineSize});
	Draw->AddLine({right, top},{right,top + yd/3}, color, {LineSize});
	
	// 左下角
	Draw->AddLine({left,bottom},{left - xd*2,bottom}, color, {LineSize});
	Draw->AddLine({left,bottom},{left,bottom - yd/3}, color, {LineSize});
	
	// 右下角
	Draw->AddLine({right,bottom},{right + xd*2,bottom},color, {LineSize});
	Draw->AddLine({right,bottom},{right,bottom - yd/3}, color, {LineSize});
	
}

void Draw3DBox(float Camera, Vector3A Max,Vector3A Min,ImColor color,float matrix[16])
{
	float LineSize = 1.5f;
	
    Min.X = Max.X - 50.f;
    Min.Y = Max.Y - 50.f; // foot
    Min.Z = Min.Z - 10.f;

    Max.X = Max.X + 50.f;
    Max.Y = Max.Y + 50.f; // head
    Max.Z = Max.Z + 50.f;

    Vector3A v1, v2, v3, v4, v5, v6, v7, v8;
    v1 = Min;v8 = Max;v2 = Min;v3 = Min;v4 = Min;

    // 转换
    v2.X = Max.X;
    v3.Y = Max.Y;
    v4.X = Max.X;
    v4.Y = Max.Y;

    v5 = v1;
    v5.Z = Max.Z;
    v6 = v2;
    v6.Z = Max.Z;
    v7 = v3;
    v7.Z = Max.Z;

    Vector2A vv1, vv2, vv3, vv4, vv5, vv6, vv7, vv8;
    
    vv1 = WorldToScreen(v1, matrix,Camera);
    vv2 = WorldToScreen(v2, matrix,Camera);
    vv3 = WorldToScreen(v3, matrix,Camera);
    vv4 = WorldToScreen(v4, matrix,Camera);
    vv5 = WorldToScreen(v5, matrix,Camera);
    vv6 = WorldToScreen(v6, matrix,Camera);
    vv7 = WorldToScreen(v7, matrix,Camera);
    vv8 = WorldToScreen(v8, matrix,Camera);
        
    ImGui::GetForegroundDrawList()->AddLine({vv1.X, vv1.Y}, {vv2.X, vv2.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv1.X, vv1.Y}, {vv3.X, vv3.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv1.X, vv1.Y}, {vv5.X, vv5.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv2.X, vv2.Y}, {vv4.X, vv4.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv2.X, vv2.Y}, {vv6.X, vv6.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv3.X, vv3.Y}, {vv4.X, vv4.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv3.X, vv3.Y}, {vv7.X, vv7.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv4.X, vv4.Y}, {vv8.X, vv8.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv5.X, vv5.Y}, {vv6.X, vv6.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv5.X, vv5.Y}, {vv7.X, vv7.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv7.X, vv7.Y}, {vv8.X, vv8.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv6.X, vv6.Y}, {vv8.X, vv8.Y}, color, {LineSize});
}
