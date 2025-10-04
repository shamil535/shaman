#include "includes.h"
#include "process.h"
#include "ntos.h"
#include "driver.h"
#include "core.h"

bool process::isRunning(DWORD pid)
{
	HANDLE pss = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe = { 0 };
	pe.dwSize = sizeof(pe);

	if (Process32First(pss, &pe))
	{
		do
		{
			// pe.szExeFile can also be useful  
			if (pe.th32ProcessID == pid)
				return true;
		} while (Process32Next(pss, &pe));
	}

	CloseHandle(pss);

	return false;
}

bool process::Attach(std::string processName)
{
	this->pid = zenith::communicate_t::inst()->find_process(processName.c_str());

#ifdef ONLY_MENU
	return this->pid;
#endif	

	zenith::communicate_t::inst()->attach(this->pid);

	return (this->pid);
}

DWORD process::getPID()
{
	return this->pid;
}

uintptr_t process::getProcBase()
{
	if (this->procBase)
		return this->procBase;

	this->procBase = zenith::communicate_t::inst()->get_image_base();

	return this->procBase;
}

process* process::instance = 0;
process* process::get_instance()
{
	if (!instance)
		instance = new process();
	return instance;
}

std::string process::getFullPathFromPartialName(const std::string& exeName) {
	char appDataPath[MAX_PATH];
	HRESULT result = SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, appDataPath);
	if (result != S_OK) {
#ifdef __VERBOSE
		std::cerr << "Error: Could not get AppData path. Error code: " << result << std::endl;
#endif // __VERBOSE
		return "";
	}

	std::string fullPath = std::string(appDataPath) + xor ("\\FiveM\\FiveM.app\\data\\cache\\subprocess\\") + exeName;
	return fullPath;
}

std::string process::getExecutableName(const std::string& partialName)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32W pe32;
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
#ifdef __VERBOSE
		std::cerr << "CreateToolhelp32Snapshot (of processes) failed." << std::endl;
#endif // __VERBOSE
		return "";
	}

	pe32.dwSize = sizeof(PROCESSENTRY32W);

	if (!Process32FirstW(hProcessSnap, &pe32))
	{
#ifdef __VERBOSE
		std::cerr << "Process32FirstW failed." << std::endl;
#endif // __VERBOSE
		CloseHandle(hProcessSnap);
		return "";
	}

	do
	{
		std::wstring exeFileW(pe32.szExeFile);
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::string exeFile = converter.to_bytes(exeFileW);

		if (exeFile.find(partialName) != std::string::npos)
		{
			CloseHandle(hProcessSnap);
			return exeFile;
		}
	} while (Process32NextW(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return "";
}

std::string process::extractFiveMVersion(const std::string& exeName, std::string suffix)
{
	std::string prefix = "FiveM_";
	size_t start = exeName.find(prefix);
	size_t end = exeName.find(suffix);

	if (start == std::string::npos || end == std::string::npos || start >= end)
	{
		return ""; // Não encontrado
	}

	start += prefix.length();
	this->FiveMVersion = exeName.substr(start, end - start);
	return exeName.substr(start, end - start);
}

bool process::terminarProcessoPeloNome(const std::wstring& nomeDoProcesso) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE) {
		return false;
	}

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe)) {
		CloseHandle(hSnapshot);
		return false;
	}

	do {
		if (wcscmp(pe.szExeFile, nomeDoProcesso.c_str()) == 0) {
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe.th32ProcessID);
			if (hProcess != NULL) {
				TerminateProcess(hProcess, 0);
				CloseHandle(hProcess);
			}
		}

	} while (Process32Next(hSnapshot, &pe));

	CloseHandle(hSnapshot);
	return true;
}

std::string process::randomString(int length)
{
	srand(time(0));

	std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
	std::string newstr;
	for (int i = 0; i < length; ++i)
	{
		newstr += str[rand() % str.size()];
	}
	return newstr;
}

void process::ChangePriority()
{
	SetPriorityClass((HANDLE)-1, REALTIME_PRIORITY_CLASS);
	SetThreadPriority((HANDLE)-2, THREAD_BASE_PRIORITY_MAX);
}