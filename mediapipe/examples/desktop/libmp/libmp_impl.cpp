#include "libmp_impl.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "google/protobuf/message_lite.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/formats/image_frame.h"

namespace mediapipe {

LibMPImpl::~LibMPImpl()
{
    LOG(INFO) << "Shutting down.";
    absl::Status status = m_graph.CloseInputStream(m_input_stream);
    if (status.ok()){
    	absl::Status status1 = m_graph.WaitUntilDone();
        if (!status1.ok()) {
            LOG(INFO) << "Error in WaitUntilDone(): " << status1.ToString();
        }
    } else {
        LOG(INFO) << "Error in CloseInputStream(): " << status.ToString();
    }
}

absl::Status LibMPImpl::Init(const char* graph, const char* inputStream){
    LOG(INFO) << "Parsing graph config";
    mediapipe::CalculatorGraphConfig config = mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(graph);

    LOG(INFO) << "Initializing the calculator graph.";
    MP_RETURN_IF_ERROR(m_graph.Initialize(config));

    m_input_stream.assign(inputStream);

    return absl::OkStatus();
}

bool LibMPImpl::AddOutputStream(const char* outputStream){
    m_pollers.emplace(outputStream, m_graph.AddOutputStreamPoller(outputStream));
    return m_pollers.at(outputStream).ok() ? true : false;
}

bool LibMPImpl::Start(){
    LOG(INFO) << "Start running the calculator graph.";
    const std::map<std::string, mediapipe::Packet>& extra_side_packets = {};
    return m_graph.StartRun(extra_side_packets).ok();
}

bool LibMPImpl::Process(uint8_t* data, int width, int height) 
{
    if (data == nullptr){
        LOG(INFO) << __FUNCTION__ << " input data is nullptr!";
        return false;
    }

    // Clear packets
    m_packets.clear();

    int width_step = width * ImageFrame::ByteDepthForFormat(ImageFormat::SRGB) * ImageFrame::NumberOfChannelsForFormat(ImageFormat::SRGB);
    auto input_frame_for_input = absl::make_unique<ImageFrame>(ImageFormat::SRGB, width, height, width_step, (uint8*)data, ImageFrame::PixelDataDeleter::kNone);

    m_frame_timestamp++;

    if (!m_graph.AddPacketToInputStream(m_input_stream, mediapipe::Adopt(input_frame_for_input.release()).At(mediapipe::Timestamp(m_frame_timestamp))).ok()) {
        LOG(INFO) << "Failed to add packet to input stream. Call m_graph.WaitUntilDone() to see error (or destroy LibMP object)";
        return false;
    }

    return true;
}

const void* LibMPImpl::GetOutputPacket(const char* outputStream){
    if (m_pollers.find(outputStream) == m_pollers.end()){
        LOG(INFO) << "No poller found for output stream '" << outputStream << "'. Was it created using AddOutputStream beforehand?";
        return nullptr;
    }
    if (m_packets.find(outputStream) == m_packets.end()){
        m_packets.emplace(outputStream, mediapipe::Packet());
        if (!m_pollers.at(outputStream)->Next(&m_packets.at(outputStream))){
            LOG(INFO) << "Poller for output stream '" << outputStream << "' has no next packet. Call m_graph.WaitUntilDone() to see error (or destroy LibMP object). Are models available under mediapipe/models and mediapipe/modules?";
            return nullptr;
        }
    }
    return reinterpret_cast<const void*>(&m_packets.at(outputStream));
}

}
