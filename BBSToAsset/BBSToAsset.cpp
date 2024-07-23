
#include <Windows.h>
#include <iostream>
#include "../WinError.h"

// .bbscript file, original .REDAssetCharaScript, new .REDAssetCharaScript

int wmain(int argc, wchar_t** argv)
{

    if (argc != 3) {
        printf(".bbscript to REDAssetCharaScript converter. Usage:\n"
            "  BBSToAsset BBSCRFILE ORIGINAL_REDASSET NEW_REDASSET\n"
            ", where:\n"
            "  BBSCRFILE - the .bbscript file to convert to a new .REDAssetCharaScript file,\n"
            "  ORIGINAL_REDASSET - the original REDAssetCharaScript file from which you originally obtained the bbscript\n"
            "  NEW_REDASSET - where to create the new .REDAssetCharaScript file\n");
        return -1;
    }

    DWORD bytesWritten;
    HANDLE bbscrFile = CreateFileW(argv[1],
        GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (bbscrFile == INVALID_HANDLE_VALUE) {
        WinError err;
        std::cout << "Failed to open file\n";
        std::wcout << err.getMessage() << L'\n';
        return -1;
    }

    struct CloseStuffOnExit {
    public:
        ~CloseStuffOnExit() {
            if (bbscrFile) CloseHandle(bbscrFile);
            if (readHandle) CloseHandle(readHandle);
            if (writeHandle) CloseHandle(writeHandle);
        }
        HANDLE bbscrFile = NULL;
        HANDLE readHandle = NULL;
        HANDLE writeHandle = NULL;
    } closeStuffOnExit;
    closeStuffOnExit.bbscrFile = bbscrFile;

    const int bbscrSize = GetFileSize(bbscrFile, NULL);
    void* bbscrData = malloc(bbscrSize);
    if (!bbscrData) {
        std::cout << "Failed to allocate memory: " << bbscrSize << '\n';
        return -1;
    }
    ReadFile(bbscrFile, bbscrData, bbscrSize, &bytesWritten, NULL);
    CloseHandle(bbscrFile);
    closeStuffOnExit.bbscrFile = NULL;

    HANDLE writeHandle = CreateFileW(argv[3], GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (writeHandle == INVALID_HANDLE_VALUE) {
        WinError err;
        std::wcout << L"Failed to create file " << argv[3]
            << L'\n' << err.getMessage() << L'\n';
        return -1;
    }
    closeStuffOnExit.writeHandle = writeHandle;

    HANDLE readHandle = CreateFileW(argv[2], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (readHandle == INVALID_HANDLE_VALUE) {
        WinError err;
        std::wcout << L"Failed to open file " << argv[2]
            << L'\n' << err.getMessage() << L'\n';
        return -1;
    }
    closeStuffOnExit.readHandle = readHandle;

    BYTE stackbuf[1024];
    ReadFile(readHandle, stackbuf, 7*4, &bytesWritten, NULL);
    WriteFile(writeHandle, stackbuf, 7*4, &bytesWritten, NULL);

    int oldBbscrSize;
    ReadFile(readHandle, &oldBbscrSize, 4, &bytesWritten, NULL);
    WriteFile(writeHandle, &bbscrSize, 4, &bytesWritten, NULL);

    ReadFile(readHandle, stackbuf, 3*4, &bytesWritten, NULL);
    WriteFile(writeHandle, stackbuf, 3*4, &bytesWritten, NULL);

    SetFilePointer(readHandle, 12, NULL, FILE_CURRENT);
    WriteFile(writeHandle, &bbscrSize, 4, &bytesWritten, NULL);
    WriteFile(writeHandle, &bbscrSize, 4, &bytesWritten, NULL);
    int zeros = 0;
    WriteFile(writeHandle, &zeros, 4, &bytesWritten, NULL);

    SetFilePointer(readHandle, oldBbscrSize, NULL, FILE_CURRENT);
    WriteFile(writeHandle, bbscrData, bbscrSize, &bytesWritten, NULL);

    memset(stackbuf, 0, 16);
    WriteFile(writeHandle, stackbuf, 16, &bytesWritten, NULL);

    return 0;
}
