
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <stdint.h>
#include <strsafe.h>
#include <crtdbg.h>

/**
 * @brief
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
void print(_In_ const char* fmt, _In_ ...)
{
	char log_buffer[2048];
    va_list args;

    va_start(args,fmt);
    HRESULT hRes = StringCbVPrintfA(log_buffer, sizeof(log_buffer), fmt, args);
    if (S_OK != hRes)
    {
        fprintf(
            stderr, 
            "%s, StringCbVPrintfA() failed. res = 0x%08x",
            __FUNCTION__, 
            hRes
            );
        return;
    }

    OutputDebugStringA(log_buffer);
    fprintf(stdout, "%s \n", log_buffer);
}


/**
 * @brief
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
bool is_file_existsW(_In_ const wchar_t* file_path)
{
	_ASSERTE(NULL != file_path);
	_ASSERTE(TRUE != IsBadStringPtrW(file_path, MAX_PATH));
	if ((NULL == file_path) || (TRUE == IsBadStringPtrW(file_path, MAX_PATH))) return false;

	WIN32_FILE_ATTRIBUTE_DATA info = {0};

	//
	// CreateFile()이 아닌 GetFileAttributesEx()를 이용하면 파일이 다른 process에 의해 lock되어 있어도
	// 파일 존재여부를 정확히 체크할 수 있다.
	//
	if (GetFileAttributesExW(file_path, GetFileExInfoStandard, &info)==0) 
		return false;
	else
		return true;
}

bool create_bob_txt()
{
    // current directory 를 구한다.
    wchar_t *buf=NULL;
    uint32_t buflen = 0;
	DWORD default1 = 0;
	TCHAR string[10];
	wchar_t str[1000] = {0,};
	char strutf[1000] = {0,};
	int wow = WideCharToMultiByte(CP_UTF8, 0, str, lstrlenW(str), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, str, lstrlenW(str), strutf, nLen, NULL, NULL);
    buflen = GetCurrentDirectoryW(buflen, buf);
	if (0 == buflen)
	{
        print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
        return false;
	}

	buf = (PWSTR) malloc(sizeof(WCHAR) * buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}

    // current dir \\ bob.txt 파일명 생성
    wchar_t file_name[4000];
	wchar_t file_name1[4000];
    if (!SUCCEEDED(StringCbPrintfW(
                            file_name, 
                            sizeof(file_name), 
                            L"%ws\\bob.txt", 
                            buf)))
    {  
        print("err, can not create file name");
        free(buf);
        return false;
    }
    free(buf); buf = NULL;

    if (true == is_file_existsW(file_name))
    {
        ::DeleteFileW(file_name);
    }

    // 파일 생성
    HANDLE file_handle = CreateFileW(
                                file_name, 
						        GENERIC_WRITE, 
						        FILE_SHARE_READ, 
						        NULL, 
						        CREATE_NEW, 
						        FILE_ATTRIBUTE_NORMAL, 
						        NULL);	
	if(file_handle == INVALID_HANDLE_VALUE)
	{						
        print("err, CreateFile(path=%ws), gle=0x%08x", file_name, GetLastError());
        return false;
	}

    // 파일에 데이터 쓰기
    DWORD bytes_written = 0;
    wchar_t string_buf[1024];
    if (!SUCCEEDED(StringCbPrintfW(
                        string_buf, 
                        sizeof(string_buf), 
                        L"동해물과 백두산이 마르고 닳도록 하느님이 보우하사 우리나라만세")))
    {
        print("err, can not create data to write.");
        CloseHandle(file_handle);
        return false;
    }

    if (!WriteFile(file_handle, string_buf, wcslen(string_buf), &bytes_written, NULL))
    {
        print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
        CloseHandle(file_handle);
        return false;
    }

    // 영어로 쓰기
    if (!SUCCEEDED(StringCbPrintfW(
                        string_buf, 
                        sizeof(string_buf), 
                        L"All work and no play makes jack a dull boy.")))
    {
        print("err, can not create data to write.");
        CloseHandle(file_handle);
        return false;
    }

    if (!WriteFile(file_handle, string_buf, wcslen(string_buf), &bytes_written, NULL))
    {
        print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
        CloseHandle(file_handle);
        return false;
    }




    char string_bufa[1024];
    if (!SUCCEEDED(StringCbPrintfA(
                        string_bufa, 
                        sizeof(string_bufa), 
                        "동해물과 백두산이 마르고 닳도록 하느님이 보우하사 우리나라만세")))
    {
        print("err, can not create data to write.");
        CloseHandle(file_handle);
        return false;
    }

    if (!WriteFile(file_handle, string_bufa, strlen(string_bufa), &bytes_written, NULL))
    {
        print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
        CloseHandle(file_handle);
        return false;
    }

    // 영어로 쓰기
    if (!SUCCEEDED(StringCbPrintfA(
                        string_bufa, 
                        sizeof(string_bufa), 
                        "All work and no play makes jack a dull boy.")))
    {
        print("err, can not create data to write.");
        CloseHandle(file_handle);
        return false;
    }

    if (!WriteFile(file_handle, string_bufa, strlen(string_bufa), &bytes_written, NULL))
    {
        print("err, WriteFile() failed. gle = 0x%08x", GetLastError());
        CloseHandle(file_handle);
        return false;
    }
/*
약간의 에러가 존재합니다 흑..
*/
	CopyFile(file_name,L"bob2.txt",true);
	HANDLE file_handle2 = CreateFileW(
                                file_name1, 
						        GENERIC_WRITE, 
						        FILE_SHARE_READ, 
						        NULL, 
								OPEN_EXISTING, 
						        FILE_ATTRIBUTE_NORMAL, 
						        NULL);
	ReadFile(flle_handle2,string,sizeof(TCHAR),&default1,NULL);
	printf("%s\n",string);
    // 파일 닫기
	DeleteFile((LPCWSTR)file_name);
	DeleteFile((LPCWSTR)file_name1);
    CloseHandle(file_handle);
    return true;

}

int  _tmain(int argc, char *argv[]) {
	create_bob_txt();
	return 0;
}

