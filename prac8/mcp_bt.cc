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

// Cota optimista -> programacion dinamica
// Almacenar en el vector los movimientos, para hacer el árbol
// Un nodo hoja será cuando se llegue al final
//          En caso contrario seguir expandiendo

/*
mcp_bt(mapa, fila, columna)

    solucion parcial
        variables --> vector<vector<bool>> &visited, unsigned x, unsigned y, unsigned current_cost
        Posible uso -> hacer un vector de los movimientos
            problema: que se de bucle, ej (-> | -> | <-)
            solución: vector<vector<bool>> &visited
        if(x == n-1 && y = m-1) {
            //Fulla
        }

    Mejor solucion hasta el momento
        &bestCost, vector<vector<bool>> &bestPath --> bestCost inicializarlo a infinito
        
    enum Step{N, S, E, O, NO, NE, SE, SO} --> 

            Ejemplo: switch(i) {
                case N:
                    incx=-1;
                    incy =1;
                    break;
                case NE:
                    (...)
                    break;
            }
        Importante  --> mirar que no nos salimos del mapa
            En caso de estar dentro del mapa comprobar que no haya sido visitada

            if(!visited[newx][newy]...) {
                visited[newx][newy] = true;
                mcp_bt(..., newx, newy, visited, currentCost + mapa[newx][newy])
            }

    Mejoras:
        Cota pesimista inicial -> programacion dinámica con 3 movimientos (práctica 6)
        Cotas optimista:
            1) Camino actual -> guardar el valor minimo de los caminos y compararlo con el camino que se esta realizando, actual > anterior :: poda
            2) Asumir que las casillas que quedan max(n-1-x, m-1-y) * (minimo valor del mapa --> cogerlo cuando de se hace un registro del mapa)
            3) Ma x(suma(min_columnas_restantes), suma(min_filas_restante)) --> no hacer (hacer para el final o la siguiente práctica)
            4) Hacer una matriz en la que cada casilla tenga su coste minimo donde se va almacenando --> mejor cota (poda por memoizacion)
                    Antes rellenar la matriz con programción dinamica con tres movimientos como valores de inicio.

*/

int poda_memo(const vector<vector<int>>& mapa, int i, int j, vector<vector<int>>& memo) {
    int n = mapa.size();
    int m = mapa[0].size();

    // Inicializar memo si aún no tiene el tamaño adecuado
    if (memo.empty()) {
        memo.resize(n, vector<int>(m, -1));
    }

    // Verificar los límites de la matriz mapa y memo
    if (i >= n || j >= m || i < 0 || j < 0) {
        return INT_MAX;
    }
        
    if (i == n - 1 && j == m - 1) {
        return mapa[i][j];
    }
    
    if (memo[i][j] != -1) {
        return memo[i][j];
    }
    
    int right = poda_memo(mapa, i, j + 1, memo);
    int down = poda_memo(mapa, i + 1, j, memo);
    int diagonal = poda_memo(mapa, i + 1, j + 1, memo);
    return 0;

    memo[i][j] = mapa[i][j] + min({right, down, diagonal});
    
    return memo[i][j];
}



int mcp_bt(vector<vector<int>>& mapa, int fila_objetivo, int columna_objetivo, int fila_actual, int columna_actual, vector<vector<int>>& memo, vector<vector<bool>>& path, int& Step, int& difficulty) {
    // Definir los movimientos posibles en el tablero
    static const int movimientos[8][2] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}, {-1, -1}, {1, -1}, {-1, 1}, {1, 1}};

    // Aplicar el movimiento correspondiente al paso actual
    fila_actual += movimientos[Step - 1][0];
    columna_actual += movimientos[Step - 1][1];

    // Si se alcanza la posición objetivo, retornar la dificultad acumulada hasta el momento
    if (fila_actual == fila_objetivo && columna_actual == columna_objetivo) {
        return difficulty + mapa[fila_actual][columna_actual];
    }

    // Verificar límites del tablero
    if (fila_actual < 0 || fila_actual >= mapa.size() || columna_actual < 0 || columna_actual >= mapa[0].size()) {
        return INT_MAX;
    }

    // Verificar si la posición actual ya ha sido visitada
    if (path[fila_actual][columna_actual]) {
        return INT_MAX;
    }

    // Verificar si se supera la cota superior almacenada en memo
    if (memo[fila_actual][columna_actual] < difficulty) {
        return INT_MAX;
    }

    // Marcar la posición actual como visitada
    path[fila_actual][columna_actual] = true;

    // Intentar realizar cada movimiento posible recursivamente
    int min_difficulty = INT_MAX;
    for (int i = 1; i <= 8; ++i) {
        int new_difficulty = mcp_bt(mapa, fila_objetivo, columna_objetivo, fila_actual, columna_actual, memo, path, i, difficulty);
        if (new_difficulty < min_difficulty) {
            min_difficulty = new_difficulty;
        }
    }

    // Desmarcar la posición actual antes de regresar
    path[fila_actual][columna_actual] = false;

    // Actualizar la cota superior en memo
    memo[fila_actual][columna_actual] = min_difficulty;

    return min_difficulty;
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
    vector<vector<int>> mapa, memo_matrix = read_map(filename, n, m);
    int difficulty, nvisit, nexplored, nleaf, nunfeasible, nnot_promising = 0;
    int fila_actual, columna_actual, steps = 0;

    vector<vector<bool>> path(n, vector<bool>(m, false));
    
    poda_memo(mapa, 0, 0, memo_matrix);
    return 0;
    mcp_bt(mapa, n ,m, fila_actual, columna_actual, memo_matrix, path, steps , difficulty);
    t = clock() - t;

    cout << difficulty << endl;
    //cout << "<" << nvisit << " " << nexplored << " " << nleaf << " " << nunfeasible << " " << nnot_promising << ">" << endl; 
    cout << t;
    if (show_path_2D) {
    
    }

    if (show_path_p) {
    
    }

    return 0;
}


               
