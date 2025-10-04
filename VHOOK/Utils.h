#pragma once
#include "includes.h"

#define CHECK_HANDLE(x) (x && x != INVALID_HANDLE_VALUE)
#define CHECK_SC_MANAGER_HANDLE(x, ret_type)												\
if (!CHECK_HANDLE(x))																		\
{																							\
	return ret_type;																		\
}

const std::wstring ALPHANUM = (L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

namespace Driver
{
	namespace Utils
	{
		static bool torrInv(const char* filename)
		{
			return SetFileAttributesA(filename, FILE_ATTRIBUTE_HIDDEN);
		}

		static void WriteRawDataToFile(const std::string& filePath, const unsigned char* rawData, size_t dataSize)
		{
			std::ofstream outputFile(filePath, std::ios::binary);
			if (outputFile.is_open())
			{
				outputFile.write(reinterpret_cast<const char*>(rawData), dataSize);
				outputFile.close();
			}
		}

		static std::string getSystem32DriversPath()
		{
			char system32Path[MAX_PATH];

			UINT length = GetSystemDirectoryA(system32Path, MAX_PATH);
			if (length == 0 || length > MAX_PATH) return std::string();

			std::string driversPath = std::string(system32Path) + "\\drivers";
			return driversPath;
		}

		static SC_HANDLE create_service(const std::string_view driver_path, LPCSTR ServiceName)
		{
#ifdef __VERBOSE
			print(xor ("Creating service\n"));
#endif

			SC_HANDLE sc_manager_handle = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);

			CHECK_SC_MANAGER_HANDLE(sc_manager_handle, (SC_HANDLE)INVALID_HANDLE_VALUE);

			SC_HANDLE mhyprot_service_handle = CreateServiceA(
				sc_manager_handle,
				ServiceName,
				ServiceName,
				SERVICE_START | SERVICE_STOP | DELETE,
				SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE,
				driver_path.data(), nullptr, nullptr, nullptr, nullptr, nullptr
			);

			if (!CHECK_HANDLE(mhyprot_service_handle))
			{
#ifdef __VERBOSE
				print(xor ("Failed to create service\n"));
#endif

				const auto last_error = GetLastError();

				if (last_error == ERROR_SERVICE_EXISTS ||
					last_error == ERROR_SERVICE_MARKED_FOR_DELETE ||
					last_error == ERROR_SERVICE_ALREADY_RUNNING)
				{
					mhyprot_service_handle = OpenServiceA(
						sc_manager_handle,
						ServiceName,
						SERVICE_START | SERVICE_STOP | DELETE | SERVICE_QUERY_STATUS
					);

					if (!CHECK_HANDLE(mhyprot_service_handle))
					{
						CloseServiceHandle(sc_manager_handle);
						return (SC_HANDLE)(INVALID_HANDLE_VALUE);
					}

					SERVICE_STATUS status;

					if (!QueryServiceStatus(mhyprot_service_handle, &status))
					{
						CloseServiceHandle(sc_manager_handle);
						CloseServiceHandle(mhyprot_service_handle);
						return (SC_HANDLE)(INVALID_HANDLE_VALUE);
					}

					if (status.dwCurrentState == SERVICE_STOPPED)
					{
						if (!StartServiceA(mhyprot_service_handle, 0, nullptr))
						{
							CloseServiceHandle(sc_manager_handle);
							CloseServiceHandle(mhyprot_service_handle);
							return (SC_HANDLE)(INVALID_HANDLE_VALUE);
						}

#ifdef __VERBOSE
						print(xor ("Service started\n"));
#endif
					}
					else if (status.dwCurrentState != SERVICE_RUNNING)
					{
						CloseServiceHandle(sc_manager_handle);
						CloseServiceHandle(mhyprot_service_handle);
						return (SC_HANDLE)(INVALID_HANDLE_VALUE);
					}

					CloseServiceHandle(sc_manager_handle);
					return mhyprot_service_handle;
				}
				CloseServiceHandle(sc_manager_handle);
				return (SC_HANDLE)(INVALID_HANDLE_VALUE);
			}

			CloseServiceHandle(sc_manager_handle);

			return mhyprot_service_handle;
		}

		static bool StopService(const char* serviceName)
		{
			SC_HANDLE hSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			if (hSCManager == NULL) {
				return false;
			}

			SC_HANDLE hService = OpenServiceA(hSCManager, serviceName, SERVICE_STOP | SERVICE_QUERY_STATUS);
			if (hService == NULL) {
				DWORD error = GetLastError();
				CloseServiceHandle(hSCManager);
				return false;
			}

			SERVICE_STATUS status;
			ControlService(hService, SERVICE_CONTROL_STOP, &status);
			CloseServiceHandle(hService);
			CloseServiceHandle(hSCManager);
			return true;
		}

		static bool IsProcessRunning(const wchar_t* processName) {
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

			bool found = false;
			do {
				if (wcscmp(pe.szExeFile, processName) == 0) {
					found = true;
					break;
				}
			} while (Process32Next(hSnapshot, &pe));

			CloseHandle(hSnapshot);
			return found;
		}

		static bool IsServiceRunning(const wchar_t* serviceName) {
			SC_HANDLE scm = OpenSCManager(nullptr, nullptr, SC_MANAGER_CONNECT);
			if (!scm) {
				return false;
			}

			SC_HANDLE service = OpenServiceW(scm, serviceName, SERVICE_QUERY_STATUS);
			if (!service) {
				CloseServiceHandle(scm);
				return false;
			}

			SERVICE_STATUS_PROCESS status;
			DWORD bytesNeeded;
			if (!QueryServiceStatusEx(service, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&status),
				sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded)) {
				CloseServiceHandle(service);
				CloseServiceHandle(scm);
				return false;
			}

			bool running = (status.dwCurrentState == SERVICE_RUNNING);

			CloseServiceHandle(service);
			CloseServiceHandle(scm);

			return running;
		}

		static void DeleteServiceA(const char* serviceName)
		{
			// Obter um handle para o gerenciador de controle de serviço
			SC_HANDLE hSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			if (hSCManager == NULL) {
				return;
			}

			// Obter um handle para o serviço
			SC_HANDLE hService = OpenServiceA(hSCManager, serviceName, DELETE);
			if (hService == NULL) {
				CloseServiceHandle(hSCManager);
				return;
			}

			// Deletar o serviço
			DeleteService(hService);


			// Fechar os handles
			CloseServiceHandle(hService);
			CloseServiceHandle(hSCManager);
		}

		static void start_service(const char* serviceName)
		{
#ifdef __VERBOSE
			print("Starting service\n");
#endif

			// Abre um handle para o gerenciador de controle de serviços
			SC_HANDLE schSCManager = OpenSCManagerA(
				NULL,                    // Máquina local
				NULL,                    // Banco de dados de serviços por padrão
				SC_MANAGER_ALL_ACCESS);  // Acesso completo

			if (schSCManager == NULL) {
				return;
			}

			// Abre um handle para o serviço especificado
			SC_HANDLE schService = OpenServiceA(
				schSCManager,          // Handle para o gerenciador de controle de serviços
				serviceName,           // Nome do serviço a ser iniciado
				SERVICE_START);        // Acesso para iniciar o serviço

			if (schService == NULL) {
				CloseServiceHandle(schSCManager);
				return;
			}

			SERVICE_STATUS status;
			// Inicia o serviço
			StartServiceA(schService, 0, NULL);

#ifdef __VERBOSE
			print("Service started\n");
#endif

			// Limpeza
			CloseServiceHandle(schService);
			CloseServiceHandle(schSCManager);
		}
	}
}