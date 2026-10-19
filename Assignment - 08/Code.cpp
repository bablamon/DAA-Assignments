#include <bits/stdc++.h>
using namespace std;

const int INF = INT_MAX;

struct Node {
    vector<vector<int>> matrix;
    vector<int> path;
    int city;
    int level;
    int cost;  // lower bound
};

struct Compare {
    bool operator()(const Node &a, const Node &b) const {
        return a.cost > b.cost;
    }
};

// Row and column reduction, returns total reduction
int reduceMatrix(vector<vector<int>> &m) {
    int N = m.size();
    int reduction = 0;
    for (int i = 0; i < N; i++) {
        int mn = INF;
        for (int j = 0; j < N; j++)
            mn = min(mn, m[i][j]);
        if (mn != INF && mn > 0) {
            reduction += mn;
            for (int j = 0; j < N; j++)
                if (m[i][j] != INF)
                    m[i][j] -= mn;
        }
    }
    for (int j = 0; j < N; j++) {
        int mn = INF;
        for (int i = 0; i < N; i++)
            mn = min(mn, m[i][j]);
        if (mn != INF && mn > 0) {
            reduction += mn;
            for (int i = 0; i < N; i++)
                if (m[i][j] != INF)
                    m[i][j] -= mn;
        }
    }
    return reduction;
}

// LC Branch and Bound for TSP
int tspBranchAndBound(
    const vector<vector<int>> &cost,
    int start,
    vector<int> &bestPath,
    int &explored,
    int &pruned
) {
    int N = cost.size();
    priority_queue<Node, vector<Node>, Compare> pq;

    Node root;
    root.matrix = cost;
    root.path = {start};
    root.city = start;
    root.level = 0;
    root.cost = reduceMatrix(root.matrix);
    cout << "Initial Lower Bound (root) : " << root.cost << endl;
    pq.push(root);

    int bestCost = INF;
    explored = pruned = 0;

    while (!pq.empty()) {
        Node node = pq.top();
        pq.pop();

        if (node.cost >= bestCost) {
            pruned++;
            continue;
        }
        explored++;

        // All cities visited - close the tour
        if (node.level == N - 1) {
            bestCost = node.cost;
            bestPath = node.path;
            bestPath.push_back(start);
            continue;
        }

        int u = node.city;
        for (int v = 0; v < N; v++) {
            if (node.matrix[u][v] == INF)
                continue;

            Node child;
            child.matrix = node.matrix;
            // Block row u, column v and the return edge v -> start
            for (int k = 0; k < N; k++) {
                child.matrix[u][k] = INF;
                child.matrix[k][v] = INF;
            }
            child.matrix[v][start] = INF;

            child.path = node.path;
            child.path.push_back(v);
            child.city = v;
            child.level = node.level + 1;
            child.cost = node.cost + node.matrix[u][v] +
                         reduceMatrix(child.matrix);

            if (child.cost < bestCost)
                pq.push(child);
            else
                pruned++;
        }
    }
    return bestCost;
}

int main() {
    int N;
    cout << " SWIFTSHIP DELIVERY ROUTE OPTIMIZATION\n";
    cout << " TSP using LC Branch and Bound\n";
    cout << endl;

    cout << "Enter number of cities: ";
    cin >> N;

    vector<vector<int>> cost(N, vector<int>(N));
    cout << "Enter cost matrix (fuel cost i -> j, 0 on diagonal):\n";
    cout << endl;

    for (int i = 0; i < N; i++) {
        cout << "City " << i + 1 << ": ";
        for (int j = 0; j < N; j++) {
            cin >> cost[i][j];
            if (i == j)
                cost[i][j] = INF;
        }
    }

    int start;
    cout << "Enter starting city: ";
    cin >> start;
    start--;

    cout << endl;
    cout << " BRANCH AND BOUND SEARCH\n";
    cout << endl;

    vector<int> bestPath;
    int explored, pruned;
    int best = tspBranchAndBound(cost, start, bestPath, explored, pruned);

    cout << "Nodes Explored : " << explored << endl;
    cout << "Nodes Pruned   : " << pruned << endl;

    cout << endl;
    cout << " OPTIMAL DELIVERY ROUTE\n";
    cout << endl;

    if (best == INF) {
        cout << "No valid tour exists.\n";
        return 0;
    }

    cout << "Optimal Route: ";
    int check = 0;
    for (int i = 0; i < (int)bestPath.size(); i++) {
        cout << bestPath[i] + 1;
        if (i != (int)bestPath.size() - 1) {
            cout << " -> ";
            check += cost[bestPath[i]][bestPath[i + 1]];
        }
    }
    cout << endl;
    cout << "Minimum Tour Cost : " << best << endl;
    cout << "Verified Cost (sum of edges) : " << check << endl;

    long long brute = 1;
    for (int i = 2; i < N; i++)
        brute *= i;
    cout << "Brute force would check " << brute << " permutations.\n";

    cout << endl;
    cout << " SYSTEM FINISHED\n";
    cout << endl;

    return 0;
}
