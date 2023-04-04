cmake_minimum_required(VERSION 3.0)

project(PuppetMaster)

set(AC SynaesthetiQ-audiocapture/src)
set(LED SynaesthetiQ-LED-Library)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fsanitize=address -fno-omit-frame-pointer")
set(SOURCES main.cpp ${LED}/synaesthetiq.cpp ${AC}/AudioCapture/PingPongBuffer.cpp ${AC}/AudioCapture/AudioCapture.cpp ${AC}/GenreClassification/GenreClassifier.cpp ${AC}/FFTProcessor.cpp)
find_package( CURL REQUIRED )

add_executable(${PROJECT_NAME} ${SOURCES})

include_directories(/usr/local/lib ${LED} ${AC}/AudioCapture ${AC}/GenreClassification ${AC}/GenreClassification/cpp_genre_classifier)

target_link_libraries(${PROJECT_NAME} /usr/local/lib/libws2811.a asound SDL2 stdc++ pthread fftw3 CURL::libcurl)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.exe")

set(HEADERS synaesthetiq.hpp Colour.hpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
