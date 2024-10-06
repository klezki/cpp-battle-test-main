#include <iostream>
#include <fstream>
#include <RealSimulation.h>


int main(int argc, char** argv)
{
	using namespace sw;

	if (argc != 2) {
		throw std::runtime_error("Error: No file specified in command line argument");
	}

	std::ifstream file(argv[1]);
	if (!file) {
		throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
	}


	RealSimulation sim;
	io::CommandParser parser;
	
	sim.registerParser(parser);

	parser.parse(file);

	sim.run();

	return 0;
}
