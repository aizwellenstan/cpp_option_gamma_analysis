#include "fileLister.h"
#include <dirent.h>

std::vector<std::string> listFilesInDirectory(const std::string& folderPath) {
    std::vector<std::string> files;
    DIR* dir;
    struct dirent* ent;

    if ((dir = opendir(folderPath.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_type == DT_REG) {
                files.push_back(folderPath + "/" + ent->d_name);
            }
        }
        closedir(dir);
    }

    return files;
}
