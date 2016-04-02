//
// Created by alesapin on 23.03.16.
//

#ifndef COREFRESOLUTION_TURBOPARSER_H
#define COREFRESOLUTION_TURBOPARSER_H
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include<boost/algorithm/string/split.hpp>
#include<boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>
#include <limits>
namespace synt {
    /**
     * All interesting speech parts.
     */
    enum class SpeechPart{
        UNDEF = 0, NOUN = 1, NPRO = 2
    };
    /**
     * Parser word output.
     */
    struct ParsedPharse {
        std::string text;
        std::string mainWord;
        std::string mainWordNormalForm;
        SpeechPart sp;
        std::size_t shift;
        std::string groupType;
        std::vector<std::string> morphChars;
    };
    std::ostream& operator<<(std::ostream& os,const ParsedPharse& obj);

    class TurboParser {
    public:
        TurboParser(const std::string &programsPath):path(programsPath){};
        std::vector<ParsedPharse> invokeTurboParser(const  std::vector<std::string>& req) const;
    private:

        ParsedPharse parseOneBlock(const std::string& text, std::ifstream& syntStream, std::ifstream& morphStream) const;
        bool tryMainWord(ParsedPharse& phrase, std::ifstream& syntStream, std::ifstream& morphStream) const ;
        bool tryFirstSpeechPart(ParsedPharse& phrase, std::ifstream& syntStream, std::ifstream& morphStream) const;
        bool tryFirstWord(ParsedPharse& phrase, std::ifstream& syntStream, std::ifstream& morphStream) const;
        std::string prepareText(const  std::vector<std::string>& req) const;
        std::size_t getSyntPos(const std::string& syntLine) const;
        bool isSepLine(const std::string& line) const;
        void fixStreams(std::ifstream& syntStream, std::ifstream& morphStream) const;
        bool checkNounOrNproLine(const std::string &line) const;
        void generateLine(ParsedPharse& ph,const std::string& morphLine) const;
        const std::string path;
        static const std::string TURBO_PARSER_OUT;
        static const std::string RUSDEPS_PARSER_OUT;
        static const std::string SEP_START;
        static const std::string SEP_END;
        static const std::size_t SYNT_REFERED_NUM_COL;
        static const std::size_t SYNT_SP_COL;
        static const std::size_t SYNT_WORDFORM_COL;
        static const std::size_t MORPH_CHARS_COL;
        static const std::size_t MORPH_SP_COL;
        static const std::size_t MORPH_WORDFORM_COL;
        static const std::size_t MORPH_NORMALFORM_COL;

    };
}

#endif //COREFRESOLUTION_TURBOPARSER_H
