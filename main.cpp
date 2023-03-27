/*PATH "C:\\TestingFileSys"*/

// git controlled env
// now in collaborative mode

#include<iostream>
#include<windows.h>
#include<string>
using namespace std;

class FileManagement{
    private:
    int x;
    DWORD arr[10];
    string sPath;
    HANDLE hFile;
    HANDLE hProcess;
    HANDLE hThread;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    DWORD dwProcessId = 0;
    DWORD deThreadId = 0;
    // ZeroMemory(&si,sizeof(si));
    // ZeroMemory(&pi,sizeof(pi));

    public:
    void rOpen()
    {
        LPSTR path;
        cout << "Inside Open function"<<endl;
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
        WaitForSingleObject(pi.hProcess,INFINITE);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }


    bool deleteFile(const char* filePath)
    {
        // Attempt to delete the file
        if (DeleteFileA(filePath))
        {
            return true;
        }
        else
        {
            // Get the last Win32 error code
            DWORD errorCode = GetLastError();

            if (errorCode == ERROR_FILE_NOT_FOUND)
            {
                // The file doesn't exist, so consider it deleted
                cout << "Such a file doesn't exist in the system" << endl;
                return true;
            }
            else
            {
                cout << "File could not be deleted" << endl;
                // add error handling here to some extent
                return false;
            }
        }
    }

    bool createFileBackup(const char* filePath)
    {
        // Get the current time
        SYSTEMTIME currentTime;
        GetSystemTime(&currentTime);

        // Construct the backup file name by appending a timestamp to the original file name
        char backupFilePath[MAX_PATH];
        sprintf_s(backupFilePath, "%s.%04d%02d%02d%02d%02d%02d.bak",
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


    bool moveFileToFolder(const char* filePath, const char* destFolderPath)
    {
        // Construct the destination file path by appending the file name to the destination folder path
        char destFilePath[MAX_PATH];
        PathCombineA(destFilePath, destFolderPath, PathFindFileNameA(filePath));

        // Attempt to move the file to the destination file path
        if (MoveFileA(filePath, destFilePath))
        {
            return true;
        }
        else
        {
            cout << "Failed to move file" << endl;
            // add error handling
            return false;
        }
    }


    bool copyFileToFolder(const char* filePath, const char* destFolderPath)
    {
        // Construct the destination file path by appending the file name to the destination folder path
        char destFilePath[MAX_PATH];
        PathCombineA(destFilePath, destFolderPath, PathFindFileNameA(filePath));

        // Attempt to copy the file to the destination file path
        if (CopyFileA(filePath, destFilePath, FALSE))
        {
            return true;
        }
        else
        {
            cout << "Failed to copy file" << endl;
            // add error handling
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
    FileManagement file;
    
    do
    {
        /* code */
        cout << "FILE MANAGEMENT SYSTEM" << endl;
        cout << "0} Exit" <<endl;
        cout << "1} Run/Open" << endl;
        cout << "2} Delete File" <<endl;
        cout << "3} Create Backups"<<endl;
        cout << "Enter Choice" << endl;

        cin>>iChoice;
        switch (iChoice)
        {
        case 0:
            exit(1);
            break;
        case 1:
            file.rOpen();
            break;
        
        default:
            break;
        }
    } while (iChoice);
    
}
