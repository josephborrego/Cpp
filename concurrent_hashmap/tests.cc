// CSE 303 Assignment #3
// Spring 2018

#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <vector>
#include "config_t.h"
#include "tests.h"
#include "simplemap.h"

static double exec_time_i;

void printer(int k, float v) {
	std::cout << "<" << k << "," << v << ">" << std::endl;
}

//pass by reference, so you modify the object
void deposit(simplemap_t<int, float> &map)
{
	int acc1 = rand() % map.getmapsize();
	int acc2 = rand() % map.getmapsize();
	if (acc1 == acc2)
		return;

	auto keys = new std::vector<int>;
	keys->push_back(acc1);
	keys->push_back(acc2);
	//map.acquirelocks(*keys);
	map.acquirelock();
	FILE* sml = fopen("simplemaplog.log", "w");
	fputs("Lock\n", sml);

	//write "lock" to the file sml

	const float amt = (rand() % 50) / map.getmapsize();
	float balance1 = map.lookup(acc1).first;
	float balance2 = map.lookup(acc2).first;
	float store = balance1 - amt;
	float fap = balance1 + amt;
	if (balance1 >= amt)
	{
		map.update(acc1, store);
		fprintf(sml, "Update<%d, %f>\n", acc1, store);
		map.update(acc2, fap);
		fprintf(sml, "Update<%d, %f>\n", acc2, fap);
	}
	else
	{
		printf("not enough money \n");
	}

	//write unlock to the file sml
	fputs("Unlock\n", sml);
	fclose(sml);
	map.releaselock();
	//map.releaselocks(*keys);
	delete keys;
}

// Define a function "balance" that sums the amount of all the
// bank accounts in the map.
void balance(simplemap_t<int, float> &map)
{
	map.acquirelock();
	float total = 0.f;
	for (auto i = 0; i < map.getmapsize(); i++)
	{
		if (map.lookup(i).second)
		{
			total += map.lookup(i).first;
			//printf("added %f to total \n", map.lookup(i).first);
		}
	}
	printf("total balance %f\n", total);
	map.releaselock();
}

void do_work(config_t& cfg, simplemap_t<int, float> &map)
{
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < cfg.iters; i++)
	{
		//turnary operator
		((rand() % 101) <= 80) ? deposit(map) : balance(map);
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = end - start;
	exec_time_i = diff.count();

	printf("do_work exec_time_i %.8f\n", exec_time_i);
}

void run_custom_tests(config_t& cfg) {

	simplemap_t<int, float> map = simplemap_t<int, float>(cfg.numaccounts);        // map for bank accounts
	srand(time(NULL));
	const float raw = 1000.f / cfg.numaccounts;

	//insert(key, value)
	int index = 0;

//if there's nothing in the map here, then use the for loop
	FILE* smd = fopen("simplemapdisk.log", "r");
	//if(
	for (auto i = 0; i < cfg.numaccounts; i++)
	{
		map.insert(i, raw);
		fprintf(smd, "<%d%c%d%c%f>\n", index, 59, i, 59, raw);
		index++;
	}

//if there is a file already, then read from that file and insert from the file

	fclose(smd);
	do_work(cfg, map);
	map.removeall();
}

void test_driver(config_t &cfg) {
	run_custom_tests(cfg);
}
