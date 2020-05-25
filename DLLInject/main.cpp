#include "iostream"
#include <Windows.h>



//远程线程注入是在目标进程中创建远程线程，并用该线程去执行LoadLibraryA函数


void Inject(int pId, char* Path)
{

	//返回进程句柄
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pId);

	//申请一块内存给DLL路径,参数+1是为了防止溢出
	LPVOID pReturnAddress = VirtualAllocEx(hProcess, NULL, strlen(Path) + 1, MEM_COMMIT, PAGE_READWRITE);

	//写入路径到上一行申请的内存中
	WriteProcessMemory(hProcess, pReturnAddress, Path, strlen(Path) + 1, NULL);

	//以下两句是获取LoadLibraryA函数的地址
	HMODULE hModule = GetModuleHandleA("kernel32.dll");
	LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "LoadLibraryA");
	
	//创建远程线程,并获取线程句柄
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, lpStartAddress, pReturnAddress, 0, NULL);

	WaitForSingleObject(hThread, 2000);

	//防止内存泄漏，HANDLE类型都需要关闭
	CloseHandle(hThread);
	CloseHandle(hProcess);

}

int main()
{
	
	const char* a = "C:\\Users\\Administrator\\Desktop\\Acid_burn\\FIRSTDLL2.dll";
	Inject(10940, (char*)a);

	return 0;
}