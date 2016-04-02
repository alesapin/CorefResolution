//
// Created by alesapin on 31.03.16.
//

#ifndef COREFRESOLUTION_DOCUMENT_H
#define COREFRESOLUTION_DOCUMENT_H
#include <string>
#include <map>
#include "../SyntParser/TurboParser.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
namespace coref {
    class Document {
        const std::string path;
        const std::string name;
        std::vector<std::vector<synt::ParsedPharse>> coreferences;
        const synt::TurboParser *prs;
    public:
        Document(const std::string &p, const std::string &n, const synt::TurboParser *pars) : path(p), name(n),prs(pars) { }

        void addCorefChain(const std::vector<synt::ParsedPharse> &coref);

        void loadCorefFromXml(const std::string &path);
    };
}

#endif //COREFRESOLUTION_DOCUMENT_H
