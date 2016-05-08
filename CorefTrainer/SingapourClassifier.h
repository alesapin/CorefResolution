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
        typedef std::vector <synt::ParsedPhrase> Chain;
    private:
        Classifier cl;
        Chain resolveOne(const synt::ParsedPhrase& m, const std::vector<ClassifiedTriple>& triples) const;
    public:
        bool findCorefence(const std::vector<Chain>& neuronCorefernce, const synt::ParsedPhrase& main, const synt::ParsedPhrase& alt) const;
        SingapourClassifier(Classifier c):cl(c){}
        SingapourClassifier() = default;
        std::vector<Chain> classify(const coref::Document &d) const;
        std::tuple<double,double> validate(const std::vector<Document> & docs) const;
        const Classifier &getCl() const {
            return cl;
        }

        void setCl(const Classifier &cl) {
            SingapourClassifier::cl = cl;
        }



    };
    double countPrecision(const Document& d, const std::vector<Chain>& predicted);

    double countRecall(const Document& d, const std::vector<Chain>& predicted);

}

#endif //COREFRESOLUTION_SINGAPOURCLASSIFIER_H
