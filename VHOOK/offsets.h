#pragma once
#include "includes.h"
#include "vector.h"

namespace Signatures
{
	static LPCSTR SignatureWorldPTR = "\x48\x8b\x05\x00\x00\x00\x00\x45\x00\x00\x00\x00\x48\x8b\x48\x08\x48\x85\xc9\x74\x07";
	static LPCSTR MaskWorldPTR = "xxx????x????xxxxxxxxx";

	static LPCSTR viewPortPTR = "\x48\x8B\x15\x00\x00\x00\x00\x48\x8D\x2D\x00\x00\x00\x00\x48\x8B\xCD";
	static LPCSTR MaskviewPortPTR = "xxx????xxx????xxx";

	static LPCSTR replayInterfacePTR = "\x48\x8D\x0D\x00\x00\x00\x00\x48\x8B\xD7\xE8\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\x8A\xD8\xE8\x00\x00\x00\x00\x84\xDB\x75\x13\x48\x8D\x0D\x00\x00\x00\x00";
	static LPCSTR MaskreplayInterfacePTR = "xxx????xxxx????xxx????xxx????xxxxxxx????";

	static LPCSTR CameraPTR = "\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\x98\x00\x00\x00\x00\xEB";
	static LPCSTR MaskCameraPTR = "xxx????xxx????x";

	/*static LPCSTR replayInterfacePTR = "\x48\x8D\x0D\x00\x00\x00\x00\x89\x44\x24\x30\xE8\x00\x00\x00\x00\x48\x83\xC4\x28\xC3\x48\x8B\x05";
	static LPCSTR MaskreplayInterfacePTR = "xxx????xxxxx????xxxxxxxx";

	static LPCSTR SignatureBlipPTR = "\x48\x8B\x05\x00\x00\x00\x00\x38\x98\x00\x00\x00\x00\x8A\xC3";
	static LPCSTR MaskBlipPTR = "xxx????xx????xx";

	static LPCSTR SignaturePlayerPTR = "\x48\x8b\x0d\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x48\x8b\xc8\xe8\x00\x00\x00\x00\x48\x8b\xcf";
	static LPCSTR MaskPlayerPTR = "xxx????x????xxxx????xxx";

	static LPCSTR SignatureGlobalPTR = "\x4c\x8d\x05\x00\x00\x00\x00\x4d\x8b\x08\x4d\x85\xc9\x74\x11";
	static LPCSTR MaskGlobalPTR = "xxx????xxxxxxxx";

	static LPCSTR camGameplayDirectorPTR = "\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\x98\x00\x00\x00\x00\xEB\x32";
	static LPCSTR MaskcamGameplayDirectorPTR = "xxx????xxx????xx";*/

}

class Offsets {
private:
	static Offsets* instance;
	static std::string Version;
public:
	uint64_t world;
	uint64_t viewPort;
	uint64_t replayInterface;
	uint64_t Camera;

	uint64_t visibleFlag;
	uint64_t health;
	uint64_t maxhealth;
	uint64_t bone_offset;

	uint64_t weapon_manager;
	uint64_t armor_offset;
	uint64_t vehicle_offset;
	uint64_t vehicle_lock_offset;
	uint64_t globals_offset;
	uint64_t waypoint_offset;
	uint64_t ammo_offset;
	uint64_t body_health;
	uint64_t tank_health;
	uint64_t driver_offset;

	uint64_t net_id_offset;
	uint64_t freeze_offset;
	uint64_t playerinfo_offset;
	uint64_t c_weapon_offset;
	uint64_t handling_offset;
	uint64_t run_speed_offset;
	uint64_t frame_flags_offset;
	uint64_t recoil_offset;
	uint64_t spread_offset;
	uint64_t range_offset;
	uint64_t task_offset;
	uint64_t reload_multiplier_offset;

	static Offsets* get_instance();

	void SET(std::string Version);
}; 



