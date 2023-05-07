#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

bool hasAccess(const char* filename, int mode) {
  // Check if current user has access to file
  return access(filename, mode) == 0;
}
bool isHidden(const char* filename) {
  // Check if filename starts with a dot (.)
  std::string str(filename);
  return str.size() > 0 && str[0] == '.';
}
int main() {
  const char* filename = "X:\\FileManagement_project\\test\\ReadOnly.txt";

  try {
    // Get file status
    struct stat file_stat;
    if (stat(filename, &file_stat) != 0) {
      throw std::runtime_error("Failed to get file status.");
    }

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
  if(!(file_stat.st_mode & S_IRUSR) && !(file_stat.st_mode & S_IWUSR) && !(file_stat.st_mode & S_IXUSR))
    std::cout << "User Access Denied";
  std::cout << std::endl;

   if (isHidden(filename)) {
    std::cout << "*" << std::endl;
  } else {
    std::cout << "-*-" << std::endl;
  }
  }
  catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }


//   // Check if user has access to file
//   if (hasAccess(filename, R_OK)) {
//     std::cout << "User has read access to file." << std::endl;
//   } else {
//     std::cout << "User does not have read access to file." << std::endl;
//   }
//   if (hasAccess(filename, W_OK)) {
//     std::cout << "User has write access to file." << std::endl;
//   } else {
//     std::cout << "User does not have write access to file." << std::endl;
//   }
//   if (hasAccess(filename, X_OK)) {
//     std::cout << "User has execute access to file." << std::endl;
//   } else {
//     std::cout << "User does not have execute access to file." << std::endl;
//   }

  return 0;
}
