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

void mcp_greedy(const vector<vector<int>>& mapa, int &forward, int &backward, vector<vector<char>>& path) {
    int n = mapa.size();
    int m = mapa[0].size();

    // Avanzando desde el origen hasta el destino
    forward = mapa[0][0];
    int row = 0, col = 0;
    path[row][col] = 'x';

    while (row != n - 1 || col != m - 1) {
        int min_cost = INT_MAX;
        int next_row = row;
        int next_col = col;

        if (row < n - 1 && col < m - 1) {
            if (mapa[row + 1][col + 1] < min_cost) {
                min_cost = mapa[row + 1][col + 1];
                next_row = row + 1;
                next_col = col + 1;
            }
        }

        if (col < m - 1) {
            if (mapa[row][col + 1] < min_cost) {
                min_cost = mapa[row][col + 1];
                next_row = row;
                next_col = col + 1;
            }
        }

        if (row < n - 1) {
            if (mapa[row + 1][col] < min_cost) {
                min_cost = mapa[row + 1][col];
                next_row = row + 1;
                next_col = col;
            }
        }

        row = next_row;
        col = next_col;
        path[row][col] = 'x';
        forward += mapa[row][col];
    }

    // Retrocediendo desde el destino hasta el origen
    backward = mapa[n - 1][m - 1];
    row = n - 1, col = m - 1;

    while (row != 0 || col != 0) {
        int min_cost = INT_MAX;
        int prev_row = row;
        int prev_col = col;

        if (row > 0 && col > 0) {
            if (mapa[row - 1][col - 1] < min_cost) {
                min_cost = mapa[row - 1][col - 1];
                prev_row = row - 1;
                prev_col = col - 1;
            }
        }

        if (col > 0) {
            if (mapa[row][col - 1] < min_cost) {
                min_cost = mapa[row][col - 1];
                prev_row = row;
                prev_col = col - 1;
            }
        }

        if (row > 0) {
            if (mapa[row - 1][col] < min_cost) {
                min_cost = mapa[row - 1][col];
                prev_row = row - 1;
                prev_col = col;
            }
        }

        row = prev_row;
        col = prev_col;
        backward += mapa[row][col];
    }
}



int main(int argc, char *argv[]) {
    bool show_path_2D = false;

    if (argc < 3) {
        cerr << "ERROR: missing filename.\n";
        cerr << "Usage:\n";
        cerr << "mcp_greedy [--p2D] -f file\n";
        return 1;
    }

    string filename;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--p2D") {
            show_path_2D = true;
        } else if (arg == "-f" && i + 1 < argc) {
            filename = argv[++i];
        } else {
            cerr << "ERROR: unknown option " << arg << "\n";
            cerr << "Usage:\n";
            cerr << "mcp_greedy [--p2D] -f file\n";
            return 1;
        }
    }

    if (filename.empty()) {
        cerr << "ERROR: missing filename.\n";
        cerr << "Usage:\n";
        cerr << "mcp_greedy [--p2D] -f file\n";
        return 1;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: can’t open file: " << filename << ".\n";
        cerr << "Usage:\n";
        cerr << "mcp_greedy [--p2D]    -f file\n";
        return 1;
    }

    vector<vector<int>> mapa = read_map(filename);
    int rows = mapa.size();
    int cols = mapa[0].size();
    int forward = 0;
    int backward = 0;

    vector<vector<char>> path(rows, vector<char>(cols, '.'));
    mcp_greedy(mapa, forward, backward, path);
    cout << forward << " " << backward << endl;

    if (show_path_2D) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cout << path[i][j];
            }
        cout << endl;
        }
        cout << min(forward, backward) << endl;
    }

    return 0;
}


               
