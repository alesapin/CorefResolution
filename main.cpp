#include <iostream>
#include <uima/api.hpp>
#include "SyntParser/TurboParser.h"
#include "CorefTrainer/Document.h"
#include <dirent.h>

using namespace std;

int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

int main() {
    //Путь к турбопарсеру.
    synt::TurboParser tp("/home/alesapin/Code/cpp/mmaot/depsparser");
    //synt::TurboParser tp("/home/kir/UIMA/RussianDependencyParser");
    //string dir = string("/home/kir/UIMA/CorefResolution/bin/anaphoraTrain2016/AnaphFiles/OFC");
    string dir = string("/home/alesapin/Code/cpp/mmaot/depsparser/bin/anaphoraTrain2016/AnaphFiles/OFC");
    vector<string> files = vector<string>();
    getdir(dir,files);
    std::sort(files.begin(), files.end());
    int percent = files.size()*0.9;
    for (int i=0; i<percent; i++)
    {
        if (files[i][files[i].length()-1]=='t') {
            coref::Document d("OFC/",files[i].c_str(),&tp);
            d.loadCorefFromXml("anaphoraTrain2016/anaph_new.xml");
            //std::cerr<<d<<"\n";
            d.writeTiplesToFile("/home/kir/UIMA/CorefResolution/bin/matrix.txt");
        }
    }



    return 0;
}