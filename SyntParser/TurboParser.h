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
        ParsedPharse invokeTurboParser(const std::string& text) const;
    private:
        bool checkSyntLine(const std::string& line) const;
        void generateLine(ParsedPharse& ph,const std::string& morphLine) const;
        const std::string path;
        static const std::string TURBO_PARSER_OUT;
        static const std::string RUSDEPS_PARSER_OUT;
        static const std::size_t SYNT_REFERED_NUM_COL;
        static const std::size_t SYNT_SP_COL;
        static const std::size_t MORPH_CHARS_COL;
        static const std::size_t MORPH_SP_COL;
        static const std::size_t MORPH_WORDFORM_COL;
        static const std::size_t MORPH_NORMALFORM_COL;
    };
}

#endif //COREFRESOLUTION_TURBOPARSER_H
