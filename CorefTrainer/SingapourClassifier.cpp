//
// Created by kir on 05.04.16.
//

#include "SingapourClassifier.h"
namespace coref {

    double countPrecision(const Document &d, const std::vector<Chain> &predicted) {
        long foundRight = 0;
        std::vector<Chain> realChains = d.getCorefs();
        if(realChains.size() == 0) return 1;
        if(predicted.size() == 0) return 0;
        for(const Chain& real : realChains){
            if(real.size() > 2) continue;
            for(const Chain& pred : predicted){
                if((real[0] == pred[0] && real[1] == pred[1])
                   || (real[1] == pred[0] && real[0] == pred[1]) ){
                    foundRight++;
                    break;
                }
            }
        }
        return (double)(foundRight)/predicted.size();
    }

    double countRecall(const Document &d, const std::vector<Chain> &predicted) {
        long foundRight = 0;
        std::vector<Chain> realChains = d.getCorefs();
        if(realChains.size() == 0) return 1;
        if(predicted.size() == 0) return 0;
        for(const Chain& real : realChains){
            if(real.size() > 2) continue;
            for(const Chain& pred : predicted){
                if((real[0] == pred[0] && real[1] == pred[1])
                   || (real[1] == pred[0] && real[0] == pred[1]) ){
                    foundRight++;
                    break;
                }
            }
        }
        return (double)(foundRight)/realChains.size();
    }

    std::tuple<double, double> SingapourClassifier::validate(const std::vector<Document> &docs) const{
        double sumRec = 0;
        double sumPrec = 0;
        int counter = 0;
        for(const Document &d : docs){
            std::vector<coref::Chain> result = classify(d);
            double prec = countPrecision(d,result);
            double rec = countRecall(d,result);
            sumRec += rec;
            sumPrec +=prec;
            counter++;
        }
        return std::make_tuple(sumPrec/counter, sumRec/counter);
    }


    std::vector<Chain> SingapourClassifier::classify(const coref::Document &d) const{
        std::vector<coref::SingapourClassifier::Chain> result;
        std::set<synt::ParsedPhrase> phrases = d.getEntities();
        std::vector<Triple> triples = d.getTriples();
        std::vector<coref::ClassifiedTriple> neuronResult = cl.run(triples);
        for(const synt::ParsedPhrase &m : phrases){
            Chain c = resolveOne(m,neuronResult);
            if(c.size() > 1){
                result.push_back(c);
            }
        }
        return result;
    }

    Chain SingapourClassifier::resolveOne(const synt::ParsedPhrase &m, const std::vector<ClassifiedTriple> &triples) const{
        Chain result;
        result.push_back(m);
        std::map<synt::ParsedPhrase,int> candidates;
        for(const ClassifiedTriple& t: triples){
            if(std::get<0>(t) == m) {
                candidates[std::get<1>(t)] = 0;
                candidates[std::get<2>(t)] = 0;
            }
        }
        for(const ClassifiedTriple& t: triples){
            if(std::get<0>(t) == m){
                DetectedCoref current = std::get<3>(t);
                if(current == DetectedCoref::NO){
                    candidates[std::get<1>(t)]--;
                    candidates[std::get<2>(t)]--;
                }else if (current == DetectedCoref::FIRST){
                    candidates[std::get<1>(t)]++;
                    candidates[std::get<2>(t)]--;
                } else {
                    candidates[std::get<1>(t)]--;
                    candidates[std::get<2>(t)]++;
                }
            }
        }
        typedef std::pair<synt::ParsedPhrase,int> PairType;
        if(candidates.size() >= 1) {
            auto maxItr = std::max_element(candidates.begin(), candidates.end(),
                                           [](const PairType &p1, const PairType &p2) {
                                               return p1.second < p2.second;
                                           });
            if(maxItr->second > 0) {
                result.push_back(maxItr->first);
            }
        }
        return result;
    }


}