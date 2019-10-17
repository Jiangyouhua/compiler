//
//  lexer.h
//  compiler
//
//  Created by 姜友华 on 2019/10/16.
//  Copyright © 2019 姜友华. All rights reserved.
//

#ifndef lexer_h
#define lexer_h

#include <stdio.h>

/**
 1. 词法分析器的数据。
 */
// K语言类型。
enum Kind {
    BOOLEAN,        // 0 布尔。
    INT,            // 1 整型。
    STRING,         // 2 字符串。
    FUNCTION,       // 3 函数。
    IDENTITY,       // 4 变量名。
    DIGIT,          // 5 数字字面量。
    CHARACTERS,     // 6 字符串字面量。
    IF,             // 7 条件。
    FOR,            // 8 循环。
    IN,             // 9 之内。
    RETURN,         // 10 返回。
    ASSIGN,         // 11 赋值。
    COMMA,          // 12 逗号。
    SEMICOLON,      // 13 分号。
    COLON,          // 14 分号。
    LPARENTHESES,   // 15 左小括号。
    RPARENTHESES,   // 16 右小括号。
    LBRACKETS,      // 17 左中括号。
    RBRACKETS,      // 18 右中括号。
    LBRACES,        // 19 左大括号。
    RBRACES,        // 20 右大括号。
    ADD,            // 21 加。
    LTHAN,          // 22 小于。
    END,            // 23 结束。
    ISTRUE,         // 24 true。
    ISFALSE,        // 25 false。
};

// 记号流格式。
struct Token {
    enum Kind key;
    char value[256];
};

// 获取字节流。
struct Token* getTokens(void);

// 获取字节流长度。
int getTokensLen(void);

// 运行词法分析器。
void lexerRun(char *file);

#endif /* lexer_h */
