#include <windows.h>
#include <Shlwapi.h>
#include <stdio.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HANDLE traktor;
	char iniName[2048];
	char exeName[1024];
	char windowTitle[1024];

	GetModuleFileName(NULL, iniName, sizeof(iniName));
	PathRemoveFileSpec(iniName);
	lstrcat(iniName, "\\trakrun.ini");
	GetPrivateProfileString("TrakRun", "ExeName", "Traktor.exe", exeName, sizeof(exeName), iniName);
	GetPrivateProfileString("TrakRun", "WindowTitle", "Traktor", windowTitle, sizeof(windowTitle), iniName);
	WritePrivateProfileString("TrakRun", "ExeName", exeName, iniName);
	WritePrivateProfileString("TrakRun", "WindowTitle", windowTitle, iniName);

	/* Try to find a running instance of Traktor */
	HWND hTraktorWindow = FindWindow(NULL, windowTitle);
	if (hTraktorWindow) {
		DWORD pid;
		DWORD tid = GetWindowThreadProcessId(hTraktorWindow, &pid);
		traktor = OpenProcess(SYNCHRONIZE, true, pid);
		if (!traktor) {
			MessageBox(0, "Failed to open Traktor process", "TrakLoad", MB_ICONEXCLAMATION);
			ExitProcess(1);
		}
	}
	else {
		PROCESS_INFORMATION pi;
		STARTUPINFOA si;
		ZeroMemory(&si, sizeof(si));
		ZeroMemory(&pi, sizeof(pi));
		si.cb = sizeof(si);

		/* Construct the name of Traktor.exe in the same directory as the program */
		char modName[2048];
		GetModuleFileName(NULL, modName, sizeof(modName));
		PathRemoveFileSpec(modName);
		lstrcat(modName, "\\");
		lstrcat(modName, exeName);

		/* Try the constructed path first; if that doesn't work try the current directory */
		if (!CreateProcess(modName, lpCmdLine, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi) &&
			!CreateProcess(exeName, lpCmdLine, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi)) {
			char error[2048];
			snprintf(error, sizeof(error), 
				"Failed to start %s!\nPlease make sure TrakRun.exe is in the same directory as %s.\n\n"
				"If the program is not named %s, please change trakrun.ini accordingly.", exeName, exeName, exeName);
			MessageBox(0, error, "TrakLoad", MB_ICONEXCLAMATION);
			ExitProcess(1);
		}
		traktor = pi.hProcess;
	}

	/* Prevent sleep and screen saver */
	SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);

	/* Wait for Traktor to exit before we return */
	WaitForSingleObject(traktor, INFINITE);
	return 0;
}