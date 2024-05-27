// JAIME HERNÁNDEZ DELGADO 48776654W

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <ctime>

using namespace std;

const int MAX_COST = INT_MAX;

// Definición de movimientos posibles en 8 direcciones
int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};

// Estructura que representa un nodo en el árbol de búsqueda
struct Node {
    int row, col;
    int cost;
    int depth;
    Node* parent;

    Node(int r, int c, int co, int d, Node* p) : row(r), col(c), cost(co), depth(d), parent(p) {}
};

// Comparador para la cola de prioridad, basado en el costo de los nodos
struct CompareNode {
    bool operator()(const Node* a, const Node* b) {
        return a->cost > b->cost;
    }
};

// Función para leer el mapa desde un archivo
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

// Algoritmo para calcular un costo inicial utilizando DP con hasta 3 movimientos permitidos
int dp_3_movements(const vector<vector<int>>& mapa, int n, int m) {
    vector<vector<int>> dp(n, vector<int>(m, MAX_COST));
    dp[0][0] = mapa[0][0];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (i == 0 && j == 0) continue;

            int min_cost = MAX_COST;
            for (int k = 0; k < 3; k++) {
                int prev_i = i - dx[k];
                int prev_j = j - dy[k];
                if (prev_i >= 0 && prev_j >= 0) {
                    min_cost = min(min_cost, dp[prev_i][prev_j]);
                }
            }

            if (min_cost != MAX_COST) {
                dp[i][j] = min_cost + mapa[i][j];
            }
        }
    }

    return dp[n - 1][m - 1];
}

// Función para calcular la cota optimista de un nodo
int get_optimistic_bound(Node* node, const vector<vector<int>>& mapa, int n, int m) {
    int remaining_rows = n - node->row - 1;
    int remaining_cols = m - node->col - 1;
    int min_remaining_cost = MAX_COST;

    for (int i = node->row; i < n; i++) {
        for (int j = node->col; j < m; j++) {
            min_remaining_cost = min(min_remaining_cost, mapa[i][j]);
        }
    }

    return node->cost + min_remaining_cost * max(remaining_rows, remaining_cols);
}

// Algoritmo de ramificación y poda para encontrar el camino de costo mínimo
void mcp_bb(const vector<vector<int>>& mapa, int n, int m, int& bestCost, vector<int>& bestPath,
            unordered_map<int, int>& best_costs, int& n_visit, int& n_explored, int& n_leaf, int& n_unfeasible,
            int& n_not_promising, int& n_promising_but_discarded, int& n_best_solution_updated_from_leafs,
            int& n_best_solution_updated_from_pessimistic_bound) {
    priority_queue<Node*, vector<Node*>, CompareNode> pq;
    Node* initialNode = new Node(0, 0, mapa[0][0], 0, nullptr);
    pq.push(initialNode);

    while (!pq.empty()) {
        Node* node = pq.top();
        pq.pop();
        n_visit++;

        // Si el nodo es la meta, actualiza el mejor costo y el mejor camino
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
                n_best_solution_updated_from_leafs++;
            }
            delete node;
            continue;
        }

        int pos = node->row * m + node->col;
        if (best_costs.count(pos) != 0 && node->cost >= best_costs[pos]) {
            n_not_promising++;
            delete node;
            continue;
        }
        best_costs[pos] = node->cost;

        int optimistic_bound = get_optimistic_bound(node, mapa, n, m);
        if (optimistic_bound >= bestCost) {
            n_promising_but_discarded++;
            delete node;
            continue;
        }

        n_explored++;

        // Expande los nodos hijos
        for (int i = 0; i < 8; i++) {
            int new_row = node->row + dx[i];
            int new_col = node->col + dy[i];

            if (new_row >= 0 && new_row < n && new_col >= 0 && new_col < m) {
                int new_cost = node->cost + mapa[new_row][new_col];

                if (new_cost >= bestCost) {
                    n_unfeasible++;
                    continue;
                }

                Node* child = new Node(new_row, new_col, new_cost, node->depth + 1, node);
                pq.push(child);
            }
        }

        delete node;
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
    vector<vector<int>> mapa;
    mapa = read_map(filename, n, m);

    int bestCost = dp_3_movements(mapa, n, m);
    vector<int> bestPath;
    unordered_map<int, int> best_costs;

    int n_visit = 0, n_explored = 0, n_leaf = 0, n_unfeasible = 0, n_not_promising = 0;
    int n_promising_but_discarded = 0, n_best_solution_updated_from_leafs = 0, n_best_solution_updated_from_pessimistic_bound = 0;

    mcp_bb(mapa, n, m, bestCost, bestPath, best_costs, n_visit, n_explored, n_leaf,
           n_unfeasible, n_not_promising, n_promising_but_discarded, n_best_solution_updated_from_leafs,
           n_best_solution_updated_from_pessimistic_bound);

    clock_t end = clock();
    float time = float(end - start) / CLOCKS_PER_SEC * 1000;

    cout << bestCost << endl;
    cout << n_visit << " " << n_explored << " " << n_leaf << " " << n_unfeasible << " " << n_not_promising << " "
         << n_promising_but_discarded << " " << n_best_solution_updated_from_leafs << " "
         << n_best_solution_updated_from_pessimistic_bound << endl;
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

    return 0;
}
