//
// Created by alesapin on 23.03.16.
//

#include <cursesw.h>
#include "TurboParser.h"
namespace synt{
    const std::string TurboParser::TURBO_PARSER_OUT = "turbo.txt";
    const std::string TurboParser::RUSDEPS_PARSER_OUT = "tmpBin";
    const std::size_t TurboParser::MORPH_SP_COL = 3;
    const std::size_t TurboParser::MORPH_WORDFORM_COL = 1;
    const std::size_t TurboParser::MORPH_NORMALFORM_COL = 2;
    const std::size_t TurboParser::SYNT_REFERED_NUM_COL = 6;
    const std::size_t TurboParser::MORPH_CHARS_COL = 5;
    const std::size_t TurboParser::SYNT_SP_COL = 3;
    const std::size_t TurboParser::SYNT_WORDFORM_COL = 1;

    const std::string TurboParser::SEP_START = "{ ";
    const std::string TurboParser::SEP_END = " }";

    std::vector<ParsedPhrase> TurboParser::invokeTurboParser(const std::vector<std::string>& req) const {

        std::string program = path + "/launch.sh \"" + prepareText(req) +"\" " + TURBO_PARSER_OUT;
        std::system(program.c_str());
        std::ifstream syntInform(TURBO_PARSER_OUT,std::ifstream::in);
        std::ifstream morfInform(RUSDEPS_PARSER_OUT,std::ifstream::in);
        std::string syntLine, morfLine;
        //Skip first line
        std::getline(syntInform,syntLine);
        std::getline(morfInform,morfLine);

        std::vector<ParsedPhrase> result;
        for(const std::string& phrase: req){
            result.push_back(parseOneBlock(phrase,syntInform,morfInform));
            fixStreams(syntInform,morfInform);
        }
        //std::remove(TURBO_PARSER_OUT.c_str());
        //std::remove(RUSDEPS_PARSER_OUT.c_str());
        return result;
    }

    bool TurboParser::checkNounOrNproLine(const std::string &line) const {
        std::vector<std::string> chars;
        boost::split(chars,line,boost::is_any_of("\t "));
        if(chars.size() <= SYNT_SP_COL) return false;
        return chars[SYNT_SP_COL] == "NOUN" || chars[SYNT_SP_COL] == "NPRO";
    }

    void TurboParser::generateLine(ParsedPhrase &ph, const std::string &morphLine) const{
        std::vector<std::string> chars;
        boost::split(chars,morphLine,boost::is_any_of("\t "));
        if(chars[MORPH_SP_COL] == "NOUN"){
            ph.sp = SpeechPart::NOUN;
        }else {
            ph.sp = SpeechPart::NPRO;
        }
        ph.mainWord = chars[MORPH_WORDFORM_COL];
        ph.mainWordNormalForm = chars[MORPH_NORMALFORM_COL];
        std::string morphChars = chars[MORPH_CHARS_COL];
        ph.gen = getGender(morphChars);
        ph.num = getNumber(morphChars);
        ph.pers = getPerson(morphChars);
    }

    std::string TurboParser::prepareText(const std::vector<std::string> &req) const {
        std::string result;
        for(const std::string& phrase : req){
            result += SEP_START + phrase + SEP_END + " ";
        }
        return result;
    }

    bool TurboParser::isSepLine(const std::string &line) const {
        std::vector<std::string> chars;
        boost::split(chars,line,boost::is_any_of("\t "));
        if(chars.size() <= SYNT_WORDFORM_COL) return false;
        char firstSym = chars[SYNT_WORDFORM_COL][0];
        //firstSym == '{' || '}' ?
        return firstSym == SEP_START[0] || firstSym == SEP_END[1];
    }

    bool TurboParser::tryMainWord(ParsedPhrase &phrase,  std::ifstream &syntStream,
                                  std::ifstream &morphStream) const {
        std::string syntLine;
        std::string morphLine;
        std::string minLine;
        int minLinePos = std::numeric_limits<int>::max();
        while(std::getline(syntStream,syntLine) && std::getline(morphStream,morphLine) && !isSepLine(syntLine)) {
            if (!syntLine.empty()) {
                int syntPos = getSyntPos(syntLine);
                if (minLinePos > syntPos) {
                    minLine = morphLine;
                    minLinePos = syntPos;
                }
            }
        }
        if(checkNounOrNproLine(minLine)){
            generateLine(phrase,minLine);
            return true;
        }
        return false;
    }

    std::size_t TurboParser::getSyntPos(const std::string &syntLine) const{
        std::vector<std::string> chars;
        boost::split(chars,syntLine,boost::is_any_of("\t "));
        if(chars.size() <= SYNT_REFERED_NUM_COL) return std::numeric_limits<int>::max() - 1;
        return std::stoi(chars[SYNT_REFERED_NUM_COL]);
    }

    bool TurboParser::tryFirstSpeechPart(ParsedPhrase &phrase,  std::ifstream &syntStream,
                                         std::ifstream &morphStream)  const{
        std::string syntLine;
        std::string morphLine;
        while(std::getline(syntStream,syntLine) && std::getline(morphStream,morphLine) && !isSepLine(syntLine)) {
            if (!syntLine.empty()) {
                if (checkNounOrNproLine(morphLine)) {
                    generateLine(phrase,morphLine);
                    return true;
                }
            }
        }
        return false;
    }

    bool TurboParser::tryFirstWord(ParsedPhrase &phrase, std::ifstream &syntStream,
                                   std::ifstream &morphStream) const {
        std::string morphLine;
        std::string syntLine;
        std::getline(syntStream,syntLine);
        std::getline(morphStream,morphLine);
        generateLine(phrase,morphLine);
        return true;
    }

    ParsedPhrase TurboParser::parseOneBlock(const std::string &text, std::ifstream &syntStream,
                                            std::ifstream &morphStream) const {
        ParsedPhrase result;
        result.text = text;
        int startSyntPos = syntStream.tellg();
        int startMorphPos = morphStream.tellg();
        if(tryMainWord(result,syntStream,morphStream)) return result;
        syntStream.seekg(startSyntPos);
        morphStream.seekg(startMorphPos);
        if(tryFirstSpeechPart(result,syntStream,morphStream)) return result;
        syntStream.seekg(startSyntPos);
        morphStream.seekg(startMorphPos);
        tryFirstWord(result,syntStream,morphStream);
        //Skip to end of block;
        std::string syntLine;
        std::string morphLine;
        while(std::getline(syntStream,syntLine) && std::getline(morphStream,morphLine)) {
            if(isSepLine(syntLine)) break;
        }
        return result;
    }

    void TurboParser::fixStreams(std::ifstream &syntStream, std::ifstream &morphStream) const {
        std::string syntLine;
        std::string morphLine;
        int prevSyntPos = syntStream.tellg();
        int prevMorphPos = morphStream.tellg();
        while(std::getline(syntStream,syntLine) && std::getline(morphStream,morphLine)) {
            if (!isSepLine(syntLine)) {
                syntStream.seekg(prevSyntPos);
                morphStream.seekg(prevMorphPos);
                break;
            }
            prevSyntPos = syntStream.tellg();
            prevMorphPos = morphStream.tellg();
        }
    }

    Gender TurboParser::getGender(const std::string &morphChars) const {
        if(morphChars.find(toString(Gender::MASC)) !=  std::string::npos){
            return Gender::MASC;
        }else if(morphChars.find(toString(Gender::FEMN)) !=  std::string::npos){
            return Gender::FEMN;
        }else if(morphChars.find(toString(Gender::NEUT)) !=  std::string::npos){
            return Gender::NEUT;
        }else if(morphChars.find(toString(Gender::OVERALL)) !=  std::string::npos){
            return Gender::OVERALL;
        }
        return Gender::UNDEF;
    }

    Number TurboParser::getNumber(const std::string &morphChars) const {
        if(morphChars.find(toString(Number::SNG)) !=  std::string::npos){
            return Number::SNG;
        }else if (morphChars.find(toString(Number::PLR)) !=  std::string::npos){
            return Number::PLR;
        }
        return Number::UNDEF;
    }

    Person TurboParser::getPerson(const std::string &morphChars) const {
        if(morphChars.find(toString(Person::FP)) !=  std::string::npos){
            return Person::FP;
        }else if(morphChars.find(toString(Person::SP)) !=  std::string::npos){
            return Person::SP;
        }else if(morphChars.find(toString(Person::TP)) !=  std::string::npos){
            return Person::TP;
        }
        return Person::UNDEF ;
    }
}



