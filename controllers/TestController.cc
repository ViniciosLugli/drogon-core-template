#include "TestController.h"

void TestController::asyncHandleHttpRequest(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
	// Create a JSON response
	Json::Value json;
	json["message"] = "Hello, World!";

	// Create a response object
	auto resp = HttpResponse::newHttpJsonResponse(json);
	callback(resp);
}
