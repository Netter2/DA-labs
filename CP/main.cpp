#include <iostream>
#include <map>
#include <string>
#include <math.h>

using namespace std;

int isSpam (map <string, pair <int, int>> & dictionary, string & letter, int training, pair <int, int> & trainingSpam, pair <int, int> & trainingHam) {
    map <string, pair <int, int>> :: iterator it;
    double spamChance = log (double (trainingSpam.first) / training);
    double hamChance = log (double (trainingHam.first) / training);
    //cout << "MS:" << dictionary.size () << endl << "SC:" << spamChance << endl << "HC:" << hamChance << endl << "TSpam:" << trainingSpam.first << " " << trainingSpam.second << endl << "THam:" << trainingHam.first << " " << trainingHam.second << endl;
    string word;

    for (int i = 0; i < letter.size (); i ++) {
        if ((letter [i] >= 'A') && (letter [i] <= 'Z')) {
            letter [i] -= 'A';
            letter [i] += 'a';
        }
        if (letter [i] != ' '){
            word.push_back (letter [i]);
            if (i != letter.size () - 1) {
                continue; 
            }
        }
        it = dictionary.find (word);
        if (it != dictionary.end ()) {
            spamChance += log (double((it->second.first + 1)) / (dictionary.size () + trainingSpam.second));
            hamChance += log (double((it->second.second + 1)) / (dictionary.size () + trainingHam.second));
            //cout << "Word: " << word << endl << "WSpam:" << it->second.first << endl << "WHam:" << it->second.second << endl;
        }
        else {
            spamChance += log (1.0 / (dictionary.size () + trainingSpam.second));
            hamChance += log (1.0 / (dictionary.size () + trainingHam.second));
        }        
        word.clear ();
    }
    //cout << "SPAM:" << spamChance << endl << "HAM:" << hamChance << endl;
    if (spamChance > hamChance) {
        return 1;
    }
    else {
        return 2;
    }
    return 0;
}

int main () {
    map <string, pair <int, int>> dictionary;
    map <string, pair <int, int>> :: iterator it;
    int training, testing;
    pair <int, int> trainingSpam, trainingHam;
    cin >> training >> testing;
    for (int i = 0; i < training; i ++) {
        int classBuffer;
        string letter;
        cin >> classBuffer;
        getchar ();
        if (classBuffer == 0) {
            trainingSpam.first ++;
            getline (cin, letter);
            string word;
            for (int i = 0; i < letter.size (); i ++) {
                if ((letter [i] >= 'A') && (letter [i] <= 'Z')) {
                    letter [i] -= 'A';
                    letter [i] += 'a';
                }                
                if (letter [i] != ' '){
                    word.push_back (letter [i]);
                    if (i != letter.size () - 1) {
                       continue; 
                    }
                }
                pair <int, int> bufferpair (1, 0);
                it = dictionary.find (word);
                trainingSpam.second ++;
                if (it != dictionary.end ()) {
                    it->second.first ++;
                }
                else {
                    dictionary.insert(make_pair(word, bufferpair));
                }
                word.clear ();
            }
        }
        else if (classBuffer == 1) {
            trainingHam.first ++;
            getline (cin, letter);
            string word;
            for (int i = 0; i < letter.size (); i ++) {
                if ((letter [i] >= 'A') && (letter [i] <= 'Z')) {
                    letter [i] -= 'A';
                    letter [i] += 'a';
                }
                if (letter [i] != ' '){
                    word.push_back (letter [i]);
                    if (i != letter.size () - 1) {
                       continue; 
                    }
                }
                pair <int, int> bufferpair (0, 1);
                it = dictionary.find (word);
                trainingHam.second ++;
                if (it != dictionary.end ()) {
                    it->second.second ++;
                }
                else {
                    dictionary.insert(make_pair(word, bufferpair));
                }
                word.clear ();
            }
        }
    }
    for (int i = 0; i < testing; i ++) {
        string letter;
        
        getline (cin, letter);
        int sOrH = 0;
        sOrH = isSpam (dictionary, letter, training, trainingSpam, trainingHam);
        if (sOrH == 1) {
            cout << 0 << endl;
        }
        else if (sOrH == 2) {
            cout << 1 << endl;
        }
    }
    return 0;
}