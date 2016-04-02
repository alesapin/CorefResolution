#include <iostream>
#include <uima/api.hpp>
//#include <../../uimacpp-build/include/uima/api.h>
#include "SyntParser/TurboParser.h"
using namespace std;

int main() {
    //Путь к турбопарсеру.
    synt::TurboParser tp("/home/kir/UIMA/RussianDependencyParser/TurboParser2");
    std::vector<synt::ParsedWord> res = tp.parse("Мама мыла окно, которое было разбито.");
    for(synt::ParsedWord p : res){
        std::cout << p.form <<" ";
        std::cout << p.normalForm <<" ";
        std::cout << (int)p.sp <<" ";
        //std::cout << (int)p.referedSp <<" ";
        //std::cout << p.referNum <<"\n";Fy
        for(int i = 0;i<p.morphChars.size();++i) {
            std::cout << p.morphChars[i] << " ";
        }
        std::cout<<"\n";
    }
    return 0;
}
