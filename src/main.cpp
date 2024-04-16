#include <drogon/drogon.h>

int main() {
	drogon::app().addListener("0.0.0.0", 3000).run();
	return 0;
}