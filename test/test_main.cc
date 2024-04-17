#define DROGON_TEST_MAIN
#include <drogon/drogon.h>
#include <drogon/drogon_test.h>

int sum_all(int n) {
	int result = 0;
	for (int i = 1; i <= n; i++) {
		result += i;
	}
	return result;
}

DROGON_TEST(Sum) {
	CHECK(sum_all(1) == 1);
	CHECK(sum_all(2) == 3);
	CHECK(sum_all(3) == 6);
}

int main(int argc, char** argv) {
	using namespace drogon;

	std::promise<void> p1;
	std::future<void> f1 = p1.get_future();

	// Start the main loop on another thread
	std::thread thr([&]() {
		// Queues the promise to be fulfilled after starting the loop
		app().getLoop()->queueInLoop([&p1]() { p1.set_value(); });
		app().run();
	});

	// The future is only satisfied after the event loop started
	f1.get();
	int status = test::run(argc, argv);

	// Ask the event loop to shutdown and wait
	app().getLoop()->queueInLoop([]() { app().quit(); });
	thr.join();
	return status;
}
