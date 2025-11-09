#ifndef LAB04_CAMERA_H
#define LAB04_CAMERA_H

#include <string>
#include <thread>
#include <atomic>
#include <opencv2/videoio.hpp>
#include "crow/json.h"

class Camera {
public:
    Camera();
    ~Camera();

    bool start_recording();

    std::string stop_recording();

    bool is_recording() const;

    crow::json::wvalue get_camera_properties() const;

private:
    void write_loop();
    void grab_loop();

    std::thread m_write_thread;
    std::thread m_grab_thread;

    std::atomic<bool> m_is_recording;
    std::atomic<bool> m_is_capturing;

    cv::Mat m_shared_frame;
    std::mutex m_frame_mutex;

    std::string m_output_filepath;
    double m_fps;
    int m_frame_width;
    int m_frame_height;
    
    cv::VideoCapture m_cap;
    cv::VideoWriter m_writer;
};

#endif // LAB04_CAMERA_H