/*
 * PersonGraph.hpp
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <utility>
#include <queue>
#include <set>
#include <algorithm>
#include "Node.hpp"
#include "Edge.hpp"
#include "Vertex.hpp"

class PersonGraph {
 public:
   bool called;
   unordered_map<string, Node*> *aMap;
   unordered_map<string, Edge<Node>*> *fMap;

   // for awards ceremony 
   unordered_map<string, Vertex*> *vMap; 
   PersonGraph();

   bool loadFromFile(const char *in_filename, const bool use_weighted_edges);
   // PA3 Part 1
   string dijkstra(string actor1, string actor2);
   string shortestPath(string actor1, string actor2);
   string getPath(string actor2);
   // PA3 Part 2.1
   pair<string, string> predictLinks(string actor); 
   // PA3 Part 2.2
   void populateVMap(); 
   void genList(unsigned int k, vector<Vertex*>* guestList);
   ~PersonGraph(); 

// overloading comparator for standard vector sort in predicting connections 
struct {
  bool operator() (Node* n1, Node* n2) const {
    if(n1->freq >= n2->freq) {
      if(n1->freq == n2->freq) {
        return n1->name < n2->name; 
      }
      return true;
    }
    return false;
  }
} valueComp;

// overloading comparator for standard vector sort in predicting new connections 
struct {
  bool operator() (Node* n1, Node* n2) const {
    if(n1->freq2 >= n2->freq2) {
      if(n1->freq2 == n2->freq2) {
        return n1->name < n2->name; 
      }
      return true;
    }
    return false;
  }
} valueComp2;

};

#endif  // ACTORGRAPH_HPP
