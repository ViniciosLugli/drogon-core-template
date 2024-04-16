#include "TestController.h"

void TestController::asyncHandleHttpRequest(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
	Json::Value json;
	json["message"] = "Hello, World!";
	auto resp = HttpResponse::newHttpJsonResponse(json);
	callback(resp);
}
