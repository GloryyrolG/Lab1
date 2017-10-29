#include "editor.h"

using namespace std;

int main() {
    bool flag = 0, flag1 = 0;
    int no;
    char str[256], _str[256];
    String fin, fout;
    Buffer buf;
    cout << "> ";
    char op[2];
    for (; ; ) {
        scanf("%s", op);
        if (!flag && op[0] != 'R' && op[0] != 'H') {
            cout << "Please open a file first!" << endl;
            cout << "> ";
            continue;
        }
        switch (op[0]) {
            case 'R': {
                for (; ; ) {
                    cout << "Please enter the name of input file: ";
                    cin >> fin;
                    if (buf.Fin(fin._str()) != -1) {
                        flag = 1;
                        break;
                    }
                }
                break;
            }
            case 'W': {
                cout << "Please enter the name of output file: ";
                cin >> fout;
                flag1 = 1;
                buf.Fout(fout._str());
                break;
            }
            case 'I': {
                printf("Please enter the line number(/%d): ", buf.CountRow());
                cin >> no;
                cout << "Please enter the content of the line: " << endl;
                getchar(); gets(str);
                buf.Insert(no, Row(str));
                break;
            }
            case 'D': {
                buf.Delete();
                break;
            }
            case 'F': {
                printf("Point: %d\n", buf.CurrNo());
                cout << "Please enter the key: " << endl;
                getchar(); gets(str);
                cout << "The first position: " << buf.Find(str) << endl;
                break;
            }
            case 'C': {
                bool flag2 = 0;
                cout << "Please enter the key: " << endl;
                getchar(); gets(str);
                for (; str[0] == '\0'; ) {
                    gets(str);
                }
                cout << "Substitute it by: " << endl;
                gets(_str);
                printf("Please select the range(1.current row(%d)/2.whole file): \n", buf.CurrNo());
                cout << "No: ";
                cin >> no;
                for (; !flag2; ) {
                    switch (no) {
                        case 1: {
                            buf.ReplaceCurr(str, _str);
                            flag2 = 1;
                        }
                        case 2: {
                            buf.ReplaceAll(str, _str);
                            flag2 = 1;
                        }
                    }
                }
                break;
            }
            case 'H': {
                cout << "R: Read from file" << endl;
                cout << "W: Write into file" << endl;
                cout << "I: Insert a line" << endl;
                cout << "D: Delete a line" << endl;
                cout << "F: Find the first line which contains a certain string" << endl;
                cout << "C: Substitute one string with another among the current line or the whole file" << endl;
                cout << "N: Move point to the next" << endl;
                cout << "P: Move point to the previous" << endl;
                cout << "B: Move point to the beginning" << endl;
                cout << "E: Move point to the end" << endl;
                cout << "G: Move point to a certain line" << endl;
                cout << "V: Display the content of buffer" << endl;
                cout << "Q: Quit and save the file" << endl;
                break;
            }
            case 'N': {
                buf.MoveToNext();
                printf("Point: %d\n", buf.CurrNo());
                break;
            }
            case 'P': {
                buf.MoveToPre();
                printf("Point: %d\n", buf.CurrNo());
                break;
            }
            case 'B': {
                buf.MoveToStart();
                break;
            }
            case 'E': {
                buf.MoveToEnd();
                break;
            }
            case 'G': {
                printf("Please enter the line number(%d/%d): ", buf.CurrNo(), buf.CountRow());
                cin >> no;
                buf.MoveToPos(no);
                break;
            }
            case 'V': {
                buf.Display();
                printf("Total chars: %d\n", buf.CountChar());
                break;
            }
            case 'Q': {
                if (!flag1) {
                    cout << "Please enter the name of output file: ";
                    cin >> fout;
                    flag1 = 1;
                }
                buf.Fout(fout._str());
                return 0;
            }
        }
        cout << "> ";
    }
    return 0;
}
