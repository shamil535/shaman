#include "includes.h"
#include "offsets.h"
#include "driver.h"
#include "ESP.h"
#include "CFG.h"
#include "process.h"
#include "core.h"
#include "ImDraw.h"
#include "math.h"
#include "menu.h"
#include "Aimbot.h"

void ESP::HeadLines(float esp_matrix[16], int entity, ImColor Color) {
    auto core = core::get_instance();
    auto proc = process::get_instance();

    // Defina os pontos de início e fim da linha
    Vector3 forward_vector = get_forward_vector(esp_ped.item[entity].rotation);
    Vector3 end_pos = esp_ped.item[entity].HeadPos + (forward_vector * 2.5f); // Multiplicar para ajustar o comprimento da linha

    // Converter as coordenadas 3D para 2D
    Vector3 start_screen, end_screen;
    bool start_on_screen = core->W2S(esp_ped.item[entity].HeadPos, start_screen, esp_matrix, proc->windowWidth, proc->windowHeight);
    bool end_on_screen = core->W2S(end_pos, end_screen, esp_matrix, proc->windowWidth, proc->windowHeight);

    // Desenhar a linha se ambos os pontos estiverem na tela
    if (start_on_screen && end_on_screen) {
        // Linha mais fina
        ImGui::GetWindowDrawList()->AddLine(ImVec2(start_screen.x, start_screen.y), ImVec2(end_screen.x, end_screen.y), Color, 0.003f);

        // Bolinha menor no final da linha
        ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(end_screen.x, end_screen.y), 1.8f, Color);
    }
}

void ESP::Skeleton(int i, float esp_matrix[16], ImColor Color) {

    auto proc = process::get_instance();
    auto core = core::get_instance();

    for (const auto& bone_position : esp_ped.item[i].SkeletonPHealth.bone_positions) {
        Vector3 bone_start_screen, bone_end_screen;

        bool start_on_screen = core->W2S(bone_position.start, bone_start_screen, esp_matrix, proc->windowWidth, proc->windowHeight);
        bool end_on_screen = core->W2S(bone_position.end, bone_end_screen, esp_matrix, proc->windowWidth, proc->windowHeight);

        if (start_on_screen && end_on_screen) {
            ImGui::GetBackgroundDrawList()->AddLine(
                { bone_start_screen.x, bone_start_screen.y },
                { bone_end_screen.x, bone_end_screen.y },
                Color, CFG.RageMP.ItensLSize
            );
        }
    }
}

void ESP::DrawHealthBar(const ImVec2& top_left, const ImVec2& bottom_right, float health, float maxhealth) {
    // Calcula a altura da barra de vida proporcional à vida e ao tamanho do box
    float box_height = bottom_right.y - top_left.y;
    float health_height = std::clamp((health / maxhealth) * box_height, 0.0f, box_height);

    // Define a posição da barra de vida
    ImVec2 health_bar_top = { top_left.x - 5.0f, bottom_right.y - health_height };
    ImVec2 health_bar_bottom = { top_left.x - 2.0f, bottom_right.y };

    // Calcula a cor da barra de vida baseada na proporção da saúde atual
    ImColor health_color;
    float health_factor = health / maxhealth;

    if (health_factor > 0.5f) {
        float factor = (health_factor - 0.5f) / 0.5f;
        health_color = ImColor(1.0f - factor, 1.0f, 0.0f, 1.0f); // Verde para Amarelo
    }
    else {
        float factor = health_factor / 0.5f;
        health_color = ImColor(1.0f, factor, 0.0f, 1.0f); // Amarelo para Vermelho
    }

    // Desenha a barra de vida
    ImGui::GetBackgroundDrawList()->AddRectFilled(health_bar_top, health_bar_bottom, health_color);
}

void ESP::Run()
{

    auto proc = process::get_instance();
    auto core = core::get_instance();
    auto ctx = zenith::communicate_t::inst();

    if (core->getCache().local.LocalPlayerPTR == NULL)
        return;
    //Vector3 Test = Vector3{ 0,0,0 };
    //ctx->write(ctx->read<uintptr_t>(core->getCache().Camera) + 0x40, Test);
    //ctx->write(ctx->read<uintptr_t>(core->getCache().Camera) + 0x3D0, Test);
    //ctx->write(core->getCache().local.LocalPlayerPTR + 0x280, 200.f);

    float esp_matirx[16];
    Vector3 LocalPos = ctx->read<Vector3>(core->getCache().local.LocalPlayerL + 0x50);
    ctx->read_virtual(ctx->read<uintptr_t>(core->getCache().viewPort) + 0x250, esp_matirx, 16 * 4);

#ifdef _DEBUG
    if (CFG.debug.drawGNames) {

        ImGui::GetBackgroundDrawList()->AddText({ 5, 15 }, ImColor(255, 255, 255), (std::format("Vehicle: {}\nPed: {}\nPickup: {}\nObject: {}\nOVERLAYFPS: {:.1f}", core->getCache().esp_num_vehicle, core->getCache().esp_num_ped, core->getCache().esp_num_pickup, core->getCache().esp_num_object, ImGui::GetIO().Framerate).c_str()));
        ImGui::GetBackgroundDrawList()->AddText({ 140, 15 }, ImColor(255, 255, 255), (std::format("LocalPlayerPTR: {:#x} ", core->getCache().local.LocalPlayerPTR).c_str()));
        ImGui::GetBackgroundDrawList()->AddText({ 140, 31 }, ImColor(255, 255, 255), (std::format("LocalPlayer X: {} ", LocalPos.x)).c_str());
        ImGui::GetBackgroundDrawList()->AddText({ 140, 42 }, ImColor(255, 255, 255), (std::format("LocalPlayer Y: {} ", LocalPos.y)).c_str());
        ImGui::GetBackgroundDrawList()->AddText({ 140, 53 }, ImColor(255, 255, 255), (std::format("LocalPlayer Z: {} ", LocalPos.z)).c_str());

    }
#endif

    //for (int i = 0; i < sizeof(esp_vehicle.item) / sizeof(esp_vehicle.item[0]); i++)
    //{
    //    if (esp_vehicle.item[i].instance)
    //    {
    //        esp_vehicle.item[i].world_coords = get_instance_world_coords(esp_vehicle.item[i].instance);

    //        Vector3 image_coords;
    //        Vector3 world_coords_2 = esp_vehicle.item[i].world_coords; world_coords_2.z += 1;
    //        Vector3 image_coords_2;
    //        if (core->W2S(esp_vehicle.item[i].world_coords, image_coords, esp_matirx, proc->windowWidth, proc->windowHeight) && core->W2S(world_coords_2, image_coords_2, esp_matirx, proc->windowWidth, proc->windowHeight))
    //        {
    //            int box_height = (int)((image_coords.y - image_coords_2.y) * 2);
    //            int box_width = (int)(box_height / 2.4);
    //            if (/*g_settings->esp.vehicle.line*/ true)
    //            {
    //                //ImGui::GetBackgroundDrawList()->AddLine({ (float)proc->windowWidth / 2, 0 }, { image_coords.x, image_coords.y }, ImColor{ 255,255,255,255 });
    //            }
    //            if (/*g_settings->esp.vehicle.text*/ true)
    //            {
    //                ImGui::GetBackgroundDrawList()->AddText({ image_coords.x + box_width, image_coords.y }, ImColor{ 255,255,255,255 }, std::format("dist: {:.2f}\nhash: {}\nname: {}\nname: {}", esp_vehicle.item[i].text.dist, esp_vehicle.item[i].text.hash, esp_vehicle.item[i].text.name_1, esp_vehicle.item[i].text.name_2).c_str());
    //            }
    //        }
    //    }
    //}


    for (int i = 0; i < sizeof(esp_ped.item) / sizeof(esp_ped.item[0]); i++)
    {
        if (esp_ped.item[i].instance && esp_ped.item[i].instance != core->getCache().local.LocalPlayerPTR)
        {
            esp_ped.item[i].world_coords = get_instance_world_coords(esp_ped.item[i].instance);
            Vector3 image_coords;
            Vector3 world_coords_2 = esp_ped.item[i].world_coords; world_coords_2.z += 1;
            Vector3 image_coords_2;
            if (core->W2S(esp_ped.item[i].world_coords, image_coords, esp_matirx, proc->windowWidth, proc->windowHeight) && core->W2S(world_coords_2, image_coords_2, esp_matirx, proc->windowWidth, proc->windowHeight))
            {

                int box_height = (int)((image_coords.y - image_coords_2.y) * 2 * 0.5f);
                int box_width = (int)(box_height / 2.6);

                // Calcular coordenadas top_left e bottom_right
                ImVec2 top_left = { image_coords.x - box_width, (image_coords.y - box_height) + 7.f };
                ImVec2 bottom_right = { image_coords.x + box_width, image_coords.y + box_height };

                if (CFG.RageMP.b_EspHeadPlayers) {
                    if (CFG.RageMP.b_AimbotVisibleCheck && IsVisible(esp_ped.item[i].instance))
                        HeadLines(esp_matirx, i, CFG.RageMP.PlayersHeadColor);
                    else if (!CFG.RageMP.b_AimbotVisibleCheck)
                        HeadLines(esp_matirx, i, CFG.RageMP.PlayersHeadColor);
                }

#ifdef _DEBUG
                if (CFG.debug.drawGNames) {
                    if (esp_ped.item[i].aimboted)
                        ImGui::GetBackgroundDrawList()->AddText({ image_coords.x + box_width, image_coords.y - 20 }, CFG.RageMP.PlayersAimedColor, (std::format("PTR: {:#x} ", esp_ped.item[i].instance).c_str()));
                    else
                        ImGui::GetBackgroundDrawList()->AddText({ image_coords.x + box_width, image_coords.y - 20 }, CFG.RageMP.PlayersColor, (std::format("PTR: {:#x} ", esp_ped.item[i].instance).c_str()));
                }
#endif

                if (CFG.RageMP.b_EspLinePlayers) {
                    if (esp_ped.item[i].aimboted)
                        ImGui::GetBackgroundDrawList()->AddLine({ (float)proc->windowWidth / 2, 0 }, { image_coords.x, (image_coords.y - box_height) + 7.f }, CFG.RageMP.PlayersAimedColor, CFG.RageMP.ItensLSize);
                    else
                        ImGui::GetBackgroundDrawList()->AddLine({ (float)proc->windowWidth / 2, 0 }, { image_coords.x, (image_coords.y - box_height) + 7.f }, CFG.RageMP.PlayersColor, CFG.RageMP.ItensLSize);
                }

                /*       if (CFG.RageMP.b_EspNamePlayers) {
                           if (esp_ped.item[i].aimboted)
                               ImGui::GetBackgroundDrawList()->AddText({ image_coords.x + box_width, image_coords.y }, CFG.RageMP.PlayersAimedColor, std::format("dist    : {:.2f}\nhash    : {}\nhealth  : {}\nped type: {}", esp_ped.item[i].text.dist, esp_ped.item[i].text.hash, esp_ped.item[i].text.health, esp_ped.item[i].text.ped_type).c_str());
                           else
                               ImGui::GetBackgroundDrawList()->AddText({ image_coords.x + box_width, image_coords.y }, CFG.RageMP.PlayersColor, std::format("dist    : {:.2f}\nhash    : {}\nhealth  : {}\nped type: {}", esp_ped.item[i].text.dist, esp_ped.item[i].text.hash, esp_ped.item[i].text.health, esp_ped.item[i].text.ped_type).c_str());
                       }*/

                if (CFG.RageMP.b_EspSkeletonPlayers) {
                    if (esp_ped.item[i].aimboted)
                        Skeleton(i, esp_matirx, CFG.RageMP.PlayersAimedColor);
                    else
                        Skeleton(i, esp_matirx, CFG.RageMP.PlayersColor);
                }

                if (CFG.RageMP.b_EspBoxPlayers) {

                    // Verifique se as coordenadas são válidas antes de desenhar o retângulo
                    if (top_left.x != bottom_right.x && top_left.y != bottom_right.y) {
                        if (esp_ped.item[i].aimboted)
                            ImGui::GetBackgroundDrawList()->AddRect(top_left, bottom_right, CFG.RageMP.PlayersAimedColor, CFG.RageMP.ItensLSize);
                        else
                            ImGui::GetBackgroundDrawList()->AddRect(top_left, bottom_right, CFG.RageMP.PlayersColor, CFG.RageMP.ItensLSize);
                    }
                }

                if (CFG.RageMP.b_EspHealthPlayers)
                    DrawHealthBar(top_left, bottom_right, esp_ped.item[i].SkeletonPHealth.health, esp_ped.item[i].SkeletonPHealth.maxhealth);

                if (CFG.RageMP.b_EspDistancePlayers) {
                    int distance = static_cast<int>(std::floor(LocalPos.Distance(esp_ped.item[i].HeadPos)));
                    std::string distance_text = std::format("{}m", distance);
                    ImVec2 text_size = ImGui::CalcTextSize(distance_text.c_str());
                    float text_x = top_left.x + (box_width * 2 - text_size.x) / 2;

                    if (esp_ped.item[i].aimboted)
                        ImGui::GetBackgroundDrawList()->AddText({ text_x, bottom_right.y + 7.f }, CFG.RageMP.PlayersAimedColor, distance_text.c_str());
                    else
                        ImGui::GetBackgroundDrawList()->AddText({ text_x, bottom_right.y + 7.f }, CFG.RageMP.PlayersColor, distance_text.c_str());
                }
            }
        }
    }


    if (/*g_settings->esp.pickup.enable*/ true)
    {
        //for (int i = 0; i < sizeof(esp_pickup.item) / sizeof(esp_pickup.item[0]); i++)
        //{
        //    if (esp_pickup.item[i].instance)
        //    {
        //        esp_pickup.item[i].world_coords = get_instance_world_coords(esp_pickup.item[i].instance);
        //        Vector3 image_coords;
        //        Vector3 world_coords_2 = esp_pickup.item[i].world_coords; world_coords_2.z += 1;
        //        Vector3 image_coords_2;
        //        if (core->W2S(esp_pickup.item[i].world_coords, image_coords, esp_matirx, proc->windowWidth, proc->windowHeight) && core->W2S(world_coords_2, image_coords_2, esp_matirx, proc->windowWidth, proc->windowHeight))
        //        {
        //            int box_height = (int)((image_coords.y - image_coords_2.y) * 2);
        //            int box_width = (int)(box_height / 2.4);
        //            if (/*g_settings->esp.pickup.line*/ true)
        //            {
        //                ImGui::GetBackgroundDrawList()->AddLine({ (float)proc->windowWidth / 2, 0 }, { image_coords.x, image_coords.y }, ImColor{ 255,255,255,255 });
        //            }
        //            if (/*g_settings->esp.pickup.text*/ true)
        //            {
        //                ImGui::GetBackgroundDrawList()->AddText({ image_coords.x + box_width, image_coords.y }, ImColor{ 255,255,255,255 }, std::format("dist: {:.2f}\nhash: {}", esp_pickup.item[i].text.dist, esp_pickup.item[i].text.hash).c_str());
        //            }
        //        }
        //    }
        //}
    }
    if (/*g_settings->esp.object.enable*/ true)
    {
        //for (int i = 0; i < sizeof(esp_object.item) / sizeof(esp_object.item[0]); i++)
        //{
        //    if (esp_object.item[i].instance)
        //    {
        //        esp_object.item[i].world_coords = get_instance_world_coords(esp_object.item[i].instance);
        //        Vector3 image_coords;
        //        Vector3 world_coords_2 = esp_object.item[i].world_coords; world_coords_2.z += 1;
        //        Vector3 image_coords_2;
        //        if (core->W2S(esp_object.item[i].world_coords, image_coords, esp_matirx, proc->windowWidth, proc->windowHeight) && core->W2S(world_coords_2, image_coords_2, esp_matirx, proc->windowWidth, proc->windowHeight))
        //        {
        //            int box_height = (int)((image_coords.y - image_coords_2.y) * 2);
        //            int box_width = (int)(box_height / 2.4);
        //            if (/*g_settings->esp.object.line*/ true)
        //            {
        //                ImGui::GetBackgroundDrawList()->AddLine({ (float)proc->windowWidth / 2, 0 }, { image_coords.x, image_coords.y }, ImColor{ 255,255,255,255 });
        //            }
        //            if (/*g_settings->esp.object.text*/ true)
        //            {
        //                ImGui::GetBackgroundDrawList()->AddText({ image_coords.x + box_width, image_coords.y }, ImColor{ 255,255,255,255 }, std::format("dist: {:.2f}\nhash: {}", esp_object.item[i].text.dist, esp_object.item[i].text.hash).c_str());
        //            }
        //        }
        //    }
        //}
    }

}