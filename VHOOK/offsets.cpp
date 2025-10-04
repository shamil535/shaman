#include "offsets.h"
#include "loguer.h"

Offsets* Offsets::instance = 0;
Offsets* Offsets::get_instance()
{
    if (!instance)
        instance = new Offsets();
    return instance;
}

void Offsets::SET(std::string Version) {

    Version.erase(0, 1);
    int VersionNumber = std::stoi(Version);

    switch (VersionNumber) {

    case 1604:
        world = 0x247F840;
        viewPort = 0x2087780;
        replayInterface = 0x1EFD4C8;
        Camera = 0x2088360;

        visibleFlag = 0x142C;
        health = 0x280;
        maxhealth = 0x02A0;
        bone_offset = 0x1400;

        weapon_manager = 0x10D8;
        armor_offset = 0x14B8;
        vehicle_offset = 0xD28;
        vehicle_lock_offset = 0x1350;
        waypoint_offset = 0x2093320;
        ammo_offset = 0x1C2E81C;
        body_health = 0x820;
        tank_health = 0x824;
        driver_offset = 0xC28;

        net_id_offset = 0x88;
        freeze_offset = 0x218;
        playerinfo_offset = 0x10C8;
        c_weapon_offset = 0x340;
        handling_offset = 0x918;
        run_speed_offset = 0xCF0;
        frame_flags_offset = 0x1F8;
        recoil_offset = 0x2F8;
        spread_offset = 0x74;
        range_offset = 0x25C;
        task_offset = 0x146B;
        reload_multiplier_offset = 0x12C;

        break;

    case 2060:
        world = 0x24C8858;
        viewPort = 0x1F6A7E0;
        replayInterface = 0x1EC3828;
        Camera = 0x1F6B3C0;

        visibleFlag = 0x142C;
        health = 0x280;
        maxhealth = 0x02A0;
        bone_offset = 0x1400;

        weapon_manager = 0x10D8;
        armor_offset = 0x14E0;
        vehicle_offset = 0xD28;
        vehicle_lock_offset = 0x1390;
        waypoint_offset = 0x1F76370;
        ammo_offset = 0x1C72E04;
        body_health = 0x840;
        tank_health = 0x844;
        driver_offset = 0xC68;

        net_id_offset = 0x88;
        freeze_offset = 0x218;
        playerinfo_offset = 0x10C8;
        c_weapon_offset = 0x340;
        handling_offset = 0x908;
        run_speed_offset = 0xCF0;
        frame_flags_offset = 0x218;
        recoil_offset = 0x2F4;
        spread_offset = 0x84;
        range_offset = 0x28C;
        task_offset = 0x146B;
        reload_multiplier_offset = 0x134;

        break;

    case 2189:
        world = 0x24E6D90;
        viewPort = 0x1F888C0;
        replayInterface = 0x1EE18A8;
        Camera = 0x1F89498;

        visibleFlag = 0x142C;
        health = 0x280;
        maxhealth = 0x02A0;
        bone_offset = 0x1400;

        weapon_manager = 0x10D8;
        armor_offset = 0x14E0;
        vehicle_offset = 0xD30;
        vehicle_lock_offset = 0x1390;
        waypoint_offset = 0x1F94460;
        ammo_offset = 0x1C8F364;
        body_health = 0x840;
        tank_health = 0x844;
        driver_offset = 0xC68;

        net_id_offset = 0x88;
        freeze_offset = 0x218;
        playerinfo_offset = 0x10C8;
        c_weapon_offset = 0x340;
        handling_offset = 0x938;
        run_speed_offset = 0xCF0;
        frame_flags_offset = 0x218;
        recoil_offset = 0x2F4;
        spread_offset = 0x84;
        range_offset = 0x28C;
        task_offset = 0x146B;
        reload_multiplier_offset = 0x134;

        break;

    case 2372:
        world = 0x252DCD8;
        viewPort = 0x1F9E9F0;
        replayInterface = 0x1F05208;
        Camera = 0x1F9E9F0;

        visibleFlag = 0x142C;
        health = 0x280;
        maxhealth = 0x02A0;
        bone_offset = 0x1400;

        weapon_manager = 0x10D8;
        armor_offset = 0x14E0;
        vehicle_offset = 0xD30;
        vehicle_lock_offset = 0x1390;
        waypoint_offset = 0x1FBF2B0;
        body_health = 0x840;
        tank_health = 0x844;
        ammo_offset = 0x1CD8F04;
        driver_offset = 0xC68;

        net_id_offset = 0x88;
        freeze_offset = 0x218;
        playerinfo_offset = 0x10C8;
        c_weapon_offset = 0x340;
        handling_offset = 0x938;
        run_speed_offset = 0xCF0;
        frame_flags_offset = 0x218;
        recoil_offset = 0x2F4;
        spread_offset = 0x84;
        range_offset = 0x28C;
        task_offset = 0x146B;
        reload_multiplier_offset = 0x134;

        break;

    case 2545:
        world = 0x25667E8;
        viewPort = 0x1FD6F70;
        replayInterface = 0x1F2E7A8;
        Camera = 0x1FD7B48;

        visibleFlag = 0x142C;
        health = 0x280;
        maxhealth = 0x02A0;
        bone_offset = 0x1450;

        weapon_manager = 0x10D8;
        armor_offset = 0x1530;
        vehicle_offset = 0xD30;
        vehicle_lock_offset = 0x1390;
        waypoint_offset = 0x1FF7A10;
        ammo_offset = 0x1D00E04;
        body_health = 0x840;
        tank_health = 0x844;
        driver_offset = 0xC68;

        net_id_offset = 0x88;
        freeze_offset = 0x238;
        playerinfo_offset = 0x10C8;
        c_weapon_offset = 0x340;
        handling_offset = 0x938;
        run_speed_offset = 0xCF0;
        frame_flags_offset = 0x218;
        recoil_offset = 0x2F4;
        spread_offset = 0x84;
        range_offset = 0x28C;
        task_offset = 0x146B;
        reload_multiplier_offset = 0x134;

        break;

    case 2612:
        world = 0x2567DB0;
        viewPort = 0x1FD8570;
        replayInterface = 0x1F77EF0;
        Camera = 0x1FD9148;

        visibleFlag = 0x142C;
        health = 0x280;
        maxhealth = 0x02A0;
        bone_offset = 0x1450;

        weapon_manager = 0x10D8;
        armor_offset = 0x1530;
        vehicle_offset = 0xD30;
        vehicle_lock_offset = 0x1390;
        waypoint_offset = 0x1FF9010;
        ammo_offset = 0x1D01E34;
        body_health = 0x840;
        tank_health = 0x844;
        driver_offset = 0xCF0;

        net_id_offset = 0x88;
        freeze_offset = 0x238;
        playerinfo_offset = 0x10C8;
        c_weapon_offset = 0x340;
        handling_offset = 0x938;
        run_speed_offset = 0xCF0;
        frame_flags_offset = 0x218;
        recoil_offset = 0x2F4;
        spread_offset = 0x84;
        range_offset = 0x28C;
        task_offset = 0x146B;
        reload_multiplier_offset = 0x134;

        break;

    case 2699:
        world = 0x26684D8;
        viewPort = 0x20D8C90;
        replayInterface = 0x20304C8;
        Camera = 0x20D9868;

        visibleFlag = 0x147C;
        health = 0x280;
        maxhealth = 0x02A0;
        bone_offset = 0x1450;

        weapon_manager = 0x10D8;
        armor_offset = 0x1530;
        vehicle_offset = 0xD30;
        vehicle_lock_offset = 0x1390;
        waypoint_offset = 0x20F9750;
        ammo_offset = 0x1E01EC4;
        body_health = 0x840;
        tank_health = 0x844;
        driver_offset = 0xC68;

        net_id_offset = 0x88;
        freeze_offset = 0x238;
        playerinfo_offset = 0x10C8;
        c_weapon_offset = 0x340;
        handling_offset = 0x938;
        run_speed_offset = 0xCF0;
        frame_flags_offset = 0x218;
        recoil_offset = 0x2F4;
        spread_offset = 0x84;
        range_offset = 0x28C;
        task_offset = 0x146B;
        reload_multiplier_offset = 0x134;

        break;

    case 2802:
        world = 0x254D448;
        viewPort = 0x1FBC100;
        replayInterface = 0x1F5B820;
        Camera = 0x1FBCCD8;

        visibleFlag = 0x145C;
        health = 0x280;
        maxhealth = 0x02A0;
        bone_offset = 0x1430;

        weapon_manager = 0x10B8;
        armor_offset = 0x150C;
        vehicle_offset = 0xD10;
        vehicle_lock_offset = 0x1370;
        waypoint_offset = 0x1FDD510;
        ammo_offset = 0x1D2EAE4;
        body_health = 0x820;
        tank_health = 0x824;
        driver_offset = 0xC48;

        net_id_offset = 0xE8;
        freeze_offset = 0x238;
        playerinfo_offset = 0x10A8;
        c_weapon_offset = 0x320;
        handling_offset = 0x918;
        run_speed_offset = 0xCF0;
        frame_flags_offset = 0x270;
        recoil_offset = 0x2F4;
        spread_offset = 0x84;
        range_offset = 0x28C;
        task_offset = 0x144B;
        reload_multiplier_offset = 0x134;

        break;

    case 2944:
        world = 0x257BEA0;
        viewPort = 0x1FEAAC0;
        replayInterface = 0x1F42068;
        Camera = 0x1FEB698;

        visibleFlag = 0x145C;
        health = 0x280;
        maxhealth = 0x284;
        bone_offset = 0x1430;

        weapon_manager = 0x10B8;
        armor_offset = 0x150C;
        vehicle_offset = 0xD10;
        waypoint_offset = 0x200BEB0;
        vehicle_lock_offset = 0x1370;
        ammo_offset = 0x1D59BD4;
        body_health = 0x820;
        tank_health = 0x824;
        driver_offset = 0xC48;

        net_id_offset = 0xE8;
        freeze_offset = 0x238;
        playerinfo_offset = 0x10A8;
        c_weapon_offset = 0x320;
        handling_offset = 0x918;
        run_speed_offset = 0xD40;
        frame_flags_offset = 0x270;
        recoil_offset = 0x2F4;
        spread_offset = 0x84;
        range_offset = 0x28C;
        task_offset = 0x144B;
        reload_multiplier_offset = 0x134;

        break;

    case 3095:
        world = 0x2593320;
        viewPort = 0x20019E0;
        replayInterface = 0x1F58B58;
        Camera = 0x20025B8;

        visibleFlag = 0x145C;
        health = 0x280;
        maxhealth = 0x284;
        bone_offset = 0x1430;

        weapon_manager = 0x10B8;
        armor_offset = 0x150C;
        vehicle_offset = 0xD10;
        vehicle_lock_offset = 0x1370;
        globals_offset = 0x161E258;
        waypoint_offset = 0x2022DE0;
        ammo_offset = 0x1D701B4;
        body_health = 0x820;
        tank_health = 0x824;
        driver_offset = 0xC98;

        net_id_offset = 0xE8;
        freeze_offset = 0x238;
        playerinfo_offset = 0x10A8;
        c_weapon_offset = 0x320;
        handling_offset = 0x960;
        run_speed_offset = 0xCF0;
        frame_flags_offset = 0x270;
        recoil_offset = 0x2F4;
        spread_offset = 0x84;
        range_offset = 0x28C;
        task_offset = 0x144B;
        reload_multiplier_offset = 0x134;

        break;

    case 3258:
        world = 0x25B14B0;
        viewPort = 0x201DBA0;
        replayInterface = 0x1FBD4F0;
        Camera = 0x201E7D0;

        visibleFlag = 0x145C;
        health = 0x280;
        maxhealth = 0x284;
        bone_offset = 0x1430;

        weapon_manager = 0x10B8;
        armor_offset = 0x150C;
        vehicle_offset = 0xD10;
        vehicle_lock_offset = 0x1370;
        globals_offset = 0x161E258;
        waypoint_offset = 0x2022DE0;
        ammo_offset = 0x1D701B4;
        body_health = 0x820;
        tank_health = 0x824;
        driver_offset = 0xC98;

        net_id_offset = 0xE8;
        freeze_offset = 0x238;
        playerinfo_offset = 0x10A8;
        c_weapon_offset = 0x320;
        handling_offset = 0x960;
        run_speed_offset = 0xCF0;
        frame_flags_offset = 0x270;
        recoil_offset = 0x2F4;
        spread_offset = 0x84;
        range_offset = 0x28C;
        task_offset = 0x144B;
        reload_multiplier_offset = 0x134;

        break;

    case 3274:
        world = 0x25B24D8;
        viewPort = 0x201EC20;
        replayInterface = 0x1F76428;
        Camera = 0x201F848;

        visibleFlag = 0x145C;
        health = 0x280;
        maxhealth = 0x284;
        bone_offset = 0x1430;

        weapon_manager = 0x10B8;
        armor_offset = 0x150C;
        vehicle_offset = 0xD10;
        vehicle_lock_offset = 0x1370;
        globals_offset = 0x161E258;
        waypoint_offset = 0x2022DE0;
        ammo_offset = 0x1D701B4;
        body_health = 0x820;
        tank_health = 0x824;
        driver_offset = 0xC98;

        net_id_offset = 0xE8;
        freeze_offset = 0x238;
        playerinfo_offset = 0x10A8;
        c_weapon_offset = 0x320;
        handling_offset = 0x960;
        run_speed_offset = 0xCF0;
        frame_flags_offset = 0x270;
        recoil_offset = 0x2F4;
        spread_offset = 0x84;
        range_offset = 0x28C;
        task_offset = 0x144B;
        reload_multiplier_offset = 0x134;

        break;

    default:
        std::string error = ("This version (") + Version + (") isn't supported, call our support to update.");
        MessageBoxA(NULL, error.c_str(), ("ERROR"), MB_ICONERROR | MB_OK);
        logger::ExtLog(("GTAV-log.txt"), error);
        Sleep(1000);exit(0);
    }
    
}