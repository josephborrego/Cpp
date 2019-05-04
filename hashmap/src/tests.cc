// CSE 303 Assignment #1
// Spring 2018
//
// Description: This file implements a function 'run_custom_tests' that should be able to use
// the configuration information to drive tests that evaluate the correctness
// and performance of the map_t object.

#include <iostream>
#include <ctime>
#include <thread>
#include "config_t.h"
#include "tests.h"

#include "simplemap.h"

static time_t exec_time_i;

	void printer(int k, float v) {
			std::cout<<"<"<<k<<","<<v<<">"<< std::endl;
	}
	//pass by reference, so you modify the object
	void deposit(simplemap_t<int, float> &map)
	{
		int acc1 = rand() % 1024 + 1;
		int acc2 = rand() % 1024 + 1;
		if(acc1 == acc2)
		{
			deposit(map);
		}
		const float amt = (rand() % 95) / 100.f;
		float balance1 = map.lookup(acc1).first;
		float balance2 = map.lookup(acc2).first;
		if(balance1 >= amt)
		{
			map.update(acc1, balance1-amt);
			map.update(acc2, balance2+amt);
		}
		else
		{
			printf("not enough money \n");
			return;
		}
	}

	void balancehelper(int k, float v)
	{
		static float balance = 0.f;
		balance+=v;
	}

	void balance(simplemap_t<int, float> &map)
	{
		float balance = 0;
		map.apply(balancehelper);
	}

	void do_work(config_t& cfg, simplemap_t<int, float> &map)
	{
		timespec ts;
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
		auto inittime = ts.tv_sec;
		for(int i = 0; i < cfg.iters; i++)
		{
			//turnary operator
			((rand() % 101) <= 80) ? deposit(map) : balance(map);
		}
		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ts);
		exec_time_i = ts.tv_sec - inittime;
	}

	void run_custom_tests(config_t& cfg) {
		// Step 1
		// Define a simplemap_t of types <int,float>
		// this map represents a collection of bank accounts:
		// each account has a unique ID of type int;
		// each account has an amount of fund of type float.
        simplemap_t<int,float> map = simplemap_t<int,float>();        // map for bank accounts

		// Step 2
		// Populate the entire map with the 'insert' function
		// Initialize the map in a way the sum of the amounts of
		// all the accounts in the map is 1000
		srand(time(NULL));
		//assume 1024 accounts
		const float raw = 10024/1000.f;
		for(auto i = 1; i < 1025; i++)
		{
			map.insert(i, raw);
		}
		// Step 3
		// Define a function "deposit" that selects two random bank accounts
		// and an amount. This amount is subtracted from the amount
		// of the first account and summed to the amount of the second
		// account. In practice, give two accounts B1 and B2, and a value V,
		// the function performs B1-=V and B2+=V.
    	deposit(map);

		// Step 4
		// Define a function "balance" that sums the amount of all the
		// bank accounts in the map.

		balance(map);
		// Step 5
		// Define a function 'do_work', which has a for-loop that
		// iterates for config_t.iters times. Each iteration, the function should
		// call the function 'deposit' with 80% of the probability;
		// otherwise (the rest 20%) the function 'balance' should be called.
		// The function 'do_work' should measure 'exec_time_i', which is the
		// time needed to perform the entire for-loop.
		// The main thread should be able to access this amount of time once the
        // thread executing the 'do_work' joins its execution with the main thread.
		do_work(cfg, map);

		// Step 6
		// The evaluation should be performed in the following way:
		// - the main thread creates one thread
		//   << use std:threads >>
		// - the spawned thread executes the function 'do_work' until completion
		// - the (main) thread waits for the spawned thread to be executed
		//   << use std::thread::join() >>
		//	 and collects the 'exec_time_i' from the spawned thread
		// - once the spawned thread has joined, the function "balance" must be called

		std::thread t1(do_work, std::ref(cfg), std::ref(map));
		t1.join();
		balance(map);
		// WHAT IS THE OUTPUT OF this call of "balance"?
		// DOES IT MATCH WHAT YOU EXPECT?
		// WHAT DO YOU EXPECT?
		// WHAT ARE THE OUTCOMES OF ALL THE "balance" CALLS DURING THE EXECUTION?
		// IS THAT WHAT YOU EXPECT?

		// Step 7
		// Remove all the items in the map by leveraging the 'remove' function of the map
		// Execution terminates.
		// If you reach this stage happy, then you did a good job!

		map.removeall();

		// You might need the following function to print the entire map.
		// Attention if you use it while multiple threads are operating
	//	map.apply(printer);


	}

void test_driver(config_t &cfg) {
	run_custom_tests(cfg);
}
