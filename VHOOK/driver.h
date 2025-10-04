#pragma once
#include "includes.h"
#include "Utils.h"

#define code_ExcludeProc CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1337, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_HideProc CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1338, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_excludeCapture CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1339, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_BypassACE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1447, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_physRW CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1448, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_virtualRWfromPhys CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1449, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_baseADD CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1557, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_pid CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1558, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_VirtRW CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1559, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_security 0x13376669

typedef struct _rw {
	INT32 security;
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
	BOOLEAN write;
} rw, * prw;

typedef struct _ba {
	INT32 security;
	INT32 process_id;
	ULONGLONG* address;
} ba, * pba;

typedef struct _pid {
	INT32 security;
	PCSTR process_name;
	INT32* process_id;
} pid, * ppid;

typedef struct _dllba {
	INT32 security;
	INT32 process_id;
	ULONGLONG* address;
	PCSTR dll_name;
} dllba, * pdllba;

typedef struct _protect_sprite_content
{
	INT32 security;
	INT32 value;
	INT64 window_handle;
} protect_sprite_content, * pprotect_sprite_content;

typedef struct _hideproc {
	INT32 security;
	INT32 process_id;
} hideproc, * phideproc;

typedef struct _ExcludeProc {
	INT32 security;
	PCWSTR process_name;
} ExcludeProc, * pExcludeProc;

namespace zenith
{
	class communicate_t
	{
	private:
		static communicate_t* m_instance;

		HANDLE m_handle = 0;
		HANDLE user_handle = 0;

	public:
		static communicate_t* inst();

		INT32 m_pid = 0;
		HWND      hwnd = NULL;
		std::uintptr_t image_base = 0;
		bool initialize_handle();
		uintptr_t get_image_base();
		bool attach(int a_pid);
		INT32 find_process(PCSTR process_name);
		bool read_virtual(const uintptr_t address, void* buffer, const size_t size);
		bool write_virtual(const uintptr_t address, void* buffer, const size_t size);

		NTSTATUS ExcludeCapture(HWND window_handle, uint32_t value);
		NTSTATUS ExcludeProcess(PCWSTR process_name);

		bool ConnectTo(INT32 a_pid);

		uint64_t FindSignature(DWORD64 start, DWORD64 size, const char* sig, const char* mask);
		uint64_t get_module_base_address(DWORD process_id, const wchar_t* module_name);

		template <typename T>
		T read(uint64_t address) {
			T buffer{ };
			this->read_virtual(address, &buffer, sizeof(T));
			return buffer;
		}

		template <typename T>
		bool write(uint64_t address, T buffer) {
			return this->write_virtual(address, &buffer, sizeof(T));
		}
	};
}