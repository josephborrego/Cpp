/*
CSE 109: Spring 2017
<Joseph Borrego>
<job218>
<Templated hash table>
Program #8
*/

#include <stdio.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <cstring>
#include <stdexcept>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <string>
#include <iostream>
#include <sstream>
#include "Hash.h"

#include "Node.h"

//static int basic_hash(void*, int);

int main(int argc, char **argv)
{
  Hash<int>* h1 = new Hash<int>();
  auto h2 = new Hash<double, 24>();
  auto h3 = new Hash<std::string, 50>();
  auto h4 = new Hash<char, 99>();
  auto h5 = new Hash<int, 103, basic_hash, true>();
  
  return 0;
}
