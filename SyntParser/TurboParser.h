//
// Created by alesapin on 23.03.16.
//

#ifndef COREFRESOLUTION_TURBOPARSER_H
#define COREFRESOLUTION_TURBOPARSER_H
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>
#include <limits>
#include "ParsedPhrase.h"
namespace synt {


    class TurboParser {
    public:
        TurboParser(const std::string &programsPath):path(programsPath){};
        std::vector<ParsedPhrase> invokeTurboParser(const  std::vector<std::string>& req) const;
    private:

        ParsedPhrase parseOneBlock(const std::string& text, std::ifstream& syntStream, std::ifstream& morphStream) const;
        bool tryMainWord(ParsedPhrase& phrase, std::ifstream& syntStream, std::ifstream& morphStream) const ;
        bool tryFirstSpeechPart(ParsedPhrase& phrase, std::ifstream& syntStream, std::ifstream& morphStream) const;
        bool tryFirstWord(ParsedPhrase& phrase, std::ifstream& syntStream, std::ifstream& morphStream) const;
        std::string prepareText(const  std::vector<std::string>& req) const;
        std::size_t getSyntPos(const std::string& syntLine) const;
        bool isSepLine(const std::string& line) const;
        void fixStreams(std::ifstream& syntStream, std::ifstream& morphStream) const;
        bool checkNounOrNproLine(const std::string &line) const;
        void generateLine(ParsedPhrase& ph,const std::string& morphLine) const;

        Gender getGender(const std::string& morphChars) const;
        Number getNumber(const std::string& morphChars) const;
        Person getPerson(const std::string& morphChars) const;


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
