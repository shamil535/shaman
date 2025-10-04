#pragma once

#include "includes.h"
#include "vector.h"
#include "config.h"
#include "offsets.h"

enum eAnimBoneTag : int
{
    BONETAG_NULL = -1,
    BONETAG_ROOT = 0,
    BONETAG_PELVIS = 11816,
    BONETAG_SPINE = 23553,
    BONETAG_SPINE1 = 24816,
    BONETAG_SPINE2 = 24817,
    BONETAG_SPINE3 = 24818,
    BONETAG_NECK = 39317,
    BONETAG_HEAD = 31086,
    BONETAG_R_CLAVICLE = 10706,
    BONETAG_R_UPPERARM = 40269,
    BONETAG_R_FOREARM = 28252,
    BONETAG_R_HAND = 57005,
    BONETAG_R_FINGER0 = 58866,
    BONETAG_R_FINGER01 = 64016,
    BONETAG_R_FINGER02 = 64017,
    BONETAG_R_FINGER1 = 58867,
    BONETAG_R_FINGER11 = 64096,
    BONETAG_R_FINGER12 = 64097,
    BONETAG_R_FINGER2 = 58868,
    BONETAG_R_FINGER21 = 64112,
    BONETAG_R_FINGER22 = 64113,
    BONETAG_R_FINGER3 = 58869,
    BONETAG_R_FINGER31 = 64064,
    BONETAG_R_FINGER32 = 64065,
    BONETAG_R_FINGER4 = 58870,
    BONETAG_R_FINGER41 = 64080,
    BONETAG_R_FINGER42 = 64081,
    BONETAG_L_CLAVICLE = 64729,
    BONETAG_L_UPPERARM = 45509,
    BONETAG_L_FOREARM = 61163,
    BONETAG_L_HAND = 18905,
    BONETAG_L_FINGER0 = 26610,
    BONETAG_L_FINGER01 = 4089,
    BONETAG_L_FINGER02 = 4090,
    BONETAG_L_FINGER1 = 26611,
    BONETAG_L_FINGER11 = 4169,
    BONETAG_L_FINGER12 = 4170,
    BONETAG_L_FINGER2 = 26612,
    BONETAG_L_FINGER21 = 4185,
    BONETAG_L_FINGER22 = 4186,
    BONETAG_L_FINGER3 = 26613,
    BONETAG_L_FINGER31 = 4137,
    BONETAG_L_FINGER32 = 4138,
    BONETAG_L_FINGER4 = 26614,
    BONETAG_L_FINGER41 = 4153,
    BONETAG_L_FINGER42 = 4154,
    BONETAG_L_THIGH = 58271,
    BONETAG_L_CALF = 63931,
    BONETAG_L_FOOT = 14201,
    BONETAG_L_TOE = 2108,
    BONETAG_R_THIGH = 51826,
    BONETAG_R_CALF = 36864,
    BONETAG_R_FOOT = 52301,
    BONETAG_R_TOE = 20781,
    BONETAG_PH_L_HAND = 60309,
    BONETAG_PH_R_HAND = 28422,
    MH_R_Elbow = 0xBB0,
    MH_L_Elbow = 0x58B7,
};

inline std::pair<eAnimBoneTag, eAnimBoneTag> bone_pairs[] = {
    { BONETAG_PELVIS, BONETAG_SPINE },
    { BONETAG_SPINE, BONETAG_SPINE1 },
    { BONETAG_SPINE1, BONETAG_SPINE2 },
    { BONETAG_SPINE2, BONETAG_SPINE3 },
    { BONETAG_SPINE3, BONETAG_NECK },
    { BONETAG_NECK, BONETAG_HEAD },

    { BONETAG_NECK, BONETAG_L_UPPERARM },
    { BONETAG_L_UPPERARM, BONETAG_L_FOREARM },
    { BONETAG_L_FOREARM, BONETAG_L_HAND },

    { BONETAG_NECK, BONETAG_R_UPPERARM },
    { BONETAG_R_UPPERARM, BONETAG_R_FOREARM },
    { BONETAG_R_FOREARM, BONETAG_R_HAND },

    { BONETAG_PELVIS, BONETAG_L_THIGH },
    { BONETAG_L_THIGH, BONETAG_L_CALF },
    { BONETAG_L_CALF, BONETAG_L_FOOT },

    { BONETAG_PELVIS, BONETAG_R_THIGH },
    { BONETAG_R_THIGH, BONETAG_R_CALF },
    { BONETAG_R_CALF, BONETAG_R_FOOT }
};

struct Mat34V
{
    __m128 col0;
    __m128 col1;
    __m128 col2;
    __m128 col3;
};

struct Mat34Vi
{
    __m128i col0;
    __m128i col1;
    __m128i col2;
    __m128i col3;
};

struct BoneData {
    uintptr_t fragInst;
    uintptr_t ptr;
    uintptr_t m_pSkeleton;
    int boneIndexStart;
    int boneIndexEnd;
    Mat34V matrixStart;
    Mat34V matrixEnd;
};

struct BonePosition {
    Vector3 start;
    Vector3 end;
};

struct esp_vehicle_ {
    struct {
        uintptr_t instance;
        Vector3 world_coords;
        struct {
            float dist;
            uint32_t hash;
            char name_1[20];
            char name_2[20];
        }text;
    }item[300];
};
inline esp_vehicle_ esp_vehicle;

struct esp_ped_ {
    struct {
        uintptr_t instance;
        Vector3 world_coords;
        bool aimboted;
        Vector3 HeadPos;
        Rotation rotation;

        struct {
            float dist;
            uint32_t hash;
            uint32_t ped_type;
        }text;
        struct {
            std::vector<BonePosition> bone_positions;
            float health;
            float maxhealth;
        }SkeletonPHealth;
    }item[256];
};
inline esp_ped_ esp_ped;

struct esp_pickup_ {
    struct {
        uintptr_t instance;
        Vector3 world_coords;
        struct {
            float dist;
            uint32_t hash;
        }text;
    }item[73];
};
inline esp_pickup_ esp_pickup;

struct esp_object_ {
    struct {
        uintptr_t instance;
        Vector3 world_coords;
        struct {
            float dist;
            uint32_t hash;
        }text;
    }item[2300];
};
inline esp_object_ esp_object;

class core
{
private:
    static core* instance;
    zenith::communicate_t* ctx = nullptr;

    struct _cache
    {
        uintptr_t uWorld = 0;
        uintptr_t viewPort = 0;
        uintptr_t replayInterface = 0;
        uintptr_t Camera = 0;

        int esp_game_width;
        int esp_game_height;
        int esp_num_vehicle;
        int esp_num_ped;
        int esp_num_pickup;
        int esp_num_object;

        struct _Local
        {
            uintptr_t LocalPlayerPTR = 0;
            uintptr_t LocalPlayerL = 0;
            Vector3 LocalPosition;
        } local;
    } cache;

public:
    static core* get_instance();

    core()
    {
		ctx = zenith::communicate_t::inst();
    }

    _cache getCache()
    {
        return cache;
    }

    // core
    uintptr_t getUWorld();
    bool W2S(vec3_t pos, vec3_t& screen, float matrix[16], int windowWidth, int windowHeight);
    bool crSkeletonData_ConvertBoneIdToIndex(__int64 skeletonData, unsigned __int16 boneMask, int* outBoneIdx);
    void crSkeleton_GetGlobalMtx(__int64 m_pSkeleton, unsigned int boneIdx, Mat34V* outMtx);
    Vector3 GetBonePosition(uintptr_t entity, eAnimBoneTag wMask);

    // entitys
    void UpdateLocalInfos(); 
    void UpdateActors();
    void UpdateSkeleton();

    template<typename T> T RPM(uintptr_t address, std::vector<int64_t> offsets = {})
    {
        auto ctx = zenith::communicate_t::inst();

        for (auto& offset : offsets)
        {
            ctx->read_virtual(address, &address, sizeof(address));
            address += offset;
        }
        T value;
        ctx->read_virtual(address, &value, sizeof(value));
        return value;
    }

};

extern inline float get_ped_health(uintptr_t instance) { auto ctx = zenith::communicate_t::inst(); return ctx->read<float>(instance + Offsets::get_instance()->health); }
extern inline float get_ped_maxhealth(uintptr_t instance) { auto ctx = zenith::communicate_t::inst(); return ctx->read<float>(instance + Offsets::get_instance()->maxhealth); }
extern inline uint32_t get_ped_ped_type(uintptr_t instance) { auto ctx = zenith::communicate_t::inst(); return ctx->read<uint32_t>(instance + 0x1098) << 11 >> 25; }
extern inline void get_vehicle_name_1(uintptr_t instance, char(&name_1)[20]) { auto ctx = zenith::communicate_t::inst(); auto core = core::get_instance(); ctx->read_virtual((core->RPM<uintptr_t>(instance + 0x20) + 0x298), name_1, 20); }
extern inline void get_vehicle_name_2(uintptr_t instance, char(&name_2)[20]) { auto ctx = zenith::communicate_t::inst(); auto core = core::get_instance(); ctx->read_virtual((core->RPM<uintptr_t>(instance + 0x20) + 0x2A4), name_2, 20); }
extern inline void get_instance_world_coords(uintptr_t instance, Vector3& world_coords) { auto core = core::get_instance(); world_coords = core->RPM<Vector3>(instance + 0x30, { 0x50 }); }
extern inline Vector3 get_instance_world_coords(uintptr_t instance) { auto core = core::get_instance(); return core->RPM<Vector3>(instance + 0x30, { 0x50 }); }
extern inline uint32_t get_instance_hash(uintptr_t instance) { auto core = core::get_instance(); return core->RPM<uint32_t>(instance + 0x20, { 0x18 });}
extern inline void get_instace_model_matrix(uintptr_t instance, float(&matrix)[16]) { auto ctx = zenith::communicate_t::inst(); auto core = core::get_instance(); ctx->read_virtual((core->RPM<uintptr_t>(instance + 0x30) + 0x20), matrix, 16 * 4); }
extern inline void model_to_world(vec3_t model_coords, vec3_t& world_coords, float model_matrix[16])
{
    world_coords.x = model_coords.x * model_matrix[0] + model_coords.y * model_matrix[4] + model_coords.z * model_matrix[8] + model_matrix[12];
    world_coords.y = model_coords.x * model_matrix[1] + model_coords.y * model_matrix[5] + model_coords.z * model_matrix[9] + model_matrix[13];
    world_coords.z = model_coords.x * model_matrix[2] + model_coords.y * model_matrix[6] + model_coords.z * model_matrix[10] + model_matrix[14];
}