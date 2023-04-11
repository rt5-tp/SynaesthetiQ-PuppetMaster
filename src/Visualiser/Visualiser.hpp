#include "../SynaesthetiQ-LED-Library/src/SynaesthetiQ/synaesthetiq.hpp"
#include "../SynaesthetiQ-audiocapture/src/AudioCapture/AudioCapture.h"
#include "../SynaesthetiQ-audiocapture/src/GenreClassification/GenreClassifier.h"
#include "../SynaesthetiQ-audiocapture/src/FFTProcessor.h"

class Visualiser {
    private:
    AudioCapture audioCapture; 
    FFTProcessor fftProcessor;
    GenreClassifier genreClassifier;
    SynaesthetiQ synaesthetiQ;

    static std::vector<std::pair<std::string, float>> predictions;
    static std::vector<double> fft_data;

    // Keeps predictions vector up-to-date with latest predictions
    static void genre_prediction_callback(const std::vector<std::pair<std::string, float>>& predictions){
        Visualiser::predictions = predictions;
    }

    static void fft_callback(const std::vector<double> & fft_data){
        Visualiser::fft_data = fft_data;
    }

    public:
    Visualiser();

    void visualise();
};