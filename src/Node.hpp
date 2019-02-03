#ifndef NODE_HPP
#define NODE_HPP
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <climits>
#include "Edge.hpp"
using namespace std;

class Node {

  public:
  string name;
  Node* prevPerson;
  Edge<Node>* prevFilm; 
  vector<Edge<Node>*> connections;
  bool visited;
  int dist;
  int freq = 0; // only for linkpredictor pt 1 
  int freq2 = 0; // only for linkpredictor pt 2 
  int numNeighbors; 
  
  Node(string n) {
    name = n;
    prevPerson = nullptr; 
    visited = false;
    dist = INT_MAX;
  } 

  int getNumN() {
    int numN = 0; 
    for(Edge<Node>* e: connections) {
      numN += e->getCastSize(); 
    }
    return numN;
  }

  ~Node() {
    if (prevPerson != nullptr)
	delete(prevPerson); 
  }

};

#endif  // NODE_HPP

