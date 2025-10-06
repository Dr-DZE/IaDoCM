// main.cpp
#include "crow.h"
#include "lab/lab01.hpp"

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

    app.port(8080).multithreaded().run();
}