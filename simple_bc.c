#include <stdio.h>
#include <stdlib.h>

struct Peek {
    enum PeekKind {
        Start,
        Char,
    } kind;
    char c;
} peek = { Start,  '0' };

int is_num(char c) { return '0' <= c && c <= '9'; }
int ctoi(char c) { return c - '0'; }

int valid_char(char c)
{
    return c == EOF
        || c == '+' || c == '-'
        || c == '*' || c == '/'
        || c == '(' || c == ')'
        || is_num(c);
}

char next_char()
{
    char c;
    do {
        c = getchar();
    } while (!valid_char(c));

    if (peek.kind == Start) {
        peek.kind = Char;
    }
    peek.c = c;
    return peek.c;
}

char peek_char() {
    if (peek.kind == Start) {
        return next_char();
    } else {
        return peek.c;
    }
}

struct Token {
    enum TokenKind {
        Plus,
        Minus,
        Mult,
        Div,
        Open,
        Close,
        Number,
        Eof,
    } kind;
    int number;
};

struct Token next_token()
{
    char c;
    struct Token token;

    switch (c = peek_char()) {
    case '+':
        token.kind = Plus;
        next_char();
        break;
    case '-':
        token.kind = Minus;
        next_char();
        break;
    case '*':
        token.kind = Mult;
        next_char();
        break;
    case '/':
        token.kind = Div;
        next_char();
        break;
    case '(':
        token.kind = Open;
        next_char();
        break;
    case ')':
        token.kind = Close;
        next_char();
        break;
    case EOF:
        token.kind = Eof;
        next_char();
        break;
    default:
        token.kind = Number;
        token.number = ctoi(c);
        while (is_num((c = next_char()))) {
            token.number = token.number * 10 + ctoi(c);
        }
        break;
    }
    return token;
}

int calc();
int calc_(int lhs)
{
    struct Token token = next_token(), ntoken;

    switch (token.kind) {
    case Eof:
    case Close:
        return lhs;
    case Plus:
        ntoken = next_token();
        switch (ntoken.kind) {
        case Number:
            return lhs + calc_(ntoken.number);
        case Open:
            return lhs + calc_(calc());
        default:
            fprintf(stderr, "%s", "[FATAL ERROR]: not number or open\n");
            exit(1);
        }
    case Minus:
        ntoken = next_token();
        switch (ntoken.kind) {
        case Number:
            return lhs + calc_(-ntoken.number);
        case Open:
            return lhs + calc_(-calc());
        default:
            fprintf(stderr, "%s", "[FATAL ERROR]: not number or open\n");
            exit(1);
        }
    case Mult:
        ntoken = next_token();
        switch (ntoken.kind) {
        case Number:
            return calc_(lhs * ntoken.number);
        case Open:
            return calc_(lhs * calc());
        default:
            fprintf(stderr, "%s", "[FATAL ERROR]: not number or open\n");
            exit(1);
        }
    case Div:
        ntoken = next_token();
        switch (ntoken.kind) {
        case Number:
            return calc_(lhs / ntoken.number);
        case Open:
            return calc_(lhs / calc());
        default:
            fprintf(stderr, "%s", "[FATAL ERROR]: not number or open\n");
            exit(1);
        }
    default:
        fprintf(stderr, "%s", "[FATAL ERROR]: number or open\n");
        exit(1);
    }
}

int calc()
{
    struct Token token = next_token();
    switch (token.kind) {
    case Number:
        return calc_(token.number);
    case Open:
        return calc_(calc());
    default:
        fprintf(stderr, "%s", "[FATAL ERROR]: not number or eof\n");
        exit(1);
    }
}

int main()
{
    printf("%d\n", calc());
    return 0;
}
