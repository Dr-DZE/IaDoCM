#include "lab04.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <opencv2/imgproc.hpp>

Camera::Camera() : m_is_recording(false), m_is_capturing(false), m_fps(30.0), m_frame_width(0), m_frame_height(0) {}

Camera::~Camera() {
    if (is_recording()) {
        stop_recording();
    }
}

bool Camera::is_recording() const {
    return m_is_recording;
}

bool Camera::start_recording() {
    if (is_recording()) {
        std::cerr << "Error: Recording is already in progress." << std::endl;
        return false;
    }

    m_cap.open(0, cv::CAP_DSHOW);
    if (!m_cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return false;
    }

    double camera_fps = m_cap.get(cv::CAP_PROP_FPS);
    if (camera_fps > 0) {
        m_fps = camera_fps;
    } else {
        std::cout << "Warning: Could not get camera FPS. Using default " << m_fps << std::endl;
    }

    m_frame_width = m_cap.get(cv::CAP_PROP_FRAME_WIDTH);
    m_frame_height = m_cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    if (m_frame_width <= 0 || m_frame_height <= 0) {
        m_frame_width = 640;
        m_frame_height = 480;
    }

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << "static/videos/rec_" << std::put_time(std::localtime(&in_time_t), "%Y%m%d_%H%M%S") << ".mp4";
    m_output_filepath = ss.str();

    std::cout << "Info: Opening VideoWriter with FPS: " << m_fps << std::endl;
    m_writer.open(m_output_filepath, cv::VideoWriter::fourcc('a', 'v', 'c', '1'), m_fps, cv::Size(m_frame_width, m_frame_height));
    if (!m_writer.isOpened()) {
        std::cerr << "Error: Could not open VideoWriter with avc1 in MP4 container." << std::endl;
        m_cap.release();
        return false;
    }   

    m_is_capturing = true;
    m_grab_thread = std::thread(&Camera::grab_loop, this);

    m_is_recording = true;
    m_write_thread = std::thread(&Camera::write_loop, this);

    std::cout << "Recording started. Output file: " << m_output_filepath << std::endl;
    return true;
}

std::string Camera::stop_recording() {
    if (!is_recording()) {
        return "";
    }

    m_is_recording = false;
    if (m_write_thread.joinable()) {
        m_write_thread.join();
    }

    m_is_capturing = false;
    if (m_grab_thread.joinable()) {
        m_grab_thread.join();
    }

    m_writer.release();
    m_cap.release();

    std::cout << "Recording stopped. File saved: " << m_output_filepath << std::endl;
    return m_output_filepath;
}

crow::json::wvalue Camera::get_camera_properties() const {
    crow::json::wvalue props;
    
    props["Resolution"] = std::to_string(m_frame_width) + "x" + std::to_string(m_frame_height);
    props["FPS"] = m_fps;

    props["Brightness"] = m_cap.get(cv::CAP_PROP_BRIGHTNESS);
    props["Contrast"] = m_cap.get(cv::CAP_PROP_CONTRAST);
    props["Saturation"] = m_cap.get(cv::CAP_PROP_SATURATION);
    props["Hue"] = m_cap.get(cv::CAP_PROP_HUE);
    props["Gain"] = m_cap.get(cv::CAP_PROP_GAIN);
    props["Exposure"] = m_cap.get(cv::CAP_PROP_EXPOSURE);

    return props;
}

void Camera::grab_loop() {
    cv::Mat local_frame;
    while (m_is_capturing) {
        if (!m_cap.read(local_frame) || local_frame.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        std::lock_guard<std::mutex> lock(m_frame_mutex);
        m_shared_frame = local_frame.clone();
    }
}

void Camera::write_loop() {
    cv::Mat local_frame;
    auto frame_duration = std::chrono::milliseconds(static_cast<long>(1000.0 / m_fps));

    while (m_is_recording) {
        {
            std::lock_guard<std::mutex> lock(m_frame_mutex);
            if (!m_shared_frame.empty()) {
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    std::cout << "Info: Writer thread has received the first frame. Starting to write." << std::endl;

    while (m_is_recording) {
        auto frame_start_time = std::chrono::steady_clock::now();

        {
            std::lock_guard<std::mutex> lock(m_frame_mutex);
            local_frame = m_shared_frame.clone();
        }

        if (!local_frame.empty()) {
            m_writer.write(local_frame);
        }

        auto frame_end_time = std::chrono::steady_clock::now();
        auto processing_time = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end_time - frame_start_time);

        if (processing_time < frame_duration) {
            std::this_thread::sleep_for(frame_duration - processing_time);
        }
    }
}
