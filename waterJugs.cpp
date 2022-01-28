/*
 * Name: Charlie Hartsell
 * Date Submitted: April 19, 2021
 * Lab Section: 004
 * Assignment Name: Lab 10
 */

#include <iostream>
#include <vector>
#include <map>
#include <queue>
using namespace std;

// Reflects what each node represents...
// First value units of water in A, second units of water in B
typedef pair<int,int> state;

// Each string in edge_label must be one of the following:
const string actions[] = {"Fill A",
                          "Fill B",
                          "Empty A",
                          "Empty B",
                          "Pour A->B",
                          "Pour B->A"};

// GENERIC -- these shouldn't need to be changed...
map<state, bool> visited;         // have we queued up this state for visitation?
map<state, state> pred;           // predecessor state we came from
map<state, int> dist;             // distance (# of hops) from source node
map<state, vector<state>> nbrs;   // vector of neighboring states

map<pair<state,state>, string> edge_label;

// GENERIC (breadth-first search, outward from source_node)
void search(state source_node)
{
  queue<state> to_visit;
  to_visit.push(source_node);
  visited[source_node] = true;
  dist[source_node] = 0;
  
  while (!to_visit.empty()) {
    state curnode = to_visit.front();
    to_visit.pop();
    for (state n : nbrs[curnode])
      if (!visited[n]) {
	pred[n] = curnode;
	dist[n] = 1 + dist[curnode];
	visited[n] = true;
	to_visit.push(n);
      }
  }
}

// GENERIC
void print_path(state s, state t)
{
  if (s != t) {
    print_path(s, pred[t]);
    cout << edge_label[make_pair(pred[t], t)] << ": " << "[" << t.first << "," << t.second << "]\n";
  } else {
    cout << "Initial state: " << "[" << s.first << "," << s.second << "]\n";
  }
}

// helper function
string neighbor_label(state prev, state curr) {
  if((prev.first < curr.first) && (prev.second == curr.second)) {
            // A filled
            return actions[0];
          } else if((prev.second < curr.second) && (prev.first == curr.first)) {
            // B filled
            return actions[1];
          } else if((prev.first > curr.first) && (prev.second == curr.second)) {
            // A emptied
            return actions[2];
          } else if((prev.second > curr.second) && (prev.first == curr.first)) {
            // B emptied
            return actions[3];
          } else if((prev.first > curr.first) && (prev.second < curr.second)) {
            // A -> B
            return actions[4];
          } else if((prev.first < curr.first) && (prev.second > curr.second)) {
            // B -> A
            return actions[5];
          } else {
            // This case should never show up
            return "This should never show up!";
          }
}

void build_graph(void)
{
  // fill map<pair<state,state>, string> edge_label

  // loop through cur states
  for(int i = 0; i <= 3; i++) {
    for(int j = 0; j <= 4; j++) {
      // loop through pred states
      for(int predi = 0; predi <= 3; predi++) {
        for(int predj = 0; predj <= 4; predj++) {
          // Save the position as current and previous states
          state prev = {predi, predj};
          state curr = {i, j};

          edge_label[{prev, curr}] = neighbor_label(prev, curr);
        }
      }
    }
  }

  // filling in the correct moves. i solved it by hand to find these
  nbrs[{0,0}].push_back({0,4});
  nbrs[{0,4}].push_back({3,1});
  nbrs[{3,1}].push_back({0,1});
  nbrs[{0,1}].push_back({1,0});
  nbrs[{1,0}].push_back({1,4});

  // filling in every other neighbor procedurally
  for(int a = 3; a <= 3; a++) {
    for(int b = 0; b <= 4; b++) {
      nbrs[{a,b}].push_back({a,4}); // fill b
      nbrs[{a,b}].push_back({3,b}); // fill a
      nbrs[{a,b}].push_back({a,0}); // empty b
      nbrs[{a,b}].push_back({0,b}); // empty a

      // pour a->b:
      if(b + a <= 4 && a <= 3 && b <= 4) {
        nbrs[{a,b}].push_back({0, b + a}); // a fits in b, pour a->b
      } else {
        nbrs[{a,b}].push_back({b + a - 4, 4}); // a doesn't fit in b, fill b and leave remainder in a
      }

      // pour b->a:
      if(a + b >= 3 && a <= 3 && b <= 4) {
        nbrs[{a,b}].push_back({a + b, 0}); // b fits in a, pour b->a
      } else {
        nbrs[{a,b}].push_back({3, a + b - 3}); // b doesn't fit in a, fill a and leave remainder in b
      }
    }
  }
}

int main(void)
{
  build_graph();

  state start = make_pair(0,0);
  
  for (int i=0; i<5; i++)
    nbrs[make_pair(i,5-i)].push_back(make_pair(-1,-1));
  search (start);
  if (visited[make_pair(-1,-1)]) 
    print_path (start, pred[make_pair(-1,-1)]);
  else
    cout << "No path!\n";
  
  return 0;
}
