#define UNIT_TESTING
#define CATCH_CONFIG_RUNNER
#include "catch/catch.hpp"
#include "easylogging/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main( int argc, char* const argv[] ) {

	el::Configurations conf("easylogging.conf");
	el::Loggers::reconfigureLogger("default", conf);

	int result = Catch::Session().run( argc, argv );
	return result;
}
