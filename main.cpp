#include "crow.h"
#include "lab/lab01.hpp"
#include "lab/lab_02.hpp"
#include "lab/lab04.hpp"
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <fstream>
#include <sstream>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    // The command now includes the full path, so no need to cd.
    std::string command_to_run = std::string(cmd) + " 2>&1";
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command_to_run.c_str(), "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

Camera camera;

int main()
{
    // Start the USB monitor in the background
    const char* usb_monitor_path = "D:\\lr1\\Hadsome_Jack\\usb_mon\\usb_monitor.exe";
    FILE* usb_monitor_pipe = _popen(usb_monitor_path, "r");
    if (!usb_monitor_pipe) {
        std::cerr << "Error: Failed to start usb_monitor.exe. Make sure the path is correct." << std::endl;
    }
    // We don't close the pipe here, to keep the process running.
    // Note: This is a simplified approach. For a production scenario,
    // using CreateProcess with more control would be better.

    crow::SimpleApp app;
    crow::mustache::set_base("templates");

    CROW_ROUTE(app, "/")([]()
                         {
        crow::mustache::context ctx;
        return crow::mustache::load("main.html").render(ctx); });

    CROW_ROUTE(app, "/loading")
        ([]() {
        crow::mustache::context ctx;
        return crow::mustache::load("loading.html").render(ctx);
    });

    CROW_ROUTE(app, "/loading2")
        ([]() {
        crow::mustache::context ctx;
        return crow::mustache::load("loading2.html").render(ctx);
    });

    CROW_ROUTE(app, "/lab01")([]()
                         {
        batteryMonitor battery;
        crow::mustache::context ctx;
        ctx["charge"] = battery.getCharge();
        ctx["powerMode"] = battery.getPowerMode();
        ctx["status"] = battery.getStatus();
        ctx["timeLeft"] = battery.getTimeLeft();
        ctx["eco"] = battery.isEco();
        auto rendered = crow::mustache::load("lab01.html").render(ctx);
        return rendered; });

    CROW_ROUTE(app, "/api/lab01")
        ([]() {
        batteryMonitor battery;
        crow::json::wvalue json;
        json["charge"] = battery.getCharge();
        json["powerMode"] = battery.getPowerMode();
        json["status"] = battery.getStatus();
        json["timeLeft"] = battery.getTimeLeft();
        json["eco"] = battery.isEco();
        json["batteryInfo"] = battery.getBatteryInfo();
        return json;
    });

    CROW_ROUTE(app, "/api/sleep")
        ([]() {
        batteryMonitor battery;
        crow::json::wvalue json;
        json["success"] = battery.sleep();
        return json;
    });

    CROW_ROUTE(app, "/api/hibernate")
        ([]() {
        batteryMonitor battery;
        crow::json::wvalue json;
        json["success"] = battery.hibernate();
        return json;
    });

    CROW_ROUTE(app, "/lab02")([]() {
        crow::mustache::context ctx;
        auto rendered = crow::mustache::load("lab02.html").render(ctx);
        return rendered;
    });

    CROW_ROUTE(app, "/api/lab04/start")
        ([]() {
        crow::json::wvalue json;
        if (camera.is_recording()) {
            json["success"] = false;
            json["error"] = "Recording is already in progress.";
        } else {
            if (!camera.start_recording()) {
                json["success"] = false;
                json["error"] = "Failed to start recording. Check server logs for details.";
            } else {
                json["success"] = true;
                json["properties"] = camera.get_camera_properties();
            }
        }
        return json;
    });

    CROW_ROUTE(app, "/api/lab04/stop")
        ([]() {
        crow::json::wvalue json;
        if (!camera.is_recording()) {
            json["success"] = false;
            json["error"] = "No recording is in progress.";
        } else {
            std::string filepath = camera.stop_recording();
            if (filepath.empty()) {
                json["success"] = false;
                json["error"] = "Failed to stop recording or file not saved.";
            } else {
                json["success"] = true;
                json["filepath"] = "/" + filepath;
            }
        }
        return json;
    });


    CROW_ROUTE(app, "/api/lab02")([]() {
        auto devices = EnumeratePCIDevices();
        crow::json::wvalue json;
        for (size_t i = 0; i < devices.size(); ++i) {
            std::string vendor_full = std::get<0>(devices[i]);
            std::string ven_id = "";
            std::string vendor_name = "";
            size_t start_pos = vendor_full.find("[");
            size_t end_pos = vendor_full.find("]");
            if (start_pos != std::string::npos && end_pos != std::string::npos) {
                ven_id = vendor_full.substr(start_pos + 1, end_pos - start_pos - 1);
                vendor_name = vendor_full.substr(end_pos + 2);
            }

            json[i]["vendor_name"] = vendor_name;
            json[i]["ven_id"] = ven_id;
            json[i]["dev_id"] = std::get<1>(devices[i]);
            std::string device_name = std::get<2>(devices[i]);
            size_t dev_pos = device_name.find("(DEV_");
            if (dev_pos != std::string::npos) {
                device_name = device_name.substr(0, dev_pos);
            }
            json[i]["device_name"] = device_name;
        }
                                return json;
                            });

    CROW_ROUTE(app, "/lab05")([]() 
                         {
        crow::mustache::context ctx;
        return crow::mustache::load("lab05.html").render(ctx); });

    CROW_ROUTE(app, "/api/lab05/logs")([]() {
        std::ifstream log_file("D:\\lr1\\Hadsome_Jack\\usb_log.txt");
        if (!log_file.is_open()) {
            return crow::response(500, "Log file not found. Is usb_monitor.exe running?");
        }
        std::stringstream buffer;
        buffer << log_file.rdbuf();
        return crow::response(buffer.str());
    });

    CROW_ROUTE(app, "/api/lab05/devices")([]() {
        try {
            std::string result = exec("D:\\lr1\\Hadsome_Jack\\usb_mon\\usb_monitor.exe --list-drives-json");
            // Check if the result is a valid JSON array, otherwise it's an error
            if (result.rfind('[', 0) == 0) {
                return crow::response(200, "application/json", result);
            }
            return crow::response(500, result);
        } catch (const std::runtime_error& e) {
            return crow::response(500, e.what());
        }
    });

    CROW_ROUTE(app, "/api/lab05/eject/<string>").methods("POST"_method)([](const std::string& drive_letter) {
        if (drive_letter.length() != 1 || !isalpha(drive_letter[0])) {
            return crow::response(400, "Invalid drive letter.");
        }
        std::string command = "D:\\lr1\\Hadsome_Jack\\usb_mon\\usb_monitor.exe --eject " + drive_letter;
        try {
            std::string result = exec(command.c_str());
            if (result.find("failed") != std::string::npos) {
                return crow::response(500, result);
            }
            crow::json::wvalue json;
            json["output"] = result;
            return crow::response(json);
        } catch (const std::runtime_error& e) {
            return crow::response(500, e.what());
        }
    });

    CROW_ROUTE(app, "/api/lab05/disable-mouse").methods("POST"_method)([]() {
        try {
            std::string result = exec("D:\\lr1\\Hadsome_Jack\\usb_mon\\usb_monitor.exe --disable-hid-mouse");
            if (result.find("failed") != std::string::npos) {
                return crow::response(500, result);
            }
            crow::json::wvalue json;
            json["output"] = result;
            return crow::response(json);
        } catch (const std::runtime_error& e) {
            return crow::response(500, e.what());
        }
    });
                        
                            CROW_ROUTE(app, "/lab04")([]() {
                                crow::mustache::context ctx;
                                auto rendered = crow::mustache::load("lab04.html").render(ctx);
                                return rendered;
                            });
                        
                            app.port(8080).multithreaded().run();
                        }