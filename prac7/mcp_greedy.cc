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

int mcp_naive(vector<vector<int>>& mapa, int m, int n) {
    int rows = mapa.size();
    int cols = mapa[0].size();

    if (m < 0 || n < 0 || m >= rows || n >= cols) {
        return INT_MAX;
    }

    if (m == rows - 1 && n == cols - 1) {
        return mapa[m][n];
    }

    int move_down = mcp_naive(mapa, m + 1, n);
    int move_right = mcp_naive(mapa, m, n + 1);
    int move_diagonal = mcp_naive(mapa, m + 1, n + 1);

    return mapa[m][n] + min({move_down, move_right, move_diagonal});
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

int mcp_greedy(const vector<vector<int>>& map) {
    int n = map.size();
    int m = map[0].size();
    int cost = 0;

    int i = 0, j = 0;
    while (i < n - 1 || j < m - 1) {
        cost += map[i][j];
        int min_cost = map[i][j];
        int next_i = i, next_j = j;

        // Evaluar los tres movimientos posibles: derecha, abajo y diagonal
        if (j < m - 1 && map[i][j + 1] < min_cost) {
            min_cost = map[i][j + 1];
            next_j = j + 1;
        }
        if (i < n - 1 && map[i + 1][j] < min_cost) {
            min_cost = map[i + 1][j];
            next_i = i + 1;
            next_j = j;
        }
        if (i < n - 1 && j < m - 1 && map[i + 1][j + 1] < min_cost) {
            min_cost = map[i + 1][j + 1];
            next_i = i + 1;
            next_j = j + 1;
        }

        i = next_i;
        j = next_j;
    }
    cost += map[i][j];
    return cost;
}

int main(int argc, char *argv[]) {
    bool show_table = false;
    bool show_path_2D = false;
    bool ignore_naive = false;

    if (argc < 3) {
        cerr << "ERROR: missing filename.\n";
        cerr << "Usage:\n";
        cerr << "mcp [--p2D] [-t] [--ignore-naive] -f file\n";
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
            cerr << "mcp [--p2D] [-t] [--ignore-naive] -f file\n";
            return 1;
        }
    }

    if (filename.empty()) {
        cerr << "ERROR: missing filename.\n";
        cerr << "Usage:\n";
        cerr << "mcp [--p2D] [-t] [--ignore-naive] -f file\n";
        return 1;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: can’t open file: " << filename << ".\n";
        cerr << "Usage:\n";
        cerr << "mcp [--p2D] [-t] [--ignore-naive] -f file\n";
        return 1;
    }

    vector<vector<int>> mapa = read_map(filename);
    int rows = mapa.size();
    int cols = mapa[0].size();

    vector<vector<char>> path(rows, vector<char>(cols, '.'));
    int parser = mcp_parser(mapa, path);
    int greedy = mcp_greedy(mapa);
    cout << parser << " " << greedy << endl;

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


               
