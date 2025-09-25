// main.cpp
#include "crow.h"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([]()
     { return "Hello World!"; });

    CROW_ROUTE(app, "/json")
    ([]()
     {
        crow::json::wvalue x;
        x["message"] = "Hello";
        x["number"] = 42;
        return x; });

    app.port(8080).multithreaded().run();
}