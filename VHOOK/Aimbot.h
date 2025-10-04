#pragma once

inline eAnimBoneTag bone_map[] = {
    eAnimBoneTag::BONETAG_HEAD,        // head
    eAnimBoneTag::BONETAG_SPINE1,      // chest
    eAnimBoneTag::BONETAG_PELVIS,      // pevis
    eAnimBoneTag::BONETAG_L_UPPERARM,  // left_arm
    eAnimBoneTag::BONETAG_L_HAND,      // left_hand
    eAnimBoneTag::BONETAG_R_UPPERARM,  // right_arm
    eAnimBoneTag::BONETAG_R_HAND,      // right_hand
    eAnimBoneTag::BONETAG_L_CALF,      // left_knee
    eAnimBoneTag::BONETAG_L_FOOT,      // left_foot
    eAnimBoneTag::BONETAG_R_CALF,      // right_knee
    eAnimBoneTag::BONETAG_R_FOOT       // right_foot
};

class Aimbot {
private:

public:
    static void Run();
};

inline bool Vec3_Empty(const Vector3& value) {
    return (value.x == 0 && value.y == 0 && value.z == 0);
}

inline void NormalizeAngles(Vector3& angle) {
    while (angle.x > 89.0f) angle.x -= 180.f;
    while (angle.x < -89.0f) angle.x += 180.f;
    while (angle.y > 180.f) angle.y -= 360.f;
    while (angle.y < -180.f) angle.y += 360.f;
}

inline float GetDistance(Vector3 value1, Vector3 value2) {
    float num = value1.x - value2.x;
    float num2 = value1.y - value2.y;
    float num3 = value1.z - value2.z;
    return sqrt(num * num + num2 * num2 + num3 * num3);
}

inline Vector3 CalcAngle(Vector3 local_cam, Vector3 to_point) {
    Vector3 vOut{};
    float distance = GetDistance(local_cam, to_point);
    vOut.x = (to_point.x - local_cam.x) / distance;
    vOut.y = (to_point.y - local_cam.y) / distance;
    vOut.z = (to_point.z - local_cam.z) / distance;
    return vOut;
}

// Função para ajustar a altura do alvo com base na distância
inline void AdjustHeightBasedOnDistance(Vector3& target, const Vector3& local_pos, float adjustment_factor) {
    float distance = GetDistance(local_pos, target);
    target.z -= distance / adjustment_factor; // Ajuste o fator conforme necessário
}

inline Vector3 SmoothAim(Vector3 current_angle, Vector3 target_angle, float smoothing) {
    Vector3 smoothed_angle;
    smoothed_angle.x = current_angle.x + (target_angle.x - current_angle.x) / smoothing;
    smoothed_angle.y = current_angle.y + (target_angle.y - current_angle.y) / smoothing;
    smoothed_angle.z = current_angle.z + (target_angle.z - current_angle.z) / smoothing;
    return smoothed_angle;
}

inline bool IsVisible(uintptr_t Entity) {
    BYTE VisibilityFlag = zenith::communicate_t::inst()->read<BYTE>(Entity + Offsets::get_instance()->visibleFlag);
    if (VisibilityFlag == 36 || VisibilityFlag == 0 || VisibilityFlag == 4)
        return false;
    return true;
}
