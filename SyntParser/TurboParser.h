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
namespace synt {
    /**
     * All possible speech parts.
     */
    enum class SpeechPart{
        UNDEF = 0, NOUN = 1, VERB = 2, ADJF = 3, ADJS = 4, COMP = 5, INFN = 6, PRTF = 7, PRTS = 8, GRND = 9,
        NUMR = 10, ADVB = 11, NPRO = 12, PRED = 13, PREP = 14, CONJ = 15, PRCL = 16, INTJ = 17,
    };
    /**
     * Parser word output.
     */
    struct ParsedWord {
        std::string form;
        std::string normalForm;
        SpeechPart sp;
        SpeechPart referedSp;
        std::size_t referNum;
        std::vector<std::string> morphChars;
    };

    class TurboParser {
    public:
        TurboParser(const std::string &programsPath):path(programsPath){};
        std::vector<ParsedWord> parse(const std::string& text) const;
        std::vector<ParsedWord> invokeTurboParser(const std::string& text) const;

    private:
        const std::string path;
        static const std::string TURBO_PARSER_INP;
        static const std::string TURBO_PARSER_OUT;
        static const std::string MYSTEM_OUT;
        static const std::string MYSTEM_INP;
        static const std::string BASIC_XML_PREFIX;
        SpeechPart getSpeechPartFromCorpString(const std::string& spName) const;
        ParsedWord parseTurboLine(const std::string &line) const;
//        void prepareTurboparser(const std::string& text);
        void invokeMorph(const std::string& str, std::vector<ParsedWord>& data) const;

    };
}

#endif //COREFRESOLUTION_TURBOPARSER_H
