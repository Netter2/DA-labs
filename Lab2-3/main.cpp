#include <iostream>//

using namespace std;//

class TString {//
    public://
        int size;//
        char str [258];//
        TString () {//
            size = 0;//
        }//

        int Size () {//
            return size;//
        }//

        bool operator== (TString& second) {//
            if (size == second.Size ()) {//
                for (int i = 0; i < size; i ++) {//
                    if (str [i] != second.str [i]) {//
                        return false;//
                    }//
                }//
                return true;//
            }//
            else {//
                return false;//
            }//
        }// 

        char operator[] (int idx) {//
            if (idx >= 258) {
                cout << "Error" << endl;
                return -1;
            }
            return str [idx];//
        }//

        void Clear () {//
            size = 0;//
            str [0] = '\0';//
        }//

        void Pushback (char c) {//
            if (size >= 256) {
                cout << "Error";
                return;
            }
            str [size] = c;//
            size ++;//
            str [size] = '@';//
            str [size + 1] = '\0';//
        }//
};//

struct TKV {//
    TString key;//
    unsigned long long int value;//
};//

int code (char c) {//
    if (c < 63) {
        cout << "Error" << endl/*  << "\"" << c <<"\" " << int (c) */;
        return -1;
    } 
    int id = int (c);//
    id -= 64;//
    return id;//
}//

bool bitcheck (int number, TString & str) {//
    char c;//
    if (number < 0) {//
        return 0;//
    }//
    else {//
        if (/*str.Size () >= (number / 5 - 1)*/number <= (str.Size () * 5)) {//
            c = str [number / 5];//
        }//
        else {//
            c = '@';//
        }//
        return ((code (c) >> (number % 5)) & 1 );//
    }//
}//

struct TNode {//
    struct TKV data;//
    TNode* lpointer;//
    TNode* rpointer;//
    int bitnumber;//
    unsigned long long int nodeId;//
    };//

class TPatricia {//
    private://
        unsigned long long int amount;//
        unsigned long long int id;//
        const int dpointer = 1;//
        const int upointer = 0;//
        const int spointer = 2;//

    public://
        TNode* root;//
        int correctBits;//

        TPatricia () {//
            root = new TNode;//
            root->data.key.Pushback ('@');//
            root->data.value = 0;//
            root->lpointer = root;//
            root->rpointer = nullptr;//
            root->bitnumber = -1;//
            root->nodeId = 0;//
            amount = 1;//
            correctBits = 0;//
        }//
        
        TNode* ParFind (TString& word, TNode** prev, const int& difbit, bool& f) {//
            int number = -2;//
            TNode* current = root;//
            *prev = root;//
            while ((number < current->bitnumber) && (difbit > current->bitnumber)) {//
                correctBits = current->bitnumber;//
                number = current->bitnumber;//
                if (bitcheck (current->bitnumber, word)) {//
                    if (current->rpointer == current) {//
                        f = 1;//
                        break;//
                    }//
                    else {//
                        *prev = current;//
                        current = current->rpointer;//
                    }//
                }//
                else {//
                    if (current->lpointer == current) {//
                        f = 1;//
                        break;//
                    }//
                    else {//
                        *prev = current;//
                        current = current->lpointer;//
                    }//
                }//
            }//

            return current;//
        }//

        TNode* FastParFind (TString& word, TNode** prev, bool& f, TNode* current) {//
            int number = -2;//
            *prev = root;
            while (number < current->bitnumber) {
                correctBits = current->bitnumber;
                number = current->bitnumber;
                if (bitcheck (current->bitnumber, word)) {
                    if (current->rpointer == current) {
                        f = 1;
                        break;
                    }
                    else {
                        *prev = current;
                        current = current->rpointer;
                    }
                }
                else {
                    if (current->lpointer == current) {
                        f = 1;
                        break;
                    }
                    else {
                        *prev = current;
                        current = current->lpointer;
                    }
                }
            }

            return current;
        }

        TNode* FastExtraParFind (TNode* nodeWord, TNode** prev, bool& f, TNode* current, TNode* & trueparent) {
            int number = -2;
            *prev = root;
            bool iwnf = true;
            while (number < current->bitnumber) {
                correctBits = current->bitnumber;
                number = current->bitnumber;
                if (bitcheck (current->bitnumber, nodeWord->data.key)) {
                    if (current->rpointer == current) {
                        f = 1;
                        break;
                    }
                    else {
                        *prev = current;
                        if ((current->rpointer == nodeWord) && (iwnf)) {////
                            trueparent = current;
                            iwnf = false;
                        }
                        current = current->rpointer;
                    }
                }
                else {
                    if (current->lpointer == current)  {
                        f = 1;
                        break;
                    }
                    else {
                        *prev = current;
                        if ((current->lpointer == nodeWord) && (iwnf)) {////
                            trueparent = current;
                            iwnf = false;
                        }
                        current = current->lpointer;
                    }
                }
            }

            return current;
        }

        TNode* Find (TString& word) {
            int number = -2;
            TNode* current = root;
            while (number < current->bitnumber) {
                correctBits = current->bitnumber;
                number = current->bitnumber;
                if (bitcheck (current->bitnumber, word)) {
                    current = current->rpointer;
                }
                else {
                    current = current->lpointer;
                }
            }

            return current;
        }

        void Add (TKV& addingnode) {
            TNode* foundparent;
            TNode* found = Find (addingnode.key);
            int difbit = 0;

            if (addingnode.key == found->data.key) {
                cout << "Exist" << endl;
            }
            else {
                for (int i = 0; i < 256; i ++) {
                    if (found->data.key [i] == addingnode.key [i]) {
                        difbit += 5;
                    }
                    else {
                        int firstCode = code (found->data.key [i]);
                        int secondCode = code (addingnode.key [i]);
                        for (int ii = 0; ii < 5; ii ++) {
                            if (((firstCode >> ii) & 1) != ((secondCode >> ii) & 1)) {
                                difbit += ii;
                                break;
                            }
                        }
                        break;
                    }
                }

                bool f = 0;
                TNode* found = (ParFind (addingnode.key, &foundparent, difbit, f));
                TNode* newnode = new TNode;
                newnode->data = addingnode;
                newnode->bitnumber = difbit;
                
                if (bitcheck (difbit, newnode->data.key)) {
                    newnode->rpointer = newnode;
                    newnode->lpointer = found;
                }
                else {
                    newnode->rpointer = found;
                    newnode->lpointer = newnode;
                }

                if (f) {
                    if (found->lpointer == found) {
                        found->lpointer = newnode;
                    }
                    else if (found->rpointer == found) {
                        found->rpointer = newnode;
                    }
                }
                else {
                    if (foundparent->lpointer == found) {
                        foundparent->lpointer = newnode;
                    }
                    else if (foundparent->rpointer == found) {
                        foundparent->rpointer = newnode;
                    }                    
                }
                amount ++;
                cout << "OK" << endl;
            }
        }

        void Delete (TString & deletingnodekey) {
            bool f = 0;            
            TNode* foundparent;
            TNode* found = (ParFind (deletingnodekey, &foundparent, 257, f));

            if (deletingnodekey == found->data.key) {//если элемент был найден
                if (f) {//если found был найден переходом по круговой ссылке
                    if (foundparent->lpointer == found) {//если в found ведет левая ссылка foundparent
                        if (found->lpointer == found) {//если левая ссылка found - круговая
                            foundparent->lpointer = found->rpointer;//переподвязка правой ссылки found к левой ссылке foundparent
                        }
                        else {//если правая ссылка found - круговая
                            foundparent->lpointer = found->lpointer;//переподвязка левой ссылки found к левой ссылке foundparent
                        }
                    }
                    else {//если в found ведет правая ссылка foundparent
                        if (found->lpointer == found) {//если левая ссылка found - круговая
                            foundparent->rpointer = found->rpointer;//переподвязка правой ссылки found к правой ссылке foundparent
                        }
                        else {//если правая ссылка found - круговая
                            foundparent->rpointer = found->lpointer;//переподвязка левой ссылки found к правой ссылке foundparent
                        }
                    }
                    delete found;//удаление found, завершение удаления
                    amount --;
                    cout << "OK" << endl;
                }
                else {//если found был найден по ссылке вверх
                    if ((foundparent->lpointer->bitnumber <= foundparent->bitnumber) && (foundparent->rpointer->bitnumber <= foundparent->bitnumber)) {//если обе ссылки foundparent ведут наверх, то есть, foundparent - лист
                        found->data = foundparent->data;//переписываем данные из foundparent в found
                        found = (FastParFind (foundparent->data.key, &foundparent, f, found));//находим родителя бывшего foundparent
                        if (foundparent->lpointer == found) {//если в found ведет левая ссылка foundparent
                            if (found->lpointer == found) {//если левая ссылка found - круговая
                                foundparent->lpointer = found->rpointer;//переподвязка правой ссылки found к левой ссылке foundparent
                            }
                            else {//если правая ссылка found - круговая
                                foundparent->lpointer = found->lpointer;//переподвязка левой ссылки found к левой ссылке foundparent
                            }
                        }
                        else {//если в found ведет правая ссылка foundparent
                            if (found->lpointer == found) {//если левая ссылка found - круговая
                                foundparent->rpointer = found->rpointer;//переподвязка правой ссылки found к правой ссылке foundparent
                            }
                            else {//если правая ссылка found - круговая
                                foundparent->rpointer = found->lpointer;//переподвязка левой ссылки found к правой ссылке foundparent
                            }
                        }
                        delete found;//удаление found, завершение удаления
                        amount --;
                        cout << "OK" << endl;
                    }
                    else {//у foundparent есть ссылки вниз, foundparent - не лист
                        found->data = foundparent->data;//переписываем данные из foundparent в found
                        TNode* trueparent = nullptr;
                        found = (FastExtraParFind (foundparent, &foundparent, f, found, trueparent));//находим родителя бывшего foundparent
                        if (trueparent->lpointer == found) {//если на удаляемый элемент указывает левая ссылка trueparent
                            if (found->lpointer->bitnumber <= found->bitnumber) {//если левая ссылка удаляемого элемента ведет вверх
                                if (foundparent->lpointer == found) {//если на удаляемый элемент указывает левая ссылка foundparent
                                    trueparent->lpointer = found->rpointer;//переподвязка нижней (правой) ссылки found к левой ссылке trueparent
                                    foundparent->lpointer = found->lpointer;//переподвязка верхней (левой) ссылки found к левой ссылке foundparent
                                }
                                else {//если на удаляемый элемент указывает правая ссылка foundparent
                                    trueparent->lpointer = found->rpointer;//переподвязка нижней (правой) ссылки found к левой ссылке trueparent
                                    foundparent->rpointer = found->lpointer;//переподвязка верхней (левой) ссылки found к правой ссылке foundparent
                                }
                            }
                            else {//если правая ссылка удаляемого элемента ведет вверх
                                if (foundparent->lpointer == found) {//если на удаляемый элемент указывает левая ссылка foundparent
                                    trueparent->lpointer = found->lpointer;//переподвязка нижней (левой) ссылки found к левой ссылке trueparent
                                    foundparent->lpointer = found->rpointer;//переподвязка верхней (правой) ссылки found к левой ссылке foundparent
                                }
                                else {//если на удаляемый элемент указывает правая ссылка foundparent
                                    trueparent->lpointer = found->lpointer;//переподвязка нижней (левой) ссылки found к левой ссылке trueparent
                                    foundparent->rpointer = found->rpointer;//переподвязка верхней (правой) ссылки found к правой ссылке foundparent
                                }
                            }
                        }
                        else {//если на удаляемый элемент указывает правая ссылка trueparent
                            if (found->lpointer->bitnumber <= found->bitnumber) {//если левая ссылка удаляемого элемента ведет вверх
                                if (foundparent->lpointer == found) {//если на удаляемый элемент указывает левая ссылка foundparent
                                    trueparent->rpointer = found->rpointer;//переподвязка нижней (правой) ссылки found к правой ссылке trueparent
                                    foundparent->lpointer = found->lpointer;//переподвязка верхней (левой) ссылки found к левой ссылке foundparent
                                }
                                else {//если на удаляемый элемент указывает правая ссылка foundparent
                                    trueparent->rpointer = found->rpointer;//переподвязка нижней (правой) ссылки found к правой ссылке trueparent
                                    foundparent->rpointer = found->lpointer;//переподвязка верхней (левой) ссылки found к правой ссылке foundparent
                                }
                            }
                            else {//если правая ссылка удаляемого элемента ведет вверх
                                if (foundparent->lpointer == found) {//если на удаляемый элемент указывает левая ссылка foundparent
                                    trueparent->rpointer = found->lpointer;//переподвязка нижней (левой) ссылки found к правой ссылке trueparent
                                    foundparent->lpointer = found->rpointer;//переподвязка верхней (правой) ссылки found к левой ссылке foundparent
                                }
                                else {//если на удаляемый элемент указывает правая ссылка foundparent
                                    trueparent->rpointer = found->lpointer;//переподвязка нижней (левой) ссылки found к правой ссылке trueparent
                                    foundparent->rpointer = found->rpointer;//переподвязка верхней (правой) ссылки found к правой ссылке foundparent
                                }
                            }
                        }
                        delete found;
                        amount --;
                        cout << "OK" << endl;
                    }
                }
            }
            else {//если элемента в дереве нет
                cout << "NoSuchWord" << endl;
            }
        }
        
        void Clean (TNode* current) {
            
            if (current->lpointer->bitnumber > current->bitnumber) {
                Clean (current->lpointer);
            }

            if (current->rpointer == nullptr) {
                current->lpointer = current;
                return;
            }
            else if (current->rpointer->bitnumber > current->bitnumber) {
                Clean (current->rpointer);
            }

            delete current;
        }

        void RecParcer (TNode* current, FILE* fd, unsigned long long int& corrres, unsigned long long int& res) {
            int count = current->data.key.Size ();
            res += fwrite (&count, sizeof (int), 1, fd);
            corrres ++;
            res += fwrite (&(current->data.key.str), sizeof (char), count, fd);
            corrres += count;
            res += fwrite (&(current->data.value), sizeof (unsigned long long int), 1, fd);
            corrres ++;
            res += fwrite (&(current->bitnumber), sizeof (int), 1, fd);
            corrres ++;
            res += fwrite (&(id), sizeof (unsigned long long int), 1, fd);
            corrres ++;
            current->nodeId = id;
            id ++;

            if (current->lpointer->bitnumber > current->bitnumber) {
                res += fwrite (&(dpointer), sizeof (int), 1, fd);
                corrres ++;
                RecParcer (current->lpointer, fd, corrres, res);
            }
            else if (current->lpointer == current) {
                res += fwrite (&(spointer), sizeof (int), 1, fd);
                corrres ++;
            }
            else {
                res += fwrite (&(upointer), sizeof (int), 1, fd);
                corrres ++;
                res += fwrite (&(current->lpointer->nodeId), sizeof (unsigned long long int), 1, fd);
                corrres ++;
            }

            if (current != root) {
                if (current->rpointer->bitnumber > current->bitnumber) {
                    res += fwrite (&(dpointer), sizeof (int), 1, fd);
                    corrres ++;
                    RecParcer (current->rpointer, fd, corrres, res);
                }
                else if (current->rpointer == current) {
                    res += fwrite (&(spointer), sizeof (int), 1, fd);
                    corrres ++;
                }
                else {
                    res += fwrite (&(upointer), sizeof (int), 1, fd);
                    corrres ++;
                    res += fwrite (&(current->rpointer->nodeId), sizeof (unsigned long long int), 1, fd);
                    corrres ++;
                }    
            }
            
        }

        void Save (TString & filename) {
            FILE* fd = NULL;
            fd = fopen(filename.str, "wb");
            if (fd == NULL) {
                cout << "ERROR: something went wrong during saving." << endl;
                return;
            }
            unsigned long long int res = 0;
            unsigned long long int corrres = 1;
            res += fwrite (&amount, sizeof (unsigned long long int), 1, fd);
            id = 1;
            if (amount > 1) {
                RecParcer (root->lpointer, fd, corrres, res);
            }
            if (corrres != res) {
                cout << "ERROR: incorrect control summary" << endl;
            }
            else {
                cout << "OK" << endl;
            }
            fclose (fd);
        }

        TNode* RecReader (FILE* fd, TNode** vault, unsigned long long int& corrres, unsigned long long int& res) {
            int count;
            char c [258];
            int pid;
            unsigned long long int upid;
            TNode* newNode = new TNode;
            res += fread (&count, sizeof (int), 1, fd);
            corrres ++;
            res += fread (&c, sizeof (char), count, fd);
            corrres += count;
            for (int j = 0; j < count; j ++) {
                newNode->data.key.Pushback (c [j]);
            }
            res += fread (&(newNode->data.value), sizeof (unsigned long long int), 1, fd);
            corrres ++;
            res += fread (&(newNode->bitnumber), sizeof (int), 1, fd);
            corrres ++;
            res += fread (&(id), sizeof (unsigned long long int), 1, fd);
            corrres ++;
            vault [id] = newNode;
            res += fread (&(pid), sizeof (int), 1, fd);
            corrres ++;
            if (pid == 1) {
                newNode->lpointer = RecReader (fd, vault, corrres, res);
            }
            else if (pid == 2) {
                newNode->lpointer = newNode;
            }
            else {
                res += fread (&(upid), sizeof (unsigned long long), 1, fd);
                corrres ++;
                newNode->lpointer = vault [upid];
            }
            res += fread (&(pid), sizeof (int), 1, fd);
            corrres ++;
            if (pid == 1) {
                newNode->rpointer = RecReader (fd, vault, corrres, res);
            }
            else if (pid == 2) {
                newNode->rpointer = newNode;
            }
            else {
                res += fread (&(upid), sizeof (unsigned long long), 1, fd);
                corrres ++;
                newNode->rpointer = vault [upid];
            }

            return newNode;
        }
        
        void Load (TString & filename) {
            unsigned long long int res = 0;
            unsigned long long int corrres = 1;
            FILE* fd = NULL;
            fd = fopen(filename.str, "rb");
            if (fd == NULL) {
                cout << "ERROR: something went wrong during loading." << endl;
                return;
            }
            Clean (root);
            res += fread (&amount, sizeof (unsigned long long int), 1, fd);
            if (amount > 1) {
                TNode** vault = new TNode* [amount];
                vault [0] = root;
                root->lpointer = RecReader (fd, vault, corrres, res);
                delete [] vault;
            }
            else {
                root->lpointer = root;
            }
            if (corrres != res) {
                cout << "ERROR: incorrect control summary" << endl;
                Clean (root);
                root->lpointer = root;
            }
            else {
                cout << "OK" << endl;
            }
            fclose (fd);
        }

        ~TPatricia () {
            delete root;
        }
    
};

int main () {
    TString in;
    TKV nnode;
    TPatricia patricia;
    int res;
    while (scanf ("%s", in.str) > 0) {
        if (in [0] == '+') {
            res += scanf ("%s", in.str);
            int i = 0;
            while (in [i] != '\0') {
                if ((in [i] >= 'a') && (in [i] <= 'z')) {
                    in.str [i] -= 32;
                }
                i ++;
            }
            in.size = i;
            in.str [in.size] = '@';
            in.str [in.size + 1] = '\0';
            nnode.key = in;
            cin >> nnode.value;
            patricia.Add (nnode);
        }
        else if (in [0] == '-') {
            res += scanf ("%s", in.str);
            int i = 0;
            while (in [i] != '\0') {
                if ((in [i] >= 'a') && (in [i] <= 'z')) {
                    in.str [i] -= 32;
                }
                i ++;
            }
            in.size = i;
            in.str [in.size] = '@';
            in.str [in.size + 1] = '\0';
            patricia.Delete (in);
        }
        else if (in [0] == '!') {
            res += scanf ("%s", in.str);
            if (in [0] == 'S') {
                res += scanf ("%s", in.str);
                patricia.Save (in);
            }
            else if (in [0] == 'L') {
                res += scanf ("%s", in.str);
                patricia.Load (in);
            }
        }
        else if (((in [0] >= 'A') && (in [0] <= 'Z')) || ((in [0] >= 'a') && (in [0] <= 'z'))) {
            int i = 0;
            while (in [i] != '\0') {
                if ((in [i] >= 'a') && (in [i] <= 'z')) {
                    in.str [i] -= 32;
                }
                i ++;
            }
            in.size = i;
            in.str [in.size] = '@';
            in.str [in.size + 1] = '\0';
            TNode* found = patricia.Find (in);
            if (in == found->data.key) {////
                cout << "OK: " << found->data.value << endl;
            }
            else {
                cout << "NoSuchWord" << endl;
            }
        }
        else {
            break;
        }
        in.Clear ();
    }
    res = res;//псевдоиспользование переменной res
    patricia.Clean (patricia.root);
    //cout << "000";
    return 0;
}