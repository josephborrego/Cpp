/*
CSE 109: Spring 2017
<Joseph Borrego>
<Job218>
<HashTable>
Program #8
*/

//if not defined, define a file of these things
#ifndef NODE_H_
#define NODE_H_

class Node
{
public:
  //Node();
  //~Node();
  struct Node* nextNode;
  void* data;
  size_t dataSize;

  // bool operator!=(Node n) { return false; }
  // bool operator==(Node n) { return true; }
};



#endif


