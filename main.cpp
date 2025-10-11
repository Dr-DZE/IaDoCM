// main.cpp
#include "crow.h"
#include "lab/lab01.hpp"
#include "lab/lab_02.hpp"

int main()
{
    crow::SimpleApp app;
    crow::mustache::set_base("templates");

    CROW_ROUTE(app, "/")([]()
                         {
        crow::mustache::context ctx;
        auto rendered = crow::mustache::load("main.html").render(ctx);\
        return rendered; });

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

    CROW_ROUTE(app, "/api/lab02")([]() {
        auto devices = EnumeratePCIDevices();
        crow::json::wvalue json;
        for (size_t i = 0; i < devices.size(); ++i) {
            std::string vendor_full = devices[i].first;
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
            json[i]["dev_id"] = devices[i].second;
        }
        return json;
    });

    app.port(8080).multithreaded().run();
}