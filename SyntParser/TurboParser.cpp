//
// Created by alesapin on 23.03.16.
//

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

    ParsedPharse TurboParser::invokeTurboParser(const std::string& text) const{
        std::string program = path + "/launch.sh \"" + text +"\" " + TURBO_PARSER_OUT;
        std::system(program.c_str());
        std::ifstream syntInform(TURBO_PARSER_OUT,std::ifstream::in);
        std::ifstream morfInform(RUSDEPS_PARSER_OUT,std::ifstream::in);
        std::string syntLine, morfLine;
        ParsedPharse result;
        bool found = false;
        while(std::getline(syntInform,syntLine) && std::getline(morfInform,morfLine)) {
            if (!syntLine.empty() && checkSyntLine(syntLine)) {
                found = true;
                generateLine(result,morfLine);
                result.text = text;
                break;
            }
        }
        if(!found) {
            syntInform.clear();
            morfInform.clear();
            syntInform.seekg(0, std::ios::beg);
            morfInform.seekg(0, std::ios_base::beg);
            while (std::getline(syntInform, syntLine) && std::getline(morfInform, morfLine)) {
                if (!syntLine.empty()) {
                    std::vector<std::string> chars;
                    boost::split(chars, syntLine, boost::is_any_of("\t "));
                    if(chars[SYNT_SP_COL] == "NOUN" || chars[SYNT_SP_COL] == "NPRO"){
                        generateLine(result,morfLine);
                        result.text = text;
                        found = true;
                        break;
                    }
                }
            }
        }
        if(! found){
            syntInform.clear();
            morfInform.clear();
            syntInform.seekg(0, std::ios::beg);
            morfInform.seekg(0, std::ios_base::beg);
            std::getline(syntInform, syntLine);
            std::getline(morfInform, morfLine);
            generateLine(result,morfLine);
            result.text = text;
        }
//        std::remove(TURBO_PARSER_OUT.c_str());
//        std::remove(RUSDEPS_PARSER_OUT.c_str());
        return result;
    }

    bool TurboParser::checkSyntLine(const std::string &line) const {
        std::vector<std::string> chars;
        boost::split(chars,line,boost::is_any_of("\t "));
        if((chars[SYNT_SP_COL] == "NOUN" || chars[SYNT_SP_COL] == "NPRO")
           && chars[SYNT_REFERED_NUM_COL] == "0") return true;
        return false;
    }

    void TurboParser::generateLine(ParsedPharse &ph, const std::string &morphLine) const{
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
        boost::split(ph.morphChars,morphChars,boost::is_any_of("|"));
    }


}

std::ostream &::synt::operator<<(std::ostream &os, const synt::ParsedPharse &obj) {
    os <<"[Text: " << obj.text<<";";
    os <<" Main Word: "<<obj.mainWord<<";";
    os <<" Main word NF: "<<obj.mainWordNormalForm<<";";
    os <<" SP: ";
    if(obj.sp == synt::SpeechPart::NOUN) {
        os << "NOUN;";
    }else{
        os <<"NPRO;";
    }
    os << " Chars: ";
    for(const std::string& chr : obj.morphChars){
        os << chr <<"|";
    }
    os<<";";
    os<<" Shift:" << obj.shift<<";]";
    return os;
}

