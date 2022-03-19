#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <ctime>

using namespace std;
 
vector <int> Zfunc (vector <string> input) {
    vector <int> Z;
    for (int i = 0; i < int (input.size ()); i ++) {
        Z.push_back (0);
    }
    int count;
    int l = 0;
    int r = 0;
    for (int i = 1; i < int (input.size ()); i ++) {
        count = 0;
        int k = i;
        int j = 0;
        if ((l <= i) && (i <= r)) {
            if (r - i >= Z [i - l]) {
                Z [i] = Z [i - l];
            }
            else {
                l = k;
                count += r - k + 1;
                j += r - k + 1;
                k = r + 1;

                if ((k < int (input.size ())) && (input [k] == input [j])) {
                    l = i;
                }
 
                while ((k < int (input.size ())) && (input [k] == input [j])) {
                    r = k;
                    k ++;
                    j ++;
                    count ++;
                }
            }
        }
        else {
            if ((k < int (input.size ())) && (input [k] == input [j])) {
                l = i;
            }
 
            while ((k < int (input.size ())) && (input [k] == input [j])) {
                r = k;
                k ++;
                j ++;
                count ++;
            }
        }
        Z [i] = count;
    }
    return Z;
}
 
struct TPos {
    int line= 1;
    int col = 1;
};

int main () {
    vector <string> p;
    vector <string> pr;
    vector <string> t;
    vector <int> N;
    vector <int> L;
    vector <int> M;
    vector <TPos> place;
    TPos pl;
    int j1 = 0;
    string s;
    int l = 0;
    char c = 1;
    bool f = false;
    int plus = 1;
 
    while (c > 0) {
        c = getchar();  
        
        if (c >= 65 && c <= 90) {
            s += c;
            f = true;
        }
 
        if (c >= 97 && c <= 122) {
            c -= 32; 
            s += c;
            f = true;
        }
 
        if (f && ((c == '\t') || (c == ' '))) {
            p.push_back (s);
            s = "";
            f = false;
        }
 
        if (f && ((c == '\n') || (c == EOF))) {
            p.push_back (s);
            s = "";
            f = false;
            break;
        }
 
        if (!f && ((c == '\n') || (c == EOF))) {
            break;
        }
    }
    
    f = false;
    if (int (p.size ()) > 0) {
    while (c > 0) {
        c = getchar ();
        if (c == '\n') {
            if (f) {
                t.push_back (s);
                place.push_back (pl);
                s = "";
                f = false;
            }
            pl.line++;
            pl.col = 1;
        }
 
        if (c == EOF) {
            if (f) {
                t.push_back (s);
                place.push_back (pl);
                s = "";
                f = false;
            }
            pl.line++;
            pl.col = 1;
            break;
        }
 
        if ((c == ' ') || (c == '\t')) {
            if (f) {
                t.push_back (s);
                place.push_back (pl);
                s = "";
                f = false;
                pl.col ++;
            }
            
        }
 
        if (c >= 65 && c <= 90) {
            s += c;
            f = true;
        }
 
        if (c >= 97 && c <= 122) {
            c -= 32;
            s += c;
            f = true;
        }
    }
    if (t.size () >= p.size ()) {

    f = false;
    pr = p;
    reverse (pr.begin (), pr.end ());
    N = Zfunc (pr);
    reverse (N.begin (), N.end ());

    for (int i = 0; i < int (N.size ()); i ++) {
        L.push_back (-1);
    } 
 
    for (int i = 0; i < int (t.size ()); i ++) {
        M.push_back (-1);
    } 
 
    for (int i = 0; i < int (N.size ()); i ++) {
        int j = N.size () - N [i];
        if (j < int (N.size ())) {
            L [j] = i;
        }
    }
 
    for (int i = int (p.size ()); i > 0; i --) {
        int j = int (p.size ()) - i + 1;
        if (N [j - 1] == j) {
            l = max (j, l);
        }
    }

    map <string, int> bsv;
    for (int i = 0; i < int (p.size ()); i++) {
        bsv[p [i]] = i;
    }
 
    int i = 0;
        while (i <= int (t.size ()) - int (p.size ())) {
            
            int j = int (p.size ()) - 1;
            int h = i + j;
 
            while (1) {
                if ((M [h] == -1) || (M [h] == 0)) {//1
                    if ((t [h] == p [j]) && (j == 0)) {
                        f = true;
                        M [i + int (p.size ()) - 1] = int (p.size ());
                        break;
                    }
                    else if ((t [h] == p [j]) && (j > 0)) {
                            h --;
                            j --;
                    } 
                    else if (t [h] != p [j]) {
                        M [i + int (p.size ()) - 1] = i + int (p.size ()) - 1 - h;
                        f = false;
                        j1 = j;
                        break;
                    }
                }
                else if (M [h] < N [j]) {//2
                    j -= M [h];
                    h -= M [h];
                }
                else if ((M [h] >= N [j]) && (N [j] == j + 1) && (j + 1 > 0)) {//3
                    f = true;                
                    M [i + int (p.size ()) - 1] = i + int (p.size ()) - 1 - h;
                    break;
                }
                else if ((M [h] > N [j]) && (N [j] < j + 1)) {//4
                    M [i + int (p.size ()) - 1] = i + int (p.size ()) - 1 - h;
                    f = false;
                    j1 = j - N [j];
                    break;
                }
                else if ((M [h] == N [j]) && (0 <= N [j]) && (N [j] < j + 1)) {//5
                    j -= M [h];
                    h -= M [h];
                }
                else {
                    break;
                }
            }

            if (f) {
                cout << place [i].line<< ", "<< place [i].col << endl;
                i += int (p.size ()) - l;
            } else {
                if (bsv.count (t [j1 + i]) > 0) {
                    int x = (*bsv.find (t [j1 + i])).second;
                    if (x > 1) {
                        plus = max (j1 - x, plus);
                    }
                }
                else {
                    plus = max (j1 + 1, plus);
                }

                if ((j1 + 1 >= int (p.size ())) || ((L [j1 + 1]) == -1)) {
                    plus = max (1, plus);
                } else {
                    plus = max ((int (p.size ()) - 1 - L [j1 + 1]), plus);
                }            
 
                plus = max (1, plus);
                if (i + plus + int (p.size ()) <= int (t.size ())) {
                    i += plus;
                } 
                else {
                    break;
                }
                plus = 1;
            }
            f = false;
        }  
    }
    }
    return 0;
}