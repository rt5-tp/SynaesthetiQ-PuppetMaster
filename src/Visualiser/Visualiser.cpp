#include "Visualiser.hpp"

std::vector<std::pair<std::string, float>> Visualiser::predictions;

/* 
 * @brief Sets up callbacks
 * 
 *        ||=> genreClassifier  =||
 * audio =||=> envelopeFollower =||=> Visualiser
 *        ||=> fftProcessor     =||
*/
Visualiser::Visualiser() : audioCapture(""), synaesthetiQ(), genreClassifier(), fftProcessor(), envelopeFollower(44100,10){
    
    // data flowing into visualiser, which can be accessed in visualise()
    genreClassifier.register_genre_callback(&Visualiser::genre_prediction_callback);
    fftProcessor.registerCallback(&Visualiser::fft_callback);
    envelopeFollower.registerCallback(&Visualiser::envelope_callback);
    
    // data flowing from audio into genreClassifer, fftProcessor
    //        ||=> genreClassifier
    // audio =|| 
    //        ||=> fftProcessor     
    audioCapture.register_callback(genreClassifier.audio_callback);
    audioCapture.register_callback(fftProcessor.audio_callback);
    audioCapture.register_callback(envelopeFollower.audio_callback);
}

void Visualiser::visualise(){
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