#include "Visualiser.hpp"
#include <cmath>
#include <algorithm>


std::vector<std::pair<std::string, float>> Visualiser::predictions;
std::vector<double> Visualiser::fft_data;
std::vector<short> Visualiser::envelope_data;
std::vector<std::vector<bool>> Visualiser::spectrogram;

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
    Colour colour(10, 10, 10);
    
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

    short max_val = 0;
    for(auto val : envelope_data){
        if(val>max_val) max_val = val;
    }

    int led_val = 255.0*((double)max_val / 4000.0);

    std::cout << "Max_val: " << max_val << std::endl;
    std::cout << "Led_val: " << led_val << std::endl;

    colour.setRed(led_val%200);
    colour.setGreen(led_val%200);

    for(int row=0; row<spectrogram.size(); row++){
        for(int column=0; column<spectrogram[row].size(); column++){
            synaesthetiQ.setMatrixPixelColour(column, row, colour);
        }
    }

    //synaesthetiQ.setMatrixColour(colour);
    synaesthetiQ.render();
}

/**
 * This function takes in FFT data, number of rows, number of columns, and a max frequency, and outputs a 2D matrix of bools.
 * @param fftData a vector of double values representing the FFT data
 * @param rows an integer representing the number of rows in the output LED matrix
 * @param cols an integer representing the number of columns in the output LED matrix
 * @param minFrequency a double representing the minimum frequency of the LED matrix display
 * @param maxFrequency a double representing the maximum frequency of the LED matrix display
 * @return a 2D vector of bool values representing the LED matrix display
 */
std::vector<std::vector<bool>> convertFFTToLEDMatrix(const std::vector<double> &fftData, int rows, int cols, double minFrequency, double maxFrequency)
{
    // Creates a 2D matrix of bools with size rows x cols, initialized to false
    std::vector<std::vector<bool>> ledMatrix(rows, std::vector<bool>(cols, false));

    // Finds the maximum amplitude in the FFT data
    double maxAmplitude = *std::max_element(fftData.begin(), fftData.end());

    // Sets the logarithmic frequency range of the LED matrix display
    double minFrequencyLog = std::log10(minFrequency);             // Set the minimum frequency cutoff to 30 Hz
    double maxFrequencyLog = std::log10(maxFrequency);   // Set the maximum frequency cutoff to the input max frequency
    double logRange = maxFrequencyLog - minFrequencyLog; // Calculate the logarithmic range of the frequency scale

    // Sets a cutoff threshold to ignore values below a certain amount
    double cutoffThreshold = 1;                               // If the amplitude value is less than this, don't show it on the LED matrix
    // std::cout << "Cutoff = " << cutoffThreshold << std::endl; // Output the cutoff threshold value for debugging

    // Calculates the scaling factor for the amplitudes based on the number of rows in the LED matrix display
    double yScaleFactor = static_cast<double>(rows) / maxAmplitude;

    // Loops through each row and column in the LED matrix display
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // Calculates the center frequency of the current column
            double targetFrequencyLog = minFrequencyLog + (j * logRange) / cols; // Calculate the logarithmic frequency of the center of the current column
            double targetFrequency = std::pow(10, targetFrequencyLog);           // Convert the logarithmic frequency to linear frequency
            double lowerBound = targetFrequency - targetFrequency / 2;           // Calculate the lower bound of the target frequency range
            double upperBound = targetFrequency + targetFrequency / 2;           // Calculate the upper bound of the target frequency range

            // Searches for the maximum amplitude value in the target frequency range
            double maxValueInBin = 0;
            int lowerIndex = static_cast<int>(lowerBound / maxFrequency * fftData.size()); // Find the lower index valuee in the bin
            int upperIndex = std::min(static_cast<int>(upperBound / maxFrequency * fftData.size()), static_cast<int>(fftData.size())); // Find the upper index value in the bin
            for (int k = lowerIndex; k < upperIndex; k++)
            {
                maxValueInBin = std::max(maxValueInBin, fftData[k]);
            }
            // std::cout << "max value in bin = " << maxValueInBin << std::endl; // Output the maximum amplitude value in the current bin for debugging

            // If the maximum amplitude value is above the cutoff threshold, set the corresponding LED matrix element to true
            if (maxValueInBin > cutoffThreshold)
            {
                double scaledAmplitude = maxValueInBin * yScaleFactor; // Scale the amplitude value based on the number of rows in the LED matrix display
                // std::cout << "Scaled Amplitude at bin " << j << ": " << scaledAmplitude << std::endl; // Output the scaled amplitude value for debugging
                int threshold = static_cast<int>(scaledAmplitude);
                if (i < threshold)
                {
                    ledMatrix[rows - i - 1][j] = true; // If the current row index is below the scaled amplitude value, set the corresponding LED matrix element to true
                }
            }
        }
    }

    return ledMatrix; // Return the final LED matrix display
}

void Visualiser::fft_callback(const std::vector<double> & fft_data){
    Visualiser::fft_data = fft_data;
    int rows = 16;
    int columns = 32;
    std::vector<std::vector<bool>> ledMatrix = convertFFTToLEDMatrix(fft_data, rows, columns, 60, 10000); // 2D boolean vector to represent LED matrix

}
