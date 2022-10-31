#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <string>

// DLL
#if _WIN32
    #define MP_CPP_EXPORT __declspec(dllexport)  // DLL Export
    #if defined(COMPILING_DLL)
        #define MP_CPP_API __declspec(dllexport)  // DLL Export
    #else
        #define MP_CPP_API __declspec(dllimport)  // DLL Import
    #endif
#else
// Linux Function definition (LIB/DLL Source) 
    #define MP_CPP_EXPORT __attribute__((visibility("default")))  
    #define MP_CPP_API
#endif

namespace mediapipe {

class Example {
public:
    // Create and initialize using provided graph
    // Returns nullptr if initialization failed
    MP_CPP_API static Example* Create(const char* graph);
    MP_CPP_API virtual ~Example(){}

    // Processes one frame and blocks until finished
    // Input data is expected to be ImageFormat::SRGB (24bits)
    // Returns nullptr if failed to run graph
    // Returns pointer to image whose size is the same as input image
    // and whose format is ImageFormat::SRGB
    // ImageFormat::SRGB is QImage::Format_RGB888 in Qt
    // Function does not take ownership of input data
    MP_CPP_API virtual uint8_t* Process(uint8_t* data, int width, int height) = 0;
};

}
#endif
