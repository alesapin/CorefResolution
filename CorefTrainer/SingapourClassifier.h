//bool SingapourClassifier::findCorefence(const std::vector<Chain>& neuronCorefernce, const synt::ParsedPharse& main, const synt::ParsedPharse& alt) const;
// Created by kir on 05.04.16.
//
#include <vector>
#include "../SyntParser/ParsedPhrase.h"
#include "Document.h"
#include "Classifier.h"
#ifndef COREFRESOLUTION_SINGAPOURCLASSIFIER_H
#define COREFRESOLUTION_SINGAPOURCLASSIFIER_H

namespace coref {
    class SingapourClassifier {
        typedef std::vector <synt::ParsedPharse> Chain;
    public:
        bool findCorefence(const std::vector<Chain>& neuronCorefernce, const synt::ParsedPharse& main, const synt::ParsedPharse& alt) const;
        coref::Classifier* cl;
        SingapourClassifier(int nh, int maxEps, double rE = 0.0001f, int report = 1000){
            cl = new Classifier(nh,maxEps,rE,report);
        }
        std::vector<Chain> EvaluateCorefFromneuralNetwork(coref::Document &d);
    };
}

#endif //COREFRESOLUTION_SINGAPOURCLASSIFIER_H
