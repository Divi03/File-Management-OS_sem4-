// include necessary header files
#include <Windows.h>
#include <iostream>

// define the FileInfo class
class FileInfo
{
private:
    const char *filePath; // file path string
    HANDLE fileHandle;    // file handle

public:
    // constructor that initializes the file path and opens the file
    FileInfo(const char *filePath)
    {
        this->filePath = filePath; // set the file path
        // open the file with GENERIC_READ access, FILE_SHARE_READ sharing mode, and other default parameters
        fileHandle = CreateFileA(
            filePath,              // LPCTSTR lpFileName
            GENERIC_READ,          // DWORD dwDesiredAccess
            FILE_SHARE_READ,       // DWORD dwShareMode
            NULL,                  // LPSECURITY_ATTRIBUTES lpSecurityAttributes
            OPEN_EXISTING,         // DWORD dwCreationDisposition
            FILE_ATTRIBUTE_NORMAL, // DWORD dwFlagsAndAttributes
            NULL);                 // HANDLE hTemplateFile
    }

    // destructor that closes the file if it is still open
    ~FileInfo()
    {
        if (fileHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(fileHandle);
        }
    }

    void printDetails()
    {
        if (fileHandle == INVALID_HANDLE_VALUE)
        {
            std::cout << "Could not open file: " << filePath << std::endl;
            return;
        }

        // Get file size
        LARGE_INTEGER fileSize;
        if (GetFileSizeEx(fileHandle, &fileSize)) // Check if function call was successful
        {
            std::cout << "+-------------------------------+-------------------------------+" << std::endl;
            std::cout << "| File Size:\t\t\t|\t" << fileSize.QuadPart << " bytes\t\t|" << std::endl;
        }
        else // If function call failed
        {
            DWORD error = GetLastError(); // Get the error code
            switch (error)
            {
            case ERROR_INVALID_HANDLE:
                std::cerr << "Invalid file handle." << std::endl;
                break;
            case ERROR_FILE_NOT_FOUND:
                std::cerr << "File not found." << std::endl;
                break;
            case ERROR_ACCESS_DENIED:
                std::cerr << "Access denied." << std::endl;
                break;
            default:
                std::cerr << "Unknown error occurred: " << error << std::endl;
                break;
            }
        }

        // Get file timestamps
        FILETIME creationTime, lastAccessTime, lastWriteTime;
        if (!GetFileTime(fileHandle, &creationTime, &lastAccessTime, &lastWriteTime))
        {
            // Get the error code
            DWORD errorCode = GetLastError();

            // Print an error message based on the error code
            switch (errorCode)
            {
            case ERROR_INVALID_HANDLE:
                std::cout << "Invalid file handle." << std::endl;
                break;
            case ERROR_ACCESS_DENIED:
                std::cout << "Access denied." << std::endl;
                break;
            case ERROR_FILE_NOT_FOUND:
                std::cout << "File not found." << std::endl;
                break;
            default:
                std::cout << "An error occurred while getting file time information." << std::endl;
                break;
            }
        }
        else
        {
            SYSTEMTIME sysTime;
            FileTimeToSystemTime(&creationTime, &sysTime);
            std::cout << "| Creation Time:\t\t|\t" << sysTime.wYear << "-" << sysTime.wMonth << "-" << sysTime.wDay << " " << sysTime.wHour << ":" << sysTime.wMinute << ":" << sysTime.wSecond << "\t|" << std::endl;

            FileTimeToSystemTime(&lastAccessTime, &sysTime);
            std::cout << "| Last Access Time:\t\t|\t" << sysTime.wYear << "-" << sysTime.wMonth << "-" << sysTime.wDay << " " << sysTime.wHour << ":" << sysTime.wMinute << ":" << sysTime.wSecond << "\t|" << std::endl;

            FileTimeToSystemTime(&lastWriteTime, &sysTime);
            std::cout << "| Last Write Time:\t\t|\t" << sysTime.wYear << "-" << sysTime.wMonth << "-" << sysTime.wDay << " " << sysTime.wHour << ":" << sysTime.wMinute << ":" << sysTime.wSecond << "\t|" << std::endl;
        }

        // Check for alternate data streams
        BY_HANDLE_FILE_INFORMATION fileInfo;

        if (!GetFileInformationByHandle(fileHandle, &fileInfo))
        {
            DWORD errorCode = GetLastError();
            std::cout << "Error getting file information: ";

            switch (errorCode)
            {
            case ERROR_FILE_NOT_FOUND:
                std::cout << "File not found." << std::endl;
                break;
            case ERROR_INVALID_HANDLE:
                std::cout << "Invalid handle." << std::endl;
                break;
            case ERROR_ACCESS_DENIED:
                std::cout << "Access denied." << std::endl;
                break;
            case ERROR_INVALID_PARAMETER:
                std::cout << "Invalid parameter." << std::endl;
                break;
            default:
                std::cout << "Unknown error." << std::endl;
                break;
            }
        }
        else
        {
            if (fileInfo.nFileSizeHigh > 0 || fileInfo.nFileSizeLow > 0)
            {
                std::cout << "| Alternate Data Streams:\t|\tYes\t\t\t|" << std::endl;
            }
            else
            {
                std::cout << "| Alternate Data Streams:\t|\tNo\t\t\t|" << std::endl;
            }
        }

        // Check for extended file attributes
        DWORD attributeCount = GetFileAttributesA(filePath);

        if (attributeCount == INVALID_FILE_ATTRIBUTES)
        {
            DWORD errorCode = GetLastError();

            switch (errorCode)
            {
            case ERROR_FILE_NOT_FOUND:
                std::cout << "File not found: " << filePath << std::endl;
                break;
            case ERROR_INVALID_PARAMETER:
                std::cout << "Invalid parameter passed to function." << std::endl;
                break;
            case ERROR_ACCESS_DENIED:
                std::cout << "Access denied: " << filePath << std::endl;
                break;
            default:
                std::cout << "Unknown error occurred while getting file attributes." << std::endl;
                break;
            }
        }
        else
        {
            std::cout << "| Attribute Name\t\t|\tValue\t\t\t|" << std::endl;
            std::cout << "+-------------------------------+-------------------------------+" << std::endl;

            switch (attributeCount & FILE_ATTRIBUTE_COMPRESSED)
            {
            case FILE_ATTRIBUTE_COMPRESSED:
                std::cout << "| Compressed\t\t\t|\tYes\t\t\t|" << std::endl;
                break;
            default:
                std::cout << "| Compressed\t\t\t|\tNo\t\t\t|" << std::endl;
                break;
            }

            switch (attributeCount & FILE_ATTRIBUTE_ENCRYPTED)
            {
            case FILE_ATTRIBUTE_ENCRYPTED:
                std::cout << "| Encrypted\t\t\t|\tYes\t\t\t|" << std::endl;
                break;
            default:
                std::cout << "| Encrypted\t\t\t|\tNo\t\t\t|" << std::endl;
                break;
            }

            switch (attributeCount & FILE_ATTRIBUTE_HIDDEN)
            {
            case FILE_ATTRIBUTE_HIDDEN:
                std::cout << "| Hidden\t\t\t|\tYes\t\t\t|" << std::endl;
                break;
            default:
                std::cout << "| Hidden\t\t\t|\tNo\t\t\t|" << std::endl;
                break;
            }

            switch (attributeCount & FILE_ATTRIBUTE_READONLY)
            {
            case FILE_ATTRIBUTE_READONLY:
                std::cout << "| Read Only\t\t\t|\tYes\t\t\t|" << std::endl;
                break;
            default:
                std::cout << "| Read Only\t\t\t|\tNo\t\t\t|" << std::endl;
                break;
            }

            switch (attributeCount & FILE_ATTRIBUTE_ARCHIVE)
            {
            case FILE_ATTRIBUTE_ARCHIVE:
                std::cout << "| Archive\t\t\t|\tYes\t\t\t|" << std::endl;
                break;
            default:
                std::cout << "| Archive\t\t\t|\tNo\t\t\t|" << std::endl;
                break;
            }

            switch (attributeCount & FILE_ATTRIBUTE_TEMPORARY)
            {
            case FILE_ATTRIBUTE_TEMPORARY:
                std::cout << "| Temporary\t\t\t|\tYes\t\t\t|" << std::endl;
                break;
            default:
                std::cout << "| Temporary\t\t\t|\tNo\t\t\t|" << std::endl;
                break;
            }

            switch (attributeCount & FILE_ATTRIBUTE_OFFLINE)
            {
            case FILE_ATTRIBUTE_OFFLINE:
                std::cout << "| Offline\t\t\t|\tYes\t\t\t|" << std::endl;
                break;
            default:
                std::cout << "| Offline\t\t\t|\tNo\t\t\t|" << std::endl;
                break;
            }

            switch (attributeCount & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
            {
            case FILE_ATTRIBUTE_NOT_CONTENT_INDEXED:
                std::cout << "| Not Content Indexed\t\t|\tYes\t\t\t|" << std::endl;
                break;
            default:
                std::cout << "| Not Content Indexed\t\t|\tNo\t\t\t|" << std::endl;
                break;
            }

            std::cout << "+-------------------------------+-------------------------------+" << std::endl;
        }
    }
};

// int main()
// {
//     const char *filePath = "C:\\Users\\hp\\Desktop\\Operating Systems\\project\\File-Management-OS_sem4-\\lz4.h";
//     FileInfo fileInfo(filePath);
//     fileInfo.printDetails();

//     return 0;
// }
