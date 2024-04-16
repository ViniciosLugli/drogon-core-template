#pragma once

#include <drogon/HttpSimpleController.h>

using namespace drogon;

class TestController : public drogon::HttpSimpleController<TestController> {
public:
	void asyncHandleHttpRequest(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) override;
	PATH_LIST_BEGIN
	// list path definitions here;
	PATH_ADD("/", Get);

	PATH_LIST_END
};
