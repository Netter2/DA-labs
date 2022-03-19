#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class TSuffixTree {
private:
    struct TNode {
        int left, right;
        int leafNumber;
        vector <int> leaves;
        vector <TNode*> arcs;
        TNode* suffixRef= nullptr;

        TNode () {}

        TNode (int newLeft, int newRight) { // если задана только дуга, ведущая в вершину
            left = newLeft;
            right = newRight;
            leafNumber = -1; // вершина считается внутренней
            suffixRef = nullptr;
        }

        TNode (int newLeft, int newRight, int newLeafNumber) {
            left = newLeft;
            right = newRight;
            leafNumber = newLeafNumber;
            suffixRef = nullptr;
        }

    };
    string text;
    int end;
    TNode* root;
    int leafCounter;
    TNode* savedSuffixRef; // узел, из которого должна быть направлена следующая суфф. ссылка
    int GetRight (TNode* node) {
        if (node->leafNumber < 0) {
            return node->right;
        }
        else {
            return end;
        }
    }
    // Добавление дуги из одного символа к вершине
    void AddToNode (TNode* currentNode, int newStrIdx) {//создание листа
        TNode* newNode = new TNode (newStrIdx, newStrIdx, leafCounter); // создать новую вершину
        currentNode->arcs.push_back (newNode); // привязка новой вершины к текущей
        if (savedSuffixRef != nullptr) {
            savedSuffixRef->suffixRef = currentNode; // привязка суффиксной ссылки
            savedSuffixRef = nullptr;
        }
        leafCounter ++; // появился новый лист
    }
    // Разделение дуги
    void SplitArc (
        TNode* currentNode, // узел
        int arcNumber, // номер дуги
        int newNodePos, // позиция новой вершины
        int newStrRight // правый край новой строки (индекс добавляемой буквы)
        ) {
        TNode* innerNode = new TNode (currentNode->arcs [arcNumber]->left, newNodePos - 1); // создать внутреннюю вершину
        innerNode->arcs.push_back (currentNode->arcs [arcNumber]); // привязать текущую дугу к внутренней вершине
        currentNode->arcs [arcNumber] = innerNode; // привязать внутреннюю вершину к текущей
        innerNode->arcs [0]->left = newNodePos;
        TNode* newNode = new TNode (newStrRight, newStrRight, leafCounter); // создать новую вершину
        innerNode->arcs.push_back (newNode); // привязка новой вершины к внутренней
        if (savedSuffixRef != nullptr) { // если есть сохранённая суфф. ссылка
            savedSuffixRef->suffixRef = innerNode; // направить суфф. ссылку из него в новый внутренний узел
            savedSuffixRef = nullptr;
        }
        leafCounter ++; // новый лист появился
        //suffixBegin ++;
        savedSuffixRef = innerNode; // появилась новая внутренняя вершина, сохраним ссылку на неё для направления её суфф. ссылки
    }
    void SuffixRefTransition (TNode* currentNode, int strToSkipLength, int strToSkipBeginIdx, int newStrIdx);
    void ApplyExtensionRules (TNode* currentNode, int arcNumber, int posInArc, int newStrIdx) {
        if (posInArc != GetRight (currentNode->arcs [arcNumber])) { // если мы остановились посреди дуги
            if (text [newStrIdx] == text [posInArc + 1]) {
                // новая строка входит в текущую дугу
                if (savedSuffixRef != nullptr) {
                    savedSuffixRef->suffixRef = currentNode;
                    savedSuffixRef = nullptr;
                }
                return; // вставка завершена (использовано правило продолжения 3)
            }
            else {
                SplitArc (currentNode, arcNumber, posInArc + 1, newStrIdx); // (правило продолжения 2)
                if (leafCounter > newStrIdx) {
                    return;
                }
                if (currentNode != root) {
                    SuffixRefTransition (currentNode->suffixRef,
                                         GetRight (currentNode->arcs [arcNumber]) - currentNode->arcs [arcNumber]->left + 1,
                                         currentNode->arcs [arcNumber]->left,
                                         newStrIdx);
                }
                else {
                    AddNewString (currentNode, leafCounter, newStrIdx);
                }
            }
        }
        else { // если мы остановились в вершине
            for (int k = 0; k < currentNode->arcs [arcNumber]->arcs.size (); k ++) {
                if (text [newStrIdx] == text [currentNode->arcs [arcNumber]->arcs [k]->left]) {
                    // новая строка входит в текущую дугу
                    if (savedSuffixRef != nullptr) {
                        savedSuffixRef->suffixRef = currentNode->arcs [arcNumber];
                        savedSuffixRef = nullptr;
                    }
                    return; // вставка завершена (использовано правило продолжения 3)
                }
            }
            // если из этой вершины не исходит дуг, начинающихся на вставляемую букву
            AddToNode (currentNode->arcs [arcNumber], newStrIdx); // (правило продолжения 2)
            if (leafCounter > newStrIdx) {
                return;
            }
            SuffixRefTransition (currentNode->arcs [arcNumber]->suffixRef, 0, -1, newStrIdx);
        }
    }
    void ApplyExtensionRules (TNode* currentNode, int newStrIdx) {
        for (int k = 0; k < currentNode->arcs.size (); k ++) {
            if (text [newStrIdx] == text [currentNode->arcs [k]->left]) {
                // новая строка входит в текущую дугу
                if (savedSuffixRef != nullptr) {
                    savedSuffixRef->suffixRef = currentNode;
                    savedSuffixRef = nullptr;
                }
                return; // вставка завершена (использовано правило продолжения 3)
            }
        }
        // если из этой вершины не исходит дуг, начинающихся на вставляемую букву
        AddToNode (currentNode, newStrIdx); // (правило продолжения 2)
        if (leafCounter > newStrIdx) {
            return;
        }
        if (currentNode != root) {
            SuffixRefTransition (currentNode->suffixRef, 0, -1, newStrIdx);
        }
        else {
            AddNewString (currentNode, leafCounter, newStrIdx);
        }
    }
    void AddNewString (TNode* currentNode, int newStrLeft, int newStrRight) {
        if (newStrLeft == newStrRight) {
            ApplyExtensionRules (currentNode, newStrRight);
            return;
        }
        int newStringIdx = newStrLeft;
        while (true) {
            for (int i = 0; i < int (currentNode->arcs.size ()); i ++) { // Проверить каждое ребро, выходящее из этой вершины
                bool signal = false; // результат выполнения сравнения будет записан в эту переменную
                int idx = newStringIdx; // закрепить текущий левый край новой строки
                for (int j = currentNode->arcs [i]->left; j <= GetRight (currentNode->arcs [i]); j ++) { // сравнить строку в ребре с новой
                    if (text [idx] == text [j]) { // пока строки совпадают, продолжать сравнение
                        idx ++;
                    }
                    else { // если несовпадение в первой букве
                        signal = true; // перейти к другой дуге
                        break;
                    }
                    if (idx == newStrRight) { // Путь до вставки новой буквы пройден
                        ApplyExtensionRules (currentNode, i, j, newStrRight); // применить правила продолжения
                        return;
                    }
                }
                if (!signal) { // попадает в этот блок, если сравнение прошло успешно, но новая строка ещё не кончилась
                    currentNode = currentNode->arcs [i]; // перейти к вершине, к которой вела эта дуга
                    newStringIdx = idx; // в ней начать сравнение с непроверенной части строки
                    break; // выйти из цикла проверки дуг
                }
            }
        }
    }
    void CleanRecursive (TNode* currentNode) {
        if (currentNode == nullptr) {
            return;
        }
        for (int i = 0; i < int (currentNode->arcs.size ()); i ++) {
            CleanRecursive (currentNode->arcs [i]);
        }
        delete currentNode;
    }
public:
    TSuffixTree () {
        root = nullptr;
        leafCounter = 0;
        savedSuffixRef = nullptr;
    }
    ~TSuffixTree () {
        CleanRecursive (root);
    }
    int RecCounter (TNode* curr) {
        if (curr != root) {
            if (curr->leafNumber != -1) {
                curr->leaves.push_back(curr->leafNumber);
                return (curr->leafNumber);
            }
            else {
                for (int i = 0; i < curr->arcs.size (); i ++) {
                    int res = RecCounter (curr->arcs[i]);
                    if (res == -1) {
                        //sort (curr->arcs[i]->leaves.begin (), curr->arcs[i]->leaves.end ());
                        curr->leaves.insert (curr->leaves.end (), curr->arcs[i]->leaves.begin (), curr->arcs[i]->leaves.end ());
                    }
                    else {
                        curr->leaves.push_back (res);
                    }
                }
                sort (curr->leaves.begin (), curr->leaves.end ());
                return -1;
            }
            cout << "Something wrong in recursion";
            return -2;
        }
        else {
            for (int i = 0; i < curr->arcs.size (); i ++) {
                RecCounter (curr->arcs[i]);
            } 
        }
        
        return -1;
    }
    void BuildTree (string & newText) {
        text = newText + '$';
        end = 0;
        root = new TNode;
        AddToNode (root, 0);
        for (int i = 1; i < int (text.size ()); i ++) {
            end ++;
            savedSuffixRef = nullptr;
            AddNewString (root, leafCounter, i);
        }
        if (root->arcs.size () > 1) { // если в дереве есть что-то
            if (text [root->arcs [root->arcs.size () - 1]->left] == '$') {
                delete root->arcs [root->arcs.size () - 1]; // удалить исходящую из корня дугу,
                root->arcs.pop_back ();                     // содержащую только сентинел
            }
        }
        RecCounter (root);
    }
    void FindPattern (string & pattern, TNode* curr, int pb, int c) {
        if (curr == nullptr) {
            curr = root;
        }
        else if (curr->leafNumber != -1) {
            return;
        }

        for (int i = 0; i < curr->arcs.size (); i++) {
            if (pattern [pb] == text [curr->arcs[i]->left]) {
                for (int j = curr->arcs[i]->left; j <= GetRight (curr->arcs[i]); j ++) {
                    if (pattern [pb] != text [j]) {
                        return;
                    }
                    if (pb == pattern.size () - 1) {
                        cout << c << ": " << curr->arcs[i]->leaves [0] + 1;
                        for (int k = 1; k < curr->arcs[i]->leaves.size (); k ++) {
                            cout << ", " << curr->arcs[i]->leaves [k] + 1;
                        }
                        cout << endl;
                    }
                    pb ++;
                }
                FindPattern (pattern, curr->arcs[i], pb, c);
                return;
            }
        }
    }
};

void TSuffixTree::SuffixRefTransition (TNode* currentNode, int strToSkipLength, int strToSkipBeginIdx, int newStrIdx) {
    if (strToSkipLength == 0) {
        ApplyExtensionRules (currentNode, newStrIdx);
        return;
    }
    while (true) {
        for (int i = 0; i < int (currentNode->arcs.size ()); i ++) { // Проверить каждое ребро, выходящее из этой вершины
            if (text [strToSkipBeginIdx] == text [currentNode->arcs [i]->left]) { // если первый символ совпал
                int currentStrLen = GetRight (currentNode->arcs [i]) - currentNode->arcs [i]->left + 1;
                if (strToSkipLength > currentStrLen) { // если длина текущей строки меньше длины строки, которую нужно пропустить
                    strToSkipLength -= currentStrLen; // прыжок по счётчику
                    strToSkipBeginIdx += currentStrLen;
                    currentNode = currentNode->arcs [i];
                    break;
                }
                else { // если длина текущей строки больше длины строки (или равна длине строки), которую нужно пропустить
                    ApplyExtensionRules (currentNode, i, currentNode->arcs [i]->left + strToSkipLength - 1, newStrIdx);
                    return;
                }
            }
        }
    }
}



int main () {
    string text;
    string pattern;
    TSuffixTree* suffixTree = new TSuffixTree;
    vector <int> result;
    cin >> text;
    suffixTree->BuildTree (text);
    int i = 0;
    while (cin >> pattern) {
        i ++;
        if (pattern.size() <= text.size ()) {
            suffixTree->FindPattern (pattern, nullptr, 0, i);
        }
    }
    delete suffixTree;
    return 0;
}