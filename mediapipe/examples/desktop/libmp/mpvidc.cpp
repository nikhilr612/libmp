// #include <iostream>
// #include <ostream>
// #include <chrono>

// #include "libmp.h"
// #include "libmpc.h"

// // Compiled protobuf headers for MediaPipe types used
// #include "mediapipe/framework/formats/landmark.pb.h"
// #include "mediapipe/framework/formats/image_format.pb.h"

// // OpenCV
// #include <opencv2/opencv.hpp>

// MP_C_EXPORT bool MPCV_VideoCapture(unsigned int dev_id, MPHandle libmp, LandmarkBuf *lbuf, MP_VideoCaptureCb func){
// 	cv::VideoCapture cap(dev_id);
// 	if (!cap.isOpened()) {
// 		std::cerr << "Could not open device #0. Is a camera/webcam attached?" << std::endl;
// 		return false;
// 	}

// 	cv::Mat frame_bgr;
// 	while (cap.read(frame_bgr)) {
// 		// Convert frame from BGR to RGB
// 		cv::Mat frame_rgb;
// 		cv::cvtColor(frame_bgr, frame_rgb, cv::COLOR_BGR2RGB);

// 		/// Start inference clock
// 		auto t0 = std::chrono::high_resolution_clock::now();

// 		auto mpgraph = (mediapipe::LibMP*)libmp;

// 		// Feed RGB frame into MP face mesh graph (image data is COPIED internally by LibMP)
// 		bool success = mpgraph->Process(frame_rgb.data, frame_rgb.cols, frame_rgb.rows, mediapipe::ImageFormat::SRGB);
// 		if (!success) {
// 			std::cerr << "Process() failed!" << std::endl;
// 			break;
// 		}

// 		mpgraph->WaitUntilIdle();

// 		// Stop inference clock
// 		auto t1 = std::chrono::high_resolution_clock::now();
// 		int inference_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();

// 		// Get landmark coordinates in custom data structure using helper function (see above)
// 		// std::vector<std::vector<std::array<float, 3>>> normalized_landmarks = get_landmarks_new(mpgraph);

// 		// // For each face, draw a circle at each landmark's position
// 		// size_t num_detected = normalized_landmarks.size();
// 		// for (int hand_num = 0; hand_num < num_detected; hand_num++) {
// 		// 	for (const std::array<float, 3>& norm_xyz : normalized_landmarks[hand_num]) {
// 		// 		int x = static_cast<int>(norm_xyz[0] * frame_bgr.cols);
// 		// 		int y = static_cast<int>(norm_xyz[1] * frame_bgr.rows);
// 		// 		cv::circle(frame_bgr, cv::Point(x, y), 1, cv::Scalar(0, 255, 0), -1);
// 		// 	}
// 		// }

// 		// I use a unique_ptr for convenience, so that DeletePacket is called automatically
// 		// You could also manage deletion yourself, manually:
// 		// const void* packet = face_mesh->GetOutputPacket("multi_face_landmarks");
// 		// mediapipe::LibMP::DeletePacket(packet);

// 		// Read landmarks.

// 		std::unique_ptr<const void, decltype(&mediapipe::LibMP::DeletePacket)> lm_packet_ptr(nullptr, mediapipe::LibMP::DeletePacket);

// 		// Keep getting packets from queue until empty
// 		while (mpgraph->GetOutputQueueSize("landmarks") > 0) {
// 			lm_packet_ptr.reset(mpgraph->GetOutputPacket("landmarks"));
// 		}

// 		if (lm_packet_ptr.get() == nullptr || mediapipe::LibMP::PacketIsEmpty(lm_packet_ptr.get())) {
// 			goto after_processing; // skip if empty.
// 		}

// 		//  Create multi_landmarks from packet's protobuf data
// 		size_t n = mediapipe::LibMP::GetPacketProtoMsgVecSize(lm_packet_ptr.get());
// 		for (int i = 0; i < n; i++) {
// 			const void* lm_list_proto = mediapipe::LibMP::GetPacketProtoMsgAt(lm_packet_ptr.get(), i); // 	Get reference to protobuf message for face
// 			size_t lm_list_proto_size = mediapipe::LibMP::GetProtoMsgByteSize(lm_list_proto);

// 			std::shared_ptr<uint8_t[]> proto_data(new uint8_t[lm_list_proto_size]);
// 			mediapipe::LibMP::WriteProtoMsgData(proto_data.get(), lm_list_proto, static_cast<int>(lm_list_proto_size));

// 			// Initialize a mediapipe::NormalizedLandmarkList object from the buffer
// 			mediapipe::NormalizedLandmarkList landmarks;
// 			landmarks.ParseFromArray(proto_data.get(), static_cast<int>(lm_list_proto_size));

// 			// Copy the landmark data into buffer.
// 			MP_ClearBuf(lbuf); // Clear first.
// 			for (const mediapipe::NormalizedLandmark& lm : landmarks.landmark()) {
// 				float x = lm.x();
// 				float y = lm.y();
// 				MP_PushBuf(lbuf, x, y, lm.z());
// 				cv::circle(frame_bgr, cv::Point(x, y), 1, cv::Scalar(0, 255, 0), -1);
// 			}

// 			// Invoke callback on landmark buffer.
// 			func((size_t)i, n, lbuf);
// 		}

// 		// Finished processing landmarks.
// 		after_processing:
// 		cv::imshow("Video Capture", frame_bgr);

// 		// Close on any keypress
// 		if (cv::waitKey(1) >= 0) {
// 			break;
// 		}
// 	}

// 	cv::destroyAllWindows();
// 	return true;
// }