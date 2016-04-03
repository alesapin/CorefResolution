//
// Created by alesapin on 31.03.16.
//

#include "Document.h"
namespace coref {
    void Document::loadCorefFromXml(const std::string &xmlPath) {
        using boost::property_tree::ptree;
        ptree pt;
        boost::property_tree::read_xml(xmlPath,pt);
        ptree documents = pt.get_child("documents");
        std::string docName = path + name;
        ptree searchDoc;
        for(const auto & document : documents){
            if(document.second.get<std::string>("<xmlattr>.file") == docName){
                searchDoc = document.second;
                break;
            }
        }
        for(const auto & chain : searchDoc) {
            if (chain.first == "chain") {
                std::vector<synt::ParsedPharse> currentChain;
                for (const auto &item : chain.second) {
                    int sh = item.second.get<int>("<xmlattr>.sh");
                    int len = item.second.get<int>("<xmlattr>.ln");
                    std::string type = item.second.get<std::string>("<xmlattr>.str");
                    std::string text = item.second.get<std::string>("cont");
                    synt::ParsedPharse ph;
                    ph.text = text;
                    ph.shift = sh;
                    ph.groupType = type;
                    currentChain.push_back(ph);
                }
                coreferences.push_back(currentChain);
            }
        }
        std::vector<std::string> parserRequest;
        for(int i = 0;i<coreferences.size();++i) {
            for(int j = 0;j<coreferences[i].size();++j){
                parserRequest.push_back(coreferences[i][j].text);
            }
        }
        std::vector<synt::ParsedPharse> result = prs->invokeTurboParser(parserRequest);
        for(int i = 0;i<coreferences.size();++i) {
            for(int j = 0;j<coreferences[i].size();++j){
                int ind = getSingleDimenIndex(i,j);
                coreferences[i][j].mainWord = result[ind].mainWord;
                coreferences[i][j].mainWordNormalForm = result[ind].mainWordNormalForm;
                coreferences[i][j].gen = result[ind].gen;
                coreferences[i][j].num = result[ind].num;
                coreferences[i][j].pers = result[ind].pers;
                coreferences[i][j].sp = result[ind].sp;
            }
        }
        loadEntities();
    }

    int Document::getSingleDimenIndex(int i, int j) {
        return i*coreferences[i].size() + j;
    }
    std::ostream & operator<<(std::ostream &os, const Document &doc) {
        for(auto ent = doc.entites.begin(); ent != doc.entites.end(); ++ent){
            os<<*ent<<"\n";
        }
        for(int i = 0;i<doc.coreferences.size();++i){
            std::cerr<<"Chain: \n";
            for(int j = 0;j<doc.coreferences[i].size();++j){
                os<<'\t'<<doc.coreferences[i][j]<<"\n";
            }
        }
        return os;
    }

    void Document::loadEntities() {
        for(int i = 0;i<coreferences.size();++i) {
            for (int j = 0; j < coreferences[i].size(); ++j) {
                entites.insert(coreferences[i][j]);
            }
        }
    }


}


