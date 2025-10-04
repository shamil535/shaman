#include "includes.h"
#include "vector.h"
#include "offsets.h"
#include "driver.h"
#include "core.h"
#include "process.h"
#include "math.h"
#include "overlay.h"

core* core::instance = nullptr;

core* core::get_instance()
{
	if (!core::instance)
		core::instance = new core();
	return core::instance;
}

uintptr_t core::getUWorld()
{
	if (this->cache.uWorld)
		return this->cache.uWorld;
	uintptr_t world;

#ifdef GAME_NAME
	DWORD64 TempWorldPTR = ctx->FindSignature(process::get_instance()->getProcBase(), 536870912, Signatures::SignatureWorldPTR, Signatures::MaskWorldPTR);
	if (TempWorldPTR != NULL) {
		world = TempWorldPTR + ctx->read<int>(TempWorldPTR + 3) + 7;
	}
#else
	world = process::get_instance()->getProcBase() + Offsets::get_instance()->world;
#endif

#ifdef _DEBUG
	printf("world:0x%I64X		offset:0x%I64X\n", world, world - process::get_instance()->getProcBase());
#endif
	this->cache.uWorld = ctx->read<uintptr_t>(world);

	return this->cache.uWorld;
}


void core::UpdateLocalInfos()
{
	auto proc = process::get_instance();

	auto World = this->getUWorld();
	if (!World) return;

#ifdef GAME_NAME
	if (!this->cache.viewPort) {
		DWORD64 viewPortTemp = ctx->FindSignature(proc->getProcBase(), 536870912, Signatures::viewPortPTR, Signatures::MaskviewPortPTR);
		this->cache.viewPort = viewPortTemp + ctx->read<int>(viewPortTemp + 3) + 7;
	}

	if (!this->cache.replayInterface) {
		DWORD64 replayInterfaceTemp = ctx->FindSignature(proc->getProcBase(), 536870912, Signatures::replayInterfacePTR, Signatures::MaskreplayInterfacePTR);
		this->cache.replayInterface = replayInterfaceTemp + ctx->read<int>(replayInterfaceTemp + 3) + 7;
	}

	if (!this->cache.Camera) {
		DWORD64 CameraTemp = ctx->FindSignature(proc->getProcBase(), 536870912, Signatures::CameraPTR, Signatures::MaskCameraPTR);
		this->cache.Camera = CameraTemp + ctx->read<int>(CameraTemp + 3) + 7;
	}


	/*if (!this->cache.camGameplayDirectorPointer) {
		this->cache.camGameplayDirectorPointer = ctx->FindSignature(proc->getProcBase(), 536870912, offsets::camGameplayDirectorPTR, offsets::MaskcamGameplayDirectorPTR);
	}
	DWORD64 TempBlipPTR = ctx->FindSignature(procBase, 536870912, offsets::SignatureBlipPTR, offsets::MaskBlipPTR);
	auto blipPtr = TempBlipPTR + ctx->read<int>(TempBlipPTR + 3) + 7;
	printf("blipPtr : 0x%I64X\n", blipPtr - procBase);*/
#else
	if (!this->cache.viewPort) {
		this->cache.viewPort = proc->getProcBase() + Offsets::get_instance()->viewPort;
	}

	if (!this->cache.replayInterface) {
		this->cache.replayInterface = proc->getProcBase() + Offsets::get_instance()->replayInterface;
	}

	if (!this->cache.Camera) {
		this->cache.Camera = proc->getProcBase() + Offsets::get_instance()->Camera;
	}
#endif

	this->cache.local.LocalPlayerPTR = ctx->read<UINT64>(World + 0x8);
	this->cache.local.LocalPlayerL = ctx->read<uintptr_t>(this->cache.local.LocalPlayerPTR + 0x30);

#ifdef DUMP
	static uintptr_t tmplocalPlayer = 0;

	if (tmplocalPlayer != this->cache.local.LocalPlayerPTR)
	{
		print("viewPort:0x%I64X		offset:0x%I64X\n", this->cache.viewPort, this->cache.viewPort - proc->getProcBase());
		print("replayInterface:0x%I64X		offset:0x%I64X\n", this->cache.replayInterface, this->cache.replayInterface - proc->getProcBase());
		print("Camera:0x%I64X		offset:0x%I64X\n", this->cache.Camera, this->cache.Camera - proc->getProcBase());
		print("LocalPlayer:0x%llx\n", this->cache.local.LocalPlayerPTR);

		tmplocalPlayer = this->cache.local.LocalPlayerPTR;

	}
#endif

}

void core::UpdateActors()
{
	auto proc = process::get_instance();

	auto World = this->getUWorld();
	if (!World) return;

#ifdef _DUMPLOOP
	print("\n\033[2J\033[H\n");
	print("\nWorld: 0x%llx\n\n", this->getUWorld());
	print("LocalPlayer: 0x%llx\n", this->cache.local.LocalPlayerPTR);
	print("ReplayInterface: 0x%llx\n", ctx->read<uint64_t>(this->cache.replayInterface));
	print("ViewPort : 0x%llx\n", ctx->read<uint64_t>(this->cache.viewPort));
	print("Camera:0x%I64X \n\n", ctx->read<uint64_t>(this->cache.Camera));
#endif

	//if (!g_settings->esp.enable) continue;
	this->cache.esp_num_vehicle = RPM<int>(this->cache.replayInterface, { 0x10, 0x190 });
	this->cache.esp_num_ped = RPM<int>(this->cache.replayInterface, { 0x18, 0x110 });
	this->cache.esp_num_pickup = RPM<int>(this->cache.replayInterface, { 0x20, 0x110 });
	this->cache.esp_num_object = RPM<int>(this->cache.replayInterface, { 0x28, 0x168 });

	Vector3 LocalPos = ctx->read<Vector3>(this->cache.local.LocalPlayerL + 0x50);

	if (/*g_settings->esp.vehicle.enable*/ true)
	{
		uintptr_t face = RPM<uintptr_t>(this->cache.replayInterface, { 0x10 });
		uintptr_t list = RPM<uintptr_t>(face + 0x180);
		for (int i = 0; i < sizeof(esp_vehicle.item) / sizeof(esp_vehicle.item[0]); i++)
		{
			esp_vehicle.item[i].instance = RPM<uintptr_t>(list + 0x10 * i);
			esp_vehicle.item[i].world_coords = get_instance_world_coords(esp_vehicle.item[i].instance);
			if (/*g_settings->esp.vehicle.text*/ true)
			{
				esp_vehicle.item[i].text.dist = esp_vehicle.item[i].world_coords.Distance(LocalPos);
				esp_vehicle.item[i].text.hash = get_instance_hash(esp_vehicle.item[i].instance);

				get_vehicle_name_1(esp_vehicle.item[i].instance, esp_vehicle.item[i].text.name_1);
				get_vehicle_name_2(esp_vehicle.item[i].instance, esp_vehicle.item[i].text.name_2);
			}
		}
	}
	if (/*g_settings->esp.ped.enable*/ true)
	{
		uintptr_t face = RPM<uintptr_t>(this->cache.replayInterface, { 0x18 });
		uintptr_t list = RPM<uintptr_t>(face + 0x100);
		for (int i = 0; i < sizeof(esp_ped.item) / sizeof(esp_ped.item[0]); i++)
		{
			esp_ped.item[i].instance = RPM<uintptr_t>(list + 0x10 * i);
			if (/*g_settings->esp.ped.text*/ true)
			{
				esp_ped.item[i].text.dist = esp_ped.item[i].world_coords.Distance(LocalPos);
				esp_ped.item[i].text.hash = get_instance_hash(esp_ped.item[i].instance);
				esp_ped.item[i].text.ped_type = get_ped_ped_type(esp_ped.item[i].instance);
			}
		}
	}
	if (/*g_settings->esp.pickup.enable*/ true)
	{
		uintptr_t face = RPM<uintptr_t>(this->cache.replayInterface, { 0x20 });
		uintptr_t list = RPM<uintptr_t>(face + 0x100);
		for (int i = 0; i < sizeof(esp_pickup.item) / sizeof(esp_pickup.item[0]); i++)
		{
			esp_pickup.item[i].instance = RPM<uintptr_t>(list + 0x10 * i);
			esp_pickup.item[i].world_coords = get_instance_world_coords(esp_pickup.item[i].instance);
			if (/*g_settings->esp.pickup.text*/ true)
			{
				esp_pickup.item[i].text.dist = esp_pickup.item[i].world_coords.Distance(LocalPos);
				esp_pickup.item[i].text.hash = get_instance_hash(esp_pickup.item[i].instance);
			}
		}
	}
	if (/*g_settings->esp.object.enable*/ true)
	{
		uintptr_t face = RPM<uintptr_t>(this->cache.replayInterface, { 0x28 });
		uintptr_t list = RPM<uintptr_t>(face + 0x158);
		for (int i = 0; i < sizeof(esp_object.item) / sizeof(esp_object.item[0]); i++)
		{
			esp_object.item[i].instance = RPM<uintptr_t>(list + 0x10 * i);
			esp_object.item[i].world_coords = get_instance_world_coords(esp_object.item[i].instance);
			if (/*g_settings->esp.object.text*/ true)
			{
				esp_object.item[i].text.dist = esp_object.item[i].world_coords.Distance(LocalPos);
				esp_object.item[i].text.hash = get_instance_hash(esp_object.item[i].instance);
			}
		}
	}

}

void core::UpdateSkeleton() {
	while (true) {

		Sleep(1);

		if (!CFG.RageMP.b_EspHealthPlayers && !CFG.RageMP.b_EspSkeletonPlayers) {
			Sleep(500);
			continue;
		}

		skeletonMutex.lock();

		for (int i = 0; i < sizeof(esp_ped.item) / sizeof(esp_ped.item[0]); i++) {
			if (esp_ped.item[i].instance && esp_ped.item[i].instance != this->getCache().local.LocalPlayerPTR) {
				try {
					auto proc = process::get_instance();
					auto ctx = zenith::communicate_t::inst();
					auto core = core::get_instance();

					if (CFG.RageMP.b_EspHeadPlayers) {
						esp_ped.item[i].HeadPos = core->GetBonePosition(esp_ped.item[i].instance, BONETAG_HEAD);
						uintptr_t navigation = ctx->read<uintptr_t>(esp_ped.item[i].instance + 0x30);
						esp_ped.item[i].rotation = ctx->read<Rotation>(navigation + 0x30);
					}

					if (CFG.RageMP.b_EspHealthPlayers) {
						esp_ped.item[i].SkeletonPHealth.health = get_ped_health(esp_ped.item[i].instance);
						esp_ped.item[i].SkeletonPHealth.maxhealth = get_ped_maxhealth(esp_ped.item[i].instance);
					}

					if (CFG.RageMP.b_EspSkeletonPlayers) {
						std::vector<BoneData> bone_data(std::size(bone_pairs));

						auto fragInst = ctx->read<uintptr_t>(esp_ped.item[i].instance + Offsets::get_instance()->bone_offset);
						auto ptr = ctx->read<uintptr_t>(fragInst + 0x68);
						auto m_pSkeleton = ctx->read<uintptr_t>(ptr + 0x178);

						size_t total_size = sizeof(uintptr_t) * 3 + sizeof(Mat34V) * std::size(bone_pairs) * 2;
						std::vector<uint8_t> buffer(total_size);

						ctx->read_virtual(m_pSkeleton, buffer.data(), total_size);

						esp_ped.item[i].SkeletonPHealth.bone_positions.clear();
						esp_ped.item[i].SkeletonPHealth.bone_positions.reserve(std::size(bone_pairs));

						for (size_t j = 0; j < std::size(bone_pairs); ++j) {
							auto& data = bone_data[j];

							data.boneIndexStart = *reinterpret_cast<int*>(buffer.data() + j * sizeof(int) * 2);
							data.boneIndexEnd = *reinterpret_cast<int*>(buffer.data() + j * sizeof(int) * 2 + sizeof(int));

							this->crSkeletonData_ConvertBoneIdToIndex(*reinterpret_cast<uintptr_t*>(buffer.data()), bone_pairs[j].first, &data.boneIndexStart);
							this->crSkeletonData_ConvertBoneIdToIndex(*reinterpret_cast<uintptr_t*>(buffer.data()), bone_pairs[j].second, &data.boneIndexEnd);

							this->crSkeleton_GetGlobalMtx(m_pSkeleton, data.boneIndexStart, &data.matrixStart);
							this->crSkeleton_GetGlobalMtx(m_pSkeleton, data.boneIndexEnd, &data.matrixEnd);

							Vector3 bone_start_world = { data.matrixStart.col3.m128_f32[0], data.matrixStart.col3.m128_f32[1], data.matrixStart.col3.m128_f32[2] };
							Vector3 bone_end_world = { data.matrixEnd.col3.m128_f32[0], data.matrixEnd.col3.m128_f32[1], data.matrixEnd.col3.m128_f32[2] };

							esp_ped.item[i].SkeletonPHealth.bone_positions.push_back({ bone_start_world, bone_end_world });
						}
					}
				}
				catch (const std::exception& e) {
#ifdef __VERBOSE
					print("Exception: %s\n", e.what());
#endif
				}
				catch (...) {
#ifdef __VERBOSE
					print("Unknown exception occurred\n");
#endif
				}
			}
		}

		skeletonMutex.unlock();
	}
}



bool core::W2S(vec3_t pos, vec3_t& screen, float matrix[16], int windowWidth, int windowHeight)
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	vec4 clipCoords;
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	vec3_t NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	//Transform to window coordinates
	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}

bool core::crSkeletonData_ConvertBoneIdToIndex(__int64 skeletonData, unsigned __int16 boneMask, int* outBoneIdx)
{
	auto ctx = zenith::communicate_t::inst();

	if (!outBoneIdx) {
		return false;
	}

	struct SkeletonData {
		uintptr_t offset16;
		uint16_t skeletonData24;
		uint16_t skeletonData26;
		uint16_t skeletonData94;
	} data;

	ctx->read_virtual(skeletonData + 16, &data, sizeof(SkeletonData));

	if (data.skeletonData26 && data.skeletonData24) {
		uintptr_t basePtr = data.offset16 + 8 * (boneMask % data.skeletonData24);
		uintptr_t base = 0;

		ctx->read_virtual(basePtr, &base, sizeof(uintptr_t));

		struct BoneEntry {
			int boneMask;
			int boneIndex;
			uintptr_t nextPtr;
		};
		BoneEntry boneEntry;

		uintptr_t current = base;
		int iteration = 0;  // Limite de iterações
		const int max_iterations = 1000;  // Definindo um limite para evitar loop infinito

		while (current && iteration < max_iterations) {
			ctx->read_virtual(current, &boneEntry, sizeof(BoneEntry));

			if (boneEntry.boneMask == boneMask) {
				*outBoneIdx = boneEntry.boneIndex;
				return true;
			}

			current = boneEntry.nextPtr;
			iteration++;
		}

		if (iteration >= max_iterations) {
		}
	}
	else if (boneMask < data.skeletonData94) {
		*outBoneIdx = boneMask;
		return true;
	}

	return false;
}


void core::crSkeleton_GetGlobalMtx(__int64 m_pSkeleton, unsigned int boneIdx, Mat34V* outMtx)
{
	auto ctx = zenith::communicate_t::inst();

	struct ptrData {
		uintptr_t ptr;
		uintptr_t pad;
		uintptr_t result;
	};ptrData data;

	ctx->read_virtual(m_pSkeleton + 0x8, &data, sizeof(data));

	if (data.ptr)
	{
		uintptr_t result = data.result + ((unsigned __int64)boneIdx << 0x6);
		if (result)
		{
			Mat34V m1 = ctx->read<Mat34V>(data.ptr);
			Mat34Vi m2 = ctx->read<Mat34Vi>(result);
			outMtx->col0 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(m1.col1, _mm_castsi128_ps(_mm_shuffle_epi32(m2.col0, 85))), _mm_mul_ps(m1.col0, _mm_castsi128_ps(_mm_shuffle_epi32(m2.col0, 0)))), _mm_mul_ps(m1.col2, _mm_castsi128_ps(_mm_shuffle_epi32(m2.col0, 170))));
			outMtx->col1 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(m1.col1, _mm_castsi128_ps(_mm_shuffle_epi32(m2.col1, 85))), _mm_mul_ps(m1.col0, _mm_castsi128_ps(_mm_shuffle_epi32(m2.col1, 0)))), _mm_mul_ps(m1.col2, _mm_castsi128_ps(_mm_shuffle_epi32(m2.col1, 170))));
			outMtx->col3 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(m1.col0, _mm_castsi128_ps(_mm_shuffle_epi32(m2.col3, 0))), m1.col3), _mm_mul_ps(m1.col1, _mm_castsi128_ps(_mm_shuffle_epi32(m2.col3, 85)))), _mm_mul_ps(m1.col2, _mm_castsi128_ps(_mm_shuffle_epi32(m2.col3, 170))));
			outMtx->col2 = _mm_add_ps(_mm_add_ps(_mm_mul_ps(m1.col1, _mm_castsi128_ps(_mm_shuffle_epi32(m2.col2, 85))), _mm_mul_ps(m1.col0, _mm_castsi128_ps(_mm_shuffle_epi32(m2.col2, 0)))), _mm_mul_ps(m1.col2, _mm_castsi128_ps(_mm_shuffle_epi32(m2.col2, 170))));
		}
	}
}


Vector3 core::GetBonePosition(uintptr_t entity, eAnimBoneTag wMask)
{
	auto ctx = zenith::communicate_t::inst();

	struct BuffRead {
		uintptr_t ptr1;
		char __pad[0x8];
		uintptr_t ptr2;
	};BuffRead Buffer;

	uintptr_t fragInst = ctx->read<uintptr_t>(entity + Offsets::get_instance()->bone_offset);
	ctx->read_virtual(fragInst + 0x68, &Buffer, sizeof(Buffer));
	if (Buffer.ptr1 && Buffer.ptr2)
	{
		auto m_pSkeleton = ctx->read<uintptr_t>(Buffer.ptr1 + 0x178);
		if (m_pSkeleton)
		{
			int boneIndex = 0;
			this->crSkeletonData_ConvertBoneIdToIndex(ctx->read<uintptr_t>(m_pSkeleton), wMask, &boneIndex);
			if (boneIndex == -1)
				return *(Vector3*)(entity + 0x90);

			Mat34V matrix{};
			this->crSkeleton_GetGlobalMtx(m_pSkeleton, boneIndex, &matrix);
			return { matrix.col3.m128_f32[0], matrix.col3.m128_f32[1], matrix.col3.m128_f32[2] };
		}
	}

	return {};
}

