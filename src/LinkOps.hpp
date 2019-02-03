#include <stdio.h>
#include <string>
#include <string.h>
#include <cstdlib>
#include <map>
#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm> 
#include "PersonGraph.hpp"
#include "Node.hpp"
#include "Edge.hpp"

using namespace std;

class LinkOps {

  public: 
    
    string predictLinks(unordered_map<string, Node*>* aMap, string actor);

}; 
