//
// Created by alesapin on 31.03.16.
//

#include "Document.h"
namespace coref {

    const int Document::WINDOW_SIZE = 200;


    void Document::loadCorefFromXml(const std::string &xmlPath) {
        using boost::property_tree::ptree;
        ptree pt;
        boost::property_tree::read_xml(xmlPath,pt);
        ptree documents = pt.get_child("documents");
        std::string docName = path + name;
        std::cerr<<"DOCNAME:"<<docName<<"\n";
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
        //Sort chains
        orderChains();
        //Remove duplicates
        coreferences.erase(std::unique(coreferences.begin(), coreferences.end()), coreferences.end());
        //Load entities
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

    bool Document::findCorefence(const synt::ParsedPharse& main, const synt::ParsedPharse& alt) const{
        for (int i =0 ; i<this->coreferences.size(); i++) {
            if (coreferences[i][0] == main &&coreferences[i][1] == alt) return true;
        }
        return false;
    }
    int Document::findEntity(const synt::ParsedPharse& entity) const{
        int i = 0;
        for (auto curEnt = entites.begin(); curEnt != entites.end(); ++curEnt) {
            if (entity == *curEnt) i;
            i++;
        }
        return -1;
    }
    std::set<synt::ParsedPharse>::iterator Document::getEntityByOrder(int index) const{
        int i = 0;
        for (auto curEnt = entites.begin(); curEnt != entites.end(); ++curEnt) {
            if (i == index) &curEnt;
            i++;
        }
        return entites.end();
    }
    int Document::getEntitiesSIze() const{
        return entites.size();
    }

    std::vector<ClassifiedTriple> Document::getClassifiedTriples() const{
        std::vector<ClassifiedTriple> result;
        std::vector<Triple> triples = generateTriples(entites,WINDOW_SIZE);
        for(const Triple& triple:triples) {
            ClassifiedTriple trpl;
            if (findCorefence(std::get<0>(triple), std::get<1>(triple))) {
                trpl = std::make_tuple(std::get<0>(triple), std::get<1>(triple),
                                       std::get<2>(triple), DetectedCoref::FIRST);
            } else if (findCorefence(std::get<0>(triple), std::get<2>(triple))) {
                trpl = std::make_tuple(std::get<0>(triple), std::get<1>(triple),
                                       std::get<2>(triple), DetectedCoref::SECOND);
            } else {
                trpl = std::make_tuple(std::get<0>(triple), std::get<1>(triple),
                                       std::get<2>(triple), DetectedCoref::NO);
            }
            result.push_back(trpl);
        }
        return result;
    }

    std::vector<Triple> Document::getTriples() const {
        return generateTriples(entites,WINDOW_SIZE);
    }

    void Document::orderChains() {
        std::sort(coreferences.begin(),coreferences.end(),
            [](const Chain& a, const Chain& b){
                if(a.size() == 0) return false;
                if(b.size() == 0) return true;
                return a[0] < b[0];
            }
        );
    }


    std::vector<Triple> generateTriples(const std::set<synt::ParsedPharse> &entites, int windowSize) {
        std::vector<coref::Triple> result;
        for (auto curEnt = entites.begin(); curEnt != entites.end(); ++curEnt) {
            bool oneTripleCreated = false;
            for (auto firstCand = std::next(curEnt, 1); firstCand != entites.end(); firstCand++) {
                if (firstCand->shift - curEnt->shift > windowSize && oneTripleCreated) break;
                bool secondAltBreak = false;
                for (auto secondCand = std::next(firstCand, 1); secondCand != entites.end(); secondCand++) {
                    if (secondCand->shift - curEnt->shift > windowSize && oneTripleCreated) {
                        secondAltBreak = true;
                        break;
                    }
                    auto triple = std::make_tuple(*curEnt,*firstCand,*secondCand);
                    result.push_back(triple);
                    oneTripleCreated = true;
                }
                if (secondAltBreak) break;
            }
        }
        return result;
    }

    void writeClassifiedTriples(const std::vector<ClassifiedTriple>& triples,std::ostream& os){
        for(const ClassifiedTriple& trpl:triples){

            synt::ParsedPharse f = std::get<0>(trpl);
            synt::ParsedPharse s = std::get<1>(trpl);
            synt::ParsedPharse t = std::get<2>(trpl);
            DetectedCoref type = std::get<3>(trpl);
            os<<int(f.sp)<<" "<<int(f.gen)<<" "<<int(f.num) <<" "<<int(f.pers) << " ";
            os<<int(s.sp)<<" "<<int(s.gen)<<" "<<int(s.num) <<" "<<int(s.pers)<<" "<<s.shift - f.shift << " ";
            os<<int(t.sp)<<" "<<int(t.gen)<<" "<<int(t.num) <<" "<<int(t.pers)<<" "<<t.shift - f.shift<<" \n";

            if (type == DetectedCoref::NO) {
                os << "0 0\n";
            } else if (type == DetectedCoref::FIRST) {
                os << "1 0\n";
            } else {
                os << "0 1\n";
            }
        }
    }

    std::ostream &operator<<(std::ostream &os, const DetectedCoref &obj) {
        if(obj == DetectedCoref::NO){
            os << "00";
        }else if(obj == DetectedCoref::FIRST){
            os << "10";
        }else {
            os <<"01";
        }
        return os;
    }


}



