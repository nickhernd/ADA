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
    // Obtener las dimensiones del mapa
    int rows = mapa.size();
    int cols = mapa[0].size();

    // Caso base: si estamos fuera de los límites del mapa, retornar infinito
    if (m < 0 || n < 0 || m >= rows || n >= cols) {
        return INT_MAX;
    }

    // Caso base: si estamos en la casilla destino, retornar el valor de la casilla
    if (m == rows - 1 && n == cols - 1) {
        return mapa[m][n];
    }

    // Calcular recursivamente el camino mínimo moviéndose hacia abajo, derecha y diagonalmente
    int move_down = mcp_naive(mapa, m + 1, n);
    int move_right = mcp_naive(mapa, m, n + 1);
    int move_diagonal = mcp_naive(mapa, m + 1, n + 1);

    // Devolver el mínimo de los tres movimientos más la dificultad actual
    return mapa[m][n] + min({move_down, move_right, move_diagonal});
}


int mcp_memo(vector<vector<int>>& map, vector<vector<int>>& memo) {
        
        int rows = map.size();

        if(rows==0)
            return 0;

        int cols = map[0].size();

        //vector<vector<int>> memo(rows,vector<int>(cols,0));
        int i,j = 0;
        
        memo[0][0] = map[0][0];  
        
        for(i=1;i<cols;++i)
            memo[0][i] = memo[0][i-1] + map[0][i];
        
        for(i=1;i<rows;++i)
            memo[i][0] = memo[i-1][0] + map[i][0];
        
        
        for(i=1;i<rows;++i) {
            for(j=1;j<cols;++j)
                memo[i][j] = map[i][j] + min({memo[i-1][j],memo[i][j-1], memo[i-1][j-1]});
        }
        
    
    return memo[rows-1][cols-1];
}

int mcp_it_matrix(vector<vector<int>> &mapa, int n, int m) {
    vector<vector<int>> Matrix(n, vector<int>(m));

    Matrix[0][0] = mapa[0][0];
    for (int i = 1; i < n; ++i) {
        Matrix[i][0] = Matrix[i-1][0] + mapa[i][0];
    }
    for (int j = 1; j < m; ++j) {
        Matrix[0][j] = Matrix[0][j-1] + mapa[0][j];
    }

    
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

    // Inicializar el vector actual con los valores de la primera fila del mapa
    for (int i = 0; i < m; ++i) {
        actual[i] = mapa[0][i];
    }

    for (int s = 1; s < n; s++) {
        // Calcular el primer elemento del vector siguiente
        siguiente[0] = actual[0] + mapa[s][0];

        // Calcular los valores del vector siguiente para cada columna de la fila actual
        for (int i = 1; i < m; ++i) {
            int S1 = (s > 0) ? actual[i] : INT_MAX; // Valor superior
            int S2 = (i > 0) ? siguiente[i - 1] : INT_MAX; // Valor izquierdo
            int S3 = (s > 0 && i > 0) ? actual[i - 1] : INT_MAX; // Valor diagonal superior izquierdo

            // Calcular el mínimo entre los tres valores posibles para llegar a la posición (s, i)
            siguiente[i] = mapa[s][i] + min({S1, S2, S3});
        }

        // Actualizar el vector actual con los valores del vector siguiente para la siguiente iteración
        swap(actual, siguiente);
    }

    // Devolver el valor correspondiente a la última celda del camino mínimo
    return actual[m - 1];
}


void print_path(int rows, int cols, vector<vector<int>>& memo_map) {
    vector<vector<char>> path(rows, vector<char>(cols, '.'));

    // Seguir el camino mínimo desde el final hasta el inicio
    int current_row = rows - 1;
    int current_col = cols - 1;
    while (current_row != 0 || current_col != 0) {
        // Marcar la casilla actual como parte del camino mínimo
        path[current_row][current_col] = 'x';
        
        // Determinar la siguiente casilla en el camino mínimo
        int min_neighbor = INT_MAX;
        int next_row = current_row;
        int next_col = current_col;

        // Buscar el vecino con el valor mínimo en la tabla de memoización
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                int neighbor_row = current_row + i;
                int neighbor_col = current_col + j;
                if (neighbor_row >= 0 && neighbor_row < rows && neighbor_col >= 0 && neighbor_col < cols) {
                    if (memo_map[neighbor_row][neighbor_col] < min_neighbor) {
                        min_neighbor = memo_map[neighbor_row][neighbor_col];
                        next_row = neighbor_row;
                        next_col = neighbor_col;
                    }
                }
            }
        }

        // Actualizar las coordenadas actuales
        current_row = next_row;
        current_col = next_col;
    }

    // Marcar la casilla inicial como parte del camino mínimo
    path[0][0] = 'x';

    // Mostrar el camino mínimo en formato 2D
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << path[i][j];
        }
        cout << endl;
    }
}

//Función principal
int main(int argc, char *argv[]) {
    //cambiar show table
    bool show_table = false;
    bool show_path_2D = false;
    bool ignore_naive = false;

    // Verificar las opciones de la línea de comandos
    if (argc < 3) {
        cerr << "ERROR: missing filename.\n";
        cerr << "Usage:\n";
        cerr << "mcp [--p2D] [-t] [--ignore-naive] -f file\n";
        return 1;
    }

    // Variables para almacenar las opciones y el nombre del archivo
    string filename;

    // Procesar las opciones de la línea de comandos
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

    // Verificar que se haya proporcionado el nombre del archivo
    if (filename.empty()) {
        cerr << "ERROR: missing filename.\n";
        cerr << "Usage:\n";
        cerr << "mcp [--p2D] [-t] [--ignore-naive] -f file\n";
        return 1;
    }

    // Intentar abrir el archivo
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "ERROR: can’t open file: " << filename << ".\n";
        cerr << "Usage:\n";
        cerr << "mcp [--p2D] [-t] [--ignore-naive] -f file\n";
        return 1;
    }

    // Leer el mapa desde el archivo de entrada
    vector<vector<int>> mapa = read_map(filename);
    int rows = mapa.size();
    int cols = mapa[0].size();
    vector<vector<int>> memo_map(rows, vector<int>(cols, -1));

    // Calcular la dificultad del camino más favorable utilizando los algoritmos
    if (!ignore_naive) {
        int naive = mcp_naive(mapa, 0, 0);
        cout << naive << " ";
    } else {
        cout << "-" << " ";
    }

    int memo = mcp_memo(mapa, memo_map);
    cout << memo << " ";
    int matrix = mcp_it_matrix(mapa, rows, cols);
    cout << matrix << " ";
    int vector = mcp_it_vector(mapa, rows, cols);
    cout << vector << endl;

    if (show_path_2D) {
        print_path(rows, cols, memo_map);
        cout << matrix << endl;
    }

    if (show_table) {
        for(int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << memo_map[i][j] << " ";
            }
        cout << endl;
        }
    }

    return 0;
}


               
