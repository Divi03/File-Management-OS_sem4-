#include <iostream>
#include <sys/stat.h>

int main() {
  const char* filename = "X:\\FileManagement_project\\test\\ReadOnly.txt";

  // Get file status
  struct stat file_stat;
  if (stat(filename, &file_stat) != 0) {
    std::cerr << "Failed to get file status." << std::endl;
        return 1;
  }

  // Check file permissions
  std::cout << "File permissions: ";
  if (file_stat.st_mode & S_IRUSR) {
    std::cout << "r";
  } else {
    std::cout << "-";
  }
  if (file_stat.st_mode & S_IWUSR) {
    std::cout << "w";
  } else {
    std::cout << "-";
  }
  if (file_stat.st_mode & S_IXUSR) {
    std::cout << "x";
  } else {
    std::cout << "-";
  }
  std::cout << std::endl;

  return 0;
}
