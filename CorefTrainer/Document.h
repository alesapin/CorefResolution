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

    typedef std::vector<synt::ParsedPharse> Chain;
    typedef std::tuple<synt::ParsedPharse,synt::ParsedPharse,synt::ParsedPharse> Triple;
    typedef std::tuple<synt::ParsedPharse,synt::ParsedPharse,synt::ParsedPharse,DetectedCoref> ClassifiedTriple;

    class Document {
        const std::string path;
        const std::string name;
        std::set<synt::ParsedPharse> entites;
        std::vector<Chain> coreferences;
        const synt::TurboParser *prs;

        int getSingleDimenIndex(int i,int j);
        void loadEntities();
        static const int WINDOW_SIZE;
    public:
        Document(const std::string &p, const std::string &n,
                 const synt::TurboParser *pars) : path(p), name(n),prs(pars) { }

        void addCorefChain(const std::vector<synt::ParsedPharse> &coref);

        void loadCorefFromXml(const std::string &path);
        friend std::ostream& operator<<(std::ostream& os,const Document& doc);
        //@Deprecated
        void writeTiplesToFile(std::string file);
        int findEntity(const synt::ParsedPharse& entity) const;
        std::set<synt::ParsedPharse>::iterator getEntityByOrder(int index) const;
        int getEntitiesSIze() const;
        std::vector<ClassifiedTriple> getClassifiedTriples() const;
        std::vector<Triple> getTriples() const;
        bool findCorefence(const synt::ParsedPharse& main, const synt::ParsedPharse& alt) const;


    };
    static std::vector<Triple> generateTriples(const std::set<synt::ParsedPharse>& entites,int windowSize);
    static void writeClassifiedTriples(const std::vector<ClassifiedTriple>& tiples,const std::string& filename);

}

#endif //COREFRESOLUTION_DOCUMENT_H
