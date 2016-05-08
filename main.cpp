#include <iostream>
#include "SyntParser/TurboParser.h"
#include "CorefTrainer/Document.h"
#include "CorefTrainer/Classifier.h"
#include "CorefTrainer/Util.h"
#include "CorefTrainer/SingapourClassifier.h"
#include "uima/api.hpp"
#include <iomanip>

using namespace std;



int main() {
    (void) uima::ResourceManager::createInstance("SOFA_EXAMPLE_APPLICATION");

    uima::TyErrorId utErrorId;          // Variable to store return codes
    uima::ErrorInfo errorInfo;

    string descriptor = "CorefAnnotator.xml";

    uima::AnalysisEngine * pEngine = uima::Framework::createAnalysisEngine(descriptor.c_str(), errorInfo);
    std::cout << errorInfo << "\n";
    uima::CAS *aCas = pEngine->newCAS();

    uima::CAS *docPathCas = aCas->createView("DocumentPath");
    docPathCas->setSofaDataURI("example.txt","DocumentPath");
    uima::CAS *entPathCas = aCas->createView("EntitiesPath");
    entPathCas->setSofaDataURI("exmaple.xml","EntitiesPath");

    pEngine->process(*aCas);
    uima::ANIndex anIdx = aCas->getAnnotationIndex(aCas->getTypeSystem().getType("org.Coreference"));
    uima::ANIterator itr = anIdx.iterator();
    uima::Type corefType  = aCas->getTypeSystem().getType("org.Coreference");
    uima::Feature start = corefType.getFeatureByBaseName("RefStart");
    uima::Feature end = corefType.getFeatureByBaseName("RefEnd");
    itr.moveToFirst();
    while (itr.isValid()) {
        int st = itr.get().getIntValue(start);
        int en = itr.get().getIntValue(end);
        std::cout <<"Referece from: ["  << itr.get().getBeginPosition() <<", " << itr.get().getEndPosition() << "]\n";
        std::cout << "\t Reference to: [" << st << ", " << en <<"]\n";
        itr.moveToNext();
    }
    return 0;
}