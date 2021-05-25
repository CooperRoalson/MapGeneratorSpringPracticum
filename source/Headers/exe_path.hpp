//
//  exe_path.h
//  Map Generator
//
//  Taken from:
//  https://stackoverflow.com/questions/1528298/get-path-of-executable

#ifndef exe_path_hpp
#define exe_path_hpp

#include <string>

namespace ExePath {
  std::string getWorkingDir();
  std::string getExecutablePath();
  std::string getExecutableDir();
  std::string mergePaths(std::string pathA, std::string pathB);
  bool checkIfFileExists (const std::string& filePath);

}

#endif /* exe_path_hpp */
