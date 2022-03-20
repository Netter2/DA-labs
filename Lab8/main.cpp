#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

int main () {
    int n;
    cin >> n;
    long long int a = 0, b = 0, c = 0;
    long long int bestA = 0, bestB = 0, bestC = 0;
    double bestS = -1;
    vector <long long int> lines (n);
    for (int i = 0; i < n; i ++) {
        cin >> lines [i];
    }
    if (n >= 3) {    
        sort (lines.begin (), lines.end ());
        for (int i = n - 1; i >= 2; i --) {
            if (lines [i] < lines [i - 1] + lines [i - 2]) {
                a = lines [i];
                b = lines [i - 1];
                c = lines [i - 2];
                //break;
            }
            if ((a != 0) && (b != 0) && (c != 0) && (a < b + c)) {//
                double p = double ((a + b + c)) / 2;
                double s = double (sqrt (p * (p - a) * (p - b) * (p - c)));
                if (s > bestS) {
                    bestS = s;
                    bestA = a;
                    bestB = b;
                    bestC = c;
                }
            }//
        }
        /*if ((a != 0) && (b != 0) && (c != 0) && (a < b + c)) {
            double p = double ((a + b + c)) / 2;
            double s = double (sqrt (p * (p - a) * (p - b) * (p - c)));
            printf ("%.3lf\n", s);
            cout << c << " " << b << " " << a << endl;            
        }
        else {
            cout << 0 << endl;
        }*/
    }
    else {
        cout << 0 << endl;
    }
    if (bestS > 0) {//
        printf ("%.3lf\n", bestS);
        cout << bestC << " " << bestB << " " << bestA << endl; 
    }
    else {
        cout << 0 << endl;
    }//
    
    return 0;    
}