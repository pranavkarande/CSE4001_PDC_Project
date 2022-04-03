#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// Class to encapsulate node
class node {
public:
  // Each node has an integer ID corrosponding
  // to it's index in the edgeMatrix
  short unsigned int id;

  // Static counter to assign default value to id
  inline static short unsigned int counter;

  // Has node been discoverd by BFS
  bool discovered;

  // Distance form root node chosen for BFS
  // d = 0 for root node
  short int d;

  // Construct undiscovered node with
  // infinite(-1) distance form root
  node() {
    id = counter++;
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
    node::counter = 0;
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

  // Print distance of all nodes after BFS
  void print_distance() {
    cout << "Node ID\t\tDistance" << endl;
    cout << "-------------------" << endl;
    for (auto i : N) {
      cout << i.id << "\t\t" << i.d << endl;;
    }
    cout << "-------------------" << endl;
  }

  // Serial BFS
  void BFS_serial(unsigned short int);
};

// TODO Implement serial BFS
void graph::BFS_serial(unsigned short int s_id) {
  node *s = &N[s_id];
  // BFS Queue
  queue<node *> Q;



  // Distance of source will be 0
  s->d = 0;
  // Source has been discovered
  s->discovered = true;

  Q.push(s);

  while (!Q.empty()) {
    node *u = Q.front();
    Q.pop();
    for (int v = 0; v < no_of_nodes; ++v) {
      if ((edgeMatrix[u->id][v] != 0) && (!N[v].discovered)) {
        N[v].discovered = true;
        N[v].d = u->d + edgeMatrix[u->id][v];
        Q.push(&N[v]);
      }
    }
    u->discovered = true;
  }
}

int main(void) {
  graph G(4);

  G.add_edge(0, 1, 1);
  G.add_edge(0, 3, 2);
  G.add_edge(1, 2, 3);
  G.add_edge(3, 4, 4);

  G.BFS_serial(0);
  G.print_distance();
}
