#include <bits/stdc++.h>
using namespace std;

struct Item {
    string name;
    int weight;
    int utility;
    bool perishable;
};

// Perishable / life-saving items get double utility (priority)
int effectiveUtility(const Item &item) {
    return item.perishable ? item.utility * 2 : item.utility;
}

// 0/1 Knapsack - Dynamic Programming with traceback
int knapsackDP(
    const vector<Item> &items,
    int W,
    vector<int> &chosen
) {
    int N = items.size();
    vector<vector<int>> dp(N + 1, vector<int>(W + 1, 0));
    for (int i = 1; i <= N; i++) {
        for (int w = 1; w <= W; w++) {
            dp[i][w] = dp[i - 1][w];
            if (items[i - 1].weight <= w) {
                dp[i][w] = max(
                    dp[i][w],
                    effectiveUtility(items[i - 1]) +
                    dp[i - 1][w - items[i - 1].weight]
                );
            }
        }
    }
    // Trace back the selected items
    chosen.clear();
    int w = W;
    for (int i = N; i >= 1; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            chosen.push_back(i - 1);
            w -= items[i - 1].weight;
        }
    }
    reverse(chosen.begin(), chosen.end());
    return dp[N][W];
}

// Brute Force - try all 2^N subsets
int knapsackBruteForce(const vector<Item> &items, int W) {
    int N = items.size();
    int best = 0;
    for (int mask = 0; mask < (1 << N); mask++) {
        int totalW = 0, totalU = 0;
        for (int i = 0; i < N; i++) {
            if (mask & (1 << i)) {
                totalW += items[i].weight;
                totalU += effectiveUtility(items[i]);
            }
        }
        if (totalW <= W)
            best = max(best, totalU);
    }
    return best;
}

// Greedy - pick by highest utility/weight ratio (not always optimal for 0/1)
int knapsackGreedy(const vector<Item> &items, int W) {
    vector<int> order(items.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        return (double)effectiveUtility(items[a]) / items[a].weight >
               (double)effectiveUtility(items[b]) / items[b].weight;
    });
    int total = 0;
    for (int i : order) {
        if (items[i].weight <= W) {
            W -= items[i].weight;
            total += effectiveUtility(items[i]);
        }
    }
    return total;
}

void printLoad(const vector<Item> &items, const vector<int> &chosen) {
    int totalW = 0;
    for (int i : chosen) {
        cout << "  " << items[i].name
             << " (weight " << items[i].weight
             << ", utility " << effectiveUtility(items[i]) << ")\n";
        totalW += items[i].weight;
    }
    cout << "Total Weight Loaded : " << totalW << " kg\n";
}

int main() {
    int N, W;
    cout << " DISASTER RELIEF TRUCK LOADING SYSTEM\n";
    cout << " 0/1 Knapsack using Dynamic Programming\n";
    cout << endl;

    cout << "Enter number of items: ";
    cin >> N;

    cout << "Enter truck capacity (kg): ";
    cin >> W;

    vector<Item> items(N);
    cout << "Enter item details:\n";
    cout << "Format: Name Weight Utility Perishable(1/0)\n";
    cout << endl;

    for (int i = 0; i < N; i++) {
        int p;
        cout << "Item " << i + 1 << ": ";
        cin >> items[i].name >> items[i].weight >> items[i].utility >> p;
        items[i].perishable = (p == 1);
    }

    // Dynamic Programming
    vector<int> chosen;
    int best = knapsackDP(items, W, chosen);

    cout << endl;
    cout << " OPTIMAL TRUCK LOAD (DP)\n";
    cout << endl;
    printLoad(items, chosen);
    cout << "Maximum Total Utility : " << best << endl;

    // Compare approaches
    cout << endl;
    cout << " COMPARISON OF APPROACHES\n";
    cout << endl;
    cout << "Dynamic Programming O(N*W) : " << best << endl;
    if (N <= 20)
        cout << "Brute Force         O(2^N) : " << knapsackBruteForce(items, W) << endl;
    else
        cout << "Brute Force         O(2^N) : skipped (N too large)\n";
    cout << "Greedy (by ratio) O(NlogN) : " << knapsackGreedy(items, W) << endl;

    // Multiple trucks - fill one truck at a time with remaining items
    int T;
    cout << endl;
    cout << "Enter number of additional trucks available: ";
    cin >> T;

    vector<Item> remaining;
    vector<bool> used(N, false);
    for (int i : chosen)
        used[i] = true;
    for (int i = 0; i < N; i++)
        if (!used[i])
            remaining.push_back(items[i]);

    for (int t = 0; t < T && !remaining.empty(); t++) {
        int cap;
        cout << "Capacity of truck " << t + 2 << " (kg): ";
        cin >> cap;

        vector<int> load;
        int u = knapsackDP(remaining, cap, load);

        cout << endl;
        cout << " TRUCK " << t + 2 << " LOAD\n";
        cout << endl;
        printLoad(remaining, load);
        cout << "Total Utility : " << u << endl;
        cout << endl;

        vector<Item> next;
        vector<bool> taken(remaining.size(), false);
        for (int i : load)
            taken[i] = true;
        for (int i = 0; i < (int)remaining.size(); i++)
            if (!taken[i])
                next.push_back(remaining[i]);
        remaining = next;
    }

    cout << "Items left undelivered : " << remaining.size() << endl;

    cout << endl;
    cout << " SYSTEM FINISHED\n";
    cout << endl;

    return 0;
}
