#ifndef LINKNODE_HPP
#define LINKNODE_HPP
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <climits>
#include "Edge.hpp"
using namespace std;

class LinkNode {

  public:
  string name;
  LinkNode* prevPerson;
  Edge<LinkNode>* prevFilm; 
  vector<Edge<LinkNode>*> connections;
  bool visited;
  int dist;
  
  LinkNode(string n) {
    name = n;
    prevPerson = nullptr; 
    visited = false;
    dist = INT_MAX;
  } 

  ~LinkNode() {
    if (prevFilm != nullptr) 
    	delete(prevFilm);
    if (prevPerson != nullptr)
	delete(prevPerson); 
  }

};

#endif  // LINKNODE_HPP

