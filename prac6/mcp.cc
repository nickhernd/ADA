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


int mcp_memo(const vector<vector<int>>& mapa, int i, int j, vector<vector<int>>& memo) {
    int n = mapa.size();
    int m = mapa[0].size();

    if (i >= n || j >= m) 
        return INT_MAX;

    if (i == n - 1 && j == m - 1) return mapa[i][j];

    if (memo[i][j] != -1) return memo[i][j];

    int right = mcp_memo(mapa, i, j + 1, memo);
    int down = mcp_memo(mapa, i + 1, j, memo);
    int diagonal = mcp_memo(mapa, i + 1, j + 1, memo);

    memo[i][j] = mapa[i][j] + min({right, down, diagonal});
    
    return memo[i][j];
}


int mcp_it_matrix(vector<vector<int>> &mapa, int n, int m, vector<vector<int>>& Matrix) {
    
    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            int S1 = Matrix[i-1][j];
            int S2 = Matrix[i][j-1];
            int S3 = Matrix[i-1][j-1];
            Matrix[i][j] = min({S1, S2, S3}) + mapa[i][j];
        }
    }

    return Matrix[n-1][m-1];
}


int mcp_it_vector(vector<vector<int>> &mapa, int n, int m) {
    vector<int> actual(m);
    vector<int> siguiente(m);

    for (int i = 0; i < m; ++i) {
        actual[i] = mapa[0][i];
    }

    for (int s = 1; s < n; s++) {
        siguiente[0] = actual[0] + mapa[s][0];
        for (int i = 1; i < m; ++i) {
            int S1;
            if (s > 0) {
                S1 = actual[i];
            } else {
                S1 = INT_MAX;
            }

            int S2;
            if (i > 0) {
                S2 = siguiente[i - 1];
            } else {
                S2 = INT_MAX;
            }

            int S3;
            if (s > 0 && i > 0) {
                S3 = actual[i - 1];
            } else {
                S3 = INT_MAX;
            }

            siguiente[i] = mapa[s][i] + min({S1, S2, S3});
            
        }
        swap(actual, siguiente);
    }

    return actual[m-1];
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
        if (arg == "-t") {
            show_table = true;
        } else if (arg == "--p2D") {
            show_path_2D = true;
        } else if (arg == "--ignore-naive") {
            ignore_naive = true;
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
    vector<vector<int>> memo_map(rows, vector<int>(cols, -1));

    if (!ignore_naive) {
        int naive = mcp_naive(mapa, 0, 0);
        cout << naive << " ";
    } else {
        cout << "-" << " ";
    }

    int memo = mcp_memo(mapa, 0, 0, memo_map);
    cout << memo << " ";

    vector<vector<int>> Matrix(rows, vector<int>(cols));

    Matrix[0][0] = mapa[0][0];
    for (int i = 1; i < rows; ++i) {
        Matrix[i][0] = Matrix[i-1][0] + mapa[i][0];
    }
    for (int j = 1; j < cols; ++j) {
        Matrix[0][j] = Matrix[0][j-1] + mapa[0][j];
    }

    int matrix = mcp_it_matrix(mapa, rows, cols, Matrix);
    cout << matrix << " ";
    int vectorit = mcp_it_vector(mapa, rows, cols);
    cout << vectorit << endl;

    vector<vector<char>> path(rows, vector<char>(cols, '.'));
    int parser = mcp_parser(mapa, path);

    if (show_path_2D) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cout << path[i][j];
            }
        cout << endl;
        }
        cout << parser << endl;
    }

    if (show_table) {
        for(int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << Matrix[i][j] << " ";
            }
        cout << endl;
        }
    }

    return 0;
}


               
