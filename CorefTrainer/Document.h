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
#include <set>
#include <fstream>

namespace coref {
    class Document {
        const std::string path;
        const std::string name;
        std::set<synt::ParsedPharse> entites;
        std::vector<std::vector<synt::ParsedPharse>> coreferences;
        const synt::TurboParser *prs;

        int getSingleDimenIndex(int i,int j);
        void loadEntities();
    public:
        Document(const std::string &p, const std::string &n, const synt::TurboParser *pars) : path(p), name(n),prs(pars) { }

        void addCorefChain(const std::vector<synt::ParsedPharse> &coref);

        void loadCorefFromXml(const std::string &path);
        friend std::ostream& operator<<(std::ostream& os,const Document& doc);
        void writeTiplesToFile(std::string file);
        bool findCorefence(const synt::ParsedPharse* main, const synt::ParsedPharse* alt);
    };
}

#endif //COREFRESOLUTION_DOCUMENT_H
