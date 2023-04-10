#include "../SynaesthetiQ-LED-Library/src/SynaesthetiQ/synaesthetiq.hpp"
#include "../SynaesthetiQ-audiocapture/src/AudioCapture/AudioCapture.h"
#include "../SynaesthetiQ-audiocapture/src/GenreClassification/GenreClassifier.h"

class Visualiser {
    private:
    AudioCapture audioCapture;
    GenreClassifier genreClassifier;
    SynaesthetiQ synaesthetiQ;
    static std::vector<std::pair<std::string, float>> predictions;

    // Keeps predictions vector up-to-date with latest predictions
    static void genre_prediction_callback(const std::vector<std::pair<std::string, float>>& predictions){
        Visualiser::predictions = predictions;
    }

    public:
    Visualiser();

    void visualise();
};