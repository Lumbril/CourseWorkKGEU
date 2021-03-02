#include <iostream>
using namespace std;

const char LEFT_BRACKET = '(';
const char RIGHT_BRACKET = ')';
const char OP_PLUS = '+';
const char OP_MINUS = '-';
const char OP_MULT = '*';
const char OP_DIV = '/';
const char NUMBER = 'N';
const char END_STR = 'E';

struct Lexeme {
    char type;
    string val;
};

Lexeme lexems[100000];
int posLexems = 0;

Lexeme lexemBuffer[100000];
int posLB = 0;
int szLB;

void initLB() {
    szLB = posLexems;

    for (int i = 0; i < posLexems; i++) {
        lexemBuffer[i] = lexems[i];
    }
}

Lexeme nextLB() {
    return lexemBuffer[posLB++];
}

void backLB() {
    posLB--;
}

int getPosLB() {
    return posLB;
}

void lexexAnalyze(string expText) {
    int pos = 0;

    while (pos < expText.size()) {
        char ch = expText[pos];

        switch (ch) {
            case LEFT_BRACKET:
                lexems[posLexems++] = {LEFT_BRACKET, "("};
                pos++;

                break;
            case RIGHT_BRACKET:
                lexems[posLexems++] = {RIGHT_BRACKET, ")"};
                pos++;

                break;
            case OP_PLUS:
                lexems[posLexems++] = {OP_PLUS, "+"};
                pos++;

                break;
            case OP_MINUS:
                lexems[posLexems++] = {OP_MINUS, "-"};
                pos++;

                break;
            case OP_MULT:
                lexems[posLexems++] = {OP_MULT, "*"};
                pos++;

                break;
            case OP_DIV:
                lexems[posLexems++] = {OP_DIV, "/"};
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

                    lexems[posLexems++] = {NUMBER, buf};
                } else {
                    if (ch != ' ') {
                        throw ("Invalid character" + ch);
                    }

                    pos++;
                }
        }
    }

    lexems[posLexems++] = {END_STR, "END"};
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

int expr();
int plusminus();
int multdiv();
int factor();

int expr() {
    Lexeme lexeme = nextLB();

    if (lexeme.type == END_STR) {
        return 0;
    } else {
        backLB();

        return plusminus();
    }
}

int plusminus() {
    int val = multdiv();

    while (1) {
        Lexeme lexeme = nextLB();

        switch (lexeme.type) {
            case OP_PLUS:
                val += multdiv();

                break;
            case OP_MINUS:
                val -= multdiv();

                break;
            default:
                backLB();

                return val;
        }
    }
}

int multdiv() {
    int val = factor();

    while (1) {
        Lexeme lexeme = nextLB();

        switch (lexeme.type) {
            case OP_MULT:
                val *= factor();

                break;
            case OP_DIV:
                val /= factor();

                break;
            default:
                backLB();

                return val;
        }
    }
}

int factor() {
    Lexeme lexeme = nextLB();
    int val;

    switch (lexeme.type) {
        case NUMBER:
            return atoi(lexeme.val.c_str());
        case LEFT_BRACKET:
            val = expr();
            lexeme = nextLB();

            if (lexeme.type != RIGHT_BRACKET) {
                throw ("Forgot bracket");
            }

            return val;
        default:
            throw ("Incorrect entry");
    }
}

int Form(string expression, int x, int y) {
    while (expression.find('x') != string :: npos) {
        expression.replace(expression.find('x'), 1, to_string(x));
    }

    while (expression.find('y') != string :: npos) {
        expression.replace(expression.find('y'), 1, to_string(y));
    }

    expression = optimize(expression);

    lexexAnalyze(expression);

    initLB();
    int ans = expr();

    return ans;
}

int main() {
    string expression;

    cout << "Expression: ";
    getline(cin, expression); // ������ �������������� ���������

    int x, y;

    cout << "x: ";
    cin >> x;
    cout << "y: ";
    cin >> y;

    int ans = Form(expression, x, y);

    cout << "Answer: " << ans;

    return 0;
}
