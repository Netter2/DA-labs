#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

const int MAX_EDGE_COST = 1000000000; //максимальный вес ребра
long long EK(const vector <vector <int>> & edges, vector <vector <int>> & weights) {//вектора ребер и весов ребер
    long long answer = 0;//максимальный поток
    bool f = true;//флаг проверки окончания повторения итераций
    int n = edges.size () - 1;//количество вершин
    while (f) {
        f = false;//обнуление флага
        queue <int> q;//очередь обрабатываемых вершин
        vector <int> visited (n + 1);//вектор посещенных вершин
        q.push (1);//инициация очереди
        while (!q.empty ()) {//пока в очереди имеются вершины для обработки
            int t = q.front ();//сохраняем вершину для обработки
            q.pop ();          //
            if (t == n) {//если эта вершина - сток
                int mini = MAX_EDGE_COST;//нахождение минимального веса в найденном пути
                int curr = n;            //
                while (curr != 1) {      //
                    mini = min (mini, weights [visited [curr]][curr]);//
                    curr = visited [curr];//
                }
                curr = n;
                while (curr != 1) {//вычет минимального веса из всех ребер найденного пути
                    weights [visited [curr]][curr] -= mini;//
                    weights [curr][visited [curr]] += mini;//
                    curr = visited [curr];//
                }
                answer += mini;//увеличение максимального потока на то же значение
                f = true;//разрешение следующей итерации
                break;
            }
            for (int i = 0; i < edges [t].size (); ++ i) {//для всех вершин, связанных с текущей
                if ((weights [t] [edges [t][i]] != 0) && (visited [edges [t][i]] == 0)) {//если ребро существует (учитывая направление) и еще не включено в путь
                    visited [edges [t][i]] = t;//добавляем ребро в путь
                    q.push (edges [t][i]);//добавляем вершину, в которую ведет ребро в очередь обработки. Тк алгоритм работает в ширину - добавляем в конец очереди.
                }
            }
        }
    }
    return answer;
}

int main() {
    int n, m;
    cin >> n >> m;
    vector <vector <int>> edges (n + 1);//вектор ребер
    vector <vector <int>> weights (n + 1, vector <int> (n + 1));//вектор весов
    for (int i = 0; i < m; ++ i) {//считывание
        int v, u, cost;           //
        cin >> v >> u >> cost;    //
        edges [v].push_back(u);//составление списка связанных между собой вершин
        edges [u].push_back(v);//
        weights [v][u] = cost;//составление таблицы весов (с учетом направления)
    }
    long long answer = EK (edges, weights);
    cout << answer << endl;
    return 0;
}