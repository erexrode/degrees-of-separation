// awardsceremony.cpp
#include "PersonGraph.hpp"
//#include <string>
//#include <string.h>

using namespace std;

struct {
  bool operator() (Vertex* v1, Vertex* v2) const {
    return v1->name < v2->name;
  }
} comp;

int main(int argc, char *argv[]) {
  string exe = argv[0];
  string usage = exe + " called with incorrect arguments.\n" + 
                 "Usage: " + exe + " movie_casts.tsv k guest_list.tsv"; 
  
  if (argc != 4) {
    cerr << usage << endl; 
    exit(0); 
  }

  char* rawDataFile = argv[1]; 
  char* output = argv[3]; 
  char* endPtr; 
  unsigned int k = strtol(argv[2], &endPtr, 10);
  
  cout << k << endl;
  PersonGraph * graph = new PersonGraph();
  ofstream outfile(output);
/*
  if(k == 0) {
    cout << "Invalid number " << rawDataFile << endl;
    graph->loadFromFile(rawDataFile, false); 

    outfile << "Person" << "\n"; 
    cout << "Preparing Invitees List" << endl; 

    auto itr = graph->aMap->begin(); 
    vector<string> guestList;
    while(itr != graph->aMap->end()) {
      guestList.push_back(itr->second->name);
      itr++; 
    } 
    sort(guestList.begin(), guestList.end()); 
    for(string s: guestList) {
      outfile << s << "\n";
    }
  }
*/
  //else {
    graph->loadFromFile(rawDataFile, false); 
    graph->populateVMap(); 

    outfile << "Person" << "\n"; 
    cout << "Preparing Invitees List" << endl; 

    vector<Vertex*>* guestList = new vector<Vertex*>(); 
    graph->genList(k, guestList); 
    sort(guestList->begin(), guestList->end(), comp);   

    for(Vertex* v: *guestList) {
      if(v != nullptr && v->neighbors.size() >= k)
	outfile << v->name << "\n"; 
    }
    if(guestList != nullptr){
      delete(guestList); 
    }
  //}

  if(graph != nullptr) 
    delete(graph);
  return 0;
  
}
