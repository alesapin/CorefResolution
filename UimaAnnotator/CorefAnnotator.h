//
// Created by alesapin on 07.05.16.
//

#ifndef COREFRESOLUTION_COREFANNOTATOR_H
#define COREFRESOLUTION_COREFANNOTATOR_H
#include "uima/api.hpp"
#include "../CorefTrainer/SingapourClassifier.h"
#include "../CorefTrainer/Classifier.h"
#include "../CorefTrainer/Document.h"
#include "../SyntParser/TurboParser.h"
#include <string>
#include <memory>
using namespace uima;

class CorefAnnotator : public uima::Annotator {
private:

    uima::AnnotatorContext *context;
    uima::Type coreference;
    uima::Feature refStart;
    uima::Feature refEnd;
    coref::SingapourClassifier classifier;
    std::shared_ptr<synt::TurboParser> parser;
public:

    CorefAnnotator() = default;

    uima::TyErrorId initialize(uima::AnnotatorContext &rclAnnotatorContext) override;

    uima::TyErrorId typeSystemInit(uima::TypeSystem const & crTypeSystem) override;

    uima::TyErrorId destroy() override {
        return (uima::TyErrorId)UIMA_ERR_NONE;
    }
    uima::TyErrorId process(uima::CAS & rCas, uima::ResultSpecification const & crResultSpecification) override;


};

MAKE_AE(CorefAnnotator);

#endif //COREFRESOLUTION_COREFANNOTATOR_H
