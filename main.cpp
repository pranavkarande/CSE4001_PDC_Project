#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// Class to encapsulate node
class node {
public:
  // Has node been discoverd by BFS
  bool discovered;

  // Distance form root node chosen for BFS
  // d = 0 for root node
  short int d;

  // Construct undiscovered node with
  // infinite(-1) distance form root
  node() {
    discovered = false;
    d = -1;
  }
};

// Class to encapsulate graph
class graph {
public:
  // Number of nodes in the graph
  unsigned short int no_of_nodes;

  // Edge matrix vector
  vector<vector<unsigned short int>> edgeMatrix;

  // Node vector
  vector<node> N;

  // Construct trivial graph with n nodes
  graph(int n) {
    no_of_nodes = n;

    // Initialize edgeMatrix
    edgeMatrix =
        vector<vector<unsigned short int>>(n, vector<unsigned short int>(n));

    // Initialize node vector
    N = vector<node>(n);
  }

  // Add edge from node s to node d with weight w
  void add_edge(unsigned short int s, unsigned short int d,
                unsigned short int w) {
    edgeMatrix[s][d] = w;
  }

  // Print graph matrix
  void print_matrix() {
    cout << "Adjacency Matrix of the graph is: " << endl;
    for (auto i : edgeMatrix) {
      for (int j = 0; j < i.size() - 1; ++j) {
        cout << i[j] << ", ";
      }
      cout << i[i.size() - 1] << endl;
    }
  }

  // Serial BFS
  void BFS_serial(node &);
};

// TODO Implement serial BFS
void graph::BFS_serial(node &s) {
  // BFS Queue
  queue<node> Q;

  // Distance of source will be 0
  s.d = 0;
  // Source has been discovered
  s.discovered = true;

  Q.push(s);

  while (!Q.empty()) {
  }
}

int main(void) {
  graph G(10);

  G.add_edge(0, 3, 2);
  G.add_edge(4, 9, 8);
  G.add_edge(2, 4, 4);
  G.add_edge(5, 2, 2);
  G.add_edge(2, 6, 1);

  G.print_matrix();
}
