#include <windows.h>
#include <Shlwapi.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HANDLE traktor;

	/* Try to find a running instance of Traktor */
	HWND hTraktorWindow = FindWindow(NULL, "Traktor");
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
		lstrcat(modName, "\\Traktor.exe");

		/* Try the constructed path first; if that doesn't work try the current directory */
		if (!CreateProcess(modName, lpCmdLine, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi) &&
			!CreateProcess("Traktor.exe", lpCmdLine, NULL, NULL, TRUE, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi)) {
			MessageBox(0, "Failed to start Traktor.exe!\n\nMake sure trakload.exe is in the same directory as Traktor.exe", "TrakLoad", MB_ICONEXCLAMATION);
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