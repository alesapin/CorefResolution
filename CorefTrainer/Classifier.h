//
// Created by alesapin on 04.04.16.
//

#ifndef COREFRESOLUTION_CLASSIFIER_H
#define COREFRESOLUTION_CLASSIFIER_H
#include "fann.h"
#include <string>
#include <vector>
#include "../SyntParser/ParsedPhrase.h"
#include "Document.h"
#include "floatfann.h"

#include <set>



namespace coref {



    class Classifier {
        int numHidden; // Число вутреннних слоев нейронной сети (NUM_INP+1)
        int maxEpochs; // Число эпох обучения
        double reqError;
        int printEpochs;
        static const int NUM_INP; // Число входных узлов
        static const int NUM_OUTP; // Число выходых ухлов
        static const int NUM_LAYERS;
        static const std::string TRAIN_FILE_NAME;
        struct fann *neuralNetwork;
        void convetToFann(const Triple& ph, fann_type *result) const;

        int trunc(double d) const { return (int)(d+0.5);}
    public:
        Classifier() : numHidden(0), maxEpochs(0), reqError(0), printEpochs(0), neuralNetwork(NULL) {
            std::cerr<<"CONSTRUCTOR CALLED\n";
        }

        Classifier(int nh, int maxEps, double rE = 0.0001f, int report = 1000) :
                numHidden(nh), maxEpochs(maxEps), reqError(rE), printEpochs(report) {
            neuralNetwork = fann_create_standard(NUM_LAYERS, NUM_INP,
                                                 numHidden, NUM_OUTP);
            fann_set_activation_function_hidden(neuralNetwork, FANN_SIGMOID_SYMMETRIC);
            fann_set_activation_function_output(neuralNetwork, FANN_SIGMOID_SYMMETRIC);
        }

        void trainFromFile(const std::string &filename); // Обучить нейронную сеть
        void train(const std::vector<Document>& trainDocs);
        void save(const std::string &filename) const; // Сохраить нейронную сеть
        void load(const std::string &filename); // Загрузить нейронную сеть из файла
        std::vector<ClassifiedTriple> run(const std::vector<Triple> triples) const;
        ClassifiedTriple runSingle(const Triple& t) const;

        Classifier(const Classifier& o);
        Classifier& operator=(const Classifier &o);
        ~Classifier() {
            fann_destroy(neuralNetwork);
        }
    };
}

#endif //COREFRESOLUTION_CLASSIFIER_H
