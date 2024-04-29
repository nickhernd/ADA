    // JAIME HERNÁNDEZ DELGADO 48776654W

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string.h>
#include <climits>


using namespace std;

// Lee el archivo del mapa
vector<vector<int>> read_map(const string& filename) {
    ifstream file(filename);

    int n, m;
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

/*void printPath(const vector<vector<int>>& mapa, vector<vector<char>>& path) {
    int n = mapa.size();
    int m = mapa[0].size();

    // Inicializar la matriz path con '.'
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            path[i][j] = '.';
        }
    }

    int row = n - 1, col = m - 1;
    path[row][col] = 'x';

    while (row != 0 || col != 0) {
        int min_cost = INT_MAX;
        int next_row = row, next_col = col;

        // Evaluar los tres movimientos posibles: izquierda, arriba y diagonal
        if (col > 0) {
            int cost = mapa[row][col - 1];
            if (cost < min_cost) {
                min_cost = cost;
                next_row = row;
                next_col = col - 1;
            }
        }
        if (row > 0) {
            int cost = mapa[row - 1][col];
            if (cost < min_cost) {
                min_cost = cost;
                next_row = row - 1;
                next_col = col;
            }
        }
        if (row > 0 && col > 0) {
            int cost = mapa[row - 1][col - 1];
            if (cost <= min_cost) {
                min_cost = cost;
                next_row = row - 1;
                next_col = col - 1;
            }
        }

        row = next_row;
        col = next_col;
        path[row][col] = 'x';
    }
}*/

void mcp_bt() {

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

    vector<vector<int>> mapa = read_map(filename);
    int rows = mapa.size();
    int cols = mapa[0].size();
    int difficulty = 0;
    int nodos_visit = 0;
    int nodos_explored = 0;
    int nodos_leaf = 0;
    int nodos_unfeasable = 0;
    int nodos_notPromising = 0;

    vector<vector<char>> path(rows, vector<char>(cols, '.'));

    void mcp_bt(int &difficulty, int &nodos_visit, int &nodos_explored, int &nodos_leaf);
    t = clock() - t;

    cout << difficulty << endl;
    cout << nodos_visit << " " << nodos_explored << " " << nodos_leaf << " " << nodos_unfeasable << " " << nodos_notPromising << endl; 
    cout << t;
    if (show_path_2D) {
    
    }

    if (show_path_p) {
    
    }

    return 0;
}


               
