#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <TlHelp32.h>
#include <vector>
#include <dwmapi.h>
#include <format>
#include <fstream>
#include <filesystem>
#include <map>
#include <locale>                
#include <codecvt>               
#include <xmmintrin.h>
#include <emmintrin.h>
#include <tmmintrin.h>
#include <immintrin.h>
#include <softintrin.h>
#include <cstdint>
#include <cstddef>
#include <utility>
#include <random>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <sys/stat.h>
#include <cctype>
#include <array>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <string_view>
#include <chrono>
#include <psapi.h>
#include <thread>
#include <mutex>
#include <tchar.h>
#include <shlobj.h>

#include "xorstr.h"

inline std::mutex skeletonMutex;

#define PI (3.14159265358979323846264338327950288419716939937510f)
#define print(x,...) printf_s(x,__VA_ARGS__)
#define IMGUI_DEFINE_MATH_OPERATORS
#define STB_IMAGE_IMPLEMENTATION
#define xor(str) xorstr(str).crypt_get()

//Versão do GTA (Obrigatório deixar apenas 1 definição.)
//
//#define GTARageMP
#define GTAFiveM
//#define GTAOnline


//Definindo Nosafer, o executavel não estara safe para usar WBAND com motorista.
//
#define _NOSAFER


//Edição do menu (Habilita apenas o menu Obs: Deve-se deixar o bloco de notas para o cheat dar atach)
// 
//#define ONLY_MENU


//Dev Options
//#define __VERBOSE
//#define DUMP
//#define _DEBUG
//#define _DUMPLOOP
 

//Ovelay Options  (Deixar sem definições usará o DirectX11 padrão)
// 
//#define __NVidiaAMD
//#define __Medal
#define WBAND


//Não mexer!
#ifdef __NVidiaAMD
#define _DirectX9
#elif defined(__Medal)
#define _DirectX9
#else
#define _DirectX11
#endif
#ifdef GTARageMP
#define GAME_NAME "GTARageMP"
#elif defined(GTAOnline)
#define GAME_NAME "GTAOnline"
#endif

#ifndef __NVidiaAMD
#include <d3d11.h>
#include <d3d9.h>
#include <d3dx9math.h>
#include <dxgi.h>
#pragma comment(lib, "d3d11.lib")
#endif

#ifdef __NVidiaAMD
#include <d3d9.h>
#include <d3dx9.h>
#include <dxgi.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dwmapi.lib")
#include "imgui/imgui_impl_dx9.h"
#endif

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#ifndef __NVidiaAMD
#include "imgui/imgui_impl_dx11.h"
#endif

#include "imgui/imgui_impl_win32.h"
#include "imgui/etc_elements.h"
#include "imgui/nav_elements.h"

