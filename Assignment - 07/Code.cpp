#include <bits/stdc++.h>
using namespace std;

// Smallest colour not used by any coloured neighbour
int smallestFreeColour(
    int u,
    const vector<set<int>> &graph,
    const vector<int> &colour
) {
    vector<bool> used(graph.size() + 1, false);
    for (int v : graph[u])
        if (colour[v] != -1)
            used[colour[v]] = true;
    int c = 0;
    while (used[c])
        c++;
    return c;
}

// Colour vertices in the given order
vector<int> colourInOrder(const vector<set<int>> &graph, const vector<int> &order) {
    vector<int> colour(graph.size(), -1);
    for (int u : order)
        colour[u] = smallestFreeColour(u, graph, colour);
    return colour;
}

// Simple Greedy - vertices in natural order
vector<int> greedyColouring(const vector<set<int>> &graph) {
    vector<int> order(graph.size());
    iota(order.begin(), order.end(), 0);
    return colourInOrder(graph, order);
}

// Welsh-Powell - vertices in descending order of degree
vector<int> welshPowell(const vector<set<int>> &graph) {
    vector<int> order(graph.size());
    iota(order.begin(), order.end(), 0);
    stable_sort(order.begin(), order.end(), [&](int a, int b) {
        return graph[a].size() > graph[b].size();
    });
    return colourInOrder(graph, order);
}

// DSATUR - pick vertex with most distinct neighbour colours
vector<int> dsatur(const vector<set<int>> &graph) {
    int V = graph.size();
    vector<int> colour(V, -1);
    for (int step = 0; step < V; step++) {
        int best = -1, bestSat = -1;
        for (int u = 0; u < V; u++) {
            if (colour[u] != -1)
                continue;
            set<int> sat;
            for (int v : graph[u])
                if (colour[v] != -1)
                    sat.insert(colour[v]);
            int s = sat.size();
            if (s > bestSat ||
                (s == bestSat && graph[u].size() > graph[best].size())) {
                best = u;
                bestSat = s;
            }
        }
        colour[best] = smallestFreeColour(best, graph, colour);
    }
    return colour;
}

int countColours(const vector<int> &colour) {
    return *max_element(colour.begin(), colour.end()) + 1;
}

void printColouring(const string &name, const vector<int> &colour) {
    cout << name << " : " << countColours(colour) << " slots  [ ";
    for (int i = 0; i < (int)colour.size(); i++)
        cout << "C" << i + 1 << "=S" << colour[i] + 1 << " ";
    cout << "]\n";
}

int main() {
    int C, S;
    cout << " UNIVERSITY EXAM TIMETABLE SCHEDULING\n";
    cout << " Graph Colouring with Room Allocation\n";
    cout << endl;

    cout << "Enter number of courses: ";
    cin >> C;

    cout << "Enter number of students: ";
    cin >> S;

    vector<set<int>> graph(C);
    vector<int> enrolled(C, 0);

    cout << "Enter enrolments for each student:\n";
    cout << "Format: Count Course_1 Course_2 ... (courses numbered 1 to C)\n";
    cout << endl;

    // Build conflict graph
    for (int s = 0; s < S; s++) {
        int k;
        cout << "Student " << s + 1 << ": ";
        cin >> k;
        vector<int> courses(k);
        for (int i = 0; i < k; i++) {
            cin >> courses[i];
            courses[i]--;
            enrolled[courses[i]]++;
        }
        for (int i = 0; i < k; i++)
            for (int j = i + 1; j < k; j++) {
                graph[courses[i]].insert(courses[j]);
                graph[courses[j]].insert(courses[i]);
            }
    }

    int R;
    cout << "Enter number of rooms: ";
    cin >> R;

    vector<pair<int, int>> rooms(R);  // {capacity, room number}
    cout << "Enter capacity of each room:\n";
    for (int i = 0; i < R; i++) {
        cout << "Room " << i + 1 << ": ";
        cin >> rooms[i].first;
        rooms[i].second = i + 1;
    }

    // Compare algorithms
    vector<int> g = greedyColouring(graph);
    vector<int> wp = welshPowell(graph);
    vector<int> ds = dsatur(graph);

    cout << endl;
    cout << " COMPARISON OF COLOURING ALGORITHMS\n";
    cout << endl;
    printColouring("Greedy       ", g);
    printColouring("Welsh-Powell ", wp);
    printColouring("DSATUR       ", ds);

    // Use the algorithm giving fewest slots
    vector<int> colour = ds;
    if (countColours(wp) < countColours(colour))
        colour = wp;
    if (countColours(g) < countColours(colour))
        colour = g;
    int slots = countColours(colour);

    // Room allocation - largest exam gets largest free room
    sort(rooms.rbegin(), rooms.rend());
    vector<int> roomOf(C, -1);

    for (int t = 0; t < slots; t++) {
        vector<int> exams;
        for (int c = 0; c < C; c++)
            if (colour[c] == t)
                exams.push_back(c);
        sort(exams.begin(), exams.end(), [&](int a, int b) {
            return enrolled[a] > enrolled[b];
        });
        vector<bool> occupied(R, false);
        for (int e : exams) {
            for (int r = 0; r < R; r++) {
                if (!occupied[r] && rooms[r].first >= enrolled[e]) {
                    roomOf[e] = rooms[r].second;
                    occupied[r] = true;
                    break;
                }
            }
        }
    }

    cout << endl;
    cout << " FINAL EXAM TIMETABLE\n";
    cout << endl;
    cout << "Course    Students    Time Slot    Room\n";

    for (int c = 0; c < C; c++) {
        cout << "C" << c + 1 << "        "
             << setw(3) << enrolled[c] << "         "
             << "Slot " << colour[c] + 1 << "       ";
        if (roomOf[c] == -1)
            cout << "No room available\n";
        else
            cout << "R" << roomOf[c] << endl;
    }

    cout << endl;
    cout << "Total Exam Slots Required : " << slots << endl;

    cout << endl;
    cout << " SYSTEM FINISHED\n";
    cout << endl;

    return 0;
}
