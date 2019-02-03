/*
 * PersonGraph.cpp
 */
#include "PersonGraph.hpp"
using namespace std;

PersonGraph::PersonGraph() {
  aMap = new unordered_map<string, Node*>();
  fMap = new unordered_map<string, Edge<Node>*>();  
  called = false;
}

// Overloading of compare function for priority queue of Node* 
class compareNode {
  public:
  bool operator() (Node* n1, Node* n2) const {
    if (n1->dist <= n2->dist) {
      if (n1->dist == n2->dist) {
        return n1->name <= n2->name; 
      }
      else {
        return false;
      }
    }
    return true; 
  }
};

// Overloading of compare function for priority queue of Vector* 
class compareVertex {
  public:
  bool operator() (Vertex* v1, Vertex* v2) const {
    // If v1's neighbor set is smaller than v2's, v1 should have higher priority
    // eg. get popped off first  
    return v1->neighbors.size() > v2->neighbors.size(); 
    //return v1->deg > v2->deg; 
  }
};

bool PersonGraph::loadFromFile(const char *in_filename, const bool use_weighted_edges) { 
  int nodeCount = 0;
  int movieCount = 0; 
  int edgeCount = 0; 
  // Initialize the file stream
  ifstream infile(in_filename);

  bool have_header = false;
  cout << "Reading " << in_filename << "..." << endl; 

  // keep reading lines until the end of file is reached
  while (infile) {
    string s;
    // get the next line
    if (!getline(infile, s)) break;

    if (!have_header) {
      // skip the header
      have_header = true;
      continue;
    }

    istringstream ss(s);
    vector<string> record;

    while (ss) {
      string next;

      // get the next string before hitting a tab character and put it in 'next'
      if (!getline(ss, next, '\t')) break;

      record.push_back(next);
    }

    if (record.size() != 3) {
      // we should have exactly 3 columns
      continue;
    }

    string actor_name(record[0]);
    string movie_title(record[1]);
    int movie_year = stoi(record[2]);
    
    string uniqueFilm = movie_title + to_string(movie_year); 
    bool useW = use_weighted_edges;      

    // PART 1: BUILD THE GRAPH
    auto ptrA = aMap->find(actor_name);
    auto ptrF = fMap->find(uniqueFilm);
    
    edgeCount++; 
    if(ptrA == aMap->end()) { 
      // CASE 1: Person/Film Do Not Exist 
      if(ptrF == fMap->end()) { 
        Node * actor; 
        Edge<Node>* film; 
        actor = new Node(actor_name); 
        nodeCount++; 
        film = new Edge<Node>(movie_title, movie_year, useW);
        movieCount ++; 
        aMap->insert(make_pair(actor_name, actor)); 
        fMap->insert(make_pair(uniqueFilm, film));  
        actor->connections.push_back(film);
        film->film_cast.push_back(actor);  
      }
      // CASE 2: Person does not exist, Film exists  
      else {
        Node* actor = new Node(actor_name); 
        nodeCount++; 
        aMap->insert(make_pair(actor_name, actor)); 
        actor->connections.push_back(ptrF->second);
        ptrF->second->film_cast.push_back(actor); 
      }
    }    
    else {
      // CASE 3: Person exists, Film does not exist
      if(ptrF == fMap->end()) {
        Edge<Node>* film = new Edge<Node>(movie_title, movie_year, useW);
        movieCount ++; 
        fMap->insert(make_pair(uniqueFilm, film));  
        ptrA->second->connections.push_back(film);
        film->film_cast.push_back(ptrA->second); 
      }
      // CASE 4: Person/Film Both Exist 
      else {
        ptrA->second->connections.push_back(ptrF->second);
        ptrF->second->film_cast.push_back(ptrA->second); 
      }   
    }
  }
  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
    return false;
  }
  infile.close();
  cout << "#nodes: " << nodeCount << endl; 
  cout << "#movies: " << movieCount << endl; 
  cout << "#edges: " << edgeCount << endl; 
  cout << "done" << endl;
  return true;
}

// Void method called in awardsceremony to populate vMap of PersonGraph
void PersonGraph::populateVMap() {
  vMap = new unordered_map<string, Vertex*>(); 
  // Create Vertex objects and add them to vMap
  auto itr = aMap->begin();
  while(itr != aMap->end()) {
    Node* n = itr->second;
    // If actor DNE in vMap
    auto ptr = vMap->find(n->name); 
    Vertex* v; 
    if(ptr == vMap->end()) {
      v = new Vertex(n->name); 
      vMap->insert(make_pair(v->name, v)); 
    }
    // If actor does exist in vMap
    else {
      //cout << "GOT HERE!!" << endl;
      v = ptr->second; 
      if(v->neighbors.size() > 0) {
        itr++;
        continue; 
      }
    }
    // Add all neighbors of v to set 
    for(Edge<Node>* edge: n->connections) {
      for(Node* neighbor: edge->film_cast) {
        // Skip if neighbor is actor themselves
        if(neighbor->name == n->name)
          continue; 
        // Check if this neighbor has been made yet, if not 
        auto locP = vMap->find(neighbor->name);
        if(locP == vMap->end()) {
          // Create vertex for neighbor 
          Vertex* locV = new Vertex(neighbor->name); 
          vMap->insert(make_pair(locV->name, locV)); 
          // Add new neighbor to v's neighbor set
          v->neighbors.insert(locV);
        }
        // If neighbor did exist 
        else {
          Vertex* locV = locP->second;
          // Add new neighbor to v's neighbor set
          v->neighbors.insert(locV); 
        }
      }
    }
    itr++;
  }
  // vMap should now be populated with all neighbor set's initialized
}

// Dijkstra implementation of PersonGraph
string PersonGraph::dijkstra(string actor1, string actor2) {
  string path = ""; 
  cout << "Computing path for " + actor1 + " -> " + actor2 << endl;
  priority_queue<Node*, vector<Node*>, compareNode> explorePersons;
  if (aMap->find(actor1) == aMap->end()) {
    cerr << "Failed to locate node " <<  "'" << actor1 << "'" << endl;
    return path;
  }
  if (aMap->find(actor2) == aMap->end()) {
    cerr << "Failed to locate node " <<  "'" << actor2 << "'" << endl;
    return path; 
  }
  Node* first = aMap->find(actor1)->second;
  first->dist = 0;
  explorePersons.push(first);
  while(!explorePersons.empty()){
    Node* v = explorePersons.top();
    explorePersons.pop();
    v->visited = true;
    
    for(unsigned int i = 0; i < v->connections.size(); i++){
      for(unsigned int j = 0; j < v->connections[i]->film_cast.size(); j++ ){
	Node* w  = v->connections[i]->film_cast[j];
        int c = v->dist + v->connections[i]->weight; 
        if (w->visited == false && w->name != v->name && c < w->dist) {
          w->prevPerson = v;
	  w->prevFilm = v->connections[i]; 
          w->dist = c;
          explorePersons.push(w); 
        }
      }
    }
  } 
  path = getPath(actor2);

  // reset nodes 
  auto itr = aMap->begin(); 
  auto itrEnd = aMap->end();
  while(itr != itrEnd) {
    if(itr->second != nullptr){
      itr->second->dist = INT_MAX;
      itr->second->visited = false;
      itr->second->prevPerson = nullptr; 
      itr->second->prevFilm = nullptr; 
    }
    itr++;
  } 
  return path; 
}


// Implementation of BFS on our Person Graph 
string PersonGraph::shortestPath(string actor1, string actor2) {
  string path = ""; 
  cout << "Computing path for " + actor1 + " -> " + actor2 << endl;
  queue<Node*> explorePersons;
  if (aMap->find(actor1) == aMap->end()) {
    cerr << "Failed to locate node " <<  "'" << actor1 << "'" << endl;
    return path;
  }
  if (aMap->find(actor2) == aMap->end()) {
    cerr << "Failed to locate node " <<  "'" << actor2 << "'" << endl;
    return path; 
  }
  Node* first = aMap->find(actor1)->second;
  first->dist = 0;
  explorePersons.push(first);
  while(!explorePersons.empty()){
    Node* next = explorePersons.front();
    explorePersons.pop();
    next->visited = true;
    
    for(unsigned int i = 0; i < next->connections.size(); i++){
      for(unsigned int j = 0; j < next->connections[i]->film_cast.size(); j++ ){
	Node* neighbor  = next->connections[i]->film_cast[j];
	if(neighbor->visited == false){
	  neighbor->visited = true;
	  if(next->dist + 1 <  neighbor->dist) {
	    neighbor->dist = next->dist + 1;
	    neighbor->prevPerson = next;
	    neighbor->prevFilm = next->connections[i]; 
	    explorePersons.push(neighbor);
	    if(neighbor->name == aMap->find(actor2)->second->name){
	      break;
	    }
	  }
	}
      }
    }
  } 
  path = getPath(actor2);

  // reset nodes 
  auto itr = aMap->begin(); 
  auto itrEnd = aMap->end();
  while(itr != itrEnd) {
    if(itr->second != nullptr){
      itr->second->dist = INT_MAX;
      itr->second->visited = false;
      itr->second->prevPerson = nullptr; 
      itr->second->prevFilm = nullptr; 
    }
    itr++;
  } 
  return path; 
}

string PersonGraph::getPath(string actor2) {
  string path = ""; 
  Node * curr = aMap->find(actor2)->second; 
  vector<string> flip; 
  flip.push_back("(" + actor2 + ")"); 
  while (curr->prevPerson != nullptr) {
    string film = "[" + curr->prevFilm->film + "#@" + to_string(curr->prevFilm->year) + "]";
    flip.push_back(film);
    string actor = "(" +  curr->prevPerson->name + ")"; 
    flip.push_back(actor); 
    curr = curr->prevPerson; 
  }
  int count = 0; 
  for(int i = flip.size() - 1; i >= 0; i--) {
    if (count % 2 == 0) {
      if ((unsigned int)i != flip.size() - 1) 
        path = path + ">" + flip.at(i); 
      else 
        path = path + flip.at(i); 
    }
    else {
      path = path + flip.at(i);
    }
    if(i != 0)
      path = path + "--"; 
    count++;
  }
  return path; 
}

// PART 2, METHOD: predictLinks (compare for vector sort is overloaded in header file)
pair<string, string> PersonGraph::predictLinks(string actor) {

  // set of actor's neighbors (for intersection)
  set<Node*> neighbors;  
  // map of actor's neighbors (for modifications)
  unordered_map<string, Node*> neighborPtrs; 
  string predictions = ""; 
  string newPredictions = ""; 
  // Ensure actor exists
  auto ptr = aMap->find(actor); 
  if(ptr == aMap->end()) {
    return pair<string, string>(predictions, newPredictions);
  }

  Node* actorNode = ptr->second;
  // Populate set and map of actor's neighbors
  for(unsigned int i = 0; i < actorNode->connections.size(); i++) {
    for(unsigned int j = 0; j < actorNode->connections[i]->film_cast.size(); j++) {
      Node* neighbor = actorNode->connections[i]->film_cast[j];
      if(neighbor->name != actor && 
         neighborPtrs.find(neighbor->name) == neighborPtrs.end()) { 
        neighbors.insert(neighbor);
        neighborPtrs.insert(make_pair(neighbor->name, neighbor));
      }
    }
  }

  // For each of actor's neighbors 
  auto itr = neighborPtrs.begin(); 
  set<Node*> newConnections;
  unordered_map<string, Node*> newNPtrs;

  while(itr != neighborPtrs.end()) {
    set<Node*> locNeighbors;
    for(Edge<Node>* e: itr->second->connections) {
      // For each of this neighbor's neighbors 
      for(Node *n: e->film_cast) {
        if(n->name == actor || n->name == itr->second->name) 
          continue; 
        locNeighbors.insert(n); 
        // If neighbor's neighbor is not a neighbor of actor, add to this set; also if is not already in the set 
        if(neighbors.find(n) == neighbors.end() && 
           newNPtrs.find(n->name) == newNPtrs.end()) {
          newConnections.insert(n);
          newNPtrs.insert(make_pair(n->name, n)); 
        }
      }
    } 
    vector<Node*> intersect;
    if (neighbors.size() > 0 && locNeighbors.size() > 0) { 
      set_intersection(neighbors.begin(), neighbors.end(), 
		       locNeighbors.begin(), locNeighbors.end(), 
                       back_inserter(intersect)); 
      itr->second->freq = intersect.size(); 
    }
    itr++;
  }
 
  vector<Node*> neighborVect(neighbors.begin(), neighbors.end()); 
  sort(neighborVect.begin(), neighborVect.end(), valueComp);   

  
  // Add all neigbors of neighbors that are also not neighbors of actor 
  auto itr2 = newNPtrs.begin(); 
  while(itr2 != newNPtrs.end()) {
    set<Node*> locN;
    for(Edge<Node>* e: itr2->second->connections) {
      for(Node *n: e->film_cast) {
        locN.insert(n);    
      }
    }
    vector<Node*> locIntersect;
    set_intersection(neighbors.begin(), neighbors.end(),
                     locN.begin(), locN.end(),
                     back_inserter(locIntersect));
    itr2->second->freq2 = locIntersect.size();
    itr2++;
  }
  vector<Node*> newConnectVect(newConnections.begin(), newConnections.end());
  sort(newConnectVect.begin(), newConnectVect.end(), valueComp2);  

  for (unsigned int i = 0; i < 4; i++) {
    if (i < neighborVect.size()) {
      predictions += neighborVect[i]->name;
      newPredictions += newConnectVect[i]->name;
      if (i != 3) {
        predictions += "\t"; 
        newPredictions += "\t"; 
      }
    }
  } 
  pair<string, string> p(predictions, newPredictions); 
  return p; 
} 

// Original Implementation
void PersonGraph::genList(unsigned int k, vector<Vertex*>* guestList) {
  
  called = true; 
  priority_queue<Vertex*, vector<Vertex*>, compareVertex> pq; 
  auto itr = vMap->begin(); 
  while(itr != vMap->end()) {
    pq.push(itr->second);      
    itr++;
  }

  while(pq.top()->neighbors.size() < k) {
    if(pq.top()->alive == false) {
      pq.pop();
      continue;
    }
    Vertex* v = pq.top();
    v->alive = false;
    pq.pop();
    for(Vertex* neighbor: v->neighbors) {
      if(neighbor->neighbors.size() >= v->neighbors.size()) {
	if(!(neighbor->alive))
	  continue; 
	if(neighbor->neighbors.erase(v))
	  continue; 
	neighbor->alive = false;
	if(neighbor->neighbors.size() >= k) 
	  pq.push(new Vertex(neighbor));
      }
    }
  }
  
  while(!(pq.empty())) { 
    Vertex* v = pq.top();
    if(v->alive)
      guestList->push_back(v);
    pq.pop(); 
  }
}

// Destructor 
PersonGraph::~PersonGraph() {
  auto itr1 = aMap->begin(); 
  while (itr1 != aMap->end()) {
    if (itr1->second != nullptr)
      delete(itr1->second);
    itr1++; 
  }
  
  auto itr2 = fMap->begin(); 
  while (itr2 != fMap->end()) {
    if (itr2->second != nullptr)
      delete(itr2->second);
    itr2++; 
  }
  if(called) {
    if(vMap != nullptr && !(vMap->empty())) {
      auto itr3 = vMap->begin(); 
      while (itr3 != vMap->end()) {
	if (itr3->second != nullptr)
	  delete(itr3->second);
	itr3++; 
      }
      delete(vMap);
    }
  }
  if (aMap != nullptr)
    delete(aMap); 
  if (fMap != nullptr)
    delete(fMap);
}


