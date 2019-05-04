// CSE 303 Assignment #2
// Spring 2018
//
// Description: This file parses command line options, outputs them, and
// then runs the test function.

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>
#include <fstream>

#include "config_t.h"
#include "tests.h"

using namespace std;

// Report on how to use the command line to configure this program
void usage() {
	std::cout
		<< "Command-Line Options:" << std::endl
		<< "  -n <string> : a phrase to help differentiate experiments" << std::endl
		<< "  -k <int>    : the upper maximum key value when choosing random keys" << std::endl
		<< "  -i <int>    : the number of iterations" << std::endl
		<< "  -h          : display this message and exit" << std::endl << std::endl;
	exit(0);
}

// Parse command line arguments using getopt()
void parseargs(int argc, char** argv, config_t& cfg) {
	// parse the command-line options
	int opt;
	while ((opt = getopt(argc, argv, "n:k:i:h")) != -1) {
		switch (opt) {
		case 'n': cfg.name = std::string(optarg); break;
		case 'k': cfg.key_max = atoi(optarg); break;
		case 'i': cfg.iters = atoi(optarg); break;
		case 'h': usage(); break;
			// case 'a': ;
		}
	}
}

// The main routine simply parses the arguments, dumps the arguments, populates the
int main(int argc, char** argv) {
	// get the configuration, print it
	config_t config;
	parseargs(argc, argv, config);
	config.dump();

	// crash if the iterations are negative
	assert(config.iters > 0);


	const int NUMACCOUNTS[]{ 50, 100, 1000 };
	const int NUMTHREADS[]{ 1, 2, 4, 8, 16 };

	for (auto i = 0; i < 5; i++)
	{
		std::thread* check = new std::thread[NUMTHREADS[i]]; //create an array of threads
		for (auto j = 0; j < 3; j++)
		{
			printf("\n[+] Test scenario: %d threads %d accounts...\n", NUMTHREADS[i], NUMACCOUNTS[j]);
			config.numaccounts = NUMACCOUNTS[j]; //set the number of accounts

			for (int g = i; g < NUMTHREADS[i]; g++)
				check[g] = std::thread(test_driver, std::ref(config)); //starting every thread

			for (int g = i; g < NUMTHREADS[i]; g++)
				check[g].join();
		}

		printf("[+] Test scenario finished...\n\n");
		delete[] check;
	}

	// launch the tests
	test_driver(config);
}

