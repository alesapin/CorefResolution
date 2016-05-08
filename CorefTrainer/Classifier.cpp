//
// Created by alesapin on 04.04.16.
//

#include "Classifier.h"

namespace coref{

    const int Classifier::NUM_INP = 14;
    const int Classifier::NUM_OUTP = 2;
    const int Classifier::NUM_LAYERS = 3;
    const std::string Classifier::TRAIN_FILE_NAME = "train.data";

    void Classifier::trainFromFile(const std::string &filename) {
        fann_train_on_file(neuralNetwork, filename.c_str(), maxEpochs,
                           printEpochs, reqError);
    }

    void Classifier::save(const std::string &filename) const {
        fann_save(neuralNetwork,filename.c_str());
    }

    void Classifier::load(const std::string &filename) {
        fann_destroy(neuralNetwork);
        neuralNetwork = fann_create_from_file(filename.c_str());
    }
    void Classifier::destroy() {
        fann_destroy(neuralNetwork);
    }
    void Classifier::createNew()
    {
        fann_destroy(neuralNetwork);
        neuralNetwork = fann_create_standard(NUM_LAYERS, NUM_INP, numHidden, NUM_OUTP);
        //fann_set_activation_function_hidden(neuralNetwork, FANN_SIGMOID_SYMMETRIC);
        //fann_set_activation_function_output(neuralNetwork, FANN_SIGMOID_SYMMETRIC);
        fann_set_activation_function_hidden(neuralNetwork, FANN_LINEAR_PIECE_SYMMETRIC);
        fann_set_activation_function_output(neuralNetwork, FANN_LINEAR_PIECE_SYMMETRIC);
        fann_set_training_algorithm(neuralNetwork,FANN_TRAIN_INCREMENTAL);
    }

    ClassifiedTriple Classifier::runSingle(const Triple &t) const {
        fann_type *out;
        fann_type input[NUM_INP];
        convetToFann(t,input);

        out = fann_run(neuralNetwork, input);
        if(trunc(out[0])==1){
            return std::make_tuple(std::get<0>(t),std::get<1>(t),std::get<2>(t),DetectedCoref::FIRST);
        }else if(trunc(out[1]) == 1){
            return std::make_tuple(std::get<0>(t),std::get<1>(t),std::get<2>(t),DetectedCoref::SECOND);
        }else {
            return std::make_tuple(std::get<0>(t),std::get<1>(t),std::get<2>(t),DetectedCoref::NO);
        }

    }

    std::vector<ClassifiedTriple> Classifier::run(const std::vector<Triple> triples) const {
        std::vector<coref::ClassifiedTriple> result(triples.size());
        int i = 0;
        for(const Triple& t:triples){
            result[i++] = runSingle(t);
        }
        return result;
    }

    void Classifier::convetToFann(const Triple& ph, fann_type *result) const {
        synt::ParsedPhrase f = std::get<0>(ph);
        synt::ParsedPhrase s = std::get<1>(ph);
        synt::ParsedPhrase t = std::get<2>(ph);
        result[0] = int(f.sp);
        result[1] = int(f.gen);
        result[2] = int(f.num);
        result[3] = int(f.pers);
        result[4] = int(s.sp);
        result[5] = int(s.gen);
        result[6] = int(s.num);
        result[7] = int(s.pers);
        result[8] = int(s.shift - f.shift);
        result[9] = int(t.sp);
        result[10] = int(t.gen);
        result[11] = int(t.num);
        result[12] = int(t.pers);
        result[13] = int(t.shift - f.shift);
    }

    Classifier::Classifier(const Classifier &o) {
        numHidden = o.numHidden;
        maxEpochs = o.maxEpochs;
        reqError = o.reqError;
        printEpochs = o.printEpochs;
        if(o.neuralNetwork) {
            neuralNetwork = fann_copy(o.neuralNetwork);
        }

    }

    Classifier &Classifier::operator=(const Classifier &o) {
        if(neuralNetwork != o.neuralNetwork) {
            numHidden = o.numHidden;
            maxEpochs = o.maxEpochs;
            reqError = o.reqError;
            printEpochs = o.printEpochs;
            if(neuralNetwork) {
                fann_destroy(neuralNetwork);
            }
            neuralNetwork = fann_copy(o.neuralNetwork);
        }
        return *this;
    }

    void Classifier::train(const std::vector<Document> &trainDocs) {
        std::ofstream data(TRAIN_FILE_NAME);
        int size = 0;
        std::vector<std::vector<ClassifiedTriple>> docs;
        for (const Document &d : trainDocs) {
            std::vector<ClassifiedTriple> triples = d.getClassifiedTriples();
            size += triples.size();
            docs.push_back(triples);
        }
        data << size <<" " << NUM_INP <<" " << NUM_OUTP<<"\n";
        for (auto triples : docs) {
            writeClassifiedTriples(triples,data);
        }
        data.close();
        trainFromFile(TRAIN_FILE_NAME);
    }


}


