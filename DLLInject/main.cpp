#include "iostream"
#include <Windows.h>



//Զ���߳�ע������Ŀ������д���Զ���̣߳����ø��߳�ȥִ��LoadLibraryA����


void Inject(int pId, char* Path)
{

	//���ؽ��̾��
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pId);

	//����һ���ڴ��DLL·��,����+1��Ϊ�˷�ֹ���
	LPVOID pReturnAddress = VirtualAllocEx(hProcess, NULL, strlen(Path) + 1, MEM_COMMIT, PAGE_READWRITE);

	//д��·������һ��������ڴ���
	WriteProcessMemory(hProcess, pReturnAddress, Path, strlen(Path) + 1, NULL);

	//���������ǻ�ȡLoadLibraryA�����ĵ�ַ
	HMODULE hModule = GetModuleHandleA("kernel32.dll");
	LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "LoadLibraryA");
	
	//����Զ���߳�,����ȡ�߳̾��
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, lpStartAddress, pReturnAddress, 0, NULL);

	WaitForSingleObject(hThread, 2000);

	//��ֹ�ڴ�й©��HANDLE���Ͷ���Ҫ�ر�
	CloseHandle(hThread);
	CloseHandle(hProcess);

}

int main()
{
	
	const char* a = "C:\\Users\\Administrator\\Desktop\\Acid_burn\\FIRSTDLL2.dll";
	Inject(10940, (char*)a);

	return 0;
}