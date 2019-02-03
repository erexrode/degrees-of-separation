#ifndef EDGE_HPP
#define EDGE_HPP
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std;

template<typename T>
class Edge {

  public:
  string film;
  int year;
  int weight;
  vector<T*> film_cast;
  bool weighted; 

  Edge(string f, int y, bool w){
    film = f;
    year = y;
    weighted = w;
    if (weighted) {
      weight = 1 + (2018 - year);
    }
    else {
      weight = 1;
    }
  }

  int getCastSize() {
    // return num elements in film_cast vector 
    return film_cast.size(); 
  }

};

#endif  // EDGE_HPP

