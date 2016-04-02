//
// Created by alesapin on 02.04.16.
//

#include "ParsedPhrase.h"
namespace synt {

    std::string toString(Gender gen) {
        if(gen == Gender::MASC) return "masc";
        if(gen == Gender::FEMN) return "femn";
        if(gen == Gender::NEUT) return "neut";
        if(gen == Gender::OVERALL) return "Ms-f";
        return "undef";
    }

    std::string toString(Number num) {
        if(num == Number::SNG) return "sing";
        if(num == Number::PLR) return "plur";
        return "undef";
    }

    std::string toString(Person p) {
        if(p == Person::FP) return "1per";
        if(p == Person::SP) return "2per";
        if(p == Person::TP) return "3per";
        return "undef";
    }

    std::ostream &operator<<(std::ostream &os, const ParsedPharse &obj) {
        os <<"[Text: " << obj.text<<";";
        os <<" Main Word: "<<obj.mainWord<<";";
        os <<" Main word NF: "<<obj.mainWordNormalForm<<";";
        os <<" SP: ";
        if(obj.sp == synt::SpeechPart::NOUN) {
            os << "NOUN;";
        }else{
            os <<"NPRO;";
        }
        os << " Chars: ( ";
        os <<"Gen: " << toString(obj.gen) <<" | ";
        os <<"Num: "<<toString(obj.num) <<" | ";
        os <<"Pers: "<<toString(obj.pers);
        os <<");";
        os<< " Shift:" << obj.shift<<";]";
        return os;
    }

}

bool synt::ParsedPharse::operator==(const synt::ParsedPharse &o) const {
    if(text != o.text) return false;
    if(mainWord != o.mainWord) return false;
    if(mainWordNormalForm != o.mainWordNormalForm) return false;
    if(sp != o.sp) return false;
    if(gen != o.gen) return false;
    if(num != o.num) return false;
    if(pers != o.pers) return false;
    if(shift != o.shift) return false;
    return true;
}

bool synt::ParsedPharse::operator<(const ParsedPharse &o) const {
    if(shift == o.shift){
        return text.length() < o.text.length();
    }
    return shift < o.shift;

}



