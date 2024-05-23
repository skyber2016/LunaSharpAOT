#include <iostream>
#include <Windows.h>
using namespace std;


int main()
{
	LPCSTR DllPath = "./LunaSharp.dll"; // The Path to our DLL

	HWND hwnd = FindWindowA(NULL, "D3D9Test"); // HWND (Windows window) by Window Name
	DWORD procID; // A 32-bit unsigned integer, DWORDS are mostly used to store Hexadecimal Addresses
	GetWindowThreadProcessId(hwnd, &procID); // Getting our Process ID, as an ex. like 000027AC
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID); // Opening the Process with All Access

	// Allocate memory for the dllpath in the target process, length of the path string + null terminator
	LPVOID pDllPath = VirtualAllocEx(handle, 0, strlen(DllPath) + 1, MEM_COMMIT, PAGE_READWRITE);

	// Write the path to the address of the memory we just allocated in the target process
	WriteProcessMemory(handle, pDllPath, (LPVOID)DllPath, strlen(DllPath) + 1, 0);

	// Create a Remote Thread in the target process which calls LoadLibraryA as our dllpath as an argument -> program loads our dll
	HANDLE hLoadThread = CreateRemoteThread(handle, 0, 0,
		(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"), pDllPath, 0, 0);

	WaitForSingleObject(hLoadThread, INFINITE); // Wait for the execution of our loader thread to finish

	cout << "Dll path allocated at: " << hex << pDllPath << endl;

	VirtualFreeEx(handle, pDllPath, strlen(DllPath) + 1, MEM_RELEASE); // Free the memory allocated for our dll path

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
