#include "easylogging/easylogging++.h"
#include "engine.h"

INITIALIZE_EASYLOGGINGPP

int main()
{
	el::Configurations conf("easylogging.conf");
	el::Loggers::reconfigureLogger("default", conf);

	LOG(INFO) << "Snake wars started";

	Engine engine;
	engine.new_game();
	engine.loop();

	return 0;
}
