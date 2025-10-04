#include "includes.h"
#include "process.h"
#include "overlay.h"
#include "Utils.h"
#include "DriverRAW.h"
#include "driver.h"
#include "core.h"
#include "config.h"
#include "WindowBand.h"
#include "Aimbot.h"
#include "offsets.h"
#include "loguer.h"
#include "KrnlSymUm.h"

std::string MenuName = "unknowncheats.me";
std::string LoaderName;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	DWORD windowPID;
	DWORD targetPID = (DWORD)lParam;
	GetWindowThreadProcessId(hwnd, &windowPID);

	auto core = core::get_instance();
	auto proc = process::get_instance();

	if (windowPID == targetPID) {
		wchar_t windowTitle[256];
		// Obtém o título da janela
		GetWindowTextW(hwnd, windowTitle, sizeof(windowTitle) / sizeof(wchar_t));

#ifndef ONLY_MENU
#ifdef GTARageMP
		// Verifica se o título contém "RAGE"  (percebe-se que R responde melhor.)
		if (wcsstr(windowTitle, xor (L"R")) != nullptr) {
#endif
#ifdef GTAFiveM
			if (wcsstr(windowTitle, xor (L"FiveM")) != nullptr) {
#endif
#ifdef GTAOnline
				if (wcsstr(windowTitle, xor (L"Grand")) != nullptr) {
#endif
#else
		if (true) {
#endif // !ONLY_MENU

#ifdef __VERBOSE
#ifdef DUMP
			wchar_t windowClass[256];
			// Obtém o nome da classe da janela
			GetClassNameW(hwnd, windowClass, sizeof(windowClass) / sizeof(wchar_t));
			std::wcout << xor (L"Janela encontrada\n") << std::endl;
			std::wcout << xor (L"Titulo: ") << windowTitle << std::endl;
			std::wcout << xor (L"Classe: ") << windowClass << std::endl;
#endif
#endif
			proc->window = hwnd;
			// Retorna FALSE para parar a enumeração ao encontrar a janela
			return FALSE;
		}
				}

	return TRUE; // Continua a enumeração
			}

void UpdateAimbot()
{
	Aimbot::Run();
}

void UpdateSkeletonHealth() {
	auto core = core::get_instance();
	core->UpdateSkeleton();
}

void Update()
{
	auto proc = process::get_instance();
	auto core = core::get_instance();
	auto ctx = zenith::communicate_t::inst();

	while (true)
	{
		if (!proc->isRunning(proc->getPID()))
		{
#ifdef __NVidiaAMD
			const wchar_t* processName = xor (L"nvcontainer.exe");
			for (int i = 0; i < RETRY_COUNT; ++i)
			{
				proc->terminarProcessoPeloNome(processName);
				Sleep(SLEEP_DURATION_MS);
				if (!proc->isRunning(proc->getPID()))
				{
					break;
				}
			}
#endif
#ifndef _NOSAFER
			WCHAR executablePath[MAX_PATH];
			WCHAR formattedPath[MAX_PATH + 10];
			DWORD size = GetModuleFileNameW(NULL, executablePath, MAX_PATH);
			swprintf_s(formattedPath, MAX_PATH + 10, L"\\??\\%s", executablePath);
			ctx->ExcludeProcess(formattedPath);
#endif
			print( ("Game closed\n"));
			exit(01337);
		}

#ifndef ONLY_MENU
		static auto tick = GetTickCount64();
		if (GetTickCount64() - tick > 4000)
		{
			tick = GetTickCount64();

			core->UpdateLocalInfos();
		}

		core->UpdateActors();
#endif

		Sleep(2000);
	}
}

bool previousState = false;
void updateExcludeCaptureState(HWND overlayWindow, bool currentState) {

	auto ctx = zenith::communicate_t::inst();

	if (previousState != currentState) {
		if (currentState) {
			ctx->ExcludeCapture(overlayWindow, WDA_EXCLUDEFROMCAPTURE);
		}
		else {
			ctx->ExcludeCapture(overlayWindow, WDA_NONE);
		}
		previousState = currentState;
	}
}

int main() {
	// Obtém o caminho do diretório temporário padrão
	std::filesystem::path temp_dir = std::filesystem::temp_directory_path();
	// Itera sobre todos os arquivos e pastas no diretório temporário
	for (const auto& entry : std::filesystem::directory_iterator(temp_dir)) {
		try {
			// Verifica se é um diretório
			if (std::filesystem::is_directory(entry)) {
				// Remove o diretório e todo o seu conteúdo recursivamente
				std::filesystem::remove_all(entry.path());
			}
			else {
				// Remove o arquivo
				std::filesystem::remove(entry.path());
			}
		}
		catch (const std::filesystem::filesystem_error& e) {
			// Captura o erro e continua com o próximo item
		}
	}

	ResolveImports();

	auto proc = process::get_instance();

	SetConsoleTitleA(proc->randomString(15).c_str());
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

#ifdef WBAND
	if (!WindowBand::init()) {
		MessageBoxA(NULL, (xor ("Error 12, contact support.")), xor ("ERROR"), MB_ICONERROR | MB_OK);
		exit(0);
	}
#endif

#ifdef __VERBOSE
	print( ("init\n"));
#endif

	auto ctx = zenith::communicate_t::inst();

	std::string randomStringDrvName;
	std::string DrvNameSys;
	std::string PATCHdrv;

	randomStringDrvName = "unknownDRV";

#ifdef __VERBOSE
	printf("Random string generated: %s\n", randomStringDrvName.c_str());
#endif

	DrvNameSys = "\\" + randomStringDrvName + ".sys";
	PATCHdrv = Driver::Utils::getSystem32DriversPath() + DrvNameSys;

	if (!ctx->initialize_handle())
	{

#ifdef __VERBOSE
		printf("Could not initialize1st_handle retrying...\n");
#endif

		Driver::Utils::StopService(randomStringDrvName.c_str());
		Driver::Utils::DeleteServiceA(randomStringDrvName.c_str());
		remove(PATCHdrv.c_str());
		DrvNameSys = "\\" + randomStringDrvName + ".sys";
		PATCHdrv = Driver::Utils::getSystem32DriversPath() + DrvNameSys;

#ifdef __VERBOSE
		printf("Random string generated: %s\n", randomStringDrvName.c_str());
		printf("Driver path: %s\n", PATCHdrv.c_str());
#endif

		Driver::Utils::WriteRawDataToFile(PATCHdrv, rdrvData, sizeof(rdrvData));

#ifdef __VERBOSE
		printf("Driver file created\n");
#endif

		if (!Driver::Utils::create_service(PATCHdrv, randomStringDrvName.c_str()))
		{
			Driver::Utils::StopService(randomStringDrvName.c_str());
			Driver::Utils::DeleteServiceA(randomStringDrvName.c_str());
			Driver::Utils::create_service(PATCHdrv, randomStringDrvName.c_str());
		}

		Driver::Utils::start_service(randomStringDrvName.c_str());
		Driver::Utils::torrInv(PATCHdrv.c_str());

		KernelUtilsRunSet();
		Sleep(500);

		if (!ctx->initialize_handle())
		{
			MessageBoxA(NULL, ("Check if you have installed Riot Vanguard, FaceIt and others, than Uninstall!"), "ERROR", MB_ICONERROR | MB_OK);
			Driver::Utils::StopService(randomStringDrvName.c_str());
			Driver::Utils::DeleteServiceA(randomStringDrvName.c_str());
			remove(PATCHdrv.c_str());
			Sleep(1000); exit(0);
		}
	}

	auto core = core::get_instance();
	auto overlay = overlay::get_instance();

	/*if (proc->Attach(L"UAGame.exe")) {
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
			MessageBoxA(NULL, ("Close your game first!"), "ERROR", MB_ICONERROR | MB_OK);
			exit(0);
	}*/

#ifndef ONLY_MENU

#ifdef GTARageMP
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	print( ("[x] Waiting for RageMP\n"));
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	while (!proc->Attach(xor ("GTA5.exe")))
		Sleep(5);
	std::cout << (xor ("[+] Loading cheat..")) << std::endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
	Offsets::get_instance()->SET(xor ("b3274"));
	Sleep(1000);
#endif
#ifdef GTAFiveM

	std::string partialName = xor ("GTAProcess.exe");
	std::string exeName = "";

	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	print(xor ("[x] Waiting for FiveM\n"));
	do {
		exeName = proc->getExecutableName(partialName);
	} while (exeName == "");

	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
	std::string version = proc->extractFiveMVersion(exeName, xor ("_GTAProcess.exe"));
	logger::ExtLog((xor ("GTAV-log.txt")), xor ("Game Version: ") + version);

	if (version == xor("b3095")) {
		DWORD expectedSize = 3508000;
		DWORD fileSize = 0;
		std::string fullPath = proc->getFullPathFromPartialName(exeName);
		if (!fullPath.empty()) {
#ifdef __VERBOSE
			std::cout << "Full path of the executable: " << fullPath << std::endl;
#endif

			HANDLE hFile = CreateFileA(fullPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE) {
#ifdef __VERBOSE
				std::cerr << "Error: Could not open file " << fullPath << ". Error code: " << GetLastError() << std::endl;
#endif
			}
			else {
				fileSize = GetFileSize(hFile, NULL);
				if (fileSize == INVALID_FILE_SIZE) {
#ifdef __VERBOSE
					std::cerr << "Error: Could not get file size. Error code: " << GetLastError() << std::endl;
#endif
				}
				CloseHandle(hFile);
			}
#ifdef __VERBOSE
			std::cout << "Executable size: " << fileSize << " bytes." << std::endl;
			std::cout << "expectedSize size: " << expectedSize << " bytes." << std::endl;
#endif
			if (fileSize == expectedSize) {
				version = xor ("b3258");
			}
		}

#ifdef __VERBOSE
		else {
			std::cerr << "Error: Could not get full path of the executable." << std::endl;
		}
#endif
	}

#ifdef __VERBOSE
	std::cout << xor ("FiveM Exe Name: ") << exeName << std::endl;
	std::cout << xor ("Version: ") << version << std::endl;
#endif
	Offsets::get_instance()->SET(version);

	while (!proc->Attach(exeName))
		Sleep(5);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	std::cout << (xor ("[+] Loading cheat..")) << std::endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
	Sleep(1000);
#endif
#ifdef GTAOnline
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	print(xor ("[x] Waiting for GTA\n"));
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	while (!proc->Attach(L"GTA5.exe"))
		Sleep(5);
	std::cout << (xor ("[+] Loading cheat..")) << std::endl;
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
	Sleep(1000);
#endif

#ifndef __VERBOSE
	FreeConsole();
#endif

#ifndef _NOSAFER
	remove(LoaderName.c_str());
#endif

	auto procBase = proc->getProcBase();

#ifdef __VERBOSE
	print( ("attached: [0x%x] [0x%llx]\n"), proc->getPID(), procBase);
#endif

	Sleep(50);

#else
	while (!proc->Attach(xor ("Notepad.exe")))
		Sleep(2000);

	proc->window = FindWindowA(xor ("Notepad"), nullptr);
	while (!proc->window)
	{
		proc->window = FindWindowA(xor ("Notepad"), nullptr);
		Sleep(5);
	}
#endif

	do {
		EnumWindows(EnumWindowsProc, proc->getPID());
		Sleep(500);
	} while (!proc->window);

	Sleep(1000);

	Beep(100, 100);

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Update, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)UpdateAimbot, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)UpdateSkeletonHealth, 0, 0, 0);


#ifdef __VERBOSE
	print("Creating overlay\n");
#endif

	if (!overlay->create())
	{
		print(xor ("Failed to create window overlay\n"));
		exit(-1);
	}

#ifdef __VERBOSE
	print( ("Overlay created\n"));
#endif

	auto& io = ImGui::GetIO();
	io.ImeWindowHandle = proc->window;

	RECT TempRect{};
	POINT TempPoint{};
	GetClientRect(proc->window, &TempRect);

	proc->windowWidth = TempRect.right;
	proc->windowHeight = TempRect.bottom;

	proc->ChangePriority();

#ifndef ONLY_MENU

#endif

	ConfigManager::LoadConfigFromFile();

	while (true) {
		try {
			auto activeWindow = GetForegroundWindow();

#ifdef __Medal
			if (activeWindow == proc->window || activeWindow == overlay->overlayWindow) {
				HWND TargetWnd = GetWindow(activeWindow, GW_HWNDPREV);
				RECT defRect;
				GetWindowRect(GetDesktopWindow(), &defRect);
				SetWindowPos(overlay->overlayWindow, TargetWnd, defRect.left, defRect.top, defRect.right, defRect.bottom, SWP_SHOWWINDOW);
			}
			else {
				HWND TargetWnd = GetWindow(activeWindow, GW_HWNDPREV);
				SetWindowPos(overlay->overlayWindow, TargetWnd, 0, 0, 0, 0, SWP_SHOWWINDOW);
			}
#endif

#ifdef __NVidiaAMD
			if (activeWindow == proc->window || activeWindow == overlay->overlayWindow) {
				HWND TargetWnd = GetWindow(activeWindow, GW_HWNDPREV);
				RECT defRect;
				GetWindowRect(GetDesktopWindow(), &defRect);
				SetWindowPos(overlay->overlayWindow, TargetWnd, defRect.left, defRect.top, defRect.right, defRect.bottom, SWP_SHOWWINDOW);
			}
			else {
				HWND TargetWnd = GetWindow(activeWindow, GW_HWNDPREV);
				SetWindowPos(overlay->overlayWindow, TargetWnd, 0, 0, 0, 0, SWP_SHOWWINDOW);
			}
#endif

#ifndef __Medal
#ifndef __NVidiaAMD
			if (PeekMessageA(&overlay->msg, overlay->overlayWindow, 0, 0, PM_REMOVE)) {
				TranslateMessage(&overlay->msg);
				DispatchMessage(&overlay->msg);
			}

			if (activeWindow == proc->window) {
				HWND TargetWnd = GetWindow(activeWindow, GW_HWNDPREV);
				SetWindowPos(overlay->overlayWindow, TargetWnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
#endif
#endif

			GetClientRect(proc->window, &TempRect);
			ZeroMemory(&TempPoint, sizeof(TempPoint));
			ClientToScreen(proc->window, &TempPoint);
			TempRect.left = TempPoint.x;
			TempRect.top = TempPoint.y;

			proc->windowLeft = TempRect.left;
			proc->windowTop = TempRect.top;
			proc->windowWidth = TempRect.right;
			proc->windowHeight = TempRect.bottom;

			updateExcludeCaptureState(overlay->overlayWindow, CFG.RageMP.antiscreen);

			overlay->render();

		}
		catch (const std::exception& e) {
#ifdef __VERBOSE
			std::cerr << xor ("Exception: ") << e.what() << std::endl;
#endif
		}
		catch (...) {
#ifdef __VERBOSE
			std::cerr << xor ("Unknown exception occurred") << std::endl;
#endif
		}
	}

	return 0;
}