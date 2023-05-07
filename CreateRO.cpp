#include <stdexcept>
#include <Windows.h>

void makeFileReadOnly(const char* filename) {
    if (!SetFileAttributesA(filename, FILE_ATTRIBUTE_READONLY)) {
        throw std::runtime_error("Error setting file attributes");
    }
}

int main() {
    const char* filename = "example.txt";
    try {
        makeFileReadOnly(filename);
        // File is now read-only
    } catch (const std::exception& e) {
        // Handle error
    }
    return 0;
}
