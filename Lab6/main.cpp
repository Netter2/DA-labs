#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <ctime>
using namespace std;
vector <int> dividers;

//Р¤СѓРЅРєС†РёСЏ РІРІРѕРґР° С‡РёСЃР»Р°
bool read (vector <int> & number) {
    string buffer;
    cin >> buffer;
    if ((buffer [0] > '9') || (buffer [0] < '0')) {
        return true;
    }
    while (buffer.length() > 0) {
        int n = 0;
        int ib = 4;
        if (buffer.length () < 4) {
            ib = buffer.length ();
        }
        for (int i = 0; i < ib; i++) {
            n += (buffer [buffer.length () - 1] - '0') * (pow(10, i));
            buffer.pop_back ();
        }
        number.push_back (n);
    }
    for (int i = number.size () - 1; i > 0; i --) {
        if (number [i] == 0) {
            number.pop_back ();
        }
        else {
            return false;
        }
    }
    return false;
}

//Р¤СѓРЅРєС†РёСЏ РІС‹РІРѕРґР° С‡РёСЃР»Р°
void write (vector <int> & number) {
    int k = 0;
    for (int i = number.size () - 1; i >= 0; i --) {
        if (i + 1 == number.size () - k) {
            if ((number [i] != 0) || (i == 0)) {
                cout << number [i];
            }
            else {
                k ++;
            }
        }
        else {
            if ((0 <= number [i]) && (number [i] <= 9)) {
                cout << "000" << number [i];
            }
            else if ((10 <= number [i]) && (number [i] <= 99)) {
                cout << "00" << number [i];
            }
            else if ((100 <= number [i]) && (number [i] <= 999)) {
                cout << "0" << number [i];
            }
            else {
                cout << number [i];
            } 
        }
                   
    }
    cout << endl;
}

//Р¤СѓРЅРєС†РёСЏ СЃСЂР°РІРЅРµРЅРёСЏ РґРІСѓС… С‡РёСЃРµР» 1:"1>2"   2:"1<2"   3:"1=2"
int comparsion (vector <int> & first, vector <int> & second) {
    int fs = first.size ();
    int ss = second.size ();
    for (int i = first.size () - 1; i >= 0; i --) {
        if (first [i] != 0) {
            break;
        }
        else {
            fs --;
        }
    }

    for (int i = second.size () - 1; i >= 0; i --) {
        if (second [i] != 0) {
            break;
        }
        else {
            ss --;
        }
    }

    if (fs > ss) {
        return 1;
    }
    else if (fs < ss) {
        return 2;
    }
    else {
        for (int i = fs - 1; i >= 0; i --) {    
            if (first [i] > second [i]) {
                return 1;
            }
            else if (first [i] < second [i]) {
                return 2;
            }
        }
        return 3;
    }
}

//Р¤СѓРЅРєС†РёСЏ СЃР»РѕР¶РµРЅРёСЏ РґРІСѓС… С‡РёСЃРµР»
void lplus (vector <int> & first, vector <int> & second) {
    for (int i = 0; i < max(first.size (), second.size ()); i ++) {
        if (first.size () == i) {
            first.push_back (0);
        }
        if (second.size () - 1 < i){
            first [i] += 0;
        }
        else {
            first [i] += second [i];
        } 
        if (first [i] > 9999) {
            first [i] -= 10000;
            if (first.size () == i + 1) {
                first.push_back (0);
            }
            first [i + 1] ++;
        }
    }
    write (first);
}

//Р¤СѓРЅРєС†РёСЏ РІС‹С‡РёС‚Р°РЅРёСЏ РґРІСѓС… С‡РёСЃРµР»
void lminus (vector <int> & first, vector <int> & second) {

    if (comparsion(first, second) == 2) {
        cout << "Error" << endl;
        return;
    }
    else {
        for (int i = 0; i < first.size (); i ++) {
            if (second.size () - 1 < i) {
                first [i] -= 0;
            }
            else {
                first [i] -= second [i];
            }
            if (first [i] < 0) {
                first [i] += 10000;
                first [i + 1] -= 1;
            }
        }
        write (first);
    }
}

//Р¤СѓРЅРєС†РёСЏ СѓРјРЅРѕР¶РµРЅРёСЏ РґРІСѓС… С‡РёСЃРµР»
void lmult (vector <int> & first, vector <int> & second) {
    vector <int> result;

    for (int i = 0; i < first.size() + second.size (); i ++) {
        result.push_back (0);
    }

    for (int i = 0; i < first.size(); i ++) {
        for (int j = 0; j < second.size (); j ++) {
            result [i + j] += first [i] * second [j];
            if (result [i + j] > 9999) {
                result [i + j + 1] += result [i + j] / 10000;
                result [i + j] -= (result [i + j] / 10000) * 10000;
            }
        }
    }

    write (result);
}

vector <int> binDiv (vector <int> & first, vector <int> & second, int & divider) {
    int l = 0;
    int r = 10000;
    vector <int> result;
    for (int i = 0; i < second.size () + 1; i ++) {
        result.push_back (0);
    }

    int comp;

    while (1) {
        divider = dividers [(l + r) / 2];
        
        result [second.size ()] = 0;
        for (int i = 0; i < second.size(); i ++) {
            result [i] = second [i] * divider;
            if (i) {
                if (result [i - 1] > 9999)  {
                    result [i] += result [i - 1] / 10000;
                    result [i - 1] -= (result [i - 1] / 10000) * 10000;
                } 
            }
            
        }

        if (result [second.size() - 1] > 9999)  {
            result [second.size()] += result [second.size() - 1] / 10000;
            result [second.size() - 1] -= (result [second.size() - 1] / 10000) * 10000;
        }
        
        comp = comparsion (first, result);
            
        if (comp == 1) {
            if (l != (l + r) / 2) {
                l = (l + r) / 2;
            }
            else {
                if (result [result.size() - 1] == 0) {
                    int i = result.size() - 1;
                    while (result [i] == 0) {
                        result.pop_back ();
                        i --;
                    }
                }

                return result;
            }
        }
        else if (comp == 2) {
            r = (l + r) / 2;
        }
        else if (comp == 3) {
            if (result [result.size() - 1] == 0) {
                int i = result.size() - 1;
                while (result [i] == 0) {
                    result.pop_back ();
                    i --;
                }
            }            
            return result;
        }
    }
}

//Р¤СѓРЅРєС†РёСЏ РґРµР»РµРЅРёСЏ РґРІСѓС… С‡РёСЃРµР»
void ldiv (vector <int> & first, vector <int> & second) {
    vector <int> result;
    vector <int> buffer;
    vector <int> workplace;
    int d;
    int comp;
    bool flag = false;

    for (int i = second.size() - 1; i >= 0; i --) {
        workplace.push_back (first[first.size () - 1 - i ]);
    }

    for (int i = first.size () - second.size () - 1; i >= -1; i --) {
        while (1) {
            comp = comparsion (workplace, second);
            if (comp == 1) {
                break;
            }
            else if (comp == 2) {
                result.push_back (0);
                if (i >= 0) {
                    workplace.insert (workplace.begin(), first [i]);
                    i --;  
                }
                else {
                    flag = true;
                    break;
                }            
            }
            else if (comp == 3) {
                break;
            }    
        }
        
        if (flag) {
            break;
        }

        buffer = binDiv (workplace, second, d);
        result.push_back (d);
        for (int j = 0; j < workplace.size (); j ++) {
            if (buffer.size () == j) {
                workplace [j] -= 0;
            }
            else {
                workplace [j] -= buffer [j];
            }
            if (workplace [j] < 0) {
                workplace [j] += 10000;
                workplace [j + 1] -= 1;
            }
        }
        while ((workplace.size () > 0) && (workplace [workplace.size () - 1] == 0)) {
            workplace.pop_back ();
        }
        if (i >= 0) {
            workplace.insert (workplace.begin(), first [i]);
        }
        else {
            break;
        }
        buffer.clear ();
    }
    reverse (result.begin (), result.end ());
    write (result);
}

//Р¤СѓРЅРєС†РёСЏ РєРѕСЂРѕС‚РєРѕРіРѕ РґРµР»РµРЅРёСЏ
void sdiv (vector <int> & first, int second) {
    vector <int> result;
    int current = 0;
    int overBase = 0;
    for (int i = 0; i < int (first.size ()); i ++) {
        result.push_back (0);
    }
    for (int i = first.size () - 1; i >= 0; i --) {
        current = overBase * 10000 + first [i];
        result [i] = current / second ;
        overBase = result [i];
        overBase *= second;
        overBase = current - overBase;
    }
    int i = result.size () - 1;
    while ((i > 0) && (result [i] == 0)) {
        result.pop_back ();
        i --;
    }
    write (result);
}

vector <int> multiplication (vector <int> & first, vector <int> & second) {
    vector <int> result;

    for (int i = 0; i < first.size() + second.size (); i ++) {
        result.push_back (0);
    }

    for (int i = 0; i < first.size(); i ++) {
        int next = 0;
        for (int j = 0; j < second.size (); j ++) {
            int curr = result [i + j] + next + first [i] * second [j];
            if (curr >= 10000) {
                next = curr / 10000;
                result [i + j] = curr - next * 10000;
            }
            else {
                result [i + j] = curr;
                next = 0;
            }
        }
        result [i + second.size ()] += next;
    }
    int i = result.size () - 1;
    while ((i > 0) && (result [i] == 0)) {
        result.pop_back ();
        i --;
    }
    
    return result;
}

vector <int> division (vector <int> & first, int second) {
    vector <int> result;
    int current = 0;
    int overBase = 0;
    for (int i = 0; i < int (first.size ()); i ++) {
        result.push_back (0);
    }
    for (int i = first.size () - 1; i >= 0; i --) {
        current = overBase * 10000 + first [i];
        result [i] = current / second ;
        overBase = result [i];
        overBase *= second;
        overBase = current - overBase;
    }
    int i = result.size () - 1;
    while ((i > 0) && (result [i] == 0)) {
        result.pop_back ();
        i --;
    }
    return result;
}

//Р¤СѓРЅРєС†РёСЏ РІРѕР·РІРµРґРµРЅРёСЏ РІ СЃС‚РµРїРµРЅСЊ
void ldegr (vector <int> & first, vector <int> & second) {
    vector <int> result;
    if ((second.size () == 1) && (second [0] == 0) && (first.size () == 1) && (first [0] == 0)) {
        cout << "Error" << endl;
        return;
    }

    if ((second.size () == 1) && (second [0] == 0)) {
        cout << "1" << endl;
        return;
    }

    if ((first.size () == 1) && ((first [0] == 0) || (first [0] == 1))) {
        result.push_back (first [0]);
        write (result);
        return;
    }

    vector <int> part = {1};
    result = first;
    while ((second.size () != 1) || (second [0] != 0)) {
        if (second [0] % 2 == 1) {
            part = multiplication (part, result);
        }
        result = multiplication (result, result);
        second = division (second, 2);
    }
    write (part);
    return;
}

int main () {
    
    vector <int> first;
    vector <int> second;

    for (int i = 0; i <= 10000; i ++) {
        dividers.push_back (i);
    }

    while (cin) {
        if (read (first)) {
            break;
        }
        if (read (second)) {
            break;
        }
        char sign = 'q';
        cin >> sign;
        if (sign == '+') {
            lplus (first, second);
        }
        else if (sign == '-') {
            lminus (first, second);
        }
        else if (sign == '*') {
            lmult (first, second);
        }
        else if (sign == '/') {
            if ((second [0] == 0) && (second.size () == 1)) {
                cout << "Error" << endl;
            }
            else {
                int comp = comparsion (first, second);
                if (comp == 1) {
                    if (second.size () == 1) {
                        sdiv (first, second [0]);
                    }
                    else {
                        ldiv(first, second);
                    }
                }
                else if (comp == 2) {
                    cout << "0" << endl;
                }
                else if (comp == 3) {
                    cout << "1" << endl;
                }
            }
            
        }
        else if (sign == '^') {
            ldegr (first, second);
        }
        else if (sign == '>') {
            if (comparsion (first, second) == 1) {
                cout << "true" << endl;
            }
            else {
                cout << "false" << endl;
            }
        }
        else if (sign == '<') {
            if (comparsion (first, second) == 2) {
                cout << "true" << endl;
            }
            else {
                cout << "false" << endl;
            }
        }
        else if (sign == '=') {
            if (comparsion (first, second) == 3) {
                cout << "true" << endl;
            }
            else {
                cout << "false" << endl;
            }
        }
        else {
            break;
        }
        first.clear ();
        second.clear ();
    }
    return 0;
}