/*PATH "C:\\TestingFileSys"*/

// git controlled env
// now in collaborative mode
// define preprocessor macro to exclude rarely-used headers from windows.h
#define WIN32_LEAN_AND_MEAN

// include necessary headers
#include <fstream>
#include <windows.h> // for Windows API functions and types
#include <iostream>  // for input/output stream operations
#include <tchar.h>   // for Unicode and ANSI string macros
#include <WinBase.h> // for Windows API functions
#include <string>    // for using strings in C++
#include "CFile_Details.cpp"
#include "CDiskSpace.cpp"
using namespace std;


class CFileBase
{
    protected:

    string sPath;               // a string variable to store file path
    HANDLE hFile;               // a file handle to access file objects
    HANDLE hProcess;            // a handle to access processes
    HANDLE hThread;             // a handle to access threads
    STARTUPINFOA si;             // a structure containing startup information for a process
    PROCESS_INFORMATION pi;     // a structure containing information about a newly created process
    DWORD dwProcessId = 0;      // a variable to store process ID
    DWORD deThreadId = 0;       // a variable to store thread ID
    BOOL bCreationFlag = FALSE; // a boolean flag to indicate whether a process has been created
    
    public:

    virtual void rOpen() = 0;
    virtual bool bDeleteFile() = 0;
    virtual bool bCreateFileBackup() = 0;
    virtual bool bMoveFileToFolder(const char *filePath, const char *destFolderPath) = 0;
    virtual bool bCopyFileToFolder(const char *filePath, const char *destFolderPath) = 0;
    virtual void vGetFinfo(const char *sFilePath) = 0;

};

// create a class for file management operations
class CFileManagement : public CFileBase
{
private:

    string sPath;               // a string variable to store file path
    HANDLE hFile;               // a file handle to access file objects
    HANDLE hProcess;            // a handle to access processes
    HANDLE hThread;             // a handle to access threads
    STARTUPINFOA si;            // a structure containing startup information for a process
    PROCESS_INFORMATION pi;     // a structure containing information about a newly created process
    DWORD dwProcessId = 0;      // a variable to store process ID
    DWORD deThreadId = 0;       // a variable to store thread ID
    BOOL bCreationFlag = FALSE; // a boolean flag to indicate whether a process has been created

public:
    void rOpen()
    {
        LPSTR path;
        cout << "Inside Open function" << endl;
        cout << "File Path :";
        cin >> path;
        BOOL bCreateProcess;
        bCreateProcess = CreateProcessA(
            // "Y:\\P05_childProcess.exe",
            path,
            NULL,
            NULL,
            NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }

    bool isFileOpen(const std::string &filename)
    {
        std::ifstream file(filename);
        return file.is_open();
    }

    bool bDeleteFile()
    {
        bool bflag = false;
        char *lpPath = new char[MAX_PATH];

        cout << "File Path: ";
        cin.ignore();
        cin.getline(lpPath, MAX_PATH);

        // Attempt to delete the file
        if (isFileOpen(lpPath))
            if (DeleteFileA(lpPath))
            {
                bflag = true;
            }
            else
            {
                // Get the last Win32 error code
                DWORD dwErrorCode = GetLastError();

                // The DeleteFileA WinAPI function returns only two error codes:
                // ERROR_FILE_NOT_FOUND (2): The specified file was not found.
                // ERROR_ACCESS_DENIED (5): The caller does not have the required permission to delete the file.
                switch (dwErrorCode)
                {
                case ERROR_FILE_NOT_FOUND:
                {
                    cout << "Such a file doesn't exist in the system" << endl;
                    bflag = true;
                    break;
                    // The file doesn't exist, so consider it deleted
                }

                case ERROR_ACCESS_DENIED:
                {
                    cout << "The second error code indicates that the caller does not have sufficient permission to delete the file." << endl;
                    bflag = false;
                    break;
                }

                default:
                    cout << "Undefined Error";
                }
            }

        delete[] lpPath;
        return bflag;
    }

    bool bCreateFileBackup()
    {
        bool bflag;
        LPSTR lpPath;
        cout << "File Path :";
        cin >> lpPath;

        // Get the current time
        SYSTEMTIME currentTime;
        GetSystemTime(&currentTime);

        // Construct the backup file name by appending a timestamp to the original file name
        char backupFilePath[MAX_PATH];
        sprintf(backupFilePath, "%s.%04d%02d%02d%02d%02d%02d.bak",
                lpPath,
                currentTime.wYear, currentTime.wMonth, currentTime.wDay,
                currentTime.wHour, currentTime.wMinute, currentTime.wSecond);

        // Attempt to copy the file to the backup file name
        if (CopyFileA(lpPath, backupFilePath, FALSE))
        {
            return true;
        }
        else
        {
            cout << "Failed to create backup" << endl;
            // add error handling here again
            return false;
        }
    }

    bool bMoveFileToFolder(const char *filePath, const char *destFolderPath)
    {
        // Get the file name from the file path
        const char *fileName = strrchr(filePath, '\\');
        if (!fileName)
        {
            // The file path is invalid
            return false;
        }
        fileName++; // Advance past the backslash

        // Construct the destination file path by appending the file name to the destination folder path
        char destFilePath[MAX_PATH];
        strcpy(destFilePath, destFolderPath);
        int len = strlen(destFilePath);
        if (destFilePath[len - 1] != '\\' && destFilePath[len - 1] != '/')
        {
            destFilePath[len] = '\\'; // Add a backslash if needed
            destFilePath[len + 1] = '\0';
        }
        strcat(destFilePath, fileName);

        // Move the file to the destination folder using MoveFileA
        if (MoveFileA(filePath, destFilePath))
        {
            return true;
        }
        else
        {
            DWORD dwErrorCode = GetLastError();

            switch (dwErrorCode)
            {
            case ERROR_FILE_NOT_FOUND:
                cout << "The specified file was not found." << endl;
                break;
            case ERROR_PATH_NOT_FOUND:
                cout << "The specified path was not found." << endl;
                break;
            case ERROR_ACCESS_DENIED:
                cout << "The caller does not have the required permission." << endl;
                break;
            case ERROR_INVALID_PARAMETER:
                cout << "One or more parameters passed to the function was invalid." << endl;
                break;
            case ERROR_ALREADY_EXISTS:
                cout << "The specified file already exists and cannot be overwritten." << endl;
                break;
            case ERROR_WRITE_FAULT:
                cout << "A disk error occurred while writing to the file." << endl;
                break;
            default:
                cout << "Unknown error." << endl;
                break;
            }

            return 0;
        }
    }

    bool bCopyFileToFolder(const char *filePath, const char *destFolderPath)
    {
        // Get the file name from the file path
        const char *fileName = strrchr(filePath, '\\');
        if (!fileName)
        {
            // The file path is invalid
            return FALSE;
        }
        fileName++; // Advance past the backslash

        // Construct the destination file path by appending the file name to the destination folder path
        char destFilePath[MAX_PATH];

        strcpy(destFilePath, destFolderPath);
        int len = strlen(destFilePath);

        if (destFilePath[len - 1] != '\\' && destFilePath[len - 1] != '/')
        {
            destFilePath[len] = '\\'; // Add a backslash if needed
            destFilePath[len + 1] = '\0';
        }
        strcat(destFilePath, fileName);

        // Attempt to copy the file to the destination file path
        if (checkFreeSpace(destFolderPath, (string)filePath))
        {
            if (CopyFileA(filePath, destFilePath, FALSE))
            {
                return TRUE;
            }
            else
            {
                DWORD dwErrorCode = GetLastError();
                switch (dwErrorCode)
                {
                case ERROR_FILE_NOT_FOUND:
                    // The specified file was not found.
                    cout << "The specified file was not found." << endl;
                    break;
                case ERROR_PATH_NOT_FOUND:
                    // The specified path was not found.
                    cout << "The specified path was not found." << endl;
                    break;
                case ERROR_ACCESS_DENIED:
                    // Access is denied.
                    cout << "Access is denied." << endl;
                    break;
                case ERROR_INVALID_PARAMETER:
                    // One of the parameters was invalid.
                    cout << "One of the parameters was invalid." << endl;
                    break;
                case ERROR_ALREADY_EXISTS:
                    // The specified file already exists.
                    cout << "The specified file already exists." << endl;
                    break;
                case ERROR_FILE_EXISTS:
                    // The destination file already exists.
                    cout << "The destination file already exists." << endl;
                    break;
                case ERROR_INVALID_FLAGS:
                    // The flag parameter is invalid.
                    cout << "The flag parameter is invalid." << endl;
                    break;
                case ERROR_DISK_FULL:
                    // The destination file system is full.
                    cout << "The destination file system is full." << endl;
                    break;
                case ERROR_INVALID_DRIVE:
                    // The drive that was specified is invalid or nonexistent.
                    cout << "The drive that was specified is invalid or nonexistent." << endl;
                    break;
                case ERROR_WRITE_PROTECT:
                    // The media is write-protected.
                    cout << "The media is write-protected." << endl;
                    break;
                case ERROR_NOT_SAME_DEVICE:
                    // The files cannot be replaced on a remote file system.
                    cout << "The files cannot be replaced on a remote file system." << endl;
                    break;
                case ERROR_INVALID_HANDLE:
                    // The handle that was passed to the API is invalid.
                    cout << "The handle that was passed to the API is invalid." << endl;
                    break;
                case ERROR_NEGATIVE_SEEK:
                    // An attempt was made to move the file pointer before the beginning of the file.
                    cout << "An attempt was made to move the file pointer before the beginning of the file." << endl;
                    break;
                case ERROR_SEEK_ON_DEVICE:
                    // The file pointer cannot be set on the specified device or file.
                    cout << "The file pointer cannot be set on the specified device or file." << endl;
                    break;
                case ERROR_SHARING_VIOLATION:
                    // The process cannot access the file because it is being used by another process.
                    cout << "The process cannot access the file because it is being used by another process." << endl;
                    break;
                case ERROR_LOCK_VIOLATION:
                    // The process cannot access the file because another process has locked a portion of the file.
                    cout << "The process cannot access the file because another process has locked a portion of the file." << endl;
                    break;
                case ERROR_BUFFER_OVERFLOW:
                    // The buffer that was supplied is too small.
                    cout << "The buffer that was supplied is too small." << endl;
                    break;
                case ERROR_HANDLE_EOF:
                    // The end of the file was reached.
                    cout << "The end of the file was reached." << endl;
                    break;
                case ERROR_HANDLE_DISK_FULL:
                    // The disk is full.
                    cout << "The disk is full." << endl;
                    break;
                default:
                    // An unknown error occurred.
                    cout << "An unknown error occurred." << endl;
                    break;
                }

                
            }
        }
        else{
            cout << "Space Error" <<endl;
        }
        return false;
    }

    void vGetFinfo(const char *sFilePath)
    {
        FileInfo oFinfo(sFilePath);
        oFinfo.printDetails();
    }
};

int main()
{

    int iChoice;
    HANDLE hFile;
    int iProcessID = 0;
    int ThreadId = 0;
    CFileBase * basePtr;
    CFileManagement file;
    LPSTR sFilePath = new char[MAX_PATH];
    LPSTR sDestinationPath = new char[MAX_PATH];

    do
    {
        /* code */
        basePtr = &file;
        cout << "FILE MANAGEMENT SYSTEM" << endl;
        // cout << "0} Exit" <<endl;
        cout << "1} Run/Open" << endl;
        cout << "2} Delete File" << endl;
        cout << "3} Create Backups" << endl;
        cout << "4} Copy File" << endl;
        cout << "5} Move File" << endl;
        cout << "6} Get info" << endl;
        cout << "Enter Choice" << endl;

        cin >> iChoice;
        switch (iChoice)
        {
        // case 0:
        //     exit(1);
        //     break;
        // exit program to be set latter
        case 1:
            basePtr->rOpen();
            break;
        case 2:
            // cout <<"Enter File Path : ";
            // cin >> sFilePath;
            basePtr->bDeleteFile();
            break;
        case 3:
            // cout <<"Enter File Path : ";
            // cin >> sFilePath;
            basePtr->bCreateFileBackup();
            break;
        case 4:
            cout << "Enter File Path : ";
            cin >> sFilePath;
            cout << "Enter DEtination Folder : ";
            cin >> sDestinationPath;
            basePtr->bCopyFileToFolder(sFilePath, sDestinationPath);
            break;
        case 5:
            cout << "Enter File Path : ";
            cin >> sFilePath;
            cout << "Enter DEtination Folder : ";
            cin >> sDestinationPath;
            basePtr->bMoveFileToFolder(sFilePath, sDestinationPath);
            break;
        case 6:
            cout << "Enter File Path : ";
            cin >> sFilePath;
            basePtr->vGetFinfo(sFilePath);
            break;
        default:
            break;
        }
    } while (iChoice);
}
