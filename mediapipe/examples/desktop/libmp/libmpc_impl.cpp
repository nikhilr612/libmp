#include "libmpc_impl.h"
#include "libmpc.h"

// Flat wrapper around inner, casts are safe.
struct _LMP_Opaque {
    mediapipe::LibMPImpl inner;
};

MP_C_EXPORT MPHandle MP_Create(const char* graph, const char* inputStream) {
    return (MPHandle)(mediapipe::LibMP::Create(graph, inputStream)); // iffy cast, should work.
}

MP_C_EXPORT void MP_Delete(MPHandle libMP) {
    delete (mediapipe::LibMPImpl*) libMP;
}

MP_C_EXPORT bool MP_AddOutputStream(MPHandle libMP, const char* outputStream) {
    return ((mediapipe::LibMPImpl*)libMP)->AddOutputStream(outputStream);
}

MP_C_EXPORT void MP_SetOutputStreamMaxQueueSize(MPHandle libMP, const char* outputStream, int queue_size) {
    ((mediapipe::LibMPImpl*)libMP)->SetOutputStreamMaxQueueSize(outputStream, queue_size);
}

MP_C_EXPORT bool MP_Start(MPHandle libMP) {
    return ((mediapipe::LibMPImpl*)libMP)->Start();
}

MP_C_EXPORT bool MP_Process(MPHandle libMP, uint8_t* data, int width, int height, int image_format) {
    return ((mediapipe::LibMPImpl*)libMP)->Process(data, width, height, image_format);
}

MP_C_EXPORT bool MP_WaitUntilIdle(MPHandle libMP) {
    return ((mediapipe::LibMPImpl*)libMP)->WaitUntilIdle();
}

MP_C_EXPORT int MP_GetOutputQueueSize(MPHandle libMP, const char* outputStream) {
    return ((mediapipe::LibMPImpl*)libMP)->GetOutputQueueSize(outputStream);
}

MP_C_EXPORT const void* MP_GetOutputPacket(MPHandle libMP, const char* outputStream) {
    return ((mediapipe::LibMPImpl*)libMP)->GetOutputPacket(outputStream);
}

MP_C_EXPORT size_t MP_GetOutputImageSize(MPHandle libMP, const void* outputPacketVoid) {
    return ((mediapipe::LibMPImpl*)libMP)->GetOutputImageSize(outputPacketVoid);
}

MP_C_EXPORT bool MP_WriteOutputImage(MPHandle libMP, uint8_t* dst, const void* outputPacketVoid) {
    return ((mediapipe::LibMPImpl*)libMP)->WriteOutputImage(dst, outputPacketVoid);
}

MP_C_EXPORT bool MP_PacketIsEmpty(MPHandle libMP, const void* outputPacketVoid) {
    return ((mediapipe::LibMPImpl*)libMP)->PacketIsEmpty(outputPacketVoid);
}

MP_C_EXPORT const void* MP_GetPacketProtoMsg(MPHandle libMP, const void* outputPacketVoid) {
    return ((mediapipe::LibMPImpl*)libMP)->GetPacketProtoMsg(outputPacketVoid);
}

MP_C_EXPORT const void* MP_GetPacketProtoMsgAt(MPHandle libMP, const void* outputPacketVoid, unsigned int idx) {
    return ((mediapipe::LibMPImpl*)libMP)->GetPacketProtoMsgAt(outputPacketVoid, idx);
}

MP_C_EXPORT size_t MP_GetPacketProtoMsgVecSize(MPHandle libMP, const void* outputPacketVoid) {
    return ((mediapipe::LibMPImpl*)libMP)->GetPacketProtoMsgVecSize(outputPacketVoid);
}

MP_C_EXPORT size_t MP_GetProtoMsgByteSize(MPHandle libMP, const void* outputProtoVoid) {
    return ((mediapipe::LibMPImpl*)libMP)->GetProtoMsgByteSize(outputProtoVoid);
}

MP_C_EXPORT bool MP_WriteProtoMsgData(MPHandle libMP, uint8_t* dst, const void* outputProtoVoid, int size) {
    return ((mediapipe::LibMPImpl*)libMP)->WriteProtoMsgData(dst, outputProtoVoid, size);
}

MP_C_EXPORT void MP_DeletePacket(MPHandle libMP, const void* packetVoid) {
    ((mediapipe::LibMPImpl*)libMP)->DeletePacket(packetVoid);
}

MP_C_EXPORT void MP_ClearBuf(LandmarkBuf *buf) {
    buf -> length = 0;
}

MP_C_EXPORT bool MP_InitBuf(LandmarkBuf *buf, size_t cap) {
    buf -> data = (float*)malloc(3*cap*sizeof(float));
    buf -> capacity = cap;
    buf -> length = 0;
    return (buf -> data) != NULL;
}

MP_C_EXPORT bool MP_PushBuf(LandmarkBuf *buf, float x, float y, float z){
    if (buf->length < buf->capacity) {
        float* ptr = (buf->data) + (buf->length)*3;
        ptr[0] = x;
        ptr[1] = y;
        ptr[2] = z;
        buf -> length += 1;
        return true;
    } else {
        return false;
    }
}

MP_C_EXPORT void MP_FreeBuf(LandmarkBuf *buf) {
    free(buf -> data);
    buf -> capacity = 0;
    buf -> length = 0;
}

MP_C_EXPORT bool MP_GetAtBuf(const LandmarkBuf *buf, size_t index, float *dstbuf) {
    if (index >= buf -> length) return false;
    float* st = (buf -> data) + 3*index;
    dstbuf[0] = st[0];
    dstbuf[1] = st[1];
    dstbuf[2] = st[2];
    return true;
}

MP_C_EXPORT bool MPCV_VideoCapture(unsigned int dev_id, MPHandle libmp, const char* outputStream, LandmarkBuf *lbuf, MP_VideoCaptureCb func, void* userdata){
    cv::VideoCapture cap(dev_id);
    if (!cap.isOpened()) {
        std::cerr << "Could not open device #0. Is a camera/webcam attached?" << std::endl;
        return false;
    }

    cv::Mat frame_bgr;
    while (cap.read(frame_bgr)) {
        // Convert frame from BGR to RGB
        cv::Mat frame_rgb;
        cv::cvtColor(frame_bgr, frame_rgb, cv::COLOR_BGR2RGB);

        /// Start inference clock
        auto t0 = std::chrono::high_resolution_clock::now();

        auto mpgraph = (mediapipe::LibMP*)libmp;

        // Feed RGB frame into MP face mesh graph (image data is COPIED internally by LibMP)
        bool success = mpgraph->Process(frame_rgb.data, frame_rgb.cols, frame_rgb.rows, mediapipe::ImageFormat::SRGB);
        if (!success) {
            std::cerr << "Process() failed!" << std::endl;
            break;
        }

        mpgraph->WaitUntilIdle();

        // Stop inference clock
        auto t1 = std::chrono::high_resolution_clock::now();
        int inference_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();

        // Get landmark coordinates in custom data structure using helper function (see above)
        // std::vector<std::vector<std::array<float, 3>>> normalized_landmarks = get_landmarks_new(mpgraph);

        // // For each face, draw a circle at each landmark's position
        // size_t num_detected = normalized_landmarks.size();
        // for (int hand_num = 0; hand_num < num_detected; hand_num++) {
        //  for (const std::array<float, 3>& norm_xyz : normalized_landmarks[hand_num]) {
        //      int x = static_cast<int>(norm_xyz[0] * frame_bgr.cols);
        //      int y = static_cast<int>(norm_xyz[1] * frame_bgr.rows);
        //      cv::circle(frame_bgr, cv::Point(x, y), 1, cv::Scalar(0, 255, 0), -1);
        //  }
        // }

        // I use a unique_ptr for convenience, so that DeletePacket is called automatically
        // You could also manage deletion yourself, manually:
        // const void* packet = face_mesh->GetOutputPacket("multi_face_landmarks");
        // mediapipe::LibMP::DeletePacket(packet);

        // Read landmarks.

        std::unique_ptr<const void, decltype(&mediapipe::LibMP::DeletePacket)> lm_packet_ptr(nullptr, mediapipe::LibMP::DeletePacket);

        // Keep getting packets from queue until empty
        while (mpgraph->GetOutputQueueSize(outputStream) > 0) {
            lm_packet_ptr.reset(mpgraph->GetOutputPacket(outputStream));
        }

        if (lm_packet_ptr.get() == nullptr || mediapipe::LibMP::PacketIsEmpty(lm_packet_ptr.get())) {
            goto after_processing; // skip if empty.
        }

        //  Create multi_landmarks from packet's protobuf data
        size_t n = mediapipe::LibMP::GetPacketProtoMsgVecSize(lm_packet_ptr.get());
        for (int i = 0; i < n; i++) {
            const void* lm_list_proto = mediapipe::LibMP::GetPacketProtoMsgAt(lm_packet_ptr.get(), i); //   Get reference to protobuf message
            size_t lm_list_proto_size = mediapipe::LibMP::GetProtoMsgByteSize(lm_list_proto);

            std::shared_ptr<uint8_t[]> proto_data(new uint8_t[lm_list_proto_size]);
            mediapipe::LibMP::WriteProtoMsgData(proto_data.get(), lm_list_proto, static_cast<int>(lm_list_proto_size));

            // Initialize a mediapipe::NormalizedLandmarkList object from the buffer
            mediapipe::NormalizedLandmarkList landmarks;
            landmarks.ParseFromArray(proto_data.get(), static_cast<int>(lm_list_proto_size));

            // Copy the landmark data into buffer.
            MP_ClearBuf(lbuf); // Clear first.
            for (const mediapipe::NormalizedLandmark& lm : landmarks.landmark()) {
                float x = lm.x();
                float y = lm.y();
                float z = lm.z();
                MP_PushBuf(lbuf, x, y, z);
                int radius = (int)(3 - 2*z);
                cv::circle(frame_bgr, cv::Point(x * frame_bgr.cols, y * frame_bgr.rows), radius, cv::Scalar(0, 255, 0), -1);
            }

            // Invoke callback on landmark buffer.
            func((size_t)i, n, lbuf, userdata);
        }

        // Finished processing landmarks.
        after_processing:
        cv::flip(frame_bgr, frame_bgr, 1);
        cv::imshow("Video Capture", frame_bgr);

        // Close on any keypress
        if (cv::waitKey(1) >= 0) {
            break;
        }
    }

    cv::destroyAllWindows();
    return true;
}

MP_C_EXPORT void MP_ExtractLandmarks(const void* lm_list_proto, LandmarkBuf *lbuf){
    size_t lm_list_proto_size = mediapipe::LibMP::GetProtoMsgByteSize(lm_list_proto);

    std::shared_ptr<uint8_t[]> proto_data(new uint8_t[lm_list_proto_size]);
    mediapipe::LibMP::WriteProtoMsgData(proto_data.get(), lm_list_proto, static_cast<int>(lm_list_proto_size));

    // Initialize a mediapipe::NormalizedLandmarkList object from the buffer
    mediapipe::NormalizedLandmarkList landmarks;
    landmarks.ParseFromArray(proto_data.get(), static_cast<int>(lm_list_proto_size));

    // Copy the landmark data into buffer.
    MP_ClearBuf(lbuf); // Clear first.
    for (const mediapipe::NormalizedLandmark& lm : landmarks.landmark()) {
        MP_PushBuf(lbuf, lm.x(), lm.y(), lm.z()); // Push in order.
    }
}