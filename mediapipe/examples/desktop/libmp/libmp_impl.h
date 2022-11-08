#ifndef LIBMP_IMPL_H
#define LIBMP_IMPL_H

#include <map>
#include <unordered_map>
#include "libmp.h"
#include "absl/status/status.h"
#include "mediapipe/framework/calculator_framework.h"

namespace mediapipe {

class LibMPImpl : public LibMP {
public:
    LibMPImpl(){}
    ~LibMPImpl();

    absl::Status Init(const char* graph, const char* inputStream);
    bool AddOutputStream(const char* outputStream);
    bool Start();

    bool Process(uint8_t* data, int width, int height);
    const void* GetOutputPacket(const char* outputStream);

    size_t GetOutputImageSize(const void* outputPacketVoid);
    bool WriteOutputImage(uint8_t* dst, const void* outputPacketVoid);

    const void* GetPacketProtoMsg(const void* outputPacketVoid);
    const void* GetPacketProtoMsgAt(const void* outputPacketVoid, unsigned int idx);
    size_t GetProtoMsgVecSize(const void* outputPacketVoid);
    size_t GetProtoMsgByteSize(const void* outputProtoVoid);
    bool WriteProtoMsgData(uint8_t* dst, const void* outputProtoVoid, int size);

private:
    mediapipe::CalculatorGraph m_graph;
    std::string m_input_stream;
    std::unordered_map<std::string, absl::StatusOr<OutputStreamPoller>> m_pollers;
    std::unordered_map<std::string, mediapipe::Packet> m_packets;
    size_t m_frame_timestamp = 0;
};

}
#endif // LIBMP_IMPL_H
