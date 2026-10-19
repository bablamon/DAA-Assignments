#include <bits/stdc++.h>
using namespace std;

// Multistage Graph - Backward Dynamic Programming
// Vertices are numbered stage by stage (1 = source, N = destination),
// so every edge goes from a smaller number to a larger number.
void multistageDP(
    int N,
    int destination,
    const vector<vector<pair<int, int>>> &graph,
    vector<long long> &cost,
    vector<int> &nextNode
) {
    cost.assign(N + 1, LLONG_MAX);
    nextNode.assign(N + 1, -1);
    cost[destination] = 0;
    // Move backward stage by stage
    for (int i = N - 1; i >= 1; i--) {
        for (auto edge : graph[i]) {
            int j = edge.first;
            int w = edge.second;
            if (cost[j] != LLONG_MAX &&
                cost[i] > w + cost[j]) {
                cost[i] = w + cost[j];
                nextNode[i] = j;
            }
        }
    }
}

// Display optimal route from source to destination
void printPath(
    int source,
    int destination,
    const vector<long long> &cost,
    const vector<int> &nextNode
) {
    if (cost[source] == LLONG_MAX) {
        cout << "No valid route exists.\n";
        return;
    }
    cout << "Optimal Route: ";
    int current = source;
    while (current != -1) {
        cout << current;
        if (current == destination)
            break;
        cout << " -> ";
        current = nextNode[current];
    }
    cout << endl;
    cout << "Minimum Delivery Cost : " << cost[source] << endl;
}

// Real-time update of a route cost (traffic, weather, road closure)
void updateRoute(
    vector<vector<pair<int, int>>> &graph,
    int u,
    int v,
    int newWeight
) {
    for (auto it = graph[u].begin(); it != graph[u].end(); it++) {
        if (it->first == v) {
            // Weight -1 means the road is closed
            if (newWeight < 0) {
                graph[u].erase(it);
                cout << "Road closed successfully.\n";
            }
            else {
                it->second = newWeight;
                cout << "Route cost updated successfully.\n";
            }
            return;
        }
    }
    cout << "Route not found!\n";
}

int main() {
    int N, E;
    cout << " SWIFTCARGO DELIVERY ROUTE OPTIMIZATION\n";
    cout << " Multistage Graph using Dynamic Programming\n";
    cout << endl;

    cout << "Enter number of cities (vertices): ";
    cin >> N;

    cout << "Enter number of routes (edges): ";
    cin >> E;

    vector<vector<pair<int, int>>> graph(N + 1);

    cout << "Enter route details (cities numbered stage-wise from 1 to N):\n";
    cout << "Format: From_City To_City Cost\n";
    cout << endl;

    for (int i = 0; i < E; i++) {
        int u, v, w;
        cout << "Route " << i + 1 << ": ";
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
    }

    int source = 1, destination = N;

    vector<long long> cost;
    vector<int> nextNode;
    multistageDP(N, destination, graph, cost, nextNode);

    cout << endl;
    cout << " COST TABLE (minimum cost to destination)\n";
    cout << endl;

    for (int i = 1; i <= N; i++) {
        cout << "cost(" << i << ") = ";
        if (cost[i] == LLONG_MAX)
            cout << "INF";
        else
            cout << cost[i];
        cout << "   next = " << nextNode[i] << endl;
    }

    cout << endl;
    cout << " OPTIMAL DELIVERY ROUTE\n";
    cout << endl;
    printPath(source, destination, cost, nextNode);

    // Batch processing of delivery requests
    int R;
    cout << endl;
    cout << "Enter number of batch delivery requests: ";
    cin >> R;

    for (int i = 0; i < R; i++) {
        int s;
        cout << "Request " << i + 1 << " - starting city: ";
        cin >> s;
        printPath(s, destination, cost, nextNode);
    }

    // Real-time rerouting
    char choice;
    cout << endl;
    cout << "Do you want to update route conditions? (y/n): ";
    cin >> choice;

    while (choice == 'y' || choice == 'Y') {
        int u, v, newWeight;

        cout << "Enter route (From To New_Cost, -1 = closed): ";
        cin >> u >> v >> newWeight;

        updateRoute(graph, u, v, newWeight);

        // Recalculate with updated conditions
        multistageDP(N, destination, graph, cost, nextNode);

        cout << endl;
        cout << " UPDATED OPTIMAL ROUTE\n";
        cout << endl;
        printPath(source, destination, cost, nextNode);

        cout << "Do you want to update again? (y/n): ";
        cin >> choice;
    }

    cout << endl;
    cout << " SYSTEM FINISHED\n";
    cout << endl;

    return 0;
}
