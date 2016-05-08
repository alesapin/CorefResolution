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



    enum class DetectedCoref {
        NO,FIRST,SECOND
    };
    std::ostream& operator<<(std::ostream& os,const DetectedCoref& obj);
    typedef std::vector<synt::ParsedPhrase> Chain;
    typedef std::tuple<synt::ParsedPhrase,synt::ParsedPhrase,synt::ParsedPhrase> Triple;
    typedef std::tuple<synt::ParsedPhrase,synt::ParsedPhrase,synt::ParsedPhrase,DetectedCoref> ClassifiedTriple;

    class Document {
        std::string path;
        std::string name;
        std::set<synt::ParsedPhrase> entites;
        std::vector<Chain> coreferences;
        const synt::TurboParser *prs;

        int getSingleDimenIndex(int i,int j);
        void loadEntities();
        void orderChains();

        static const int WINDOW_SIZE;
    public:
        Document(const std::string &p, const std::string &n,
                 const synt::TurboParser *pars) : path(p), name(n),prs(pars) { }
        Document(const std::string &fullPath,
                 const synt::TurboParser *pars) : prs(pars) {
            std::vector<std::string> paths;
            boost::split(paths,fullPath,boost::is_any_of("/"));
            name = paths.back();
            path = paths[paths.size()-2] + "/";
        }
        void addCorefChain(const std::vector<synt::ParsedPhrase> &coref);
        void loadCorefFromXml(const std::string &path);
        void loadEntitesFromXml(const std::string &path);
        friend std::ostream& operator<<(std::ostream& os,const Document& doc);
        //@Deprecated
        void writeTiplesToFile(std::string file);
        int findEntity(const synt::ParsedPhrase& entity) const;
        std::set<synt::ParsedPhrase>::iterator getEntityByOrder(int index) const;
        int getEntitiesSIze() const;
        std::vector<ClassifiedTriple> getClassifiedTriples() const;
        std::set<synt::ParsedPhrase> getEntities() const {return entites;}
        std::vector<Triple> getTriples() const;
        bool findCorefence(const synt::ParsedPhrase& main, const synt::ParsedPhrase& alt) const;
        std::vector<Chain> getCorefs() const { return coreferences;}

    };

    std::vector<Triple> generateTriples(const std::set<synt::ParsedPhrase>& entites,int windowSize);

    void writeClassifiedTriples(const std::vector<ClassifiedTriple>& tiples,std::ostream& os);


}

#endif //COREFRESOLUTION_DOCUMENT_H
