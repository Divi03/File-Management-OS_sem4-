#include <iostream>
#include "access.cpp"

int main() {
    const char* filename = "ReadOnly.txt";

    try {
        FileChecker fileChecker;
        fileChecker.checkWritable(filename);
        // Do something with the file here
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
