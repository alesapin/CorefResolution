#include <iostream>
#include <uima/api.hpp>
#include "SyntParser/TurboParser.h"
using namespace std;

int main() {
    //Путь к турбопарсеру.
    synt::TurboParser tp("/home/alesapin/Code/cpp/mmaot/depsparser");
    std::vector<synt::ParsedWord> res = tp.parse("шла Александра по шоссе и сосала сушку");
    for(synt::ParsedWord p : res){
        std::cout << p.form <<" ";
        std::cout << p.normalForm <<" ";
        std::cout << (int)p.sp <<" ";
        std::cout << (int)p.referedSp <<" ";
        std::cout << p.referNum <<"\n";
        for(int i = 0;i<p.morphChars.size();++i) {
            std::cout << p.morphChars[i] << " ";
        }
        std::cout<<"\n";
    }
    return 0;
}