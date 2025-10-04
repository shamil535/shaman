#include "includes.h"
#include "driver.h"
#include "process.h"
#include "core.h"
#include "Aimbot.h"
#include "offsets.h"

void Aimbot::Run() {
    while (true) {
        if (CFG.RageMP.b_AimbotPlayers) {
            auto proc = process::get_instance();
            auto core = core::get_instance();
            auto ctx = zenith::communicate_t::inst();

            // Inicializar os melhores valores
            float best_fov = FLT_MAX; // Usar FLT_MAX para inicializar best_fov
            Vector3 best_screen = { 0, 0, 0 };
            uintptr_t best_entity = 0;
            int best_index = -1;
            Vector3 bone_pos = { 0, 0, 0 };
            Vector3 BestBone = { 0, 0, 0 };

            // Função lambda para calcular a distância 2D (FOV)
            auto get_distance_2d = [](float x1, float y1, float x2, float y2) {
                return sqrtf(powf(x2 - x1, 2.0f) + powf(y2 - y1, 2.0f));
                };

            // Função lambda para calcular a distância 3D
            auto get_distance_3d = [](const Vector3& p1, const Vector3& p2) {
                return p1.Distance(p2);
                };

            for (int i = 0; i < sizeof(esp_ped.item) / sizeof(esp_ped.item[0]); i++) {
                if (esp_ped.item[i].instance == core->getCache().local.LocalPlayerPTR)
                    continue;

                if (CFG.RageMP.b_AimbotVisibleCheck && !IsVisible(esp_ped.item[i].instance))
                    continue;

                float esp_matrix[16];
                Vector3 LocalPos = ctx->read<Vector3>(core->getCache().local.LocalPlayerL + 0x50);
                ctx->read_virtual(ctx->read<uintptr_t>(core->getCache().viewPort) + 0x250, esp_matrix, 16 * 4);

                Vector3 screen;
                bool found_bone = false;

                // Iterar sobre os ossos mapeados
                for (int j = 0; j < sizeof(bone_map) / sizeof(bone_map[0]); j++) {
                    if (!CFG.RageMP.bone_selector[j]) continue;

                    bone_pos = core->GetBonePosition(esp_ped.item[i].instance, bone_map[j]);

                    if (!core->W2S(bone_pos, screen, esp_matrix, proc->windowWidth, proc->windowHeight))
                        continue;

                    if (Vec3_Empty(screen) || Vec3_Empty(bone_pos))
                        continue;

                    auto center_x = proc->windowWidth / 2;
                    auto center_y = proc->windowHeight / 2;
                    auto fov = get_distance_2d(center_x, center_y, screen.x, screen.y);
                    auto distance = get_distance_3d(LocalPos, bone_pos); // Distância 3D entre o jogador local e o osso do inimigo

                    if (distance > CFG.RageMP.AimbotMaxDist) // Verificar se a distância excede a distância máxima permitida
                        continue;

                    if (fov < best_fov) {
                        best_fov = fov;
                        best_screen = screen;
                        best_entity = esp_ped.item[i].instance;
                        best_index = i;
                        BestBone = bone_pos;
                        found_bone = true;
                    }
                }

                if (!found_bone)
                    continue;
            }

            if (best_fov <= CFG.RageMP.AimbotFOV) {

                // Redefina todos os aimboted para false
                for (int i = 0; i < sizeof(esp_ped.item) / sizeof(esp_ped.item[0]); i++) {
                    esp_ped.item[i].aimboted = false;
                }

                // Defina aimboted como true apenas para a melhor entidade
                if (best_index != -1) {
                    esp_ped.item[best_index].aimboted = true;
                }

                uintptr_t camera = ctx->read<uintptr_t>(core->getCache().Camera);
                uintptr_t camFollowPedCameraMetadata = ctx->read<uintptr_t>(camera + 0x10);

                bool is_first_person = ctx->read<float>(camFollowPedCameraMetadata + 0x30) == 0.0f;
                Vector3 ViewAngle;
                if (is_first_person)
                    ViewAngle = ctx->read<Vector3>(camera + 0x40);
                else
                    ViewAngle = ctx->read<Vector3>(camera + 0x3D0);

                Vector3 CameraPos = ctx->read<Vector3>(camera + 0x60);

                if (Vec3_Empty(BestBone))
                    continue;

                if (GetAsyncKeyState(CFG.RageMP.AimKey) & 0x8000) {
                    // Ajuste a altura do alvo com base na distância
                    AdjustHeightBasedOnDistance(BestBone, CameraPos, 500.0f); // Ajuste o valor conforme necessário
                    Vector3 Angle = CalcAngle(CameraPos, BestBone);
                    NormalizeAngles(Angle);
                    Vector3 SmoothedAngle = SmoothAim(ViewAngle, Angle, CFG.RageMP.Smoothing); // Adicionar suavização
                    NormalizeAngles(SmoothedAngle);

                    if (!Vec3_Empty(SmoothedAngle)) {
                        if (is_first_person)
                            ctx->write<Vector3>(camera + 0x40, SmoothedAngle);
                        else
                            ctx->write<Vector3>(camera + 0x3D0, SmoothedAngle);
                    }

                     if (CFG.RageMP.b_AimbotMagicBullets) {
                         uintptr_t c_weapon_manager = ctx->read<uintptr_t>(core->getCache().local.LocalPlayerPTR + Offsets::get_instance()->weapon_manager);
                         uintptr_t c_object = ctx->read<uintptr_t>(c_weapon_manager + 0x78);
                         uintptr_t c_weapon = ctx->read<uintptr_t>(c_object + Offsets::get_instance()->c_weapon_offset);
                         ctx->write<Vector3>(c_weapon + 0x20, BestBone);
                     }
                }
            }

        }
        Sleep(1);
    }
}

