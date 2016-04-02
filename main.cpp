#include <iostream>
#include <uima/api.hpp>
#include "SyntParser/TurboParser.h"
#include "CorefTrainer/Document.h"
using namespace std;

int main() {
    //Путь к турбопарсеру.
    synt::TurboParser tp("/home/alesapin/Code/cpp/mmaot/depsparser");

    coref::Document d("OFC/","5.txt",&tp);
    d.loadCorefFromXml("anaphoraTrain2016/anaph_new.xml");
    std::cerr<<d<<"\n";
    return 0;
}