#include <iostream>
#include "SyntParser/TurboParser.h"
#include "CorefTrainer/Document.h"
#include "CorefTrainer/Classifier.h"
#include "CorefTrainer/Util.h"
#include "CorefTrainer/SingapourClassifier.h"

using namespace std;



int main() {
    //Путь к турбопарсеру.
    synt::TurboParser tp("/home/alesapin/Code/cpp/mmaot/depsparser");
    vector<string> files = util::getFiles({"anaphoraTrain2016/AnaphFiles/lentaru","anaphoraTrain2016/AnaphFiles/OFC"},{".","..","lentaru014.txt","123.txt"});

    std::vector<coref::Document> documents;
    for (int i=0; i<files.size(); ++i)  {
        std::cerr<<"Filename:"<<files[i]<<"\n";
        std::cerr<<"Finished:"<<(double)(i)*100/(files.size())<<"\n";
        coref::Document d(files[i],&tp);
        d.loadCorefFromXml("anaphoraTrain2016/anaph_new.xml");
        documents.push_back(d);
    }
    std::vector<coref::Document> train(documents.begin(),documents.begin()+documents.size()-10);
    std::vector<coref::Document> test(documents.begin()+documents.size()-10,documents.end());
    coref::Classifier cls(10, 20000);

    cls.train(train);
    //cls.trainFromFile("train.data");
    cls.save("neuron.txt");
    cls.load("neuron.txt");
    coref::SingapourClassifier singClass(cls);
    std::tuple<double,double> precRec = singClass.validate(test);
    std::cerr<<"Precision: "<<std::get<0>(precRec)<<"\n";
    std::cerr<<"Recall: "<<std::get<1>(precRec)<<"\n";

    return 0;
}