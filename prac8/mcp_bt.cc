    // JAIME HERNÁNDEZ DELGADO 48776654W

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string.h>
#include <climits>


using namespace std;

// Lee el archivo del mapa
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

#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const int dx[] = {0, 1, 0, -1};  // Desplazamientos para los movimientos: derecha, abajo, izquierda, arriba
const int dy[] = {1, 0, -1, 0};

void backtrack(const vector<vector<int>>& mapa, vector<vector<bool>>& path, int x, int y, int cost, int& min_cost, int& nvisit, int& nexplored, int& nleaf, int& nunfeasible, int& nnot_promising) {
    int n = mapa.size();
    int m = mapa[0].size();

    if (x == n - 1 && y == m - 1) {
        // Encontrado el destino
        if (cost < min_cost) {
            min_cost = cost;
        }
        return;
    }

    nvisit++;

    if (path[x][y]) {
        // Nodo ya visitado, no es prometedor
        nnot_promising++;
        nleaf++;
        return;
    }

    path[x][y] = true;
    nexplored++;

    // Explorar los movimientos válidos
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && nx < n && ny >= 0 && ny < m) {
            backtrack(mapa, path, nx, ny, cost + mapa[nx][ny], min_cost, nvisit, nexplored, nleaf, nunfeasible, nnot_promising);
        } else {
            nunfeasible++;
        }
    }

    path[x][y] = false; // Backtracking
}

void mcp_bt(const vector<vector<int>>& mapa, vector<vector<bool>>& path, int& min_cost, int& nvisit, int& nexplored, int& nleaf, int& nunfeasible, int& nnot_promising) {    int n = mapa.size();
    int m = mapa[0].size();

    // Inicializar la matriz path con falsos
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            path[i][j] = false;
        }
    }

    min_cost = INT_MAX;
    nvisit = 0;
    nexplored = 0;
    nleaf = 0;
    nunfeasible = 0;
    nnot_promising = 0;

    backtrack(mapa, path, 0, 0, 0, min_cost, nvisit, nexplored, nleaf, nunfeasible, nnot_promising);
}

int main(int argc, char *argv[]) {
    bool show_path_2D = false;
    bool show_path_p = false;

    if (argc < 3) {
        cerr << "ERROR: missing filename.\n";
        cerr << "Usage:\n";
        cerr << "mcp bt [ -p] [ --p2D] -f fichero entrada\n";
        return 1;
    }

    string filename;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if(arg == "-p") {
            show_path_p = true;
        }
        if (arg == "--p2D") {
            show_path_2D = true;
        } else if (arg == "-f" && i + 1 < argc) {
            filename = argv[++i];
        } else {
            cerr << "ERROR: unknown option " << arg << "\n";
            cerr << "Usage:\n";
            cerr << "mcp bt [ -p] [ --p2D] -f fichero entrada\n";
            return 1;
        }
    }

    if (filename.empty()) {
        cerr << "ERROR: missing filename.\n";
        cerr << "Usage:\n";
        cerr << "mcp bt [ -p] [ --p2D] -f fichero entrada\n";
        return 1;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: can’t open file: " << filename << ".\n";
        cerr << "Usage:\n";
        cerr << "mcp bt [ -p] [ --p2D] -f fichero entrada\n";
        return 1;
    }

    clock_t t;
    t = clock();

    int n, m = 0;
    vector<vector<int>> mapa = read_map(filename, n, m);
    int min_cost, nvisit, nexplored, nleaf, nunfeasible, nnot_promising = 0;

    vector<vector<bool>> path(n, vector<bool>(m, false));

    mcp_bt(mapa, path, min_cost, nvisit, nexplored, nleaf, nunfeasible, nnot_promising);
    t = clock() - t;

    cout << min_cost << endl;
    cout << "<" << nvisit << " " << nexplored << " " << nleaf << " " << nunfeasible << " " << nnot_promising << ">" << endl; 
    cout << t;
    if (show_path_2D) {
    
    }

    if (show_path_p) {
    
    }

    return 0;
}


               
