#include <stdexcept>
#include <Windows.h>

class FileChecker {
public:
    void checkWritable(const char* filename) const {
        DWORD fileAttributes = GetFileAttributes(filename);

        if (fileAttributes != INVALID_FILE_ATTRIBUTES) {
            if ((fileAttributes & FILE_ATTRIBUTE_READONLY) != 0) {
                throw std::runtime_error("File is read-only");
            }
        } else {
            throw std::runtime_error("Error checking file attributes");
        }
    }
};
