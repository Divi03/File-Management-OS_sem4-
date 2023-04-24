/*PATH "C:\\TestingFileSys"*/

// git controlled env
// now in collaborative mode
// define preprocessor macro to exclude rarely-used headers from windows.h
#define WIN32_LEAN_AND_MEAN

// include necessary headers
#include <windows.h> // for Windows API functions and types
#include <iostream> // for input/output stream operations
#include <tchar.h> // for Unicode and ANSI string macros
#include <unistd.h> // for POSIX API functions
#include <WinBase.h> // for Windows API functions
#include <string> // for using strings in C++

using namespace std;

// create a class for file management operations
class CFileManagement{
    private:
    int x; // an integer variable
    DWORD arr[10]; // an array of DWORDs with 10 elements
    string sPath; // a string variable to store file path
    HANDLE hFile; // a file handle to access file objects
    HANDLE hProcess; // a handle to access processes
    HANDLE hThread; // a handle to access threads
    STARTUPINFO si; // a structure containing startup information for a process
    PROCESS_INFORMATION pi; // a structure containing information about a newly created process
    DWORD dwProcessId = 0; // a variable to store process ID
    DWORD deThreadId = 0; // a variable to store thread ID
    BOOL bCreationFlag = FALSE; // a boolean flag to indicate whether a process has been created

    public:
    void vOpen()
    {
        // Define a variable to hold the user's choice of operation
        int iOpenChoice;

        // Create STARTUPINFO and PROCESS_INFORMATION structs and zero out their memory
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        ZeroMemory(&pi, sizeof(pi));

        // Define a variable to hold the path to the file and prompt the user to input it
        LPSTR lpPath;
        cout << "File Path :";
        cin >> lpPath;

        // Convert the path to a wide character string
        int len = strlen(lpPath) + 1; 
        int size = MultiByteToWideChar(CP_UTF8, 0, lpPath, len, NULL, 0); 
        LPWSTR wPath = new WCHAR[size]; 
        MultiByteToWideChar(CP_UTF8, 0, lpPath, len, wPath, size); 

        // Prompt the user to select an operation
        cout << "\n Make a choice:";
        cout << "\n 1. Open File";
        cout << "\n 2. Create File";


        switch(iOpenChoice)
        {
            case 1:
            {
                // Informing the user that the program has entered the open file function
                cout << "Inside Open function"<<endl;

                BOOL bCreateProcess;

                // Using the CreateProcess() function to create a new process 
                bCreateProcess = CreateProcess(
                            wPath,      // Application path
                            NULL,       // Command line arguments
                            NULL,       // Process handle not inheritable
                            NULL,       // Thread handle not inheritable
                            FALSE,      // Set handle inheritance to FALSE
                            0,          // No creation flags
                            NULL,       // Use parent's environment block
                            NULL,       // Use parent's starting directory 
                            &si,        // Pointer to STARTUPINFO structure
                            &pi);       // Pointer to PROCESS_INFORMATION structure

                // Close process and thread handles
                CloseHandle(pi.hThread);
                CloseHandle(pi.hProcess);

            if (hFile)
            {
                cout << "\nSuccesfullt Opened the File";
            }

            else
            {
                DWORD dwErrorCode = GetLastError();
                switch(dwErrorCode)
                    {
                        case ERROR_FILE_NOT_FOUND:
                            // The specified file was not found.
                            cout << "The specified file was not found." << endl;
                            break;
                        case ERROR_PATH_NOT_FOUND:
                            // The specified path was not found.
                            cout << "The specified path was not found." << endl;
                            break;
                        case ERROR_BAD_EXE_FORMAT:
                            // The specified executable file is not a valid application for this operating system.
                            cout << "The specified executable file is not a valid application for this operating system." << endl;
                            break;
                        case ERROR_ACCESS_DENIED:
                            // The requested access to the file or directory is not allowed.
                            cout << "The requested access to the file or directory is not allowed." << endl;
                            break;
                        case ERROR_INVALID_HANDLE:
                            // The specified handle is invalid.
                            cout << "The specified handle is invalid." << endl;
                            break;
                        case ERROR_NOT_ENOUGH_MEMORY:
                            // Not enough memory is available to complete this operation.
                            cout << "Not enough memory is available to complete this operation." << endl;
                            break;
                        case ERROR_INVALID_PARAMETER:
                            // The parameter is incorrect.
                            cout << "The parameter is incorrect." << endl;
                            break;
                        case ERROR_SHARING_VIOLATION:
                            // The requested operation cannot be completed because the file is already in use.
                            cout << "The requested operation cannot be completed because the file is already in use." << endl;
                            break;
                        case ERROR_FILE_EXISTS:
                            // The file already exists.
                            cout << "The file already exists." << endl;
                            break;
                        case ERROR_INVALID_NAME:
                            // The filename, directory name, or volume label syntax is incorrect.
                            cout << "The filename, directory name, or volume label syntax is incorrect." << endl;
                            break;
                        default:
                            // An unknown error occurred.
                            cout << "An unknown error occurred." << endl;
                            break;
                    }
                }
            }

            case 2:
            {

            // Zero out the memory of the structs
            ZeroMemory(&si, sizeof(si));
            ZeroMemory(&pi, sizeof(pi));

            DWORD dwNewLine = 1;
            DWORD dwNewLineAddeed = 0;
            DWORD dwBytesWritten = 0;
            DWORD dwBytesToBeRead = 0;
            DWORD dwBytesRead = 0;
            BOOL bErrorFlag = false;
            DWORD dwDestinationFileSize = 0;

            hFile = CreateFileA(
                "Sample.txt",
                //"D:\\211156\\NewFile.txt"
                GENERIC_READ,
                0,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL);
            if (hFile)
            {
                cout << "\nSuccesfullt Opened the File";
            }
            else
            {
                // Get the last Win32 error code
                DWORD dwErrorCode = GetLastError();
                switch(dwErrorCode)
                {
                    case ERROR_FILE_NOT_FOUND:
                        cout << "The specified file was not found." << endl;
                        break;
                    case ERROR_PATH_NOT_FOUND:
                        cout << "The specified path was not found." << endl;
                        break;
                    case ERROR_TOO_MANY_OPEN_FILES:
                        cout << "The system cannot open the file." << endl;
                        break;
                    case ERROR_ACCESS_DENIED:
                        cout << "Access is denied." << endl;
                        break;
                    case ERROR_INVALID_HANDLE:
                        cout << "The handle that was passed to the API is invalid." << endl;
                        break;
                    case ERROR_INVALID_DRIVE:
                        cout << "The drive that was specified is invalid or nonexistent." << endl;
                        break;
                    case ERROR_NOT_SAME_DEVICE:
                        cout << "The files cannot be replaced on a remote file system." << endl;
                        break;
                    case ERROR_WRITE_PROTECT:
                        cout << "The media is write-protected." << endl;
                        break;
                    case ERROR_BAD_UNIT:
                        cout << "The specified device is not a valid device." << endl;
                        break;
                    case ERROR_NOT_READY:
                        cout << "The specified disk is not ready or the drive is not accessible." << endl;
                        break;
                    case ERROR_CRC:
                        cout << "A cyclic redundancy check (CRC) error occurred." << endl;
                        break;
                    case ERROR_SEEK:
                        cout << "The file pointer cannot be set on the specified device or file." << endl;
                        break;
                    case ERROR_NOT_DOS_DISK:
                        cout << "The specified disk or diskette is not a DOS disk." << endl;
                        break;
                    case ERROR_SECTOR_NOT_FOUND:
                        cout << "The drive cannot find the sector requested." << endl;
                        break;
                    case ERROR_OUT_OF_PAPER:
                        cout << "The printer is out of paper." << endl;
                        break;
                    case ERROR_WRITE_FAULT:
                        cout << "The system cannot write to the specified device." << endl;
                        break;
                    case ERROR_READ_FAULT:
                        cout << "The system cannot read from the specified device." << endl;
                        break;
                    case ERROR_GEN_FAILURE:
                        cout << "A device attached to the system is not functioning." << endl;
                        break;
                    case ERROR_SHARING_VIOLATION:
                        cout << "The process cannot access the file because it is being used by another process." << endl;
                        break;
                    case ERROR_LOCK_VIOLATION:
                        cout << "The process cannot access the file because another process has locked a portion of the file." << endl;
                        break;
                    case ERROR_DISK_CHANGE:
                        cout << "The drive changed while the system was paging it or an application was accessing it." << endl;
                        break;
                    case ERROR_INVALID_PARAMETER:
                        cout << "The parameter is incorrect." << endl;
                        break;
                    case ERROR_BROKEN_PIPE:
                        cout << "The pipe has been ended." << endl;
                        break;
                    case ERROR_OPEN_FAILED:
                        cout << "The system cannot open the file." << endl;
                        break;
                    case ERROR_IO_DEVICE:
                        cout << "The request could not be performed because of an I/O device error." << endl;
                        break;
                    case ERROR_DISK_FULL:
                        cout << "There is not enough space on the disk." << endl;
                        break;
                    case ERROR_INSUFFICIENT_BUFFER:
                        cout << "The data area passed to a system call is too small." << endl;
                        break;
                    case ERROR_HANDLE_EOF:
                        cout << "Reached the end of the file." << endl;
                        break;
                }
            }
        }
    }


    bool bDeleteFile(const char* filePath)
    {
        // Attempt to delete the file
        if (DeleteFileA(filePath))
        {
            return true;
        }
        else
        {
            // Get the last Win32 error code
            DWORD dwErrorCode = GetLastError();

            // The DeleteFileA WinAPI function returns only two error codes:

            // ERROR_FILE_NOT_FOUND (2): The specified file was not found.
            // ERROR_ACCESS_DENIED (5): The caller does not have the required permission to delete the file.
            switch(dwErrorCode)
            {
                case ERROR_FILE_NOT_FOUND:
                {
                    cout << "Such a file doesn't exist in the system" << endl;
                    return true;
                    // The file doesn't exist, so consider it deleted
                }

                case ERROR_ACCESS_DENIED:
                {
                    cout << "The second error code indicates that the caller does not have sufficient permission to delete the file." << endl;
                    return false;
                }
            }
        }
    }

    bool bCreateFileBackup(const char* filePath)
    {
        // Get the current time
        SYSTEMTIME currentTime;
        GetSystemTime(&currentTime);

        // Construct the backup file name by appending a timestamp to the original file name
        char backupFilePath[MAX_PATH];
        sprintf(backupFilePath, "%s.%04d%02d%02d%02d%02d%02d.bak",
                filePath,
                currentTime.wYear, currentTime.wMonth, currentTime.wDay,
                currentTime.wHour, currentTime.wMinute, currentTime.wSecond);

        // Attempt to copy the file to the backup file name
        if (CopyFileA(filePath, backupFilePath, FALSE))
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


    bool bMoveFileToFolder(const char* filePath, const char* destFolderPath)
    {
        // Get the file name from the file path
        const char* fileName = strrchr(filePath, '\\');
        if (!fileName) {
            // The file path is invalid
            return false;
        }
        fileName++; // Advance past the backslash

        // Construct the destination file path by appending the file name to the destination folder path
        char destFilePath[MAX_PATH];
        strcpy(destFilePath, destFolderPath);
        int len = strlen(destFilePath);
        if (destFilePath[len - 1] != '\\' && destFilePath[len - 1] != '/') {
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

            switch (dwErrorCode) {
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
    }


    bool bCopyFileToFolder(const char* filePath, const char* destFolderPath)
    {
        // Get the file name from the file path
        const char* fileName = strrchr(filePath, '\\');
        if (!fileName) {
            // The file path is invalid
            return false;
        }
        fileName++; // Advance past the backslash

        // Construct the destination file path by appending the file name to the destination folder path
        char destFilePath[MAX_PATH];
        strcpy(destFilePath, destFolderPath);
        int len = strlen(destFilePath);
        if (destFilePath[len - 1] != '\\' && destFilePath[len - 1] != '/') {
            destFilePath[len] = '\\'; // Add a backslash if needed
            destFilePath[len + 1] = '\0';
        }
        strcat(destFilePath, fileName);
        // Attempt to copy the file to the destination file path
        if (CopyFileA(filePath, destFilePath, FALSE))
        {
            return true;
        }
        else
        {
            DWORD dwErrorCode = GetLastError();
            switch(dwErrorCode)
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

            return false;
        }
    }
    
};

int main()
{
    
    int iChoice ;
    HANDLE hFile;
    int iProcessID = 0;
    int ThreadId = 0;
    CFileManagement file;
    LPSTR sFilePath, sDestinationPath;
    
    do
    {
        /* code */
        cout << "FILE MANAGEMENT SYSTEM" << endl;
        cout << "0} Exit" <<endl;
        cout << "1} Run/Open" << endl;
        cout << "2} Delete File" <<endl;
        cout << "3} Create Backups"<<endl;
        cout << "4} Copy File" <<endl;
        cout << "5} Move File"<<endl;
        cout << "Enter Choice" << endl;

        cin>>iChoice;
        switch (iChoice)
        {
        case 0:
            exit(1);
            break;
        case 1:
            file.bOpen();
            break;
        case 2:
            cout <<"Enter File Path : ";
            cin >> sFilePath;
            file.bDeleteFile(sFilePath);
            break;
        case 3:
            cout <<"Enter File Path : ";
            cin >> sFilePath;
            file.bCreateFileBackup(sFilePath);
            break;
        case 4:
            cout <<"Enter File Path : ";
            cin >> sFilePath;
            cout << "Enter DEtination Folder : ";
            cin >> sDestinationPath;
            file.bCopyFileToFolder(sFilePath, sDestinationPath);
            break;
        case 5:
            cout <<"Enter File Path : ";
            cin >> sFilePath;
            cout << "Enter DEtination Folder : ";
            cin >> sDestinationPath;
            file.bMoveFileToFolder(sFilePath, sDestinationPath);
            break;
        
        default:
            break;
        }
    } while (iChoice);
    
}
