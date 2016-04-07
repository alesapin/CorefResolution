//
// Created by alesapin on 07.04.16.
//
#include "Util.h"
namespace util {
    void getdir (std::string dir, std::vector<std::string> &files,const std::set<std::string>& except) {
        DIR *dp;
        struct dirent *dirp;
        if((dp  = opendir(dir.c_str())) == NULL) {
            std::cerr << "Error(" << errno << ") opening " << dir << std::endl;
            return;
        }

        while ((dirp = readdir(dp)) != NULL) {
            std::string filename(dirp->d_name);
            if(except.find(filename) == except.end()) {
                files.push_back(dir + "/" + std::string(dirp->d_name));
            }
        }
        closedir(dp);
    }

    std::vector<std::string> getFiles(std::initializer_list<std::string> paths,
                                      const std::set<std::string> &except) {
        std::vector<std::string> result;
        for(const std::string& path : paths){
            getdir(path,result,except);
        }
        std::sort(result.begin(), result.end());
        return result;
    }


}