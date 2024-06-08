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
#include <functional>
#include <tuple>
#include <future>

using namespace std;

const int MAX_COST = INT_MAX; // Constante para representar el costo máximo

// Vectores que definen los desplazamientos en las 8 direcciones posibles
int dx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dy[] = {0, 1, 1, 1, 0, -1, -1, -1};

// Matriz que mapea los desplazamientos a números de movimiento
int movesMap[3][3] = {
    {8, 1, 2},
    {7, 0, 3},
    {6, 5, 4}
};

// Definición del tipo de nodo utilizando una tupla
// Cada nodo contiene: fila, columna, costo, cota optimista, cota pesimista y vector de movimientos
using Node = tuple<int, int, int, int, int, vector<int>>;

// Clase comparadora para la cola de prioridad
// Compara los nodos según su cota optimista
class CompareNode {
public:
    bool operator()(const Node& a, const Node& b) {
        return get<3>(a) > get<3>(b);   
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

// Función para precalcular la cota pesimista utilizando programación dinámica
vector<vector<int>> precalcular_cota_pesimista(const vector<vector<int>>& mapa, int n, int m) {
    vector<vector<int>> memo(n, vector<int>(m, MAX_COST));
    memo[n-1][m-1] = mapa[n-1][m-1];

    for (int i = n-1; i >= 0; --i) {
        for (int j = m-1; j >= 0; --j) {
            if (i == n-1 && j == m-1) continue;
            int min_cost = MAX_COST;
            for (int k = 0; k < 3; ++k) {
                int ni = i + dx[k], nj = j + dy[k];
                if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                    min_cost = min(min_cost, memo[ni][nj]);
                }
            }
            memo[i][j] = min_cost + mapa[i][j];
        }
    }
    return memo;
}

// Función para precalcular la cota optimista utilizando vectores de mínimos por filas y columnas
vector<vector<int>> precalcular_cota_optimista(const vector<vector<int>>& mapa, int n, int m) {
    vector<int> min_col(m, INT_MAX), min_row(n, INT_MAX);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            min_col[j] = min(min_col[j], mapa[i][j]);
            min_row[i] = min(min_row[i], mapa[i][j]);
        }
    }

    vector<vector<int>> cota_optimista(n, vector<int>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int cota_col = 0, cota_row = 0;
            for (int k = j + 1; k < m; ++k) cota_col += min_col[k];
            for (int k = i + 1; k < n; ++k) cota_row += min_row[k];
            cota_optimista[i][j] = max(cota_col, cota_row);
        }
    }

    return cota_optimista;
}

// Función para calcular la cota optimista actualizada en base a la posición actual y el costo acumulado
int calcular_cota_optimista_actualizada(int row, int col, int cost, const vector<vector<int>>& cota_optimista) {
    return cost + cota_optimista[row][col];
}

// Función para calcular la heurística de un nodo
// Toma el máximo entre la cota optimista actualizada y la cota pesimista
int calcular_heuristica(int row, int col, int cost, const vector<vector<int>>& cota_optimista, const vector<vector<int>>& cota_pesimista) {
    int optimistic_bound = calcular_cota_optimista_actualizada(row, col, cost, cota_optimista);
    int pessimistic_bound = cota_pesimista[row][col];
    return max(optimistic_bound, pessimistic_bound);
}

// Función para generar los hijos de un nodo y agregarlos a la cola de prioridad
void generateChildren(const Node& node, const vector<vector<int>>& mapa, int n, int m, priority_queue<Node, vector<Node>, CompareNode>& pq, vector<vector<bool>>& visited, const vector<vector<int>>& cota_optimista, const vector<vector<int>>& cota_pesimista, int& bestCost) {
    int row = get<0>(node), col = get<1>(node), cost = get<2>(node);
    const vector<int>& path = get<5>(node);

    for (int i = 0; i < 8; ++i) {
        int new_row = row + dx[i], new_col = col + dy[i];
        if (new_row >= 0 && new_row < n && new_col >= 0 && new_col < m && !visited[new_row][new_col]) {
            int new_cost = cost + mapa[new_row][new_col];
            int heuristic = calcular_heuristica(new_row, new_col, new_cost, cota_optimista, cota_pesimista);

            if (heuristic < bestCost) {
                vector<int> new_path = path;
                new_path.push_back(movesMap[dx[i] + 1][dy[i] + 1]);
                pq.emplace(new_row, new_col, new_cost, heuristic, cota_pesimista[new_row][new_col], move(new_path));
            }
        }
    }
}

// Vector para almacenar los mejores movimientos encontrados
vector<int> bestMoves;

// Función asincrónica para explorar un nodo
void exploreNode(const Node& node, const vector<vector<int>>& mapa, int n, int m, priority_queue<Node, vector<Node>, CompareNode>& pq, vector<vector<bool>>& visited, const vector<vector<int>>& cota_pesimista, vector<vector<int>>& best_costs, int& bestCost, int& n_explored, int& n_leaf, int& n_unfeasible, int& n_not_promising, int& n_promising_but_discarded, int& n_best_solution_updated_from_leafs, const vector<vector<int>>& cota_optimista) {
    int row = get<0>(node);
    int col = get<1>(node);
    int cost = get<2>(node);
    int heuristic = get<3>(node);

    bool is_unfeasible = (row < 0 || row >= n || col < 0 || col >= m);
    bool is_discarded = (heuristic >= bestCost);
    bool is_leaf = (row == n - 1 && col == m - 1);
    bool is_not_promising = (cost >= best_costs[row][col]);

    if (!is_unfeasible && !is_discarded && !is_leaf && !is_not_promising) {
        visited[row][col] = true;
        ++n_explored;

        best_costs[row][col] = cost;
        generateChildren(node, mapa, n, m, pq, visited, cota_optimista, cota_pesimista, bestCost);
        visited[row][col] = false;
    }

    if (is_unfeasible) {
        ++n_unfeasible;
    } else if (is_discarded) {
        ++n_promising_but_discarded;
    } else if (is_leaf) {
        ++n_leaf;
        ++n_explored;
        if (cost < bestCost) {
            bestCost = cost;
            bestMoves = get<5>(node);
            ++n_best_solution_updated_from_leafs;
        }
    } else if (is_not_promising) {
        ++n_not_promising;
    }
}

// Función principal de Ramificación y Poda para encontrar el camino de costo mínimo
void mcp_bb(const vector<vector<int>>& mapa, int n, int m, int& bestCost, vector<vector<bool>>& visited, const vector<vector<int>>& cota_pesimista, vector<vector<int>>& best_costs, int& n_visit, int& n_explored, int& n_leaf, int& n_unfeasible, int& n_not_promising, int& n_promising_but_discarded, int& n_best_solution_updated_from_leafs, int& n_best_solution_updated_from_pessimistic_bound, const vector<vector<int>>& cota_optimista) {
    priority_queue<Node, vector<Node>, CompareNode> pq;
    int initial_heuristic = calcular_heuristica(0, 0, mapa[0][0], cota_optimista, cota_pesimista);
    Node initial_node = make_tuple(0, 0, mapa[0][0], initial_heuristic, cota_pesimista[0][0], vector<int>{});
    pq.push(initial_node);
    bestCost = INT_MAX;

    while (!pq.empty()) {
        Node node = pq.top();
        pq.pop();
        ++n_visit;

        future<void> fut = async(launch::async, exploreNode, node, ref(mapa), n, m, ref(pq), ref(visited), ref(cota_pesimista), ref(best_costs), ref(bestCost), ref(n_explored), ref(n_leaf), ref(n_unfeasible), ref(n_not_promising), ref(n_promising_but_discarded), ref(n_best_solution_updated_from_leafs), ref(cota_optimista));
        fut.wait();
    }
}

int main(int argc, char *argv[]) {
    bool show_path_2D = false, show_path_p = false;
    string filename;

    // Procesamiento de los argumentos de línea de comandos
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-p") {
            show_path_p = true;
        } else if (arg == "--p2D") {
            show_path_2D = true;
        } else if (arg == "-f" && i + 1 < argc) {
            filename = argv[++i];
        } else {
            cerr << "ERROR: unknown option " << arg << "\nUsage:\nmcp_bb [ -p] [ --p2D] -f filename\n";
            return 1;
        }
    }

    // Verificación de que se haya proporcionado un nombre de archivo
    if (filename.empty()) {
        cerr << "ERROR: missing filename.\nUsage:\nmcp_bb [ -p] [ --p2D] -f filename\n";
        return 1;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: can't open file: " << filename << ".\nUsage:\nmcp_bb [ -p] [ --p2D] -f filename\n";
        return 1;
    }

    clock_t start = clock();

    int n, m;
    vector<vector<int>> mapa = read_map(filename, n, m);
    vector<vector<int>> cota_pesimista = precalcular_cota_pesimista(mapa, n, m);
    vector<vector<int>> cota_optimista = precalcular_cota_optimista(mapa, n, m);

    int bestCost = INT_MAX;
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<vector<int>> best_costs(n, vector<int>(m, INT_MAX));

    // Variables para almacenar las estadísticas de la ejecución
    int n_visit = 0, n_explored = 0, n_leaf = 0, n_unfeasible = 0, n_not_promising = 0;
    int n_promising_but_discarded = 0, n_best_solution_updated_from_leafs = 0, n_best_solution_updated_from_pessimistic_bound = 0;

    // Llamada a la función principal de Ramificación y Poda
    mcp_bb(mapa, n, m, bestCost, visited, cota_pesimista, best_costs, n_visit, n_explored, n_leaf, n_unfeasible, n_not_promising, n_promising_but_discarded, n_best_solution_updated_from_leafs, n_best_solution_updated_from_pessimistic_bound, cota_optimista);

    clock_t end = clock();
    float time = float(end - start) * 1000 / CLOCKS_PER_SEC;

    // Impresión de los resultados
    cout << bestCost << endl;
    cout << n_visit << " " << n_explored << " " << n_leaf << " " << n_unfeasible << " " << n_not_promising << " "
         << n_promising_but_discarded << " " << n_best_solution_updated_from_leafs << " "
         << n_best_solution_updated_from_pessimistic_bound << endl;
    cout << fixed << setprecision(3) << time << endl;

    // Impresión del camino en formato 2D si se especificó la opción --p2D
    if (show_path_2D) {
        vector<vector<char>> path(n, vector<char>(m, '.'));
        int row = 0, col = 0;
        path[row][col] = 'x';

        for (int move : bestMoves) {
            if (move == 1) {
                --row;
            } else if (move == 2) {
                --row;
                ++col;
            } else if (move == 3) {
                ++col;
            } else if (move == 4) {
                ++row;
                ++col;
            } else if (move == 5) {
                ++row;
            } else if (move == 6) {
                ++row;
                --col;
            } else if (move == 7) {
                --col;
            } else if (move == 8) {
                --row;
                --col;
            }
            if (row >= 0 && row < n && col >= 0 && col < m) {
                path[row][col] = 'x';
            }
        }

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                cout << path[i][j];
            }
            cout << endl;
        }
        cout << bestCost << endl;
    }

    // Impresión del camino en formato de secuencia de movimientos si se especificó la opción -p
    if (show_path_p) {
        cout << "<";
        for (int move : bestMoves) {
            cout << move;
        }
        cout << ">" << endl;
    }

    return 0;
}
