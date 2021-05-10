//
//  exe_path.cpp
//  Map Generator
//
//  Taken from:
//  https://stackoverflow.com/questions/1528298/get-path-of-executable

#include "exe_path.hpp"

#if defined(_WIN32)
    #include <windows.h>
    #include <Shlwapi.h>
    #include <io.h>
    #include <direct.h>

    #define access _access_s
    #define GetCurrentDir _getcwd
#endif

#ifdef __APPLE__
    #include <libgen.h>
    #include <limits.h>
    #include <mach-o/dyld.h>
    #include <unistd.h>
    
    #define GetCurrentDir getcwd
#endif

#ifdef __linux__
    #include <limits.h>
    #include <libgen.h>
    #include <unistd.h>

    #define GetCurrentDir getcwd

    #if defined(__sun)
        #define PROC_SELF_EXE "/proc/self/path/a.out"
    #else
        #define PROC_SELF_EXE "/proc/self/exe"
    #endif

#endif

namespace ExePath {

    #if defined(_WIN32)

        std::string getExecutablePath() {
           char rawPathName[MAX_PATH];
           GetModuleFileNameA(NULL, rawPathName, MAX_PATH);
           return std::string(rawPathName);
        }

        std::string getExecutableDir() {
            std::string executablePath = getExecutablePath();
            char* exePath = new char[executablePath.length()];
            strcpy(exePath, executablePath.c_str());
            PathRemoveFileSpecA(exePath);
            std::string directory = std::string(exePath);
            delete[] exePath;
            return directory;
        }

        std::string mergePaths(std::string pathA, std::string pathB) {
          char combined[MAX_PATH];
          PathCombineA(combined, pathA.c_str(), pathB.c_str());
          std::string mergedPath(combined);
          return mergedPath;
        }

    #endif

    #ifdef __linux__
        std::string getExecutablePath() {
           char rawPathName[PATH_MAX];
           realpath(PROC_SELF_EXE, rawPathName);
           return  std::string(rawPathName);
        }

        std::string getExecutableDir() {
            std::string executablePath = getExecutablePath();
            char *executablePathStr = new char[executablePath.length() + 1];
            strcpy(executablePathStr, executablePath.c_str());
            char* executableDir = dirname(executablePathStr);
            delete [] executablePathStr;
            return std::string(executableDir);
        }

        std::string mergePaths(std::string pathA, std::string pathB) {
          return pathA+"/"+pathB;
        }

    #endif

    #ifdef __APPLE__
        std::string getExecutablePath() {
            char rawPathName[PATH_MAX];
            char realPathName[PATH_MAX];
            uint32_t rawPathSize = (uint32_t)sizeof(rawPathName);

            if(!_NSGetExecutablePath(rawPathName, &rawPathSize)) {
                realpath(rawPathName, realPathName);
            }
            return  std::string(realPathName);
        }

        std::string getExecutableDir() {
            std::string executablePath = getExecutablePath();
            char *executablePathStr = new char[executablePath.length() + 1];
            strcpy(executablePathStr, executablePath.c_str());
            char* executableDir = dirname(executablePathStr);
            delete [] executablePathStr;
            return std::string(executableDir);
        }

        std::string mergePaths(std::string pathA, std::string pathB) {
            return pathA+"/"+pathB;
        }
    #endif


    bool checkIfFileExists (const std::string& filePath) {
       return access( filePath.c_str(), 0 ) == 0;
    }

    std::string getWorkingDir() {
        char buff[FILENAME_MAX];
        GetCurrentDir( buff, FILENAME_MAX );
        std::string current_working_dir(buff);
        return current_working_dir;
    }

}
