//
// Created by alesapin on 23.03.16.
//

#include "TurboParser.h"
namespace synt{
    const std::string TurboParser::TURBO_PARSER_INP = "testSequence.xml";
    const std::string TurboParser::TURBO_PARSER_OUT = "turbo.txt";
    const std::string TurboParser::MYSTEM_OUT = "mstemOutp.txt";
    const std::string TurboParser::MYSTEM_INP = "mstemInp.txt";
    const std::string TurboParser::BASIC_XML_PREFIX="sentences.p.se.w";
//
//    void TurboParser::prepareTurboparser(const std::string& text){
//        using boost::property_tree::ptree;
//        ptree pt;
//        std::vector<std::string> splited;
//        boost::algorithm::split(splited,text,boost::is_any_of("\t "));
//        boost::property_tree::xml_writer_settings<char> settings(' ', 4);
//        for(const std::string& word: splited){
//            ptree w;
//            w.put_value(word);
//            ptree rel;
//            rel.put("<xmlattr>.id_head", "\"\"");
//            rel.put("<xmlattr>.type", "\"\"");
//            w.add_child("rel",rel);
//            pt.add_child(BASIC_XML_PREFIX,w);
//        }
//        write_xml(TURBO_PARSER_INP,pt,std::locale(),settings);
//    }

    std::vector<ParsedWord> TurboParser::invokeTurboParser(const std::string& text) const{
        std::string program = path + "/../launch.sh \"" + text +"\" " + TURBO_PARSER_OUT;
        std::system(program.c_str());
        std::ifstream ifs(TURBO_PARSER_OUT,std::ifstream::in);
        std::string line;
        std::vector<ParsedWord> result;
        while(std::getline(ifs,line)) {
            if (!line.empty()) {
                result.push_back(parseTurboLine(line));
            }
        }
        ifs.close();
        std::remove(TURBO_PARSER_OUT.c_str());
        return result;
    }
    ParsedWord TurboParser::parseTurboLine(const std::string &line) const {
        std::vector<std::string> splited;
        boost::algorithm::split(splited,line,boost::is_any_of("\t "));
        ParsedWord pw;
        pw.form = splited[1];
        pw.normalForm = splited[2];
        pw.sp = getSpeechPartFromCorpString(splited[3]);
        pw.referedSp = getSpeechPartFromCorpString(splited[4]);
        pw.referNum = std::stoi(splited[6]);
        return pw;
    }
    void TurboParser::invokeMorph(const std::string& str,std::vector<ParsedWord>& data) const {
        std::string program = "python3 " + path + "/../morph.py \"" +  str + "\" " + MYSTEM_OUT;
        std::system(program.c_str());
        std::ifstream ifs(MYSTEM_OUT);
        std::string line;
        int wordCounter = 0;
        while(std::getline(ifs,line)){
            if(line == "$$$"){
                bool found = false;
                while(std::getline(ifs,line)){
                    std::vector<std::string> splited;
                    boost::algorithm::split(splited,line,boost::is_any_of("\t "));
                    if(getSpeechPartFromCorpString(splited[0]) == data[wordCounter].sp){
                        data[wordCounter].morphChars = std::vector<std::string>(splited.begin() + 1,splited.end());
                        wordCounter++;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    wordCounter++;
                }
            }
        }
        ifs.close();
        std::remove(MYSTEM_OUT.c_str());
    }

    //    UNDEF = 0, NOUN = 1, VERB = 2, ADJF = 3, ADJS = 4, COMP = 5, INFN = 6, PRTF = 7, PRTS = 8, GRND = 9,
//            NUMR = 10, ADVB = 11, NPRO = 12, PRED = 13, PREP = 14, CONJ = 15, PRCL = 16, INTJ = 17,
    SpeechPart TurboParser::getSpeechPartFromCorpString(const std::string& spName) const {
        if(spName == "NOUN"){
            return SpeechPart::NOUN;
        }else if(spName == "VERB"){
            return SpeechPart::VERB;
        }else if(spName == "ADJF"){
            return SpeechPart::ADJF;
        }else if(spName == "ADJS"){
            return SpeechPart::ADJS;
        }else if(spName == "COMP"){
            return SpeechPart::COMP;
        }else if(spName == "INFN"){
            return SpeechPart::INFN;
        }else if(spName == "PRTF"){
            return SpeechPart::PRTF;
        }else if(spName == "PRTS"){
            return SpeechPart::PRTS;
        }else if(spName == "GRND"){
            return SpeechPart::GRND;
        }else if(spName == "NUMR"){
            return SpeechPart::NUMR;
        }else if(spName == "ADVB"){
            return SpeechPart::ADVB;
        }else if(spName == "NPRO"){
            return SpeechPart::NPRO;
        }else if(spName == "PRED"){
            return SpeechPart::PRED;
        }else if(spName == "PREP"){
            return SpeechPart::PREP;
        }else if(spName == "CONJ"){
            return SpeechPart::CONJ;
        }else if(spName == "PRCL"){
            return SpeechPart::PRCL;
        }else if(spName == "INTJ"){
            return SpeechPart::INTJ;
        }else{
            return SpeechPart::UNDEF;
        }
    }


    std::vector<ParsedWord> TurboParser::parse(const std::string& text) const {
        std::vector<ParsedWord> res = invokeTurboParser(text);
        invokeMorph(text,res);
        return res;
    }


}