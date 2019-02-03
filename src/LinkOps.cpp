// FILE: LinkOps.cpp
#include "LinkOps.hpp"

using namespace std;

struct {
  bool operator() (pair<string, int> p1, pair<string,int> p2) const {   
    if(p1.second >= p2.second) {
      if(p1.second == p2.second) {
        return p1.first <= p2.first;
      }
      return true;
    }
    return false;
  }   
} valueComp;



string LinkOps::predictLinks(unordered_map<string, Node*>* aMap, string actor) {
  unordered_map<string, int> neighbors;
  string predictions = ""; 
  auto ptr = aMap->find(actor); 
  if(ptr == aMap->end()) {
    return predictions;
  }
  Node* actorNode = ptr->second; 
  // For each film ACTOR is in 
  for(unsigned int i = 0; i < actorNode->connections.size(); i++) {
    // For each actor in each film that ACTOR is in 
    for(unsigned int j = 0; j < actorNode->connections[i]->film_cast.size(); j++) {
      Node * currNeighbor = actorNode->connections[i]->film_cast[j]; 
      // For each film each neighbor actor that is in 
      for(unsigned int k = 0; k < currNeighbor->connections.size(); k++){
        // For each actor in each film neighbor actor is in 
        for(unsigned int l = 0 ; l < currNeighbor->connections[k]->film_cast.size(); l++){
          Node * compPerson = currNeighbor->connections[k]->film_cast[l];
          vector<Node*> currVect = actorNode->connections[i]->film_cast;
          // If compPerson has been in films with actor and neighbor
          for(Node* n: currVect) {
	    if(compPerson->name != actor && compPerson->name != currNeighbor->name &&
	       n->name == compPerson->name) {
	      // Update frequency of existing neighbor 
	      auto ptr = neighbors.find(compPerson->name);
	      if(ptr != neighbors.end()) {
		ptr->second = ptr->second + 1; 
	      }
	      // Insert new neighbor 
	      else {
		neighbors.insert(pair<string, int>(compPerson->name, 1)); 
	      }
	    }
          }
        }       
      }      
    }
  }
  vector<pair<string, int>> neighborVect(neighbors.begin(), neighbors.end()); 
  sort(neighborVect.begin(), neighborVect.end(), valueComp); 
  
  for (int i = 0; i < 4; i++) {
    predictions += neighborVect[i].first;
    if (i != 3) 
      predictions += "\t"; 
  }
  return predictions; 
}

