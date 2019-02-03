#include "PersonGraph.hpp"
#include <string>
#include <string.h>

using namespace std;

int main(int argc, char *argv[]) {
  string exe = argv[0];
  string usage = exe + " called with incorrect arguments.\n" + 
                 "Usage: " + exe + " movie_cast_tsv_file actor_list actor_existing_predictions actor_new_predictions"; 
  
  if (argc != 5) {
    cerr << usage << endl; 
    exit(0); 
  }

  char* rawDataFile = argv[1]; 
  char* actorFile = argv[2]; 
  char* exPredFile = argv[3]; 
  char* newPredFile = argv[4]; 

  PersonGraph * graph = new PersonGraph();
  // This graph should be unweighted
  graph->loadFromFile(rawDataFile, false); 

  ifstream infile(actorFile);
  ofstream outfile1(exPredFile);
  ofstream outfile2(newPredFile);
  string header = "Person1,Person2,Person3,Person4\n";
  outfile1 << header;
  outfile2 << header;

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
    if (record.size() != 1) {
      continue;
    }
    string actor(record[0]); 
    if (flag == false) {
      flag = true; 
      continue;
    }
    cout << "Computing predictions for (" << actor << ")" << endl;
    pair<string, string> p = graph->predictLinks(actor); 
    if (p.first.size() > 0 && p.second.size() > 0) {
      outfile1 << p.first << "\n";
      outfile2 << p.second << "\n"; 
    }
    else {
      cerr << "Failed to locate node '" << actor << "'"  << endl; 
      cerr << usage << endl; 
      return 0;
    }

  }
  if(graph != nullptr) 
    delete(graph);
  return 0;
}


