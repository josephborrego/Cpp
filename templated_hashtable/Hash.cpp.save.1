
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

using namespace std;

/*
template<typename T, int _size, int (*func)(void*, int), bool sort>
Hash<T,_size,func,sort>::Hash()
{
  _size = 0;
  opcount = 0;
  buckets = new Node *[10];
  sort = false;
}
*/

template<typename T, int _size, int (*func)(void*, int), bool sort>
Hash<T,_size,func,sort>::Hash(const Hash<T,_size,func,sort> &source)
{
  buckets = new Node* [source->_size];
  func = source->func;
  sort = source->sort;
  _size = source->_size;

  for(int i = 0; i < source->_size; i++)
    {
      Node* temp = source->buckets[i];
      buckets[i] = NULL;
      //accounting for first case
      if(temp == NULL)
	//checking for null
	{
	  buckets[i] = new Node*;
	  Node* currentNode = buckets[i];
	  //need to include datasize in node.h
	  memcpy(currentNode->data, temp->data, temp->dataSize);
	  currentNode->dataSize = temp->dataSize;
	  temp = temp->nextNode;
	  currentNode = currentNode->nextNode;
	}
      //looping to connect all the nodes
      while(temp != NULL)
	//checking for null
	{
	  Node* currentNode = buckets[i];
	  currentNode->nextNode = new Node;
	  memcpy(currentNode->data, temp->data, temp->dataSize);
	  currentNode->dataSize = temp->dataSize;
	  temp = temp->nextNode;
	  currentNode = currentNode->nextNode;
	  if(temp == NULL)
	    //checking for null
		{
	      currentNode = NULL;
	    }
	}
    }
}

/*
template<typename T, int _size, int (*func)(void*, int), bool sort>
Hash <T,_size,func,sort>::Hash(int numbuckets, int hashfx, bool insert)
{
  buckets = new Node*[numbuckets];
	//
  if(hashfx == w
BASIC_HASH)
  {
	func = &basic_hash;
  }
  else if(hashfx == SUMMATE_HASH)
  {
	//func = &summate_hash;
  }
  else
  {
	fprintf(stderr, "Invalid hashing function\n");
	exit(1);
  }
  sort = insert;
  _size = numbuckets;
//  return this;
}
*/

template<typename T, int _size, int (*func)(void*, int), bool sort>
bool Hash<T,_size,func,sort>::find(T data, int datasize) const
{
  opcount++;
  int bucket = getIndex(data, datasize);
  Node* currentNode = buckets[bucket];
  while(currentNode != NULL)
    {
      opcount++;
      if(memcmp(data, currentNode->data, datasize) == 0)
	{
	  return true;
	}
      currentNode = currentNode->nextNode;
    }
  return false;
}


template<typename T, int _size, int (*func)(void*, int), bool sort>
string Hash<T,_size,func,sort>::toString() const
{
  std::ostringstream os;

  for(int i = 0; i < _size; i++)
    {
      Node* temp = buckets[i];
      os << i << ": ";
      //accounting for first case
      if(temp != NULL)
        {
	  os << temp->data;
	  temp = temp->nextNode;
        }
      else
	{
	  os << "(Empty)";
	}
      //looping to connect all the nodes
      while(temp != NULL)
        {
	  os <<" " << temp->data;
	  temp = temp->nextNode;
        }
      os << endl;
    }
  return os.str();
}


template<typename T, int _size, int (*func)(void*, int), bool sort>
int Hash<T,_size,func,sort>::getCount() const
{
  return opcount;
}

template<typename T, int _size, int (*func)(void*, int), bool sort>
size_t Hash<T,_size,func,sort>::size() const
{
  size_t count = 0;
  for(int i = 0; i < _size; i++)
    {
      Node* temp = buckets[i];
      //accounting for first case
      if(temp != NULL)
        {
	  count++;
	  temp = temp->nextNode;
        }
      //looping to connect all the nodes
      while(temp != NULL)
        {
	  count++;
	  temp = temp->nextNode;
	    }
    }
  return count;
}

template<typename T, int _size, int (*func)(void*, int), bool sort>
ostream& operator<<(ostream &out, Hash<T,_size,func,sort>& thehash)
{
  //std::ostringstream out;
  out << thehash.toString() << endl;
  return out;
}


template<typename T, int _size, int (*func)(void*, int), bool sort>
int Hash<T,_size,func,sort>::getIndex(T data, int datasize) const
{
  //int num = getIndex(data, datasize);
  int num = func(data, datasize);
  int result =  num % _size;
  return result;
}

int basic_hash(void* data, int datasize)
{	
  SHA_CTX *c;
  unsigned char *md;
  SHA1_Init(c);
  SHA1_Update(c, data, datasize);
  SHA1_Final(md, c);
  
  return static_cast<int>(*md);
}
