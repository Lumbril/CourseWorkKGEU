#include <iostream>
#include <vector>
#include "vector.h"
using namespace std;

// Enum для обозначения лексем
enum LexemeType{
    LEFT_BRACKET, RIGHT_BRACKET,            // левая, правая скобка
    OP_MINUS, OP_PLUS, OP_MULT, OP_DIV,     // операторы -, +, *, /
    NUMBER,                                 // число
    END_STR                                 // конец строки
};

// Класс для лексемы
class Lexeme {
public:
    LexemeType type;
    string val;

    Lexeme(){}

    Lexeme(LexemeType t, string s) {
        type = t;
        val = s;
    }

    Lexeme(LexemeType t, const char ch) {
        type = t;
        val = ch;
    }

    string toString() {
        string str = "Lexeme{ ";
        str += "type=" + type;
        str += ", value='" + val + '\'';
        str += "}";

        return  str;
    }
};

// Вспомогательный класс для лексем
class LexemeBuffer {
private:
    int pos;
public:
    Vector<Lexeme> lexemes;

    LexemeBuffer(){}

    LexemeBuffer(Vector<Lexeme> lex) {
        for (int i = 0; i < lex.size(); ++i) {
            lexemes.push_back(lex[i]);
        }
    }

    Lexeme next() {
        return lexemes[pos++];
    }

    void back() {
        pos--;
    }

    int getPos() {
        return pos;
    }

    LexemeBuffer operator = (Vector<Lexeme> lex) {
        for (int i = 0; i < lex.size(); ++i) {
            lexemes.push_back(lex[i]);
        }
    }
};

LexemeBuffer lex;

Vector<Lexeme> lexemeAnalyze(string expText) {
    Vector<Lexeme> lexemes; // массив лексем
    int pos = 0;

    // разбиение строки на лексемы
    while (pos < expText.size()) {
        char ch = expText[pos];

        switch (ch) {
            case '(':
                lexemes.push_back(Lexeme(LEFT_BRACKET, ch));
                pos++;

                break;
            case ')':
                lexemes.push_back(Lexeme(RIGHT_BRACKET, ch));
                pos++;

                break;
            case '+':
                lexemes.push_back(Lexeme(OP_PLUS, ch));
                pos++;

                break;
            case '-':
                lexemes.push_back(Lexeme(OP_MINUS, ch));
                pos++;

                break;
            case '*':
                lexemes.push_back(Lexeme(OP_MULT, ch));
                pos++;

                break;
            case '/':
                lexemes.push_back(Lexeme(OP_DIV, ch));
                pos++;

                break;
            default:
                if ('0' <= ch && ch <= '9') {
                    string buf = "";

                    do {
                        buf += ch;
                        pos++;

                        if (pos >= expText.size()) {
                            break;
                        }

                        ch = expText[pos];
                    } while ('0' <= ch && ch <= '9');

                    lexemes.push_back(Lexeme(NUMBER, buf));
                } else {
                    if (ch != ' ') {
                        throw ("Invalid character" + ch);
                    }

                    pos++;
                }
        }
    }

    lexemes.push_back(Lexeme(END_STR, ""));

    return lexemes;
}

string optimize(string s) {
    string buff = "";

    if (s[0] == '-') {
        s = "(0" + s;

        int i = 4;

        while ('0' <= s[i] && s[i] <= '9') {
            i++;
        }

        s.insert(i, ")");
    }

    for (int i = 0; i < s.size(); i++) {
        if ((s[i] == '(' || s[i] == '/' || s[i] == '*' || s[i] == '+' || s[i] == '-') && s[i + 1] == '-') {
            s.insert(i + 1, "(0");

            int j = i + 4;

            while ('0' <= s[j] && s[j] <= '9') {
                j++;
            }

            s.insert(j, ")");
        }
    }

    return s;
}

int plusminus();

int expr() {
    Lexeme lexeme = lex.next();

    if (lexeme.type == END_STR) {
        return 0;
    } else {
        lex.back();

        return plusminus();
    }
}

int factor() {
    Lexeme lexeme = lex.next();
    int val;

    switch (lexeme.type) {
        case NUMBER:
            return atoi(lexeme.val.c_str());
        case LEFT_BRACKET:
            val = expr();
            lexeme = lex.next();

            if (lexeme.type != RIGHT_BRACKET) {
                throw ("Forgot bracket");
            }

            return val;
        default:
            throw ("Incorrect entry");
    }
}

int multdiv() {
    int val = factor();

    while (1) {
        Lexeme lexeme = lex.next();

        switch (lexeme.type) {
            case OP_MULT:
                val *= factor();

                break;
            case OP_DIV:
                val /= factor();

                break;
            default:
                lex.back();

                return val;
        }
    }
}

int plusminus() {
    int val = multdiv();

    while (1) {
        Lexeme lexeme = lex.next();

        switch (lexeme.type) {
            case OP_PLUS:
                val += multdiv();

                break;
            case OP_MINUS:
                val -= multdiv();

                break;
            default:
                lex.back();

                return val;
        }
    }
}

int Form(string expression, int x, int y) {
    // Заменяем все X и Y на их значения
    while (expression.find('x') != string :: npos) {
        expression.replace(expression.find('x'), 1, to_string(x));
    }

    while (expression.find('y') != string :: npos) {
        expression.replace(expression.find('y'), 1, to_string(y));
    }

    //обрабатываем запись отрицательного числа в скобках/как начало строки/как -<x|y>
    expression = optimize(expression);

    Vector<Lexeme> lexemes = lexemeAnalyze(expression);

    lex = lexemes;

    int ans = expr();

    return ans;
}

int main() {
    string expression;

    cout << "Expression: ";
    getline(cin, expression); // Вводим арифметическое выражение

    int x, y;

    // Водим Х и Y
    cout << "x: ";
    cin >> x;
    cout << "y: ";
    cin >> y;

    //Вызов нашей функции
    int ans = Form(expression, x, y);

    cout << "Answer: " << ans;

    return 0;
}
