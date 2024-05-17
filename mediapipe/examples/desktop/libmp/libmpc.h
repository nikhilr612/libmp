#ifndef LIBMPC_H
#define LIBMPC_H

#include <stddef.h>
#include <stdint.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

// Windows DLL
#if _WIN32
    #define MP_C_EXPORT __declspec(dllexport)  // DLL Export
    #if defined(COMPILING_DLL)
        #define MP_C_API __declspec(dllexport)  // DLL Export
    #else
        #define MP_C_API __declspec(dllimport)  // DLL Import
    #endif
#else
// Linux SO
    #define MP_C_EXPORT __attribute__((visibility("default")))  
    #define MP_C_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _LMP_Opaque* MPHandle;

// Create a MP graph with the specified input stream
// Returns nullptr if initialization failed
MP_C_API MPHandle MP_Create(const char* graph, const char* inputStream);

// Destructor for LibMP
MP_C_API void MP_Delete(MPHandle libMP);

// Create a poller for the specified output stream
// Must be called before GetOutputPacket for the output stream
MP_C_API bool MP_AddOutputStream(MPHandle libMP, const char* outputStream);

// Sets max queue size for the specified output stream
// outputStream must have an associated poller
// i.e., AddOutputStream must have been called beforehand with outputStream
MP_C_API void MP_SetOutputStreamMaxQueueSize(MPHandle libMP, const char* outputStream, int queue_size);

// Start the MP graph
MP_C_API bool MP_Start(MPHandle libMP);

// Processes one frame
// Function copies (i.e., DOES NOT take ownership of) input data
// Returns true if succeeded; false if failed
MP_C_API bool MP_Process(MPHandle libMP, uint8_t* data, int width, int height, int image_format);

// Blocks until graph is idle
MP_C_API bool MP_WaitUntilIdle(MPHandle libMP);

// Returns number of packets in queue of an outputStream
// outputStream must have an associated poller
// i.e., AddOutputStream must have been called beforehand with outputStream
MP_C_API int MP_GetOutputQueueSize(MPHandle libMP, const char* outputStream);

// Returns the next packet available in the passed output stream as a void*
// Returns nullptr if failed
MP_C_API const void* MP_GetOutputPacket(MPHandle libMP, const char* outputStream);

// Given an output packet (passed as a void*), returns the size in bytes
// of its contained image (if stored contiguously)
MP_C_API size_t MP_GetOutputImageSize(MPHandle libMP, const void* outputPacketVoid);

// Copies the output image of the passed output packet to dst
// Format is the same as that passed to Process() (ImageFormat::SRGB)
// Returns true if succeeded; false if failed
MP_C_API bool MP_WriteOutputImage(MPHandle libMP, uint8_t* dst, const void* outputPacketVoid);

// Returns true if packet is empty, false otherwise
MP_C_API bool MP_PacketIsEmpty(MPHandle libMP, const void* outputPacketVoid);

// Get an output packet's underlying protobuf message
// Returns nullptr if failed
MP_C_API const void* MP_GetPacketProtoMsg(MPHandle libMP, const void* outputPacketVoid);

// Get an output packet's underlying protobuf message at index idx (packet must be a vector of messages)
// Returns nullptr if failed
MP_C_API const void* MP_GetPacketProtoMsgAt(MPHandle libMP, const void* outputPacketVoid, unsigned int idx);

// Get # elements in an output packet's protobuf message vec
MP_C_API size_t MP_GetPacketProtoMsgVecSize(MPHandle libMP, const void* outputPacketVoid);

// Get size (in bytes) of a single output protobuf message
MP_C_API size_t MP_GetProtoMsgByteSize(MPHandle libMP, const void* outputProtoVoid);

// Write serialized form of a protobuf message to passed byte array dst
// Returns true if succeeded; false if failed
MP_C_API bool MP_WriteProtoMsgData(MPHandle libMP, uint8_t* dst, const void* outputProtoVoid, int size);

// Deletes packet
MP_C_API void MP_DeletePacket(MPHandle libMP, const void* packetVoid);

/**
 * Landmark related utilities.
 * Functions are self-explanatory.
 * */

typedef struct {
    float* data;
    size_t capacity;
    size_t length;
} LandmarkBuf;

MP_C_API void MP_ClearBuf(LandmarkBuf* buf);
MP_C_API bool MP_InitBuf(LandmarkBuf* buf, size_t cap);
MP_C_API bool MP_PushBuf(LandmarkBuf* buf, float x, float y, float z);
MP_C_API void MP_FreeBuf(LandmarkBuf* buf);

// Get the x, y, z co-ordinates of landmark at `index`.
// dstbuf must be writable upto 3 elements.
// Returns false if index is out of range.
// Otherwise true.
MP_C_API bool MP_GetAtBuf(const LandmarkBuf* buf, size_t index, float* dstbuf);

// Define a callback to access landmarks loaded into buffer.
// `idx` - index of feature
// `n` - total number of features detected
// `buf` - buffer with XYZ landmarks
// `userdata` - argument passed of Video Capture call.
typedef void (*MP_VideoCaptureCb)(size_t idx, size_t n, LandmarkBuf* buf, void* userdata);

// Utility function for setting up a loop to capture video from camera device using opencv and call callback functions accordingly.
// Returns true, if loop was exited successfully, false otherwise.
// Note, this function does not own any of its arguments. The library and landmark buffer must be freed after function exit.
MP_C_API bool MPCV_VideoCapture(unsigned dev_id, MPHandle libmp, const char* outputStream, LandmarkBuf* buf, MP_VideoCaptureCb func, void* userdata);

// Utility function to extract packets from output stream and populate landmark buffer.
MP_C_API void MP_ExtractLandmarks(const void* lm_list_proto, LandmarkBuf* lbuf);

#ifdef __cplusplus
}
#endif

#endif // LIBMPC_H
