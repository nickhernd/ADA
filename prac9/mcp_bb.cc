// JAIME HERNÁNDEZ DELGADO 48776654W

#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <queue>
#include <unordered_map>

using namespace std;

const int MAX_COST = INT_MAX;

int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};
int movesMap[3][3] = {
    {8, 1, 2},
    {7, 0, 3},
    {6, 5, 4}
};

struct Node {
    int row, col;
    int cost;
    int optimistic_bound;
    vector<int> path;  // Almacenamos el camino de movimientos

    Node(int r, int c, int co, int ob, vector<int> p) : row(r), col(c), cost(co), optimistic_bound(ob), path(p) {}
    
    bool operator>(const Node& other) const {
        return optimistic_bound > other.optimistic_bound;
    }
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
            for (int k = 0; k < 8; ++k) {
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

vector<vector<int>> precalcular_cota_optimista(const vector<vector<int>>& mapa, int n, int m) {
    vector<vector<int>> min_cost_map(n, vector<int>(m, MAX_COST));
    for (int i = n - 1; i >= 0; --i) {
        for (int j = m - 1; j >= 0; --j) {
            if (i == n - 1 && j == m - 1) {
                min_cost_map[i][j] = mapa[i][j];
            } else {
                int min_cost = MAX_COST;
                for (int k = 0; k < 8; ++k) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                        min_cost = min(min_cost, min_cost_map[ni][nj]);
                    }
                }
                min_cost_map[i][j] = min(mapa[i][j], min_cost);
            }
        }
    }
    return min_cost_map;
}

int calcular_cota_optimista(int row, int col, int cost, const vector<vector<int>>& min_cost_map, int n, int m) {
    return cost + min_cost_map[row][col];
}

void generateChildren(Node* node, const vector<vector<int>>& mapa, int n, int m, priority_queue<Node, vector<Node>, greater<Node>>& pq, vector<vector<bool>>& visited, const vector<vector<int>>& min_cost_map) {
    int row = node->row;
    int col = node->col;

    for (int i = 0; i < 8; i++) {
        int new_row = row + dx[i];
        int new_col = col + dy[i];

        if (new_row >= 0 && new_row < n && new_col >= 0 && new_col < m && !visited[new_row][new_col]) {
            int new_cost = node->cost + mapa[new_row][new_col];
            int optimistic_bound = calcular_cota_optimista(new_row, new_col, new_cost, min_cost_map, n, m);
            vector<int> new_path = node->path;
            new_path.push_back(movesMap[dx[i] + 1][dy[i] + 1]);
            pq.emplace(new_row, new_col, new_cost, optimistic_bound, new_path);
        }
    }
}

vector<int> bestMoves;

void mcp_bb(const vector<vector<int>>& mapa, int n, int m, int& bestCost, vector<vector<bool>>& visited, const vector<vector<int>>& cota_pesimista, vector<vector<int>>& best_costs, int& n_visit, int& n_explored, int& n_leaf, int& n_unfeasible, int& n_not_promising, int& n_promising_but_discarded, int& n_best_solution_updated_from_leafs, int& n_best_solution_updated_from_pessimistic_bound, const vector<vector<int>>& min_cost_map) {
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    pq.emplace(0, 0, mapa[0][0], calcular_cota_optimista(0, 0, mapa[0][0], min_cost_map, n, m), vector<int>{});

    while (!pq.empty()) {
        Node node = pq.top();
        pq.pop();
        n_visit++;

        if (node.row == n - 1 && node.col == m - 1) {
            n_leaf++;
            n_explored++;
            if (node.cost < bestCost) {
                bestCost = node.cost;
                bestMoves = node.path;
                n_best_solution_updated_from_leafs++;
            }
            continue;
        }

        visited[node.row][node.col] = true;

        int pessimistic_bound = cota_pesimista[node.row][node.col];
        if (pessimistic_bound >= bestCost) {
            n_not_promising++;
            visited[node.row][node.col] = false;
            continue;
        }

        int optimistic_bound = node.optimistic_bound;
        if (optimistic_bound >= bestCost) {
            n_promising_but_discarded++;
            visited[node.row][node.col] = false;
            continue;
        }

        n_explored++;

        if (node.cost >= best_costs[node.row][node.col]) {
            visited[node.row][node.col] = false;
            continue;
        }
        best_costs[node.row][node.col] = node.cost;

        generateChildren(&node, mapa, n, m, pq, visited, min_cost_map);
        visited[node.row][node.col] = false;
    }
}

int main(int argc, char *argv[]) {
    bool show_path_2D = false;
    bool show_path_p = false;

    if (argc <= 2) {
        cerr << "ERROR: missing filename.\n";
        cerr << "Usage:\n";
        cerr << "mcp_bb [ -p] [ --p2D] -f filename\n";
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
            cerr << "mcp_bb [ -p] [ --p2D] -f filename\n";
            return 1;
        }
    }

    if (filename.empty()) {
        cerr << "ERROR: missing filename.\n";
        cerr << "Usage:\n";
        cerr << "mcp_bb [ -p] [ --p2D] -f filename\n";
        return 1;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: can't open file: " << filename << ".\n";
        cerr << "Usage:\n";
        cerr << "mcp_bb [ -p] [ --p2D] -f filename\n";
        return 1;
    }

    clock_t start = clock();

    int n, m;
    vector<vector<int>> mapa, cota_pesimista, min_cost_map;
    mapa = read_map(filename, n, m);
    cota_pesimista = precalcular_cota_pesimista(mapa, n, m);
    min_cost_map = precalcular_cota_optimista(mapa, n, m);

    int bestCost = INT_MAX;
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<vector<int>> best_costs(n, vector<int>(m, INT_MAX));

    int n_visit = 0, n_explored = 0, n_leaf = 0, n_unfeasible = 0, n_not_promising = 0;
    int n_promising_but_discarded = 0, n_best_solution_updated_from_leafs = 0, n_best_solution_updated_from_pessimistic_bound = 0;

    mcp_bb(mapa, n, m, bestCost, visited, cota_pesimista, best_costs, n_visit, n_explored, n_leaf, n_unfeasible, n_not_promising, n_promising_but_discarded, n_best_solution_updated_from_leafs, n_best_solution_updated_from_pessimistic_bound, min_cost_map);

    clock_t end = clock();
    float time = float(end - start) * 1000 / CLOCKS_PER_SEC;

    cout << bestCost << endl;
    cout << n_visit << " " << n_explored << " " << n_leaf << " " << n_unfeasible << " " << n_not_promising << " "
         << n_promising_but_discarded << " " << n_best_solution_updated_from_leafs << " "
         << n_best_solution_updated_from_pessimistic_bound << endl;
    cout << fixed << setprecision(3) << time << endl;

    if (show_path_2D) {
        vector<vector<char>> path(n, vector<char>(m, '.'));
        int row = 0, col = 0;
        path[row][col] = 'x';

        for (int move : bestMoves) {
            if (move == 1) {
                row--;
            } else if (move == 2) {
                row--;
                col++;
            } else if (move == 3) {
                col++;
            } else if (move == 4) {
                row++;
                col++;
            } else if (move == 5) {
                row++;
            } else if (move == 6) {
                row++;
                col--;
            } else if (move == 7) {
                col--;
            } else if (move == 8) {
                row--;
                col--;
            }
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
        for (int move : bestMoves) {
            cout << move;
        }
        cout << ">" << endl;
    }

    return 0;
}
