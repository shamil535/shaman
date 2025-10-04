#include "driver.h"
#include "includes.h"

zenith::communicate_t* zenith::communicate_t::m_instance = nullptr;
zenith::communicate_t* zenith::communicate_t::inst()
{
	if (!m_instance)
		m_instance = new communicate_t();

	return m_instance;
}

INT32 zenith::communicate_t::find_process(PCSTR process_name)
{
	_pid arguments = { NULL };
	INT32 pid = NULL;
	arguments.security = code_security;
	arguments.process_name = process_name;
	arguments.process_id = (INT32*)&pid;

	DeviceIoControl(this->m_handle, code_pid, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

	return pid;
}

auto zenith::communicate_t::initialize_handle() -> bool
{
	// Cria o handle para o dispositivo
	this->m_handle = CreateFileA("\\\\.\\UnknownME", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (!this->m_handle || (this->m_handle == INVALID_HANDLE_VALUE))
		return false;

#ifdef __VERBOSE
	std::cout << xor ("Driver: 0x") << this->m_handle << std::endl;
#endif
	return true;
}

auto zenith::communicate_t::attach(int a_pid) -> bool
{
	// Check if the provided process ID is valid
	if (!a_pid)
		return false;

	// Store the process ID and handle in the class
	this->m_pid = a_pid;

	return true;
}

auto zenith::communicate_t::get_image_base() -> uintptr_t
{
	uintptr_t image_address = { NULL };
	_ba arguments = { NULL };

	arguments.security = code_security;
	arguments.process_id = this->m_pid;
	arguments.address = (ULONGLONG*)&image_address;

	if (!DeviceIoControl(this->m_handle, code_baseADD, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL))
#ifdef __VERBOSE	
		print(xor ("Failed to get image base: 0x%x\n"), GetLastError());
#endif

	return image_address;
}

template <typename T = std::uint64_t>
bool valid_ptr(T ptr) {

	return (ptr && ptr > (T)0xFFFF && ptr < (T)0x00007FFFFFFFFFFF);
}

NTSTATUS zenith::communicate_t::ExcludeCapture(HWND window_handle, uint32_t value)
{
	protect_sprite_content req = { 0 };
	req.security = code_security;
	req.window_handle = reinterpret_cast<uint64_t>(window_handle);
	req.value = value;
	return DeviceIoControl(this->m_handle, code_excludeCapture, &req, sizeof(req), nullptr, NULL, NULL, NULL);
}

NTSTATUS zenith::communicate_t::ExcludeProcess(PCWSTR process_name)
{
	ExcludeProc req = { 0 };
	req.security = code_security;
	req.process_name = process_name;
	return DeviceIoControl(this->m_handle, code_ExcludeProc, &req, sizeof(req), nullptr, NULL, NULL, NULL);
}

auto zenith::communicate_t::write_virtual(
	const std::uintptr_t address,
	void* buffer,
	const std::size_t size) -> bool
{
	if (!valid_ptr(address) || address == NULL)
		return false;

	_rw arguments = { 0 };
	arguments.security = code_security;
	arguments.address = (ULONGLONG)address;
	arguments.buffer = (ULONGLONG)buffer;
	arguments.size = size;
	arguments.process_id = this->m_pid;
	arguments.write = TRUE;

	return DeviceIoControl(this->m_handle, code_VirtRW, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

}

bool zenith::communicate_t::ConnectTo(INT32 a_pid) {
	// Attempt to open the process with required permissions
	this->user_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, a_pid);
	if (this->user_handle == NULL) {
		// Failed to open the process
		return false;
	}
	else
		return true;
}


auto zenith::communicate_t::read_virtual(
	const std::uintptr_t address,
	void* buffer,
	const std::size_t size) -> bool
{
	if (!valid_ptr(address) || address == NULL) {
		buffer = NULL;
		return false;
	}

	std::size_t bytes_remaining = size;
	std::size_t total_bytes_read = 0;
	const std::size_t max_chunk_size = 0xFFFFFFFFF;
	bool success = true;

	while (bytes_remaining > 0) {
		_rw arguments = { 0 };
		std::size_t chunk_size = (bytes_remaining > max_chunk_size) ? max_chunk_size : bytes_remaining;

		arguments.security = code_security;
		arguments.address = (ULONGLONG)(address + total_bytes_read);
		arguments.buffer = (ULONGLONG)((std::uintptr_t)buffer + total_bytes_read);
		arguments.size = chunk_size;
		arguments.process_id = this->m_pid;
		arguments.write = FALSE;

		bool result = DeviceIoControl(this->m_handle, code_VirtRW, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);


		if (!result) {
			success = false;
			break;
		}

		total_bytes_read += chunk_size;
		bytes_remaining -= chunk_size;
	}

	return success;
}

bool MemoryCompare(const BYTE* bData, const BYTE* bMask, const char* szMask) {
	for (; *szMask; ++szMask, ++bData, ++bMask) {
		if (*szMask == 'x' && *bData != *bMask) {
			return false;
		}
	}
	return (*szMask == '\0');
}

uint64_t zenith::communicate_t::FindSignature(DWORD64 start, DWORD64 size, const char* sig, const char* mask) {
	const DWORD64 blockSize = 0x1000; // Definir o tamanho máximo do bloco como 0x1000 (4096 bytes)
	DWORD64 bytesRead = 0;
	uint64_t foundAddress = NULL;
	std::vector<BYTE> data(blockSize); // Uso de vector para gerenciamento automático de memória

	while (bytesRead < size) {
		// Calcular o tamanho do bloco a ser lido (não ultrapassando o limite de 'size' total)
		DWORD64 currentBlockSize = (bytesRead + blockSize > size) ? size - bytesRead : blockSize;

		// Ler o bloco de memória
		if (!this->read_virtual(start + bytesRead, data.data(), currentBlockSize)) {
#ifdef __VERBOSE
			printf(xor ("ERROR Reading Signature at block %llx \n"), start + bytesRead);
#endif
			return NULL;
		}

		// Procurar pela assinatura no bloco lido
		for (DWORD64 i = 0; i < currentBlockSize; i++) {
			if (MemoryCompare(data.data() + i, (const BYTE*)sig, mask)) {
				return start + bytesRead + i; // Retorna o endereço encontrado
			}
		}

		// Atualizar a quantidade de bytes lidos e continuar para o próximo bloco
		bytesRead += currentBlockSize;
	}

	return NULL; // Não encontrou a assinatura
}


uint64_t zenith::communicate_t::get_module_base_address(DWORD process_id, const wchar_t* module_name) {
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);
	if (hModuleSnap == INVALID_HANDLE_VALUE) {
		return 0;
	}

	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);

	if (!Module32First(hModuleSnap, &me32)) {
		CloseHandle(hModuleSnap);
		return 0;
	}

	do {
		if (_tcsicmp(me32.szModule, module_name) == 0) {
			CloseHandle(hModuleSnap);
			return (ULONG64)me32.modBaseAddr;
		}
	} while (Module32Next(hModuleSnap, &me32));

	CloseHandle(hModuleSnap);
	return 0;
}



