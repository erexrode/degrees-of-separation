#include "PersonGraph.hpp"
#include <string>
#include <string.h>

using namespace std;

int main(int argc, char *argv[]) {
  string exe = argv[0];
  string usage = exe + " called with incorrect arguments.\n" + 
                 "Usage: " + exe + " movie_cast_tsv_file u/w pairs_tsv_file output_paths_tsv_file"; 
  
  if (argc != 5) {
    cerr << usage << endl; 
    exit(0); 
  }

  bool useW = false; 
  if (*argv[2] == 'u') {
   useW = false;
  }
  else if (*argv[2] == 'w') {
    useW = true; 
  }
  else { 
    cout << usage << endl;
    exit(0); 
  }

  char* rawDataFile = argv[1]; 
  char* actorFile = argv[3]; 
  char* pathFile = argv[4]; 
  PersonGraph * graph = new PersonGraph();   
  graph->loadFromFile(rawDataFile, (const bool) useW); 
  // Graph is now populated
  ifstream infile(actorFile);
  ofstream outFile(pathFile);
  string header = "(actor)--[movie#@year]-->(actor)--...\n";
  outFile << header;
  bool flag = false;  
  while (infile) {
    string s; 
    if(!getline(infile, s)) break; 
    istringstream ss(s); 
    vector<string> record; 
    while (ss) {
      string next; 
      // get the next string before hitting a tab character and put it in 'next'
      if (!getline(ss, next, '\t')) break;
      record.push_back(next);
    }
    if (record.size() != 2) {
      continue;
    }
    string actor1(record[0]); 
    string actor2(record[1]); 
    if (flag == false) {
      flag = true; 
      continue;
    }
    string path;
    if (useW) {
      path = graph->dijkstra(actor1, actor2);
    }
    else {
      path = graph->shortestPath(actor1, actor2);
    }

    if(path.size() > 0) {
      outFile << path << "\n";
    }
    else {
      cerr << usage << endl; 
      exit(0); 
    } 
  }
  delete(graph);
  return 0;
}

