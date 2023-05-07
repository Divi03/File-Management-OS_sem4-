#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <iostream>
using namespace std;

DWORDLONG getFileSize(const std::string& filePath)
{
    std::string sPath(filePath.begin(), filePath.end());
    cout << filePath << endl;
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    if (!GetFileAttributesExA(sPath.c_str(), GetFileExInfoStandard, &fileInfo))
    {
        std::cerr << "Error: GetFileAttributesExW failed with error code " << GetLastError() << std::endl;
        return 0;
    }
    return (static_cast<DWORDLONG>(fileInfo.nFileSizeHigh) << 32) | fileInfo.nFileSizeLow;
}


bool checkFreeSpace(const char* path, std::string filepath ){
    DWORDLONG fileSize=getFileSize(filepath);
    ULARGE_INTEGER freeSpace;
    cout << path << "  " << fileSize << endl ;
    if (!GetDiskFreeSpaceExA(path, &freeSpace, nullptr, nullptr))
    {
        std::cerr << "Error: GetDiskFreeSpaceExW failed with error code " << GetLastError() << std::endl;
        return false;
    }

    if (freeSpace.QuadPart < fileSize)
    {
        std::cerr << "Error: Not enough disk space. Free space: " << freeSpace.QuadPart / (1024 * 1024) << " MB" << std::endl;
        return false;
    }

    std::cout << "Available space in Directory : " << freeSpace.QuadPart / (1024*1024) << " MB" << endl ;
    std :: cout << "File Size : " << fileSize << " Bytes" << endl; 
    return true;
}

// int main() {
//     LPCWSTR path = L"C:\\";
//     uint64_t fileSize = 1024 * 1024 * 2000; // 100 MB

//     if (checkFreeSpace(path, "X:\\FileManagement_project\\test\\test.txt")) {
//         return true;
//     }
//     else {
//         return false;
//     }

//     return 0;
// }
