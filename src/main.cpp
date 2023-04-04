#include <iostream>
#include "SynaesthetiQ-LED-Library/synaesthetiq.hpp"
#include "SynaesthetiQ-audiocapture/src/AudioCapture/AudioCapture.h"
#include "SynaesthetiQ-audiocapture/src/GenreClassification/GenreClassifier.h"

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
    Visualiser() : audioCapture("", false), synaesthetiQ(), genreClassifier(){
        genreClassifier.register_genre_callback(&genre_prediction_callback);
        audioCapture.register_callback(genreClassifier.audio_callback);
    }

    void visualise(){
        Colour colour(100, 100, 100);
        
        // if predictions have been made
        if(predictions.size()>0){

            // Find genre with maximum certainty
            std::string genre = "";
            float max_certainty = 0;
            for(auto prediction : predictions){
                if(prediction.second>=max_certainty){
                    max_certainty = prediction.second;
                    genre = prediction.first;
                }
            }

            if(genre=="rock") {
                colour.setBlue(155);
                colour.setRed(50);
            }
            else if(genre=="pop") {
                colour.setRed(150);
            }
            else if(genre=="blues") {
                colour.setBlue(155);
            }
            else if(genre=="reggae"){
                colour.setGreen(155);
            }
        }

        synaesthetiQ.setMatrixColour(colour);
        synaesthetiQ.render();
    }
};

int main(){
    Visualiser visualiser;
    while(true){
        visualiser.visualise();
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 10 Hz
    }
}