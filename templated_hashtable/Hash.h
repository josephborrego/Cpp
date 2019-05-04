/*
CSE 109: Spring 2017
<Joseph Borrego>
<job218>
<Templated hash table>
Program #8
*/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include "Node.h"

using namespace std;

#ifndef Hash_h
#define Hash_h

int basic_hash(void* data, int datasize);

//int (*pBasic_hash)(void* thisptr, int a2);

template<typename T, int _size = 10, int (*func)(void*, int)=basic_hash, bool sort = false>
class Hash
{
public:
  static const int BASIC_HASH = 1;
  static const int SUMMATE_HASH = 2;
  // Hash(const Hash<T> &source);
  // Hash(const Hash<T> *source);
  Hash(const Hash<T, _size, func, sort> &source);
  
  // int(*func)(void*, int);
  Hash<T,_size,func,sort>& operator=(const Hash<T,_size,func,sort> &source);
  bool m_sort;
  bool find(T data, int) const;
  bool remove(T data, int);
  // bool insert(T data, int);
  string toString() const;
  int getCount() const;
  size_t size() const;
  // ostream& operator<<(ostream&, Hash<T>&);
  int m_size;
  mutable int opcount;
  Node **buckets;
  
  Hash()
  {
    m_size = 0;
    opcount = 0;
    buckets = new Node*[10];
    m_sort = false;
  }
  
  bool insert(T data, int datasize)
  {
    opcount++;
    int bucket = getIndex(data, datasize);
    if(sort)
      {
	if(buckets[bucket] == NULL)
	  {
	    Node* newNode = new Node();
	    newNode->data = (void*)&data;
	    memcpy(newNode->data,(const void*)& data, datasize);
	    newNode->dataSize = datasize;
	    newNode->nextNode = buckets[bucket];
	    buckets[bucket] = newNode;
	    return true;
	  }
	// printf("after null check\n");
	// checking if the first node matches the data that is being inserted
	// if(memcmp(buckets[bucket]->data, data, datasize) == 0)
	if(memcmp(buckets[bucket]->data,(const void*)& data, datasize) == 0)
	  {
	    return false;
	  }
	if(strcmp((const char*)buckets[bucket]->data, (const char*)data) > 0)
	  {
	  Node* newNode = new Node();
	  newNode->data = (void*)&data;
	  memcpy(newNode->data, (const void*)&data, datasize);
	  newNode->dataSize = datasize;
	  newNode->nextNode = buckets[bucket];
	  buckets[bucket] = newNode;
	  return true;
	  }
	//	printf("current node\n");
      Node currentNode = *buckets[bucket];
      while(currentNode.nextNode != NULL)
	{
	  if(memcmp(currentNode.data, (const void*)&data, datasize) == 0)
	    {
	      return false;
	    }
	  if(memcmp(currentNode.data, (const void*)&data, datasize) > 0)
	    {
	      Node* newNode = new Node();
	      newNode->data = (void*)&data;
	      memcpy(newNode->data, (const void*)&data, datasize);
	      newNode->dataSize = datasize;
	      currentNode.nextNode = newNode;
	      return true;
	    }
	  else
	    {
	      opcount++;
	    }
	  currentNode = (struct Node&)*currentNode.nextNode;
	}
      Node* newNode = new Node();
      newNode->data = (void*)&data;
      memcpy(newNode->data, (const void*)&data, datasize);
      newNode->dataSize = datasize;
      newNode->nextNode = NULL;
      currentNode.nextNode = newNode;
      return true;
      }
    else
      {
      Node* currentNode = buckets[bucket];
      if(currentNode == NULL)
	{
	  Node* newNode = new Node();
	  newNode->data = (void*)&data;
	  memcpy(newNode->data, (const void*)&data, datasize);
	  newNode->dataSize = datasize;
	  buckets[bucket] = newNode;
	  newNode->nextNode = NULL;
	  return true;
	}
      while(currentNode->nextNode != NULL)
	{
	  opcount++;
	  if(memcmp(currentNode->data, (const void*)&data, datasize) == 0)
	    {
	      return false;
	    }
	  currentNode = currentNode->nextNode;
	}
      Node* newNode = new Node();
      newNode->data = (void*)&data;
      memcpy(newNode->data, (const void*)&data, datasize);
      newNode->dataSize = datasize;
      newNode->nextNode = NULL;
      currentNode->nextNode = newNode;
      return true;
      }
  }
  
  
private:
  int getIndex(T data, int) const;
};

//template<typename T, int _size = 10, int (*func)(void*, int)=basic_hash, bool sort = false>
template<typename T, int _size, unsigned char* (*func)(void*, int), bool sort>
ostream& operator<<(ostream&, Hash<T,_size,func,sort>&);

#endif
