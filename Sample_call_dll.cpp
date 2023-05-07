#include <Windows.h>
#include <iostream>

// Declare the classes and functions that we will use from the DLL
typedef void (*PrintFunction)();
typedef void (*SetIntFunction)(int);

int main()
{
    // Load the DLL
    HINSTANCE hDll = LoadLibraryA("mydll.dll");
    if (hDll == NULL)
    {
        std::cerr << "Failed to load DLL: " << GetLastError() << std::endl;
        return 1;
    }

    // Get pointers to the functions we need
    PrintFunction printFunction = reinterpret_cast<PrintFunction>(GetProcAddress(hDll, "printDetails"));
    
    // Call the functions
    if (printFunction != NULL)
    {
        std::cout << "Calling printMessage() from DLL..." << std::endl;
        printFunction();
    }
    else
    {
        std::cerr << "Failed to find function: printMessage" << std::endl;
    }

    // Unload the DLL
    FreeLibrary(hDll);

    return 0;
}
