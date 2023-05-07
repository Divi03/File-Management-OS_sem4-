#include <iostream>
#include <unistd.h>

bool check_file_access(const char* filename, int mode) {
  if (access(filename, mode) == 0) {
    std::cout << "User has access to " << filename << std::endl;
    return true;
  } else {
    std::cout << "User does not have access to " << filename << std::endl;
    return false;
  }
}

int main() {
  const char* filename = "X:\\FileManagement_project\\test\\ReadOnly.txt";

  // Check read access
  check_file_access(filename, R_OK);

  // Check write access
  check_file_access(filename, W_OK);

  // Check execute access
  check_file_access(filename, X_OK);

  return 0;
}
