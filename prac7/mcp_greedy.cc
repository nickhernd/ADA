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

int mcp_parser(vector<vector<int>>& mapa, vector<vector<char>>& path) {
    int n = mapa.size();
    int m = mapa[0].size();
    
    vector<vector<int>> map_parser(n, vector<int>(m, INT_MAX));
    map_parser[0][0] = mapa[0][0];

    for (int j = 1; j < m; ++j) {
        map_parser[0][j] = map_parser[0][j - 1] + mapa[0][j];
    }

    for (int i = 1; i < n; ++i) {
        map_parser[i][0] = map_parser[i - 1][0] + mapa[i][0];
    }

    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            map_parser[i][j] = mapa[i][j] + min({map_parser[i - 1][j], map_parser[i][j - 1], map_parser[i - 1][j - 1]});
        }
    }

    int row = n - 1, col = m - 1;
    path[0][0] = 'x';

    while (row > 0 || col > 0) {
        if (row == 0) {
            col--;
        } else if (col == 0) {
            row--;
        } else {
            if (map_parser[row - 1][col] <= min(map_parser[row][col - 1], map_parser[row - 1][col - 1])) {
                row--;
            } else if (map_parser[row][col - 1] <= min(map_parser[row - 1][col], map_parser[row - 1][col - 1])) {
                col--;
            } else {
                row--;
                col--;
            }
        }
        path[row][col] = 'x';
    }

    path[n-1][m-1] = 'x';

    return map_parser[n - 1][m - 1];
}

void mcp_greedy(const vector<vector<int>>& mapa, int &forward, int &backward) {
    int n = mapa.size();
    int m = mapa[0].size();

    // Avanzando desde el origen hasta el destino
    forward = mapa[0][0];
    int row = 0, col = 0;
    while (row != n - 1 || col != m - 1) {
        if (row == n - 1) {
            col++;
        } else if (col == m - 1) {
            row++;
        } else {
            if (mapa[row + 1][col] < mapa[row][col + 1]) {
                row++;
            } else {
                col++;
            }
        }
        forward += mapa[row][col];
    }

    // Retrocediendo desde el destino hasta el origen
    backward = mapa[n - 1][m - 1];
    row = n - 1, col = m - 1;
    while (row != 0 || col != 0) {
        if (row == 0) {
            col--;
        } else if (col == 0) {
            row--;
        } else {
            if (mapa[row - 1][col] < mapa[row][col - 1]) {
                row--;
            } else {
                col--;
            }
        }
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
    mcp_greedy(mapa, forward, backward);
    cout << forward << " " << backward << endl;

    if (show_path_2D) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cout << path[i][j];
            }
        cout << endl;
        }
        cout << parser << endl;
    }

    return 0;
}


               
