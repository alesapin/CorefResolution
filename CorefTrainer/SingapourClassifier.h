//bool SingapourClassifier::findCorefence(const std::vector<Chain>& neuronCorefernce, const synt::ParsedPharse& main, const synt::ParsedPharse& alt) const;
// Created by kir on 05.04.16.
//
#include <vector>
#include "../SyntParser/ParsedPhrase.h"
#include "Document.h"
#include "Classifier.h"
#include <map>
#ifndef COREFRESOLUTION_SINGAPOURCLASSIFIER_H
#define COREFRESOLUTION_SINGAPOURCLASSIFIER_H

namespace coref {
    class SingapourClassifier {
        typedef std::vector <synt::ParsedPharse> Chain;
        Classifier cl;
        Chain resolveOne(const synt::ParsedPharse& m, const std::vector<ClassifiedTriple>& triples) const;
    public:
        bool findCorefence(const std::vector<Chain>& neuronCorefernce, const synt::ParsedPharse& main, const synt::ParsedPharse& alt) const;
        SingapourClassifier(Classifier c):cl(c){}
        std::vector<Chain> resolve(const coref::Document &d) const;
        std::tuple<double,double> validate(const std::vector<Document> & docs) const;



    };
    double countPrecision(const Document& d, const std::vector<Chain>& predicted);

    double countRecall(const Document& d, const std::vector<Chain>& predicted);

}

#endif //COREFRESOLUTION_SINGAPOURCLASSIFIER_H
