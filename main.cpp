#include <iostream>
#include <queue>
#include <vector>
#include<string>
#include<fstream>
#include<sstream>

using namespace std;

// Class to encapsulate node
class Node {
public:
  // Each node has an integer ID corrosponding
  // to it's index in edgeMatrix
  short unsigned int id;

  // Static counter to assign default value to id
  inline static short unsigned int counter = 0;

  // Has node been discoverd by BFS
  bool discovered;

  // Distance form root node chosen for BFS
  // d = 0 for root node
  short int d;

  // Construct undiscovered node with
  // infinite(-1) distance form root
  Node() {
    id = counter++;
    discovered = false;
    d = -1;
  }
};

// Class to encapsulate graph
class Graph {
public:
  // Number of nodes in the graph
  unsigned short int no_of_nodes;

  // Edge matrix vector
  vector<vector<bool> > edgeMatrix;

  // Node vector
  vector<Node> N;

  // Construct trivial graph with n nodes
  Graph(int n) {
    no_of_nodes = n;

    // Initialize edgeMatrix
    edgeMatrix = vector<vector<bool> >(n, vector<bool>(n));

    // Initialize node vector
    N = vector<Node>(n);
  }

  // Add edge from node s to node d
  void add_edge(unsigned short int s, unsigned short int d) {
    edgeMatrix[s][d] = true;
  }

  // Print adjacency matrix
  void print_matrix() {
    cout << "Adjacency Matrix of the graph is: " << endl;
    for (auto i : edgeMatrix) {
      for (int j = 0; j < i.size() - 1; ++j) {
        cout << i[j] << ", ";
      }
      cout << i[i.size() - 1] << endl;
    }
  }

  // Print distance of all nodes after BFS
  void print_distance() {
    cout << "Node ID\t|Distance" << endl;
    cout << "--------|--------" << endl;
    for (auto i : N) {
      cout << i.id << "\t|" << i.d << endl;
    }
    cout << "--------|--------" << endl;
  }

  // Serial BFS
  void BFS_serial(unsigned short int);
};

void Graph::BFS_serial(unsigned short int s_id) {
  // s is now pointer to source
  Node *s = &N[s_id];

  // BFS Queue
  queue<Node *> Q;

  // Distance of source will be 0
  s->d = 0;
  // Source has been discovered
  s->discovered = true;

  Q.push(s);

  // BFS algorithm
  while (!Q.empty()) {
    Node *u = Q.front();
    Q.pop();
    for (int v = 0; v < no_of_nodes; ++v) {
      if ((edgeMatrix[u->id][v] != false) && (!N[v].discovered)) {
        N[v].discovered = true;
        N[v].d = u->d + 1;
        Q.push(&N[v]);
      }
    }
    u->discovered = true;
  }
}

int main(void) {

  ifstream fin; 
  string line; int n=0;
 
  // by default open mode = ios::in mode
  fin.open("p2p-Gnutella08.txt");

  // Execute a loop until EOF (End of File)
  while (fin) {
 
  // Read a Line from File
  getline(fin, line);
 
  // Print line in Console
  if (line[0] != '#')
    {
      n = stoi(line);
      cout << line << endl;
      break;
    }
  }

  Graph G(n);

  while (fin) {

    // Read a Line from File
    getline(fin, line);

    istringstream ss(line);
  
    string word; // for storing each word
  
    // Traverse through all words
    // while loop till we get 
    // strings to store in string word
    while (ss >> word) 
    {
        int n1 = stoi(word);
        ss >> word;
        int n2 = stoi(word);

        G.add_edge(n1, n2);
    }

  }
 
  // Close the file
  fin.close();

  G.BFS_serial(0);
  G.print_distance();
}
