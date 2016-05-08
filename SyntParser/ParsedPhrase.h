//
// Created by alesapin on 02.04.16.
//

#ifndef COREFRESOLUTION_PARSEDPHRASE_H
#define COREFRESOLUTION_PARSEDPHRASE_H
#include <string>
#include <iostream>
#include <functional>
namespace synt {
    /**
     * All interesting speech parts.
     */
    enum class SpeechPart{
        UNDEF = 0, NOUN = 1, NPRO = 2
    };
    enum class Gender {
        UNDEF = 0, MASC = 1, FEMN = 2, NEUT = 3, OVERALL = 4
    };
    enum class Number {
        UNDEF = 0, SNG = 1, PLR = 2
    };
    enum class Person {
        UNDEF = 0, FP = 1, SP = 2, TP = 3
    };

    std::string toString(Gender gen);

    std::string toString(Number num);

    std::string toString(Person p);
    /**
     * Parser word output.
     */
    struct ParsedPhrase {
        std::string text;
        std::string mainWord;
        std::string mainWordNormalForm;
        SpeechPart sp;
        std::size_t shift;
        std::string groupType;
        Gender gen;
        Number num;
        Person pers;
        bool operator<(const ParsedPhrase& o) const;
        bool operator==(const ParsedPhrase& o) const;
        bool operator!=(const ParsedPhrase& o) const {return !(*this == o);}
    };

    std::ostream & operator<<(std::ostream &os, const ParsedPhrase &obj);
}
namespace std {
    template <>
    struct hash<synt::ParsedPhrase> {
        std::size_t operator()(const synt::ParsedPhrase& k) const {
            using std::size_t;
            using std::hash;
            using std::string;


            return ((hash<string>()(k.text)
                     ^ (hash<string>()(k.mainWord) << 1)) >> 1)
                   ^ (hash<string>()(k.mainWordNormalForm) << 1) ^ (hash<int>()(k.shift));
        }
    };

}
#endif //COREFRESOLUTION_PARSEDPHRASE_H
