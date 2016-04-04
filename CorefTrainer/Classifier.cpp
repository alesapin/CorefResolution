//
// Created by alesapin on 04.04.16.
//

#include "Classifier.h"

namespace coref{

    const int Classifier::NUM_INP = 14;
    const int Classifier::NUM_OUTP = 2;
    const int Classifier::NUM_LAYERS = 3;

    void Classifier::train(const std::string &filename) {
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

    ClassifiedTriple Classifier::runSingle(const Triple &t) const {
        fann_type *out;
        fann_type input[NUM_INP];
        convetToFann(t,input);

        out = fann_run(neuralNetwork, input);
        if(out[0] == 1){
            return std::make_tuple(std::get<0>(t),std::get<1>(t),std::get<2>(t),DetectedCoref::FIRST);
        }else if(out[1] == 1){
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
        synt::ParsedPharse f = std::get<0>(ph);
        synt::ParsedPharse s = std::get<1>(ph);
        synt::ParsedPharse t = std::get<2>(ph);
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


}


