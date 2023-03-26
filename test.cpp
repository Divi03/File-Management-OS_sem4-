#include <windows.h>

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        // Get the file path from command-line argument
        const char* filePath = argv[1];

        // Use ShellExecute to open the file with default program
        HINSTANCE result = ShellExecute(NULL, "open", filePath, NULL, NULL, SW_SHOWNORMAL);

        if ((int)result <= 32)
        {
            // An error occurred, display error message
            MessageBox(NULL, "Failed to open file", "Error", MB_ICONERROR | MB_OK);
            return 1;
        }

        // File opened successfully
        return 0;
    }
    else
    {
        // Incorrect number of arguments, display usage information
        MessageBox(NULL, "Usage: OpenFile <FilePath>", "Error", MB_ICONERROR | MB_OK);
        return 1;
    }
}
