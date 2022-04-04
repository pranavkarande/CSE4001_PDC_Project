#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// Class to encapsulate node
class node {
public:
  // Each node has an integer ID corrosponding
  // to it's index in edgeMatrix
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
  vector<vector<bool>> edgeMatrix;

  // Node vector
  vector<node> N;

  // Construct trivial graph with n nodes
  graph(int n) {
    no_of_nodes = n;

    // Initialize edgeMatrix
    edgeMatrix = vector<vector<bool>>(n, vector<bool>(n));

    // Initialize node vector
    node::counter = 0;
    N = vector<node>(n);
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
  graph G(18);

  G.add_edge(0, 4);
  G.add_edge(4, 1);
  G.add_edge(0, 1);
  G.add_edge(5, 4);
  G.add_edge(2, 5);
  G.add_edge(5, 6);
  G.add_edge(4, 7);
  G.add_edge(4, 8);
  G.add_edge(8, 5);
  G.add_edge(9, 5);
  G.add_edge(9, 6);
  G.add_edge(6, 10);
  G.add_edge(7, 8);
  G.add_edge(8, 9);
  G.add_edge(7, 14);
  G.add_edge(11, 7);
  G.add_edge(9, 13);
  G.add_edge(10, 17);
  G.add_edge(11, 12);
  G.add_edge(12, 13);
  G.add_edge(13, 16);
  G.add_edge(13, 17);
  G.add_edge(14, 15);
  G.add_edge(6, 5);
  G.add_edge(8, 7);
  G.add_edge(12, 11);
  G.add_edge(13, 12);
  G.add_edge(16, 13);
  G.add_edge(15, 14);

  G.BFS_serial(0);
  G.print_distance();
}
