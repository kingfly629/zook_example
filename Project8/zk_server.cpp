#include <string>
#include <iostream>
#include <string>
#include "ZKClient.h"

int main(int argc, char* argv[])
{
	try {
		if (argc < 4) {
			std::cout << "test zkurl path data" << std::endl;
			return 0;
		}

		ZKClient cli;
		cli.init(argv[1], 100);
		std::string path = argv[2];
		std::string data = argv[3];

		cli.createEphemeralNode(path, data);

		sleep(15);

	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}

