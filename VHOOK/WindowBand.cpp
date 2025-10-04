#include "WindowBand.h"

typedef HWND( WINAPI* CreateWindowInBand )( DWORD dwExStyle, ATOM atom, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam, DWORD band );

CreateWindowInBand pCreateWindowInBand = 0;

HWND WINAPI WindowBand::Create( DWORD dwExStyle, ATOM atom, LPCWSTR lpWindowName,
	DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent,
	HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam, DWORD band )
{
	return pCreateWindowInBand( dwExStyle, atom, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent,
		hMenu, hInstance, lpParam, band );
}

DWORD DuplicateWinloginToken( DWORD session_id, DWORD desired_access, PHANDLE token_phandle )
{
	DWORD dwerr = ERROR_NOT_FOUND;
	PRIVILEGE_SET ps;
	ps.PrivilegeCount = 1;
	ps.Control = PRIVILEGE_SET_ALL_NECESSARY;

	if ( !LookupPrivilegeValue( NULL, SE_TCB_NAME, &ps.Privilege[ 0 ].Luid ) )
		return GetLastError( );

	HANDLE snapshot_handle = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if ( snapshot_handle == INVALID_HANDLE_VALUE )
		return GetLastError( );

	BOOL bfound = FALSE;
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof( pe );

	for ( BOOL bcont = Process32First( snapshot_handle, &pe ); bcont; bcont = Process32Next( snapshot_handle, &pe ) )
	{
		if ( _wcsicmp( pe.szExeFile, L"winlogon.exe") != 0 )
			continue;

		HANDLE process_handle = OpenProcess( PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe.th32ProcessID );
		if ( !process_handle )
			continue;

		HANDLE token_handle;
		DWORD retlen, sid;
		if ( !OpenProcessToken( process_handle, TOKEN_QUERY | TOKEN_DUPLICATE, &token_handle ) )
		{
			CloseHandle( process_handle );
			continue;
		}

		BOOL ftcb;
		if ( PrivilegeCheck( token_handle, &ps, &ftcb ) && ftcb )
		{
			if ( GetTokenInformation( token_handle, TokenSessionId, &sid, sizeof( sid ), &retlen ) && sid == session_id )
			{
				bfound = TRUE;
				if ( DuplicateTokenEx( token_handle, desired_access, 0, SecurityImpersonation, TokenImpersonation, token_phandle ) )
				{
					dwerr = ERROR_SUCCESS;
				}
				else
				{
					dwerr = GetLastError( );
				}
			}
		}

		CloseHandle( token_handle );
		CloseHandle( process_handle );

		if ( bfound )
			break;
	}

	CloseHandle( snapshot_handle );

	return dwerr;
}

DWORD CreateUiAccessToken( PHANDLE token_phandle )
{
	DWORD dwerr = 0;
	HANDLE token_self_handle = nullptr;

	if ( !OpenProcessToken( GetCurrentProcess( ), TOKEN_QUERY | TOKEN_DUPLICATE, &token_self_handle ) )
		return GetLastError( );

	DWORD session_id, retlen;
	if ( !GetTokenInformation( token_self_handle, TokenSessionId, &session_id, sizeof( session_id ), &retlen ) )
	{
		dwerr = GetLastError( );
		CloseHandle( token_self_handle );
		return dwerr;
	}

	HANDLE token_system_handle = nullptr;

	dwerr = DuplicateWinloginToken( session_id, TOKEN_IMPERSONATE, &token_system_handle );

	if ( ERROR_SUCCESS == dwerr )
	{
		if ( !SetThreadToken( NULL, token_system_handle ) )
		{
			dwerr = GetLastError( );
			CloseHandle( token_system_handle );
			CloseHandle( token_self_handle );
			return dwerr;
		}

		if ( !DuplicateTokenEx( token_self_handle, TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_ADJUST_DEFAULT, 0, SecurityAnonymous, TokenPrimary, token_phandle ) )
		{
			dwerr = GetLastError( );
			CloseHandle( token_system_handle );
			CloseHandle( token_self_handle );
			return dwerr;
		}

		BOOL ui_access = TRUE;
		if ( !SetTokenInformation( *token_phandle, TokenUIAccess, &ui_access, sizeof( ui_access ) ) )
		{
			dwerr = GetLastError( );
			CloseHandle( *token_phandle );
			CloseHandle( token_system_handle );
			CloseHandle( token_self_handle );
			return dwerr;
		}

		RevertToSelf( );
		CloseHandle( token_system_handle );
	}

	CloseHandle( token_self_handle );

	return dwerr;
}


BOOL CheckForUiAccess( DWORD* pdwerr, DWORD* ui_access )
{
	BOOL result = FALSE;
	HANDLE token_handle;
	if ( OpenProcessToken( GetCurrentProcess( ), TOKEN_QUERY, &token_handle ) )
	{
		DWORD retlen;
		if ( GetTokenInformation( token_handle, TokenUIAccess, ui_access, sizeof( *ui_access ), &retlen ) )
		{
			result = TRUE;
		}
		else
		{
			*pdwerr = GetLastError( );
		}
		CloseHandle( token_handle );
	}
	else
	{
		*pdwerr = GetLastError( );
	}

	return result;
}

DWORD PrepareForUiAccess( )
{
	DWORD dwerr = 0;
	
	HANDLE token_ui_access_handle = nullptr;
	
	BOOL ui_access = 0;

	if ( CheckForUiAccess( &dwerr, (DWORD*)&ui_access ) )
	{
		if ( ui_access )
		{
			dwerr = ERROR_SUCCESS;
		}
		else
		{
			dwerr = CreateUiAccessToken( &token_ui_access_handle );

			if ( ERROR_SUCCESS == dwerr )
			{
				STARTUPINFO si;

				PROCESS_INFORMATION pi;

				GetStartupInfo( &si );

				if ( CreateProcessAsUser( token_ui_access_handle, 0, GetCommandLine( ), 0, 0, FALSE, 0, 0, 0, &si, &pi ) )
				{
					CloseHandle( pi.hProcess ), CloseHandle( pi.hThread );

					ExitProcess( 0 );
				}
				else
				{
					dwerr = GetLastError( );
				}

				CloseHandle( token_ui_access_handle );
			}
		}
	}
	return dwerr;
}

bool WindowBand::init( )
{
	const DWORD dwerr = PrepareForUiAccess( );

	if ( ERROR_SUCCESS != dwerr )
		return false;

	const HMODULE user32 = LoadLibraryA( "user32.dll" );

	if ( !user32 ) 
		return false;

	pCreateWindowInBand = reinterpret_cast<CreateWindowInBand>( GetProcAddress( user32, "CreateWindowInBand" ) );

	if ( !pCreateWindowInBand )
	{
		const HMODULE win32u = LoadLibraryA( "win32u.dll" );

		if ( !win32u ) 
			return false;

		pCreateWindowInBand = reinterpret_cast<CreateWindowInBand>( GetProcAddress( win32u, "CreateWindowInBand" ) );

		if ( !pCreateWindowInBand ) 
			return false;
	}

	return true;
}