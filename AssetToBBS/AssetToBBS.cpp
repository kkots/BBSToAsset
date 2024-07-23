
#include <Windows.h>
#include <iostream>
#include "../WinError.h"

// .REDAssetCharaScript, new .bbscript file

int wmain(int argc, wchar_t** argv)
{

    if (argc != 3) {
        printf("REDAssetCharaScript to .bbscript converter. Usage:\n"
            "  AssetToBBS REDASSET NEWBBSCR\n"
            ", where:\n"
            "  REDASSET - the .REDAssetCharaScript file to convert to a .bbscript file,\n"
            "  NEWBBSCR - where to create the new .bbscript file\n");
        return -1;
    }

    DWORD bytesWritten;

    struct CloseStuffOnExit {
    public:
        ~CloseStuffOnExit() {
            if (readHandle) CloseHandle(readHandle);
            if (writeHandle) CloseHandle(writeHandle);
        }
        HANDLE readHandle = NULL;
        HANDLE writeHandle = NULL;
    } closeStuffOnExit;

    HANDLE writeHandle = CreateFileW(argv[2], GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (writeHandle == INVALID_HANDLE_VALUE) {
        WinError err;
        std::wcout << L"Failed to create file " << argv[2]
            << L'\n' << err.getMessage() << L'\n';
        return -1;
    }
    closeStuffOnExit.writeHandle = writeHandle;

    HANDLE readHandle = CreateFileW(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (readHandle == INVALID_HANDLE_VALUE) {
        WinError err;
        std::wcout << L"Failed to open file " << argv[1]
            << L'\n' << err.getMessage() << L'\n';
        return -1;
    }
    closeStuffOnExit.readHandle = readHandle;

    SetFilePointer(readHandle, 0x1c, NULL, FILE_BEGIN);
    int bbscrSize;
    ReadFile(readHandle, &bbscrSize, 4, &bytesWritten, NULL);

    SetFilePointer(readHandle, 0x38, NULL, FILE_BEGIN);
    void* bbscrData = malloc(bbscrSize);
    if (!bbscrData) {
        printf("Failed to allocate memory: %d\n", bbscrSize);
        return -1;
    }
    ReadFile(readHandle, bbscrData, bbscrSize, &bytesWritten, NULL);

    WriteFile(writeHandle, bbscrData, bbscrSize, &bytesWritten, NULL);
    free(bbscrData);

    return 0;
}
