//
// Created by kir on 05.04.16.
//

#include "SingapourClassifier.h"
namespace coref {
    bool SingapourClassifier::findCorefence(const std::vector<Chain>& neuronCorefernce, const synt::ParsedPharse& main, const synt::ParsedPharse& alt) const{
        for (int i =0 ; i<neuronCorefernce.size(); i++) {
            if (neuronCorefernce[i][0] == main &&neuronCorefernce[i][1] == alt) return true;
        }
        return false;
    }
    std::vector<Chain> SingapourClassifier::EvaluateCorefFromneuralNetwork(coref::Document& d) {
        std::vector<Chain> neuronCorefernce;
        std::vector<coref::ClassifiedTriple> neuronResult = cl->run(d.getTriples());
        int documentCountEntities = d.getEntitiesSIze();
//        std::vector<int>* scoreForEntityPtr = new std::vector<int>();
//        std::vector<int>& scoreForEntity = *scoreForEntityPtr;
        std::vector<int> scoreForEntity(documentCountEntities,0);
        if (neuronResult.begin() != neuronResult.end()) {
        synt::ParsedPharse curMain = std::get<0>(*neuronResult.begin());
        synt::ParsedPharse curFirst = std::get<1>(*neuronResult.begin());
        //for (coref::ClassifiedTriple& triple: neuronResult)
        for (auto triple = neuronResult.begin(); triple != neuronResult.end(); ++triple)
        {
            synt::ParsedPharse f = std::get<0>(*triple);
            synt::ParsedPharse s = std::get<1>(*triple);
            if (f != curMain || s!=curFirst){
                std::vector<int>::iterator maxIt = max_element(scoreForEntity.begin(),scoreForEntity.end());
                int maxIndex = std::distance(scoreForEntity.begin(),maxIt);
                if (maxIndex < documentCountEntities && scoreForEntity[maxIndex]>0) {
                    synt::ParsedPharse candidate = *d.getEntityByOrder(maxIndex);
                    if (!findCorefence(neuronCorefernce, curMain, candidate)) {
                        std::vector<synt::ParsedPharse> currentChain;
                        currentChain.push_back(curMain);
                        currentChain.push_back(candidate);
                        neuronCorefernce.push_back(currentChain);
                        for (int i = 0; i < documentCountEntities; i++) scoreForEntity[i] = 0;
                    }
                }
                curMain = f;
                curFirst = s;
            }
            synt::ParsedPharse t = std::get<2>(*triple);
            coref::DetectedCoref alternative = std::get<3>(*triple);
            int firstAlt = d.findEntity(s);
            int secondAlt = d.findEntity(t);
            if (alternative == coref::DetectedCoref::FIRST && firstAlt != -1 && secondAlt!=-1)
            {
                scoreForEntity.at(firstAlt)++;
                scoreForEntity.at(secondAlt)--;
                //scoreForEntity[firstAlt]++;
                //scoreForEntity[secondAlt]--;
            }
            else if (alternative == coref::DetectedCoref::SECOND && firstAlt != -1 && secondAlt!=-1)
            {
                scoreForEntity.at(firstAlt)--;
                scoreForEntity.at(secondAlt)++;
                //scoreForEntity[firstAlt]--;
                //scoreForEntity[secondAlt]++;
            }
            else if (alternative == coref::DetectedCoref::NO && firstAlt != -1 && secondAlt!=-1)
            {
                scoreForEntity.at(firstAlt)--;
                scoreForEntity.at(secondAlt)--;
                //scoreForEntity[firstAlt]--;
                //scoreForEntity[secondAlt]--;
            }
        }
        }
        //scoreForEntity.clear();
        return neuronCorefernce;
    }
}