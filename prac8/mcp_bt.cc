// JAIME HERNÁNDEZ DELGADO 48776654W

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <ctime>

using namespace std;

const int MAX_COST = INT_MAX;

int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};

struct Node {
    int row, col;
    int cost;
    Node* parent;
};

vector<vector<int>> read_map(const string& filename, int &n, int &m) {
    ifstream file(filename);
    file >> n >> m;
    vector<vector<int>> map(n, vector<int>(m));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            file >> map[i][j];
        }
    }

    file.close();
    return map;
}

vector<vector<int>> precalcular_cota_pesimista(const vector<vector<int>>& mapa, int n, int m) {
    vector<vector<int>> memo(n, vector<int>(m, MAX_COST));
    memo[n-1][m-1] = mapa[n-1][m-1];

    for (int i = n-1; i >= 0; --i) {
        for (int j = m-1; j >= 0; --j) {
            if (i == n-1 && j == m-1) continue;
            int min_cost = MAX_COST;
            for (int k = 0; k < 3; ++k) {
                int ni = i + dx[k];
                int nj = j + dy[k];
                if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                    min_cost = min(min_cost, memo[ni][nj]);
                }
            }
            memo[i][j] = min_cost + mapa[i][j];
        }
    }
    return memo;
}

void generateChildren(Node* node, const vector<vector<int>>& mapa, int n, int m, vector<Node*>& children, vector<vector<bool>>& visited) {
    int row = node->row;
    int col = node->col;

    for (int i = 0; i < 8; i++) {
        int new_row = row + dx[i];
        int new_col = col + dy[i];

        if (new_row >= 0 && new_row < n && new_col >= 0 && new_col < m && !visited[new_row][new_col]) {
            int new_cost = node->cost + mapa[new_row][new_col];
            Node* child = new Node{new_row, new_col, new_cost, node};
            children.push_back(child);
        }
    }
}

bool check_nodo_ok(Node* node, int bestCost, const vector<vector<int>>& poda_memo, int& n_not_promising) {
    size_t row = node->row;
    size_t col = node->col;
    int cost = node->cost;

    int pessimistic_bound = cost + poda_memo[row][col];
    if (pessimistic_bound >= bestCost) {
        n_not_promising++;
        return true;
    }

    return false;
}

vector<int> bestPath;

void mcp_bt(Node* node, const vector<vector<int>>& mapa, int n, int m, int& bestCost, vector<vector<bool>>& visited, const vector<vector<int>>& poda_memo, vector<vector<int>>& best_costs, int& n_visit, int& n_explored, int& n_leaf, int& n_unfeasible, int& n_not_promising) {
    n_visit++;

    if (node->row == n - 1 && node->col == m - 1) {
        n_leaf++;
        n_explored++;
        if (node->cost < bestCost) {
            bestCost = node->cost;
            bestPath.clear();
            Node* current = node;
            while (current != nullptr) {
                bestPath.push_back(current->row * m + current->col);
                current = current->parent;
            }
            reverse(bestPath.begin(), bestPath.end());
        }
        return;
    }

    visited[node->row][node->col] = true;

    if (check_nodo_ok(node, bestCost, poda_memo, n_not_promising)) {
        visited[node->row][node->col] = false;
        return;
    }

    n_explored++;

    if (node->cost >= best_costs[node->row][node->col]) {
        visited[node->row][node->col] = false;
        return;
    }
    best_costs[node->row][node->col] = node->cost;

    vector<Node*> children;
    generateChildren(node, mapa, n, m, children, visited);

    sort(children.begin(), children.end(), [](const Node* a, const Node* b) {
        return a->cost < b->cost;
    });

    for (Node* child : children) {
        if (child->cost >= bestCost) {
            n_unfeasible++;
            delete child;
            continue;
        }
        mcp_bt(child, mapa, n, m, bestCost, visited, poda_memo, best_costs, n_visit, n_explored, n_leaf, n_unfeasible, n_not_promising);
        delete child;
    }

    visited[node->row][node->col] = false;
}

int main(int argc, char *argv[]) {
    bool show_path_2D = false;
    bool show_path_p = false;

    if (argc <= 2) {
        cerr << "ERROR: missing filename.\n";
        cerr << "Usage:\n";
        cerr << "mcp_bt [ -p] [ --p2D] -f filename\n";
        return 1;
    }

    string filename;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-p") {
            show_path_p = true;
        } else if (arg == "--p2D") {
            show_path_2D = true;
        } else if (arg == "-f" && i + 1 < argc) {
            filename = argv[++i];
        } else {
            cerr << "ERROR: unknown option " << arg << "\n";
            cerr << "Usage:\n";
            cerr << "mcp_bt [ -p] [ --p2D] -f filename\n";
            return 1;
        }
    }

    if (filename.empty()) {
        cerr << "ERROR: missing filename.\n";
        cerr << "Usage:\n";
        cerr << "mcp_bt [ -p] [ --p2D] -f filename\n";
        return 1;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: can't open file: " << filename << ".\n";
        cerr << "Usage:\n";
        cerr << "mcp_bt [ -p] [ --p2D] -f filename\n";
        return 1;
    }

    clock_t start = clock();

    int n, m;
    vector<vector<int>> mapa, poda_memo;
    mapa = read_map(filename, n, m);
    poda_memo = precalcular_cota_pesimista(mapa, n, m);

    int bestCost = INT_MAX;
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<vector<int>> best_costs(n, vector<int>(m, INT_MAX));

    int n_visit = 0, n_explored = 0, n_leaf = 0, n_unfeasible = 0, n_not_promising = 0;

    Node* initialNode = new Node{0, 0, mapa[0][0], nullptr};
    mcp_bt(initialNode, mapa, n, m, bestCost, visited, poda_memo, best_costs, n_visit, n_explored, n_leaf, n_unfeasible, n_not_promising);

    clock_t end = clock();
    float time = float(end - start)*1000 / CLOCKS_PER_SEC;

    cout << bestCost << endl;
    cout << n_visit << " " << n_explored << " " << n_leaf << " " << n_unfeasible << " " << n_not_promising << endl;
    cout << fixed << setprecision(3) << time << endl;

    if (show_path_2D) {
        vector<vector<char>> path(n, vector<char>(m, '.'));

        for (int pos : bestPath) {
            int row = pos / m;
            int col = pos % m;
            path[row][col] = 'x';
        }

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cout << path[i][j];
            }
            cout << endl;
        }

        cout << bestCost << endl;
    }

    if (show_path_p) {
        cout << "<";
        for (size_t i = 0; i < bestPath.size() - 1; i++) {
            int curr_pos = bestPath[i];
            int next_pos = bestPath[i + 1];
            int curr_row = curr_pos / m;
            int curr_col = curr_pos % m;
            int next_row = next_pos / m;
            int next_col = next_pos % m;
            int row_diff = next_row - curr_row;
            int col_diff = next_col - curr_col;
            if (row_diff == -1 && col_diff == 0) {
                cout << "1";
            } else if (row_diff == -1 && col_diff == 1) {
                cout << "2";
            } else if (row_diff == 0 && col_diff == 1) {
                cout << "3";
            } else if (row_diff == 1 && col_diff == 1) {
                cout << "4";
            } else if (row_diff == 1 && col_diff == 0) {
                cout << "5";
            } else if (row_diff == 1 && col_diff == -1) {
                cout << "6";
            } else if (row_diff == 0 && col_diff == -1) {
                cout << "7";
            } else if (row_diff == -1 && col_diff == -1) {
                cout << "8";
            }
        }
        cout << ">" << endl;
    }

    delete initialNode;

    return 0;
}