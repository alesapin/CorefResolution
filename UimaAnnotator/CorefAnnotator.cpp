//
// Created by alesapin on 07.05.16.
//

#include "CorefAnnotator.h"
uima::TyErrorId CorefAnnotator::process(uima::CAS & rCas, uima::ResultSpecification const & crResultSpecification) {
    uima::FSIndexRepository & indexRep = rCas.getIndexRepository();
    uima::UnicodeStringRef ulstrDoc = rCas.getDocumentText();
    uima::CAS *entitiesCas = rCas.getView("EntitiesPath");
    uima::CAS *docPathCas = rCas.getView("DocumentPath");
    uima::UnicodeStringRef entPath = entitiesCas->getSofaDataURI();
    uima::UnicodeStringRef docPath = docPathCas->getSofaDataURI();
    std::string ansiEntP, ansiDocP;
    ansiEntP = entPath.asUTF8();
    ansiDocP = docPath.asUTF8();
    coref::Document current(ansiDocP,parser.get());
    current.loadEntitesFromXml(ansiEntP);
    std::vector<coref::Chain> result = classifier.classify(current);
    uima::CAS *corefPairs = rCas.createView("CoreferencePairs");
    for(int i = 0; i < result.size(); ++i){
        synt::ParsedPhrase first = result[i][0];
        synt::ParsedPhrase second = result[i][1];
        uima::AnnotationFS fann = corefPairs->createAnnotation(coreference,first.shift,first.shift + first.text.length());
        uima::AnnotationFS sann = corefPairs->createAnnotation(coreference,second.shift,second.shift + second.text.length());
        fann.setIntValue(refStart,second.shift);
        fann.setIntValue(refEnd,second.shift + second.text.length());
        sann.setIntValue(refStart,first.shift);
        sann.setIntValue(refEnd,first.shift + first.text.length());
        indexRep.addFS(fann);
        indexRep.addFS(sann);
    }
    return (uima::TyErrorId)UIMA_ERR_NONE;
}

uima::TyErrorId CorefAnnotator::initialize(uima::AnnotatorContext &rclAnnotatorContext)  {
    context = &rclAnnotatorContext;
    icu::UnicodeString turboParserPath, neuronPath;
    context->extractValue("TPPath",turboParserPath);
    context->extractValue("NeuronPath",neuronPath);
    std::string ansiTP, ansiNP;
    turboParserPath.toUTF8String(ansiTP);
    neuronPath.toUTF8String(ansiNP);
    coref::Classifier cl;
    cl.load(ansiNP);
    classifier.setCl(cl);
    synt::TurboParser *p = new synt::TurboParser(ansiTP);
    parser = std::shared_ptr<synt::TurboParser>(p);
    return (uima::TyErrorId)UIMA_ERR_NONE;
}

uima::TyErrorId CorefAnnotator::typeSystemInit(uima::TypeSystem const & crTypeSystem) {
    coreference  = crTypeSystem.getType("org.Coreference");
    refStart = coreference.getFeatureByBaseName("RefStart");
    refEnd = coreference.getFeatureByBaseName("RefEnd");
    return (uima::TyErrorId)UIMA_ERR_NONE;
}