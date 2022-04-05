#include <fstream>
#include <iostream>
#include <sstream>
#include <omp.h>
#include <queue>
#include <string>
#include <vector>

#define NO_OF_THREADS 4

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
  bool discovered_serial;
  bool discovered_parallel;

  // Distance form root node chosen for BFS
  // d = 0 for root node
  short int d_serial;
  short int d_parallel;

  // Construct undiscovered node with
  // infinite(-1) distance form root
  Node() {
    id = counter++;
    discovered_serial = false;
    discovered_parallel = false;
    d_serial = -1;
    d_parallel = -1;
  }
};

// Class to encapsulate graph
class Graph {
public:
  // Number of nodes in the graph
  unsigned short int no_of_nodes;

  // Edge matrix vector
  vector<vector<bool>> edgeMatrix;

  // Node vector
  vector<Node> N;

  // Construct graph from dataset
  Graph(string filename) {
    ifstream dataset_stream(filename);
    string line;

    // Read number of nodes and initialize graph
    while (getline(dataset_stream, line)) {
      if (line[0] != '#') {
        no_of_nodes = stoi(line);
        break;
      }
    }

    // Initialize edgeMatrix
    edgeMatrix = vector<vector<bool>>(no_of_nodes, vector<bool>(no_of_nodes));

    // Read node pairs and add edge
    while (getline(dataset_stream, line)) {
      istringstream nodepair(line);
      string word;

      nodepair >> word;
      unsigned short int n1 = stoi(word);
      nodepair >> word;
      edgeMatrix[n1][stoi(word)] = true;
    }

    dataset_stream.close();

    // Initialize node vector
    N = vector<Node>(no_of_nodes);
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
    cout << "Node ID\t|Serial\t |Parallel" << endl;
    cout << "\t|Distance|Distance" << endl;
    cout << "--------|--------|--------" << endl;
    for (auto i : N) {
      cout << i.id << "\t|" << i.d_serial << "\t |" << i.d_parallel << endl;
    }
    cout << "--------|--------|--------" << endl;
  }

  // Serial BFS
  void BFS_serial(unsigned short int);

  // TODO Implement Parallel BFS
  void BFS_parallel(unsigned short int);
};

void Graph::BFS_serial(unsigned short int s_id) {
  // s is now pointer to source
  Node *s = &N[s_id];

  // BFS Queue
  queue<Node *> Q;

  // Distance of source will be 0
  s->d_serial = 0;

  // Source has been discovered
  s->discovered_serial = true;

  Q.push(s);

  // BFS algorithm
  while (!Q.empty()) {
    Node *u = Q.front();
    Q.pop();
    for (int v = 0; v < no_of_nodes; ++v) {
      if ((edgeMatrix[u->id][v]) && (!N[v].discovered_serial)) {
        N[v].discovered_serial = true;
        N[v].d_serial = u->d_serial + 1;
        Q.push(&N[v]);
      }
    }
    u->discovered_serial = true;
  }
}

void Graph::BFS_parallel(unsigned short int s_id) {
  // s is now pointer to source
  Node *s = &N[s_id];

  // BFS Queue
  queue<Node *> Q;

  // Distance of source will be 0
  s->d_parallel = 0;

  // Source has been discovered
  s->discovered_parallel = true;

  Q.push(s);

  // BFS algorithm
  while (!Q.empty()) {
    Node *u = Q.front();
    Q.pop();
    #pragma omp parallel for
    for (int v = 0; v < no_of_nodes; ++v) {
      if ((edgeMatrix[u->id][v]) && (!N[v].discovered_parallel)) {
        N[v].discovered_parallel = true;
        N[v].d_parallel = u->d_parallel + 1;

        Q.push(&N[v]);
      }
    }
    u->discovered_parallel = true;
  }
}

int main(void) {
  omp_set_num_threads(NO_OF_THREADS);

  Graph G("test.txt");

  G.BFS_serial(0);
  G.BFS_parallel(0);
  G.print_distance();
}
