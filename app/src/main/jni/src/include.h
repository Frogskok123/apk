bool GetHandInfo(char* gname,char** name);
    bool GetItemInfo(char* gname,char** name,ImVec4* color);
    bool GetGrenadeInfo(char* gname,char** name,ImVec4* color);
    bool GetVehicleInfo(char* gname,char** name,ImVec4* color);    
    bool showItem, showShotgun, showPistol, show556, show762, showRifle, showMirror, showExpansion, showDrug, showArmor, showSubmachine, showSniper, showOtherParts, showChest, showDrop;

ImVec4 esp_color = ImVec4(255/255.f,0/255.f,0/255.f,255/255.f);
ImVec4 bot_color = ImVec4(1.f,1.f,1.f,1.f);
ImVec4 ray_color = ImVec4(1.f,1.f,1.f,1.f);
ImVec4 bone_color = ImVec4(0/255.f,255/255.f,0/255.f,255/255.f);
ImVec4 vehicle_color = ImVec4(0/255.f,255/255.f,255/255.f,255/255.f);
ImVec4 rifle_color = ImVec4(255/255.f,0/255.f,0/255.f,255.f/255.f);
ImVec4 submachine_color = ImVec4(255/255.f,110/255.f,180/255.f,255.f/255.f);
ImVec4 snipe_color = ImVec4(144/255.f,238/255.f,0/255.f,255.f/255.f);
ImVec4 box_color = ImVec4(255/255.f,192/255.f,203/255.f,255.f/255.f);
ImVec4 drop_color = ImVec4(255/255.f,0/255.f,0/255.f,255.f/255.f);
ImVec4 drug_color = ImVec4(255/255.f,105/255.f,180/255.f,255.f/255.f);
ImVec4 armor_color = ImVec4(255/255.f,215/255.f,0/255.f,255.f/255.f);
ImVec4 color_556 = ImVec4(0/255.f,255/255.f,0/255.f,255.f/255.f);
ImVec4 color_762 = ImVec4(255/255.f,255/255.f,0/255.f,255.f/255.f);
ImVec4 mirror_color = ImVec4{255.f/255.f,144.f/255.f,0.f/255.f,255.f/255.f};
ImVec4 otherparts_color = ImVec4{245.f/255.f,245.f/255.f,245.f/255.f,255.f/255.f};
ImVec4 expansion_color = ImVec4{208.f/255.f,128.f/255.f,114.f/255.f,255.f/255.f};
ImVec4 aim_color = ImVec4(255/255.f,0/255.f,0/255.f,255/255.f);

// 获取物资
bool GetItemInfo(char* gname,char** name,ImVec4* color)
{
    if(show556)
    {
        if(strcmp(gname,"BP_Ammo_556mm_Pickup_C")== 0)
        {
            *name="5.56MM";
            *color=color_556;
            return true;
        }
    }
    if(show762)
    {
        if(strcmp(gname,"BP_Ammo_762mm_Pickup_C")== 0) 
        {
            *name = "7.62MM";
            *color = color_762;
            return true;
        }
    }
    //刀
    if(strcmp(gname,"BP_WEP_Pan_Pickup_C")== 0)
    {
        *name="平底锅";
        *color=rifle_color;
        return true;
    }
    //显示步枪img
    if(showRifle)
    {
        if(strcmp(gname,"BP_Pistol_Flaregun")==0)
        {
            *name="信号枪";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_AKM_Wrapper_C")==0)
        {
            *name="AKM";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_VAL_Wrapper_C")==0)
        {
            *name="VAL";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_M416_Wrapper_C")==0)
        {
            *name="M416";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_M16A4_Wrapper_C")==0)
        {
            *name="M16A4";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_SCAR_Wrapper_C")==0)
        {
            *name="SCAR";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_QBZ_Wrapper_C")==0)
        {
            *name="QBZ";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_G36_Wrapper_C")==0)
        {
            *name="G36C";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_M762_Wrapper_C")==0)
        {
            *name="M762";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_Groza_Wrapper_C")==0)
        {
            *name="Groza";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_AUG_Wrapper_C")==0)
        {
            *name="AUG";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Other_DP28_Wrapper_C")==0)
        {
            *name="DP28";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Other_M249_Wrapper_C")==0)
        {
            *name="M249";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Other_MG3_Wrapper_C")==0)
        {
            *name="MG3";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Other_CrossBow_Wrapper_C")==0)
        {
            *name="CrossBow";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_QBU_Wrapper_C")==0)
        {
            *name="QBU";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_VSS_Wrapper_C")==0)
        {
            *name="VSS";
            *color=snipe_color;
            return true;
        }
    }
    //显示冲锋枪
    if(showSubmachine)
    {
        if(strcmp(gname,"BP_MachineGun_UMP9_Wrapper_C")==0)
        {
            *name="UMP45";
            *color=submachine_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_P90CG17_Wrapper_C")==0)
        {
            *name="P90CG17";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_TommyGun_Wrapper_C")==0)
        {
            *name="汤姆逊";
            *color=submachine_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_PP19_Wrapper_C")==0)
        {
            *name="PP19";
            *color=submachine_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_Uzi_Wrapper_C")==0)
        {
            *name="Uzi";
            *color=submachine_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_Vector_Wrapper_C")==0)
        {
            *name="Vector";
            *color=submachine_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_MP5K_Wrapper_C")==0)
        {
            *name="MP5K";
            *color=snipe_color;
            return true;
        }
        
    }
    //显示狙击枪
    if(showSniper)
    {
        if(strcmp(gname,"BP_Sniper_Kar98k_Wrapper_C")==0)
        {
            *name="Kar98k";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_Win94_Wrapper_C")==0)
        {
            *name="WIN94";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_Mini14_Wrapper_C")==0)
        {
            *name="Mini14";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_SKS_Wrapper_C")==0)
        {
            *name="SKS";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_M24_Wrapper_C")==0)
        {
            *name="M24";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_rifle_Mk47_Wrapper_C")==0)
        {
            *name="Mk47";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_WEP_Mk14_Pickup_C")==0)
        {
            *name="MK14";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_AWM_Wrapper_C")==0)
        {
            *name="AWM";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_Mosin_Wrapper_C")==0)
        {
            *name="Mosin";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_AMR_Wrapper_C")==0)
        {
            *name="AMR";
            *color=snipe_color;
            return true;
        }
        
        if(strcmp(gname,"BP_Sniper_SLR_Wrapper_C")==0)
        {
            *name="SLR";
            *color=snipe_color;
            return true;
        }
    }
    //霰弹枪
    if(showShotgun)
    {
        if(strcmp(gname,"BP_ShotGun_DP12_Wrapper_C")==0)
        {
            *name="DP12";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_ShotGun_S686_Wrapper_C")==0)
        {
            *name="S686";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_ShotGun_SawedOff_Wrapper_C")==0)
        {
            *name="SawedOff";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_ShotGun_S1897_Wrapper_C")==0)
        {
            *name="S1897";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_ShotGun_S12K_Wrapper_C")==0)
        {
            *name="S12K";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_ShotGun_SPAS-12_Wrapper_C")==0)
        {
            *name="SPAS-12";
            *color=snipe_color;
            return true;
        }
    }
    //手枪
    if(showPistol)
    {
        if(strcmp(gname,"BP_Pistol_P18C_Wrapper_C")==0)
        {
            *name="P18C";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_R1895_Wrapper_C")==0)
        {
            *name="R1895";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_P1911_Wrapper_C")==0)
        {
            *name="P1911";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_P92_Wrapper_C")==0)
        {
            *name="P92";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_R45_Wrapper_C")==0)
        {
            *name="R45";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_Vz61_Wrapper_C")==0)
        {
            *name="Vz61";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_DesertEagle_Wrapper_C")==0)
        {
            *name="DesertEagle";
            *color=snipe_color;
            return true;
        }
    }
    //倍镜
    if(showMirror)
    {
        if(strcmp(gname,"BP_MZJ_8X_Pickup_C")==0)
        {
            *name="8x镜";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_6X_Pickup_C")==0)
        {
            *name="6x镜";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_4X_Pickup_C")==0)
        {
            *name="4x镜";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_3X_Pickup_C")==0)
        {
            *name="3x镜";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_2X_Pickup_C")==0)
        {
            *name="2x镜";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_HD_Pickup_C")==0)
        {
            *name="红点";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_QX_Pickup_C")==0)
        {
            *name="全息";
            *color=otherparts_color;
            return true;
        }
    }
    //扩容
    if(showExpansion)
    {
        if(strcmp(gname,"BP_DJ_Large_EQ_Pickup_C")==0)
        {
            *name="(步枪)快速扩容";
            *color=expansion_color;
            return true;
        }
        if(strcmp(gname,"BP_DJ_Large_E_Pickup_C")==0)
        {
            *name="(步枪)扩容";
            *color=expansion_color;
            return true;
        }
        if(strcmp(gname,"BP_DJ_Sniper_EQ_Pickup_C")==0)
        {
            *name="(狙)快速扩容";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_DJ_Sniper_E_Pickup_C")==0)
        {
            *name="(狙)扩容";
            *color=mirror_color;
            return true;
        }
    }
    //其他配件
    if(showOtherParts)
    {
        if(strcmp(gname,"BP_QK_Large_Suppressor_Pickup_C")==0)
        {
            *name="(步枪)消音器";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_QK_Sniper_Suppressor_Pickup_C")==0)
        {
            *name="(狙)消音器";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_QT_Sniper_Pickup_C")==0)
        {
            *name="托腮板";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_ZDD_Sniper_Pickup_C")==0)
        {
            *name="(狙)子弹带";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_QK_Large_Compensator_Pickup_C")==0)
        {
            *name="(步枪)补偿器";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_QK_Sniper_Compensator_Pickup_C")==0)
        {
            *name="(狙)补偿器";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_WB_Vertical_Pickup_C")==0)
        {
            *name="垂直握把";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_QT_A_Pickup_C")==0)
        {
            *name="枪托";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_WB_Angled_Pickup_C")==0)
        {
            *name="直角握把";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_WB_ThumbGrip_Pickup_C")==0)
        {
            *name="拇指握把";
            *color=otherparts_color;
            return true;
        }
    }
    //药品显示
    if(showDrug)
    {
        if(strcmp(gname,"Injection_Pickup_C")==0)
        {
            *name="肾上腺素";
            *color=drug_color;
            return true;
        }
        if(strcmp(gname,"Firstaid_Pickup_C")==0)
        {
            *name="急救包";
            *color=drug_color;
            return true;
        }
        if(strcmp(gname,"FirstAidbox_Pickup_C")==0)
        {
            *name="医疗箱";
            *color=drug_color;
            return true;
        }
        if(strcmp(gname,"Pills_Pickup_C")==0)
        {
            *name="止痛药";
            *color=drug_color;
            return true;
        }
        if(strcmp(gname,"Drink_Pickup_C")==0)
        {
            *name="饮料";
            *color=drug_color;
            return true;
        }
        if(strcmp(gname,"Bandage_Pickup_C")==0)
        {
            *name="绷带";
            *color=drug_color;
            return true;
        }
    }
    //甲和头盔
    if(showArmor)
    {
        if(strcmp(gname,"PickUp_BP_Helmet_Lv3_C")==0)
        {
            *name="三级头";
            *color=armor_color;
            return true;
        }
        if(strcmp(gname,"PickUp_BP_Armor_Lv3_C")==0)
        {
            *name="三级甲";
            *color=armor_color;
            return true;
        }
        if(strcmp(gname,"PickUp_BP_Bag_Lv3_B_C")==0 || strcmp(gname,"PickUp_BP_Bag_Lv3_A_C")==0 || strcmp(gname,"PickUp_BP_Bag_Lv3_C")==0 )
        {
            *name="三级包";
            *color=armor_color;
            return true;
        }
        if(strcmp(gname,"PickUp_BP_Helmet_Lv2_C")==0)
        {
            *name="二级头";
            *color=armor_color;
            return true;
        }
        if(strcmp(gname,"PickUp_BP_Armor_Lv2_C")==0)
        {
            *name="二级甲";
            *color=armor_color;
            return true;
        }
        if(strcmp(gname,"PickUp_BP_Bag_Lv2_C")==0)
        {
            *name="二级包";
            *color=armor_color;
            return true;
        }
    }  
    if (showChest){
        if (strstr(gname, "CharacterDeadInventoryBox_C") != 0)
        {
            *name="盒子";
            *color=armor_color;
            return true;           // 盒子
        }
        if (strstr(gname, "EscapeBox_MedicaI_Lv3_C") != 0)
        {
            *name="药箱";
            *color=armor_color;
            return true;           // 盒子
        }
        if (strstr(gname, "EscapeBoxHight_SupplyBox_Lv3_C"))
        {
         *name = "3级物资箱";
         *color=armor_color;
         return true;
        }
        if (strstr(gname, "EscapeBoxHight_Weapon_Lv4_C"))
        {
         *name = "4级物资箱";
         return true;
        }
        if (strstr(gname, "EscapeBoxHight_SupplyBox_Lv4_C"))
        {
         *name = "4级物资箱";
         return true;
        }
        
        if (strstr(gname, "EscapeBoxHight_Medical_Lv3_C"))
        {
         *name = "3级物资箱";
         return true;
        }
        if (strstr(gname, "EscapeBoxHight_Medical_Lv2_C"))
        {
         *name = "3级物资箱";
         return true;
        }
        
        if (strstr(gname, "EscapeBoxHight_Weapon_Lv2_C"))
        {
         *name = "2级物资箱";
         return true;
        }
        if (strstr(gname, "EscapeBoxHight_SupplyBox_Lv1_C"))
        {
         *name = "1级物资箱";
         return true;
        }
        
        if (strstr(gname, "EscapeBoxHight_SupplyBox_Lv2_C"))
        {
         *name = "2级物资箱";
         return true;
        }
        
        if (strstr(gname, "EscapeBoxHight_Medical_Lv1_C"))
        {
         *name = "1级物资箱";
         return true;
        }
        
        if (strstr(gname, "EscapeBoxHight_Weapon_Lv1_C"))
        {
         *name = "1级物资箱";
         return true;
        }
    }
    return false;
}
// 手雷预警
bool GetGrenadeInfo(char* gname,char** name,ImVec4* color)
{
    if(strcmp(gname,"ProjGrenade_BP_C")==0)//手雷
    {
        *name="警告!小心手雷!";
        *color=ImVec4{255.f/255.f,0.f/255.f,0.f/255.f,255.f/255.f};
        return true;
    }
    if(strcmp(gname,"ProjSmoke_BP_C")==0)
    {
        *name="警告!小心烟霧彈!";
        *color=ImVec4{255.f/255.f,0.f/255.f,0.f/255.f,255.f/255.f};
        return true;
    }
    if(strcmp(gname,"ProjBurn_BP_C")==0)
    {
        *name="警告!小心燃燒瓶!";
        *color=ImVec4{255.f/255.f,0.f/255.f,0.f/255.f,255.f/255.f};
        return true;
    }
    if(strcmp(gname,"BP_Grenade_Pan_C")==0)
    {
        *name="警告!小心平底鍋!";
        *color=ImVec4{255.f/255.f,0.f/255.f,0.f/255.f,255.f/255.f};
        return true;
    }
    return false;
}
// 获取载具
bool GetVehicleInfo(char* gname,char** name,ImVec4* color)
{
    if(strcmp(gname,"VH_BRDM_C")==0)
    {
        *name="装甲车";
        *color=vehicle_color;
        return true;
    }
    if(strcmp(gname,"VH_Scooter_C")==0)
    {
        *name="小绵羊";
        *color=vehicle_color;
        return true;
    }
    if(strcmp(gname,"VH_Motorcycle_C")==0||strcmp(gname,"VH_Motorcycle_1_C")==0)
    {
        *name="摩托车";
        *color=vehicle_color;
        return true;
    }
    if(strcmp(gname,"VH_MotorcycleCart_1_C")==0||strcmp(gname,"VH_MotorcycleCart_C")==0)
    {
        *name="三轮摩托";
        *color=vehicle_color;
        return true;
    }
    if(strcmp(gname,"VH_Snowmobile_C")==0)
    {
        *name="雪地摩托";
        *color=vehicle_color;
        return true;
    }
    if(strcmp(gname,"BP_VH_Tuk_C")==0||strcmp(gname,"BP_VH_Tuk_1_C")==0)
    {
        *name="三轮摩托";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Buggy")!= nullptr)
    {
        *name="蹦蹦车";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Dacia")!= nullptr)
    {
        *name="轿车";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"UAZ")!= nullptr)
    {
        *name="吉普车";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"_PickUp")!= nullptr)
    {
        *name="皮卡车";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Rony")!= nullptr)
    {
        *name="皮卡车";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Mirado")!= nullptr)
    {
        *name="跑车";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"MiniBus")!= nullptr)
    {
        *name="迷你巴士";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"PG117")!= nullptr)
    {
        *name="快艇";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"AquaRail")!= nullptr)
    {
        *name="摩托艇";
        *color=vehicle_color;
        return true;
    }
    return false;
}

