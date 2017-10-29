#ifndef __EDITOR_H__
#define __EDITOR_H__

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// No modify;
class String {
    friend istream& operator >>(istream &fin, String &A);
    friend ostream& operator <<(ostream &fout, const String &B);

    private:
        int len;
        char *str;

    public:
        String();
        String(char *_str);
        String(const String &B);
        ~String();
        int Length();
        char* _str();
        void Set(char *_str);
        void Cpy(const String &B);
        int Fnd(const int st, const String &B);
        void Subs(const String &B, const String &_B);
        char operator [](const int pos);
};

String::String() {
    len = 0; str = NULL;
}

String::String(char *_str) {
    int i;
    char *_p = _str;
    for (len = 0; *_p != '\0'; len ++, _p ++);
    str = new char[len + 1];
    _p = _str;
    for (i = 0; i <= len; i ++) {
        str[i] = _p[i];
    }
}

String::String(const String &B) {
    int i;
    len = B.len;
    str = new char[len + 1];
    for (i = 0; i <= len; i ++) {
        str[i] = B.str[i];
    }
}

String::~String() {
    len = 0;
    if (len > 1) {
        delete [] str;
    }
    str = NULL;
}

int String::Length() {
    return len;
}

char* String::_str() {
    return str;
}

void String::Set(char *_str) {
    int i;
    char *_p = _str;
    if (len > 1) {
        delete [] str;
    }
    for (len = 0; *_p != '\0'; len ++, _p ++);
    str = new char[len + 1];
    _p = _str;
    for (i = 0; i <= len; i ++) {
        str[i] = _p[i];
    }
}

void String::Cpy(const String &B) {
    int i;
    if (len > 1) {
        delete [] str;
    }
    len = B.len;
    str = new char[len + 1];
    for (i = 0; i <= len; i ++) {
        str[i] = B.str[i];
    }
}

// KMP
int String::Fnd(const int st, const String &B) {
    assert(("The string is empty!", str));
    int i, j, pos = -1;
    int* p = new int[B.len];
    j = p[0] = -1;
    for (i = 1; i < B.len; i ++) {
        for (; j >= 0 && B.str[j + 1] != B.str[i]; j = p[j]);
        if (B.str[j + 1] == B.str[i]) {
            j ++;
        }
        p[i] = j;
    }
    j = -1;
    for (i = st; i < len; i ++) {
        for (; j >= 0 && B.str[j + 1] != str[i]; j = p[j]);
        if (B.str[j + 1] == str[i]) {
            j ++;
        }
        if (j == B.len - 1) {
            pos = i - B.len + 1;
            break;
        }
    }
    delete [] p;
    return pos;
}

void String::Subs(const String &B, const String &_B) {
    assert(("The string is empty!", str));
    int i, j, _j, k, p = 0, sz, _len;
    char *_str;
    vector<int> pos;
    pos.clear();
    for (; p < len && (p = Fnd(p, B)) != -1; p += B.len) {
        pos.push_back(p);
    }
    sz = pos.size();
    if (!sz) {
        return ;
    }
    _len = len + sz * (_B.len - B.len);
    _str = new char[_len + 1];
    pos.push_back(len);
    for (j = 0; j < pos[0]; j ++) {
        _str[j] = str[j];
    }
    _j = j;
    for (i = 1; i <= sz; i ++) {
        for (k = 0; k < _B.len; k ++, _j ++) {
            _str[_j] = _B.str[k];
        }
        j += B.len;
        for (; j < pos[i]; j ++, _j ++) {
            _str[_j] = str[j];
        }
    }
    _str[_j] = '\0';
    Set(_str);
    delete [] _str;
}

char String::operator [](const int pos) {
    assert(("Illegal access!", pos >= 0));
    if (pos >= len) {
        return '\0';
    }
    else return str[pos];
}

istream& operator >>(istream &fin, String &A) {
    char _str[256];
    fin >> _str;
    A.Set(_str);
    return fin;
}

ostream& operator <<(ostream &fout, const String &B) {
    fout << B.str;
    return fout;
}

class Row {
    friend class Buffer;

    protected:
        int no;
        String cont;
        Row *pre, *nxt;

    public:
        Row(const String &_cont, const int _no = -1, Row *_pre = NULL, Row *_nxt = NULL) {
            cont = _cont; no = _no; pre = _pre; nxt = _nxt;
        }
        ~Row();
        void Set(const String &_cont);
        void Print();
};

Row::~Row() {
    pre = nxt = NULL;
}

void Row::Set(const String &_cont) {
    cont = _cont;
}

void Row::Print() {
    cout << cont;
}

// ~
class Buffer {
    private:
        int cnt;
        Row *hed, *tail, *curr;

        void Init();

    public:
        Buffer();
        ~Buffer();
        void Clear();
        int Fin(char *fileName);
        void Fout(char *fileName);
        void MoveToStart();
        void MoveToEnd();
        void MoveToPre();
        void MoveToNext();
        void MoveToPos(const int pos);
        void Append(const Row &A);
        void Insert(const int no, const Row &A);
        void Delete();  // move to the next row;
        void Modify(const int no, const Row &A);
        void ReplaceCurr(const String &A, const String &_A);
        void ReplaceAll(const String &A, const String &_A);
        int Find(const String &A);  // start from the current row;
        int CountRow();
        int CountChar();
        int CurrNo();
        void Display();
};

void Buffer::Init() {
    cnt = 0;
    hed = tail = curr = new Row("", 0);
}

Buffer::Buffer() {
    Init();
}

void Buffer::Clear() {
    cnt = 0;
    Row *p = hed->nxt, *q;
    for (; p; ) {
        q = p;
        p = p->nxt;
        delete q;
    }
}

Buffer::~Buffer() {
    Clear();
    delete hed;
    hed = tail = curr = NULL;
}

int Buffer::Fin(char *fileName) {
    Clear(); hed->nxt = NULL;
    ifstream file(fileName, ios::in);
    if (file.fail()) {
        printf("Error in opening the file\n");
        return -1;
    }
    char str[256];
    file.getline(str, 100); // ?
    for (; !file.eof(); ) {
        Append(Row(str));
        file.getline(str, 100);
    }
    file.close();
    return 0;
}

void Buffer::Fout(char *fileName) {
    ofstream file(fileName, ios::out);
    if (file.fail()) {
        printf("Error in opening the file\n");
        return ;
    }
    Row *p = hed->nxt;
    for (; p; p = p->nxt) {
        file << p->cont << endl;
    }
    file.close();
}

void Buffer::MoveToStart() {
    if (!cnt){
        printf("The buffer is empty!\n");
        return ;
    }
    curr = hed->nxt;
}

void Buffer::MoveToEnd() {
    if (!cnt){
        printf("The buffer is empty!\n");
        return ;
    }
    curr = tail;
}

void Buffer::MoveToPre() {
    if (!cnt){
        printf("The buffer is empty!\n");
        return ;
    }
    if (curr->no <= 1){
        printf("It is the start of the buffer!\n");
        return ;
    }
    curr = curr->pre;
}

void Buffer::MoveToNext() {
    if (!cnt){
        printf("The buffer is empty!\n");
        return ;
    }
    if (curr->no >= cnt){
        printf("It is the end of the buffer!\n");
        return ;
    }
    curr = curr->nxt;
}

void Buffer::MoveToPos(const int pos) {
    if (!cnt){
        printf("The buffer is empty!\n");
        return ;
    }
    if (pos <= 0 || pos > cnt){
        printf("It is out of the range!\n");
        return ;
    }
    int i;
    curr = hed;
    for (i = 1; i <= pos; i ++) {
        curr = curr->nxt;
    }
}

void Buffer::Append(const Row &A) {
    tail = curr = curr->nxt = new Row(A.cont, ++ cnt, curr, NULL);
}

void Buffer::Insert(const int no, const Row &A) {
    if (no <= cnt) {
        MoveToPos(no - 1);
        Row *p = curr->nxt;
        curr = p->pre = curr->nxt = new Row(A.cont, no, curr, p);
        cnt ++;
        for (; p; p = p->nxt) {
            p->no ++;
        }
    }
    else {
        MoveToEnd();
        for (cnt ++; cnt < no; cnt ++) {
            curr = curr->nxt = new Row("", cnt, curr, NULL);
        }
        tail = curr = curr->nxt = new Row(A.cont, cnt, curr, NULL);
    }
}

void Buffer::Delete() {
    if (!cnt){
        printf("The buffer is empty!\n");
        return ;
    }
    Row *p = curr, *_pre = curr->pre, *_nxt = curr->nxt;
    _pre->nxt = _nxt;
    if (_nxt) {
        _nxt->pre = _pre;
    }
    else {
        tail = tail->pre;
    }
    delete p;
    cnt --;
    curr = _nxt;
    for (p = curr; p; p = p->nxt) {
        p->no --;
    }
    if (!curr) {
        curr = tail;
    }
}

void Buffer::Modify(const int no, const Row &A) {
    if (no <= 0 || no > cnt){
        printf("It is out of the range!\n");
        return ;
    }
    MoveToPos(no);
    curr->Set(A.cont);
}

void Buffer::ReplaceCurr(const String &A, const String &_A) {
    if (!cnt){
        printf("The buffer is empty!\n");
        return ;
    }
    curr->cont.Subs(A, _A);
}

void Buffer::ReplaceAll(const String &A, const String &_A) {
    if (!cnt){
        printf("The buffer is empty!\n");
        return ;
    }
    Row *p = hed->nxt;
    for (; p; p = p->nxt) {
        p->cont.Subs(A, _A);
    }
}

int Buffer::Find(const String &A) {
    int i;
    Row *p = curr;
    for (i = curr->no; p; p = p->nxt, i ++) {
        if (p->cont.Fnd(0, A) != -1) {
            return i;
        }
    }
    return -1;
}

int Buffer::CountRow() {
    return cnt;
}

int Buffer::CountChar() {
    int sum = 0;
    Row *p = hed->nxt;
    for (; p; p = p->nxt) {
        sum += p->cont.Length();
    }
    return sum;
}

int Buffer::CurrNo() {
    return curr->no;
}

void Buffer::Display() {
    Row *p = hed->nxt;
    for (; p; p = p->nxt) {
        cout << p->no << " : " << p->cont << endl;
    }
}

#endif // __EDITOR_H__
