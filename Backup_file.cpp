// Header files and their purposes:
#include <Windows.h>      // Provides access to Windows API functions and data types.
#include <string>         // Defines the string class and related functions.
#include <fstream>        // Defines file stream classes to read/write to files.
#include <iomanip>        // Provides functions to manipulate input/output formatting.
#include <chrono>         // Defines classes to measure time durations and intervals.
#include <ctime>          // Provides functions to manipulate date and time information.
#include <iostream>       // Defines input/output streams and functions for console input/output.
#include <vector>         // Defines the vector container class and related functions.
#include <sstream>        // Defines classes for string stream input/output.
#include <algorithm>      // Provides a collection of functions for performing common algorithms.
#include "C:\zlib\\zlib.h" // Provides functions for lossless data compression.

using namespace std;

// A class for file backup functionality
class FileBackup
{

    string source_dir_; // The source directory to backup
    string dest_dir_;   // The destination directory to backup to

    // Function to create a backup folder with a timestamped name
public:
    string getSourceDir() const
    {
        return source_dir_;
    }

    string CreateBackupFolder()
    {
        // Get the current system time
        auto now = chrono::system_clock::now();

        // Convert the system time to a time_t object
        auto in_time_t = chrono::system_clock::to_time_t(now);

        // Convert the time_t object to a tm struct
        tm tm = *localtime(&in_time_t);

        // Create a string stream to format the date and time
        ostringstream oss;
        oss << put_time(&tm, "%Y-%m-%d_%H-%M-%S");

        // Create the backup folder name with the formatted date and time
        string backup_folder_name = "backup_" + oss.str();

        // Create the backup folder path by appending the folder name to the destination directory
        string backup_folder_path = dest_dir_ + "\\" + backup_folder_name;

        // Attempt to create the backup folder using the Windows API function CreateDirectoryA()
        if (!CreateDirectoryA(backup_folder_path.c_str(), NULL))
        {
            // If CreateDirectoryA() returns false, there was an error
            DWORD error = GetLastError();

            // Output an error message to the standard error stream
            cerr << "Failed to create backup folder: " << error << endl;

            // Terminate the program with an error code
            exit(1);
        }

        // Return the path to the created backup folder
        return backup_folder_path;
    }

    // Returns a vector containing the names of all files in the specified directory
    vector<string> GetFiles(const string &dir)
    {
        vector<string> files;                                              // Initialize an empty vector to hold the file names
        WIN32_FIND_DATAA find_data;                                        // Struct to hold file information
        HANDLE handle = FindFirstFileA((dir + "\\*").c_str(), &find_data); // Find the first file in the directory
        if (handle == INVALID_HANDLE_VALUE)
        {                                                                                      // Check if the function failed
            DWORD error = GetLastError();                                                      // Get the error code
            cerr << "Failed to find first file in directory " << dir << ": " << error << endl; // Print an error message
            return files;                                                                      // Return the empty vector
        }
        do
        {
            if (!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {                                         // Check if the file is not a directory
                files.push_back(find_data.cFileName); // Add the file name to the vector
            }
        } while (FindNextFileA(handle, &find_data)); // Find the next file in the directory
        DWORD error = GetLastError();                // Get the error code
        if (error != ERROR_NO_MORE_FILES)
        {                                                                                     // Check if the function failed
            cerr << "Failed to find next file in directory " << dir << ": " << error << endl; // Print an error message
        }
        FindClose(handle); // Close the file handle
        return files;      // Return the vector containing the file names
    }

    void CompressFile(const string &source_path, const string &dest_path)
    {
        // Open input file for reading
        std::ifstream input(source_path, std::ios::binary);
        if (!input)
        {
            // Throw an exception if failed to open the input file
            throw std::runtime_error("Failed to open input file");
        }

        // Open output file for writing
        std::ofstream output(dest_path, std::ios::binary);
        if (!output)
        {
            // Throw an exception if failed to open the output file
            throw std::runtime_error("Failed to open output file");
        }

        // Compress input file and write to output file
        const int CHUNK = 4096;
        char in_buffer[CHUNK];
        char out_buffer[CHUNK];
        z_stream stream;
        stream.zalloc = Z_NULL;
        stream.zfree = Z_NULL;
        stream.opaque = Z_NULL;
        if (deflateInit(&stream, Z_DEFAULT_COMPRESSION) != Z_OK)
        {
            // Throw an exception if failed to initialize the compression stream
            throw std::runtime_error("Failed to initialize the compression stream");
        }
        int flush;
        do
        {
            // Read data from input file into buffer
            input.read(in_buffer, CHUNK);
            stream.avail_in = input.gcount();
            stream.next_in = reinterpret_cast<Bytef *>(in_buffer);
            flush = input.eof() ? Z_FINISH : Z_NO_FLUSH;
            do
            {
                // Compress the data in buffer and write it to output file
                stream.avail_out = CHUNK;
                stream.next_out = reinterpret_cast<Bytef *>(out_buffer);
                if (deflate(&stream, flush) == Z_STREAM_ERROR)
                {
                    // Throw an exception if failed to compress data
                    throw std::runtime_error("Failed to compress data");
                }
                output.write(out_buffer, CHUNK - stream.avail_out);
            } while (stream.avail_out == 0);
        } while (flush != Z_FINISH);
        deflateEnd(&stream);

        // Check if there was an error reading from the input file
        if (!input.eof())
        {
            throw std::runtime_error("Failed to read data from input file");
        }

        // Check if there was an error writing to the output file
        if (!output)
        {
            throw std::runtime_error("Failed to write data to output file");
        }
    }

    void WriteBackupInfo(const string &source_path, const string &dest_path)
    {
        try
        {
            // Open backup_info.txt for appending
            ofstream file(dest_dir_ + "\\backup_info.txt", ios::app);
            if (!file)
            {
                throw runtime_error("Failed to open backup_info.txt for writing");
            }

            // Write backup information to file
            auto now = chrono::system_clock::now();
            auto in_time_t = chrono::system_clock::to_time_t(now);
            file << "Time: " << put_time(localtime(&in_time_t), "%Y-%m-%d %H:%M:%S") << endl;
            file << "Original file/folder: " << source_path << endl;
            file << "Original size: " << GetFileSize(source_path) << " bytes" << endl;
            file << "Backup file: " << dest_path << endl;
            file << "Compressed size: " << GetFileSize(dest_path) << " bytes" << endl;
            file << endl;

            // Close the file
            file.close();
            if (file.fail())
            {
                throw runtime_error("Failed to close backup_info.txt");
            }
        }
        // Catch any exceptions that may occur during the file writing process
        catch (const exception &e)
        {
            cerr << "Error occurred while writing backup info: " << e.what() << endl;
        }
        // Catch any other unknown exceptions
        catch (...)
        {
            cerr << "Unknown error occurred while writing backup info" << endl;
        }
    }

    long long GetFileSize(const string &path)
    {
        // Open file for reading
        HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            // Throw runtime error if failed to open file
            throw std::runtime_error("Failed to open file");
        }

        // Get file size
        LARGE_INTEGER size;
        if (!GetFileSizeEx(hFile, &size))
        {
            // Throw runtime error if failed to get file size
            CloseHandle(hFile);
            throw std::runtime_error("Failed to get file size");
        }

        // Close file handle and return file size
        CloseHandle(hFile);
        return size.QuadPart;
    }

    // Constructor that takes source and destination directories
    FileBackup(const string &source_dir, const string &dest_dir)
    {
        source_dir_ = source_dir;
        dest_dir_ = dest_dir;
    }

    // Function to perform the backup operation
    void Backup()
    {
        try
        {
            // Create a backup folder for the current backup
            string backup_folder = CreateBackupFolder();

            // Get a list of files in the source directory
            vector<string> files = GetFiles(source_dir_);

            // Loop through each file and compress it
            for (const auto &file : files)
            {
                // Construct the source and destination paths for the file
                string source_path = source_dir_ + "\\" + file;
                string dest_path = backup_folder + "\\" + file + ".zip";

                // Compress the file and write it to the backup folder
                CompressFile(source_path, dest_path);
                WriteBackupInfo(source_path, dest_path);
            }
        }
        // Catch any exceptions that may occur during the backup process
        catch (const exception &e)
        {
            cerr << "Error occurred during backup: " << e.what() << endl;
        }
        // Catch any other unknown exceptions
        catch (...)
        {
            cerr << "Unknown error occurred during backup" << endl;
        }
    }
};

// int main()
// {
//     // Create a FileBackup object with source and destination directories
//     FileBackup backup("C:\\Users\\hp\\Desktop\\Env", "D:");

//     // Create a backup folder with a timestamped name
//     string backup_folder_path = backup.CreateBackupFolder();

//     // Get a vector containing the names of all files in the source directory
//     vector<string> files = backup.GetFiles(backup.getSourceDir());

//     // Compress each file in the source directory and write it to the backup folder
//     for (const auto &file : files)
//     {
//         string source_path = backup.getSourceDir() + "\\" + file;
//         string dest_path = backup_folder_path + "\\" + file + ".lz4";
//         try
//         {
//             backup.CompressFile(source_path, dest_path);
//         }
//         catch (const std::exception &e)
//         {
//             cerr << "Failed to compress file " << source_path << ": " << e.what() << endl;
//         }
//     }

//     return 0;
// }
