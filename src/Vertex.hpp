#ifndef VERTEX_HPP
#define VERTEX_HPP
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <climits>
#include "Edge.hpp"
using namespace std;

class Vertex {

  public:
  string name;
  set<Vertex*> neighbors;
  bool alive;
  //unsigned int deg;

  Vertex(string n) {
    name = n;
    alive = true; 
  }

  //Vertex(Vertex* v, unsigned int degree) {
  Vertex(Vertex* v) {
    name = v->name;
    neighbors = v->neighbors;
    //deg = degree; 
    alive = true; 
  }
  // TODO need destructor?
};

#endif  // VERTEX_HPP

