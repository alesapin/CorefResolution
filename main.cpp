#include <iostream>
#include "SyntParser/TurboParser.h"
#include "CorefTrainer/Document.h"
#include "CorefTrainer/Classifier.h"

#include <dirent.h>
#include "CorefTrainer/SingapourClassifier.h"

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
    synt::TurboParser tp("/home/kir/UIMA/RussianDependencyParser");
    string dir = string("/home/kir/UIMA/RussianDependencyParser/CorefResolution/bin/anaphoraTrain2016/AnaphFiles//OFC");
//    synt::TurboParser tp("/home/alesapin/Code/cpp/mmaot/depsparser");
//    //string dir = string("/home/alesapin/Code/cpp/mmaot/depsparser/bin/anaphoraTrain2016/AnaphFiles/OFC");
    vector<string> files = vector<string>();
    getdir(dir,files);
    std::sort(files.begin(), files.end());
    int percent = files.size()*0.9;
//    for (int i=0; i<percent; i++)
//    {
//        if (files[i][files[i].length()-1]=='t') {
//            coref::Document d("OFC/",files[i].c_str(),&tp);
//            d.loadCorefFromXml("anaphoraTrain2016/anaph_new.xml");
//            //std::cerr<<d<<"\n";
//            //d.writeTiplesToFile("/home/kir/UIMA/CorefResolution/bin/matrix.txt");
//        }
//    }

    //Число внутренних слоев, число эпох.
    coref::SingapourClassifier scl(15,10000);
    //cl.train("matrix.data");
    scl.cl->load("neuron.net");
    std::vector<coref::Document> notAnalizedDocuments;
    for (int i=percent+1; i<files.size();  i++)//files.size();
    //for (int i=0; i<percent; i++)
    {
        if (files[i][files[i].length()-1]=='t') {
            coref::Document d("OFC/",files[i].c_str(),&tp);
            cout<<files[i]<<"\n";
            d.loadCorefFromXml("anaphoraTrain2016/anaph_new.xml");
            //std::cerr<<d<<"\n";
            //d.writeTiplesToFile("/home/kir/UIMA/CorefResolution/bin/matrix.txt");
            notAnalizedDocuments.push_back(d);
        }
    }
    for(coref::Document& d : notAnalizedDocuments){
//        std::vector<coref::ClassifiedTriple> neuronResult = cl.run(d.getTriples());
//        std::vector<coref::ClassifiedTriple> documentResult = d.getClassifiedTriples();
//        //проверить совпадение классифицированных триплов.
//        int plus = 0;
//        int minus = 0;
//        for (int k = 0; k<documentResult.size(); k++)
//        {
//            if (std::get<3>(neuronResult[k]) == std::get<3>(documentResult[k]))
//                plus++;
//            else
//                minus++;
//        }
//        cout << "plus = "<<plus<<" \\minus = "<<minus<<"\n";
        std::vector<coref::Chain> neuronCoref = scl.EvaluateCorefFromneuralNetwork(d);
        int plus = 0;
        int minus = 0;
        for (auto curCoref = neuronCoref.begin(); curCoref != neuronCoref.end(); ++curCoref)
        {
            coref::Chain curChain = *curCoref;
            if (d.findCorefence(curChain[0], curChain[1]))
                plus++;
            else
                minus++;
        }
        cout << "plus = "<<plus<<" \\minus = "<<minus<<"\n";
    }
    return 0;
}