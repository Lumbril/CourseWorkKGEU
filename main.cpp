#include <iostream>
using namespace std;

// список лексем
enum LexemeType{
    LEFT_BRACKET, RIGHT_BRACKET,
    OP_PLUS, OP_MINUS, OP_MULT, OP_DIV,
    NUMBER,
    END_STR
};

// структура для хранения одной лексемы
// LexemeType - тип лексемы
// val - ее значение
struct Lexeme {
    LexemeType type;
    string val;
};

// массив лексем
Lexeme lexemes[100000];
int posLexemes = 0;
int szL;

// функция для получения следующей лексемы
Lexeme nextLexeme() {
    return lexemes[posLexemes++];
}

// метод для возрата индекса на одну позицию назад
void backPosLex() {
    posLexemes--;
}

// метод для инициализации массива лексем
void lexemeAnalyze(string expText);

// функция для оптимизации выражений с отриц. числами.
// пример, -3 преобразует в (0 - 3)
string optimize(string s);

// основные функции для анализа мат. выражения
double expr();
double plusminus();
double multdiv();
double factor();

//функция для замены x, y на их значения и вычисление результата
double form(string expression, double x, double y);

int main() {
    string expression; // строка для хранения мат. выражения

    cout << "Expression: ";
    getline(cin, expression);

    double x, y;

    cout << "x: ";
    cin >> x;
    cout << "y: ";
    cin >> y;

    double ans = form(expression, x, y); // вычисление результата

    cout << "Answer: " << ans;

    return 0;
}

double form(string expression, double x, double y) {
    // замена переменных на их значения
    while (expression.find('x') != string::npos) {
        expression.replace(expression.find('x'), 1, to_string(x));
    }

    while (expression.find('y') != string::npos) {
        expression.replace(expression.find('y'), 1, to_string(y));
    }

    // оптимизация отриц. чисел
    expression = optimize(expression);

    // инициализация массива лексем
    lexemeAnalyze(expression);

    double ans = expr();

    return ans;
}

string optimize(string s) {
    if (s[0] == '-') {
        s = "(0" + s;

        int i = 4;

        while ('0' <= s[i] && s[i] <= '9' || s[i] == '.') {
            i++;
        }

        s.insert(i, ")");
    }

    for (int i = 0; i < s.size(); i++) {
        if ((s[i] == '(' || s[i] == '/' || s[i] == '*' || s[i] == '+' || s[i] == '-') && s[i + 1] == '-') {
            s.insert(i + 1, "(0");

            int j = i + 4;

            while ('0' <= s[j] && s[j] <= '9' || s[j] == '.') {
                j++;
            }

            s.insert(j, ")");
        }
    }

    return s;
}

void lexemeAnalyze(string expText) {
    int pos = 0;

    while (pos < expText.size()) {
        char ch = expText[pos];

        switch (ch) {
            case '(':
                lexemes[posLexemes++] = {LEFT_BRACKET, "("};
                pos++;

                break;
            case ')':
                lexemes[posLexemes++] = {RIGHT_BRACKET, ")"};
                pos++;

                break;
            case '+':
                lexemes[posLexemes++] = {OP_PLUS, "+"};
                pos++;

                break;
            case '-':
                lexemes[posLexemes++] = {OP_MINUS, "-"};
                pos++;

                break;
            case '*':
                lexemes[posLexemes++] = {OP_MULT, "*"};
                pos++;

                break;
            case '/':
                lexemes[posLexemes++] = {OP_DIV, "/"};
                pos++;

                break;
            default:
                if (('0' <= ch && ch <= '9') || (ch == '.' || ch == ',')) {
                    string buf = "";

                    do {
                        if (ch != ','){
                            buf += ch;
                        } else {
                            buf += '.';
                        }

                        pos++;

                        if (pos >= expText.size()) {
                            break;
                        }

                        ch = expText[pos];
                    } while (('0' <= ch && ch <= '9') || (ch == '.' || ch == ','));

                    lexemes[posLexemes++] = {NUMBER, buf};
                } else {
                    if (ch != ' ') {
                        throw ("Invalid character" + ch);
                    }

                    pos++;
                }
        }
    }

    lexemes[posLexemes] = {END_STR, "END"};
    posLexemes = 0;
}

double expr() {
    Lexeme lexeme = nextLexeme();

    if (lexeme.type == END_STR) {
        return 0;
    } else {
        backPosLex();

        return plusminus();
    }
}

double plusminus() {
    double val = multdiv();

    while (1) {
        Lexeme lexeme = nextLexeme();

        switch (lexeme.type) {
            case OP_PLUS:
                val += multdiv();

                break;
            case OP_MINUS:
                val -= multdiv();

                break;
            default:
                backPosLex();

                return val;
        }
    }
}

double multdiv() {
    double val = factor();

    while (1) {
        Lexeme lexeme = nextLexeme();

        switch (lexeme.type) {
            case OP_MULT:
                val *= factor();

                break;
            case OP_DIV:
                val /= factor();

                break;
            default:
                backPosLex();

                return val;
        }
    }
}

double factor() {
    Lexeme lexeme = nextLexeme();
    double val;

    switch (lexeme.type) {
        case NUMBER:
            return stod(lexeme.val.c_str());
        case LEFT_BRACKET:
            val = expr();
            lexeme = nextLexeme();

            if (lexeme.type != RIGHT_BRACKET) {
                throw ("Forgot bracket");
            }

            return val;
        default:
            throw ("Incorrect entry");
    }
}