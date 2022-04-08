#include "indicators.hpp"
#include <fstream>
#include <iostream>
#include <omp.h>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

#define NO_OF_THREADS 4

using namespace std;

// Class to encapsulate node
class Node {
public:
  // Each node has an integer ID corrosponding
  // to it's index in edgeMatrix
  unsigned long int id;

  // Static counter to assign default value to id
  inline static unsigned long int counter = 0;

  // Has node been discoverd by BFS
  bool discovered_serial;
  bool discovered_parallel;

  // Distance form root node chosen for BFS
  // d = 0 for root node
  long long int d_serial;
  long int d_parallel;

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
  unsigned long int no_of_nodes;

  // Number of reachable nodes to calculate percentage
  unsigned long int no_of_reachable_nodes;

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
        istringstream s(line);
        string word;

        s >> word;
        no_of_nodes = stoi(word);
        s >> word;
        no_of_reachable_nodes = stoi(word);

        break;
      }
    }

    // Initialize edgeMatrix
    edgeMatrix = vector<vector<bool>>(no_of_nodes, vector<bool>(no_of_nodes));

    // Read node pairs and add edge
    while (getline(dataset_stream, line)) {
      if (line[0] != '#') {
        istringstream nodepair(line);
        string word;

        nodepair >> word;
        unsigned long int n1 = stoi(word);
        nodepair >> word;
        edgeMatrix[n1][stoi(word)] = true;
      }
    }

    dataset_stream.close();

    // Initialize node vector
    N = vector<Node>(no_of_nodes);
  }

  // Print adjacency matrix
  void print_matrix() {
    cout << "Adjacency Matrix of the graph is: " << endl;
    for (auto i : edgeMatrix) {
      for (unsigned long int j = 0; j < i.size() - 1; ++j) {
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

  // Check if serial and parallel algorithms
  // have yielded same results
  bool check_BFS_parallel() {
    for (auto node : N) {
      if (node.d_serial != node.d_parallel) {
        return false;
      }
    }
    return true;
  }

  // Serial BFS
  double BFS_serial(unsigned long int);

  // Parallel BFS
  double BFS_parallel(unsigned long int);
};

double Graph::BFS_serial(unsigned long int s_id) {
  // s is now pointer to source
  Node *s = &N[s_id];

  // BFS Queue
  queue<Node *> Q;

  Node *u;

  // Distance of source will be 0
  s->d_serial = 0;

  // Source has been discovered
  s->discovered_serial = true;

  unsigned long int no_of_nodes_discovered = 0;

  Q.push(s);

  using namespace indicators;
  BlockProgressBar bar{
      option::BarWidth{80}, option::ForegroundColor{Color::white},
      option::FontStyles{vector<FontStyle>{FontStyle::bold}},
      option::MaxProgress{no_of_reachable_nodes}};

  cout << "\nPerforming Serial BFS" << endl;

  double start_time = omp_get_wtime();
  // BFS algorithm
  // for (u = Q.front(); !Q.empty(); u->discovered_parallel = true) {
  while (!Q.empty()) {
    u = Q.front();
    Q.pop();
    for (unsigned long int v = 0; v < no_of_nodes; ++v) {
      if ((edgeMatrix[u->id][v]) && (!N[v].discovered_serial)) {
        N[v].discovered_serial = true;
        N[v].d_serial = u->d_serial + 1;
        Q.push(&N[v]);
      }
    }
    u->discovered_serial = true;


    bar.set_option(option::PostfixText{to_string(++no_of_nodes_discovered) + "/" +
                                       to_string(no_of_reachable_nodes) + " nodes discovered"});

    bar.tick();
  }

  double end_time = omp_get_wtime();

  bar.mark_as_completed();
  indicators::show_console_cursor(true);

  return (end_time - start_time);
}

double Graph::BFS_parallel(unsigned long int s_id) {
  // s is now pointer to source
  Node *s = &N[s_id];

  // BFS Queue
  queue<Node *> Q;

  Node *u;

  // Distance of source will be 0
  s->d_parallel = 0;

  // Source has been discovered
  s->discovered_parallel = true;

  Q.push(s);

  unsigned long int no_of_nodes_discovered = 0;

  indicators::show_console_cursor(false);

  using namespace indicators;
  BlockProgressBar bar{option::BarWidth{80},
                       option::ForegroundColor{Color::white},
                       option::FontStyles{vector<FontStyle>{FontStyle::bold}},
                       option::MaxProgress{no_of_reachable_nodes}};

  cout << "\nPerforming Parallel BFS" << endl;

  double start_time = omp_get_wtime();
  // BFS algorithm
  // for (u = Q.front(); !Q.empty(); u->discovered_parallel = true) {
  while (!Q.empty()) {
    u = Q.front();
    Q.pop();
    #pragma omp parallel for schedule(auto)
    for (unsigned long int v = 0; v < no_of_nodes; ++v) {
      if ((edgeMatrix[u->id][v]) && (!N[v].discovered_parallel)) {
        N[v].discovered_parallel = true;
        N[v].d_parallel = u->d_parallel + 1;

        #pragma omp critical
        Q.push(&N[v]);
      }
    }
    u->discovered_parallel = true;

    bar.set_option(option::PostfixText{to_string(++no_of_nodes_discovered) +
                                       "/" + to_string(no_of_reachable_nodes) +
                                       " nodes discovered"});

    bar.tick();
  }
  double end_time = omp_get_wtime();

  bar.mark_as_completed();
  indicators::show_console_cursor(true);

  return (end_time - start_time);
}

int main(void) {
  omp_set_num_threads(NO_OF_THREADS);

  Graph G("soc-Epinions1.txt");
  // Graph G("test.txt");
  // Graph G("p2p-Gnutella08.txt");

  double serial_time = G.BFS_serial(0);
  cout << "\nSerial time: " << serial_time << " seconds" << endl;

  double parallel_time = G.BFS_parallel(0);

  if (!G.check_BFS_parallel()) {
    cout << "Parallel BFS ran incorrectly!\n" << endl;
    return 1;
  }

  cout << "\nParallel BFS ran correctly!" << endl;

  cout << "Parallel time: " << parallel_time << " seconds" << endl;

  return 0;
}
