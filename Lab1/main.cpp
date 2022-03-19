#include <iostream>
#include <string>
using namespace std;

const int M_BIT = 65536;
const unsigned long long int MASK = (1 << 16) - 1; 

struct TData {
    unsigned long long int key;
    string* data;
};

TData* Sort (TData* active, int count) {
    unsigned long long int counter [M_BIT];
    TData *result = new TData [count];
    TData* buffer;
    for (int N = 0; N < 4; N ++) {
        for (int i = 0; i < M_BIT; i ++) {
            counter [i] = 0;
        }
        for (int i = 0; i < count; i ++) {
            counter[((active [i].key & (MASK << N * 16)) >> N * 16)] ++;
        }
        for (int i = 1; i < M_BIT; i ++) {
            counter [i] += counter [i - 1];
        }
        
        for (int i = count - 1; i >= 0; i --) {
            counter[((active [i].key & (MASK << N * 16)) >> N * 16)] --;
            result [counter[((active [i].key & (MASK << N * 16)) >> N * 16)]] = active [i];
        }
        buffer = active;
        active = result;
        result = buffer;
    }
    
    delete [] result;
    return (active);
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    TData line;
    int count = 0;
    int mCount = 2;
    TData* active = new TData [mCount];
    while (cin >> line.key) {
        string* buffer = new string;
        cin >> *buffer;
        line.data = buffer;
        if (count == mCount - 1) {
            mCount *= 2;
            TData* extra = new TData [mCount];
            for (int i = 0; i < count; i ++) {
                extra [i] = active [i];
            }
            delete [] active;
            active = extra; 
        }
        active[count].key = line.key;
        active[count].data = line.data;
        count ++;
    }
    
    active = Sort (active, count);
    
    
    
    for (int i = 0; i < count ; i++) {
        cout << active [i].key << "\t";
        cout << *(active [i].data) << "\n";
        delete active [i].data;
    }
    delete [] active;
    return 0;
}


