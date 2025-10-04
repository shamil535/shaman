#pragma once

class process
{
private:
	static process* instance;

	DWORD pid = 0;
	uint64_t procBase = 0;

public:
	static process* get_instance();
	std::string FiveMVersion;
	bool Attach(std::string processName);
	DWORD getPID();
	uintptr_t getProcBase();
	bool isRunning(DWORD pid);
	std::string getExecutableName(const std::string& partialName);
	std::string extractFiveMVersion(const std::string& exeName, std::string suffix);
	bool terminarProcessoPeloNome(const std::wstring& nomeDoProcesso);
	std::string randomString(int length);
	void ChangePriority();

	std::string getFullPathFromPartialName(const std::string& partialName);

	HWND window = 0;
	int windowWidth;
	int windowHeight;
	int windowLeft;
	int windowTop;
};
