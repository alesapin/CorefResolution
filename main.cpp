#include <iostream>
#include <uima/api.hpp>
#include "SyntParser/TurboParser.h"
#include "CorefTrainer/Document.h"
using namespace std;

int main() {
    //Путь к турбопарсеру.
    synt::TurboParser tp("/home/alesapin/Code/cpp/mmaot/depsparser");
//    synt::ParsedPharse res = tp.invokeTurboParser("который ходил");
//    std::cerr<<res.text<<"\n";
//    std::cerr<<res.mainWord<<"\n";
//    std::cerr<<res.morphChars[0]<<"\n";
    coref::Document d("OFC/","4.txt",&tp);
    d.loadCorefFromXml("anaphoraTrain2016/anaph_new.xml");
    return 0;
}