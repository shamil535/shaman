#include "KrnlSymUm.h"
#include "Utils.h"
#include "dbghelpRaw.h"
#include "symsrvRaw.h"
#include "includes.h"

void ResolveImports() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	std::string executable_path(buffer);
	std::string directory = executable_path.substr(0, executable_path.find_last_of("\\/"));
	std::string dbghelpdll = directory + "\\dbghelp.dll";
	std::string symsrvdll = directory + "\\symsrv.dll";

	remove(dbghelpdll.c_str());
	remove(symsrvdll.c_str());

	Driver::Utils::WriteRawDataToFile(dbghelpdll, dbgrawData, sizeof(dbgrawData));
	Driver::Utils::WriteRawDataToFile(symsrvdll, sysmrawData, sizeof(sysmrawData));

	SetFileAttributesA(dbghelpdll.c_str(), FILE_ATTRIBUTE_HIDDEN);
	SetFileAttributesA(symsrvdll.c_str(), FILE_ATTRIBUTE_HIDDEN);
}

void KernelUtilsRunSet()
{
	DWORD64 LoadAddress;
	DWORD32 Success = FALSE;

	//Initialize the symbol handler
	LoadAddress = InitSymbols();
	if (!LoadAddress)
		return;

	//Resolve and send the RVAs of the used kernel routines to the driver
	if (!ResolveRoutinesForKernelModule())
	{
#ifdef __VERBOSE
		printf("ResolveRoutinesForKernelModule failed !\n");
#endif
		goto cleanup;
	}

	//Resolve and send the offsets of structure fields to the driver
	if (!ResolveStructureOffsetsForKernelModule(LoadAddress))
	{
#ifdef __VERBOSE
		printf("ResolveStructureOffsetsForKernelModule failed !\n");
#endif
		goto cleanup;
	}

#ifdef __VERBOSE
	printf("Load Symbols Success !\n");
#endif

	Success = TRUE;

cleanup:
	//Tell the kernel-mode that we're done resolving so it can cleanup accordingly
	DoneResolving(Success);
	SymCleanup(UQ_HANDLE);
}

DWORD64 InitSymbols()
{
	DWORD64 LoadAddress;
	CHAR NtoskrnlPath[INFO_BUFFER_SIZE];
	ULONG SystemInformationLength;
	PRTL_PROCESS_MODULES ProcessModuleInformation;
	t_NtQuerySystemInformation NtQuerySystemInformation;
	NTSTATUS Status;

	NtQuerySystemInformation = (t_NtQuerySystemInformation)GetProcAddress(LoadLibraryA("ntdll.dll"), "NtQuerySystemInformation");
	Status = NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemModuleInformation, NULL, 0, &SystemInformationLength);
	if (Status == STATUS_INFO_LENGTH_MISMATCH)
	{
		ProcessModuleInformation = (PRTL_PROCESS_MODULES) LocalAlloc(LMEM_ZEROINIT, SystemInformationLength);
		Status = NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemModuleInformation, ProcessModuleInformation, SystemInformationLength, NULL);
		if (!NT_SUCCESS(Status))
		{
			LocalFree(ProcessModuleInformation);
			return NULL;
		}
	}
	else
	{
		return NULL;
	}

	if (!SymInitialize(UQ_HANDLE, "srv*c:\\DownstreamStore*https://msdl.microsoft.com/download/symbols", FALSE))
	{
#ifdef __VERBOSE
		printf("SymInitialize failed\n");
#endif
		return FALSE;
	}

	//Get the path to the kernel's executable (ntoskrnl, ntkrnlmp etc.)
	GetSystemDirectoryA(NtoskrnlPath, INFO_BUFFER_SIZE);
	strcat(NtoskrnlPath, (CHAR*)&ProcessModuleInformation[0].Modules[0].FullPathName[ProcessModuleInformation[0].Modules[0].OffsetToFileName] - 1);

#ifdef __VERBOSE
	printf("Contacting the symbol server, please wait ...\n");
#endif

	LoadAddress = SymLoadModuleEx(UQ_HANDLE, NULL, NtoskrnlPath, NULL, NULL, 0, NULL, 0);

	if (!LoadAddress)
	{
#ifdef __VERBOSE
		printf("SymLoadModuleEx failed\n");
#endif
		LocalFree(ProcessModuleInformation);
		SymCleanup(UQ_HANDLE);
		return NULL;
	}

#ifdef __VERBOSE
	printf("Loading done ...\n");
#endif

	LocalFree(ProcessModuleInformation);
	return LoadAddress;
}

VOID DoneResolving(DWORD32 Success)
{
	HANDLE hDevice;
	DWORD BytesReturned;

	//Open a handle to the device
	hDevice = CreateFileA(KM_DEVICE, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
#ifdef __VERBOSE
		printf("CreateFile failed !\n");
#endif
		return;
	}
	
	//Tell the kernel-mode that we're done
	if (!DeviceIoControl(hDevice, IOCTL_DONE_RESOLVING, &Success, sizeof(DWORD32), NULL, 0, &BytesReturned, NULL))
	{
#ifdef __VERBOSE
		printf("IOCTL_DONE_RESOLVING failed !\n");
#endif
		goto cleanup;
	}

cleanup:
	CloseHandle(hDevice);
}

BOOL ResolveRoutinesForKernelModule()
{
	HANDLE hDevice;
	ULONG RoutinesInfo[2]; // { Length of the concatenated routine names , Number of routines };
	ULONG RoutineNamesLength, NumberOfRoutines, i_NumberOfRoutines;
	DWORD BytesReturned;
	PCHAR RoutineNames = NULL, i_RoutineNames;
	PULONG64 RVAs = NULL, i_RVAs;
	BOOL Status = FALSE;

	//Open a handle to the device
	hDevice = CreateFileA(KM_DEVICE, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
#ifdef __VERBOSE
		printf("CreateFile failed !\n");
#endif
		return Status;
	}

	//Get the total length of the kernel function names
	if (!DeviceIoControl(hDevice, IOCTL_RETURN_KR_LENGTH, NULL, 0, RoutinesInfo, sizeof(ULONG)*2, &BytesReturned, NULL) || BytesReturned != sizeof(ULONG)*2)
	{
#ifdef __VERBOSE
		printf("IOCTL_RETURN_KR_LENGTH failed !\n");
#endif
		goto cleanup;
	}

	//The values returned from the kernel module
	RoutineNamesLength = RoutinesInfo[0];
	NumberOfRoutines = RoutinesInfo[1];

	if (NumberOfRoutines == 0)
	{
		Status = TRUE;
		goto cleanup;
	}
	
	//Allocate memory for the concatenated routine names
	RoutineNames = (PCHAR)LocalAlloc(LMEM_ZEROINIT, RoutineNamesLength);
	
	if (!DeviceIoControl(hDevice, IOCTL_RETURN_ROUTINE_NAMES, NULL, 0, RoutineNames, RoutineNamesLength, &BytesReturned, NULL) || BytesReturned != RoutineNamesLength)
	{
#ifdef __VERBOSE
		printf("IOCTL_RETURN_ROUTINE_NAMES failed !\n");
#endif
		goto cleanup;
	}

	//Allocate memory for the resolved RVA to the kernel routines
	RVAs = (ULONG64*)LocalAlloc(LMEM_ZEROINIT, NumberOfRoutines * sizeof(ULONG64));

	//Resolve the RVAs
	i_RoutineNames = RoutineNames;
	i_NumberOfRoutines = NumberOfRoutines;
	i_RVAs = RVAs;
	while (i_NumberOfRoutines)
	{
		*i_RVAs++ = GetKernelRoutineRVA(i_RoutineNames);
		i_RoutineNames += strlen(i_RoutineNames) + 1;
		i_NumberOfRoutines--;
	}

	//Send the resulting RVAs to the kernel module
	if (!DeviceIoControl(hDevice, IOCTL_SET_RESOLVED_RVAS, RVAs, sizeof(ULONG64)*NumberOfRoutines, NULL, 0, &BytesReturned, NULL))
	{
#ifdef __VERBOSE
		printf("IOCTL_SET_RESOLVED_RVAS failed !\n");
#endif
		goto cleanup;
	}

	//Success
	Status = TRUE;

cleanup:
	if (RoutineNames)
		LocalFree(RoutineNames);
	if (RVAs)
		LocalFree(RVAs);
	CloseHandle(hDevice);

	return Status;
}

ULONG64 GetKernelRoutineRVA(CHAR* Name)
{
	SYMBOL_INFO SymInfo = { 0 };

	SymInfo.SizeOfStruct = sizeof(SYMBOL_INFO);
	SymInfo.MaxNameLen = 0;

	//Get the VA inside the kernel routine by its name
	if (!SymFromName(UQ_HANDLE, Name, &SymInfo))
	{
#ifdef __VERBOSE
		printf("SymFromName failed\n");
#endif
		return INVALID_RVA;
	}

	//Return the RVA
	return SymInfo.Address - SymInfo.ModBase;
}


BOOL ResolveStructureOffsetsForKernelModule(DWORD64 LoadAddress)
{
	HANDLE hDevice;
	ULONG FieldsInfo[2];
	ULONG FieldNamesLength, NumberOfFields, i_NumberOfFields, i_FieldLength;
	PFIELD_DATA Offsets = NULL, i_Offsets;
	DWORD BytesReturned;
	PCHAR FieldNames = NULL, i_FieldNames;
	BOOL Status = FALSE;

	//Open a handle to the device
	hDevice = CreateFileA(KM_DEVICE, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
#ifdef __VERBOSE
		printf("CreateFile failed !\n");
#endif
		return Status;
	}

	//Get the total length of the structures names
	if (!DeviceIoControl(hDevice, IOCTL_RETURN_SF_LENGTH, NULL, 0, FieldsInfo, sizeof(ULONG) * 2, &BytesReturned, NULL) || BytesReturned != sizeof(ULONG) * 2)
	{
#ifdef __VERBOSE
		printf("IOCTL_RETURN_SF_LENGTH failed !\n");
#endif
		goto cleanup;
	}

	FieldNamesLength = FieldsInfo[0];
	NumberOfFields = FieldsInfo[1];

	if (NumberOfFields == 0)
	{
		Status = TRUE;
		goto cleanup;
	}

	//Allocate memory for the concatenated field names
	FieldNames = (PCHAR)LocalAlloc(LMEM_ZEROINIT, FieldNamesLength);

	if (!DeviceIoControl(hDevice, IOCTL_RETURN_FIELDS, NULL, 0, FieldNames, FieldNamesLength, &BytesReturned, NULL) || BytesReturned != FieldNamesLength)
	{
#ifdef __VERBOSE
		printf("IOCTL_RETURN_FIELDS failed !\n");
#endif
		goto cleanup;
	}

	//Allocate memory for the field offsets
	Offsets = (PFIELD_DATA)LocalAlloc(LMEM_ZEROINIT, NumberOfFields * sizeof(ULONG64));

	//Loop through the structure field and resolve their offsets
	i_FieldNames = FieldNames;
	i_NumberOfFields = NumberOfFields;
	i_Offsets = Offsets;
	while (i_NumberOfFields)
	{
		i_FieldLength = strlen(i_FieldNames);
		*i_Offsets++ = GetKernelStructureFieldOffset(i_FieldNames, LoadAddress);
		i_FieldNames += i_FieldLength + 1;
		i_NumberOfFields--;
	}

	//Send the resulting offsets to the kernel module
	if (!DeviceIoControl(hDevice, IOCTL_SET_RESOLVED_OFFSETS, Offsets, sizeof(FIELD_DATA)*NumberOfFields, NULL, 0, &BytesReturned, NULL))
	{
#ifdef __VERBOSE
		printf("IOCTL_SET_RESOLVED_OFFSETS failed !\n");
#endif
		goto cleanup;
	}

	//Success
	Status = TRUE;

cleanup:
	if (FieldNames)
		LocalFree(FieldNames);
	if (Offsets)
		LocalFree(Offsets);
	CloseHandle(hDevice);
	return Status;
}


FIELD_DATA GetKernelStructureFieldOffset(PCHAR StructureField, DWORD64 LoadAddress)
/*
StructureField : A string containing the structure and the nested field get an offset to. e.g. _EPROCESS.ApcState.Process
LoadAddress    : The base returned from SymLoadModuleEx
*/
{
	PWCHAR wStructureField = NULL, StructureName, FieldName, pSymbolName = NULL;
	TI_FINDCHILDREN_PARAMS* ChildrenTypeIndexes = NULL;
	DWORD NumberOfChildren, TypeIndex, BitPosition, Size, CurrentFieldOffset, SymTag;
	ULONG iChild, LastChildId, MbSize;
	SYMBOL_INFOW SymInfo = { 0 };
	FIELD_DATA FieldData = { 0 };

	SymInfo.SizeOfStruct = sizeof(SYMBOL_INFOW);
	SymInfo.MaxNameLen = 0;

	
	MbSize = MultiByteToWideChar(CP_UTF8, 0, StructureField, -1, NULL, 0);
	if (MbSize == 0)
	{
		FieldData.Data = INVALID_OFFSET;
		return FieldData;
	}

	wStructureField = (PWCHAR) LocalAlloc(LMEM_ZEROINIT, MbSize*sizeof(WCHAR));
	if (MultiByteToWideChar(CP_UTF8, 0, StructureField, -1, wStructureField, MbSize) == 0)
	{
		FieldData.Data = INVALID_OFFSET;
		LocalFree(wStructureField);
		return FieldData;
	}

	StructureName = wcstok(wStructureField, L".", NULL);

	//Get the type index for the symbol
	if (!SymGetTypeFromNameW(UQ_HANDLE, LoadAddress, StructureName, &SymInfo))
	{
#ifdef __VERBOSE
		printf("SymGetTypeFromName failed\n");
#endif
		FieldData.Data = INVALID_OFFSET;
		if (wStructureField)
			LocalFree(wStructureField);
		return FieldData;
	}

	TypeIndex = SymInfo.TypeIndex;
	while ( (FieldName = wcstok(NULL, L".", NULL)) != NULL)
	{
		//Get the type of the field
		if (!SymGetTypeInfo(UQ_HANDLE, LoadAddress, TypeIndex, TI_GET_SYMTAG, &SymTag))
		{
#ifdef __VERBOSE
			printf("TI_GET_SYMTAG failed\n");
#endif
			FieldData.Data = INVALID_OFFSET;
			goto cleanup;
		}

		//If the field type is a pointer, we fail here before getting the children. The function only resolves nested structure fields.
		if (SymTag == SymTagPointerType)
		{
#ifdef __VERBOSE
			printf("Error: Please only supply nested structure fields\n");
#endif
			FieldData.Data = INVALID_OFFSET;
			goto cleanup;
		}

		//Use the type index to get the number of children for the symbol
		if (!SymGetTypeInfo(UQ_HANDLE, LoadAddress, TypeIndex, TI_GET_CHILDRENCOUNT, &NumberOfChildren) || NumberOfChildren == 0 )
		{
#ifdef __VERBOSE
			printf("TI_GET_CHILDRENCOUNT failed\n");
#endif
			FieldData.Data = INVALID_OFFSET;
			goto cleanup;
		}

		//Get the type index for each child
		ChildrenTypeIndexes = (TI_FINDCHILDREN_PARAMS*)LocalAlloc(LMEM_ZEROINIT, sizeof(TI_FINDCHILDREN_PARAMS) + sizeof(ULONG)*(NumberOfChildren - 1));
		ChildrenTypeIndexes->Count = NumberOfChildren;

		//Get all the fields
		if (!SymGetTypeInfo(UQ_HANDLE, LoadAddress, TypeIndex, TI_FINDCHILDREN, ChildrenTypeIndexes))
		{
#ifdef __VERBOSE
			printf("TI_FINDCHILDREN failed\n");
#endif
			FieldData.Data = INVALID_OFFSET;
			goto cleanup;
		}

		//Lookup the field name
		for (iChild = ChildrenTypeIndexes->Start; iChild < ChildrenTypeIndexes->Count; iChild++)
		{
			if (!SymGetTypeInfo(UQ_HANDLE, LoadAddress, ChildrenTypeIndexes->ChildId[iChild], TI_GET_SYMNAME, &pSymbolName))
			{
#ifdef __VERBOSE
				printf("TI_GET_SYMNAME failed\n");
#endif
				FieldData.Data = INVALID_OFFSET;
				goto cleanup;
			}

			//Is this the field we're looking for ?
			if (!wcscmp(pSymbolName, FieldName))
			{
				//Get its offset
				if (!SymGetTypeInfo(UQ_HANDLE, LoadAddress, ChildrenTypeIndexes->ChildId[iChild], TI_GET_OFFSET, &CurrentFieldOffset))
				{
#ifdef __VERBOSE
					printf("TI_GET_OFFSET failed\n");
#endif
					FieldData.Data = INVALID_OFFSET;
					goto cleanup;
				}
				
				//Add the offset to the previous value (nested structure fields)
				FieldData.Offset += CurrentFieldOffset;
				LastChildId = ChildrenTypeIndexes->ChildId[iChild];

				//Update TypeIndex with the type index of this field for the next iteration
				if (!SymGetTypeInfo(UQ_HANDLE, LoadAddress, ChildrenTypeIndexes->ChildId[iChild], TI_GET_TYPEID, &TypeIndex))
				{
#ifdef __VERBOSE
					printf("TI_GET_TYPEID failed\n");
#endif
					FieldData.Data = INVALID_OFFSET;
					goto cleanup;
				}
			}
		}
	cleanup:
		if (pSymbolName)
		{
			LocalFree(pSymbolName);
			pSymbolName = NULL;
		}
		if (ChildrenTypeIndexes)
		{
			LocalFree(ChildrenTypeIndexes);
			ChildrenTypeIndexes = NULL;
		}
		if (FieldData.Data == INVALID_OFFSET)
		{
			if ( wStructureField )
				LocalFree(wStructureField);
			return FieldData;
		}		
	}

	//Get the size of the type (an exception is when dealing with a bitfield which we solve in the next few lines).
	SymGetTypeInfo(UQ_HANDLE, LoadAddress, TypeIndex, TI_GET_LENGTH, &Size);

	//Encode the size of the field
	FieldData.Size = Size;
	
	//Is this a bitfield bit ?
	if ( SymGetTypeInfo(UQ_HANDLE, LoadAddress, LastChildId, TI_GET_BITPOSITION, &BitPosition) )
	{
		//In case this is a bitfield, we need to set the bitcount using the child id and not the type index
		SymGetTypeInfo(UQ_HANDLE, LoadAddress, LastChildId, TI_GET_LENGTH, &Size);

		//Update the size with the bitcount
		FieldData.Size = Size;
		
		//Encode the bit position
		FieldData.BitPosition = BitPosition;
	}

	if (wStructureField)
		LocalFree(wStructureField);
	
	return FieldData;
}
