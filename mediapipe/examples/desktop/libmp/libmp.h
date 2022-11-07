#ifndef LIBMP_H
#define LIBMP_H

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

enum class LibMPOutputType {
    ImageFrame,
    Detection,
    DetectionList,
    NormalizedLandmark,
    NormalizedLandmarkList
};

// MP_CPP_API on struct def applies to all its functions
struct MP_CPP_API LibMP {
    // Create and initialize using provided graph
    // Returns nullptr if initialization failed
    static LibMP* Create(const char* graph, const char* inputStream);
    virtual ~LibMP(){}

    virtual bool AddOutputStream(const char* outputStream) = 0;
    virtual bool Start() = 0;

    // Processes one frame and blocks until finished
    // Input data is expected to be ImageFormat::SRGB (24bits)
    // Returns false if failed to run graph
    // Returns true if succeeded
    // Function does not take ownership of input data
    virtual bool Process(uint8_t* data, int width, int height) = 0;
    virtual const void* GetOutputPacket(const char* outputStream) = 0;

    // Writes output image (size is the same as input image)
    // and whose format is ImageFormat::SRGB
    // ImageFormat::SRGB is QImage::Format_RGB888 in Qt
    static size_t GetOutputImageSize(const void* outputPacketVoid);
    static bool WriteOutputImage(uint8_t* dst, const void* outputPacketVoid);

    // Get an output packet's underlying protobuf message
    static const void* GetOutputPacketProto(const void* outputPacketVoid);
    static const void* GetOutputPacketProto(const void* outputPacketVoid, unsigned int idx);

    // Get # elements in an output packet's protobuf message vec
    static size_t GetOutputProtoVecSize(const void* outputPacketVoid);

    // Get size (in bytes) of a single output protobuf message
    static size_t GetOutputProtoByteSize(const void* outputProtoVoid);

    // Write serialized form of a protobuf message to passed byte array dst
    static bool WriteOutputProtoData(uint8_t* dst, const void* outputProtoVoid, int size);
};

}
#endif // LIBMP_H
