#include <iostream>
#include "SyntParser/TurboParser.h"
#include "CorefTrainer/Document.h"
#include "CorefTrainer/Classifier.h"
#include "CorefTrainer/Util.h"
#include "CorefTrainer/SingapourClassifier.h"
#include <iomanip>

using namespace std;



int main() {
    //Путь к турбопарсеру.
    synt::TurboParser tp("/home/kir/UIMA/RussianDependencyParser");
    //synt::TurboParser tp("/home/alesapin/Code/cpp/mmaot/depsparser");
    vector<string> files = util::getFiles({"anaphoraTrain2016/AnaphFiles/lentaru","anaphoraTrain2016/AnaphFiles/OFC"},{".","..","lentaru014.txt","123.txt"});

    std::vector<coref::Document> documents;
    for (int i=0; i< /*10*/ files.size(); ++i)  {
        std::cerr<<"Filename:"<<files[i]<<"\n";
        std::cerr<<"Finished:"<<(double)(i)*100/(files.size())<<"\n";
        coref::Document d(files[i],&tp);
        d.loadCorefFromXml("anaphoraTrain2016/anaph_new.xml");
        documents.push_back(d);
    }



    int percent = files.size()*0.3; //10*0.1; //Процент текстов для тренеровки


    coref::Classifier cls(10, 20000);
    std::vector<double> precision;
    std::vector<double> recall;
    for (int i = 0; i < /*10-percent*/ files.size()-percent; i+=percent)
    {
        std::vector<coref::Document> train;
        train.insert(train.end(), documents.begin(),documents.begin()+i);
        train.insert(train.end(), documents.begin()+i+percent,documents.end());
        std::vector<coref::Document> test(documents.begin()+i,documents.begin()+i+percent);
        cls.train(train);
        cls.save("neuron.txt");
        cls.load("neuron.txt");
        coref::SingapourClassifier singClass(cls);
        std::tuple<double,double> precRec = singClass.validate(test);
        precision.push_back(std::get<0>(precRec));
        recall.push_back(std::get<1>(precRec));
        train.clear();
        test.clear();
        cls.createNew();
    }

    double totalRecall = 0;
    double totalPrecision = 0;
    for (int i = 0; i < precision.size(); i++)
    {
        double f_measure = 0;
        if (precision[i] > 0 || recall[i] >0) {
            f_measure = 2 * (precision[i] * recall[i]) / (precision[i] + recall[i]);
        }
        std::cerr << setprecision(5) << "Precision: " << precision[i] << "\tRecall: " << recall[i] <<
        "\tF-measure: " << f_measure << std::endl;
        totalPrecision += precision[i];
        totalRecall += recall[i];
    }
    double averagePrecision = totalPrecision/precision.size();
    double averageRecall = totalRecall/recall.size();
    double F_measure = 0;
    if (averagePrecision > 0 || averageRecall > 0)
        F_measure = 2*(averagePrecision*averageRecall)/(averagePrecision+averageRecall);
    std::cerr<<setprecision(5)<<"Average Precision: "<<averagePrecision<<"\tAverage Recall: "<<averageRecall<<"\tF-measure: "<<F_measure<<std::endl;

//    std::vector<coref::Document> train(documents.begin(),documents.begin()+documents.size()-10);
//    std::vector<coref::Document> test(documents.begin()+documents.size()-10,documents.end());
//    coref::Classifier cls(10, 20000);
//
//    cls.train(train);
//    //cls.trainFromFile("train.data");
//    cls.save("neuron.txt");
//    cls.load("neuron.txt");
//    coref::SingapourClassifier singClass(cls);
//    std::tuple<double,double> precRec = singClass.validate(test);
//    std::cerr<<"Precision: "<<std::get<0>(precRec)<<"\n";
//    std::cerr<<"Recall: "<<std::get<1>(precRec)<<"\n";

    return 0;
}