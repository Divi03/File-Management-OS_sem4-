/*PATH "C:\\TestingFileSys"*/

// git controlled env



#include<iostream>
#include<windows.h>
#include<string>
using namespace std;

class FileManagement{
    private:
    int x;
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

    void DeleteFile()
    {
        hFile = CreateFileA(
            "Y:\test",
            GENERIC_WRITE, 
            0,
            NULL,
            OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL


        );
        BOOL DeleteFile(
                    
                    );
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
