//
//  lexer.c
//  compiler
//
//  Created by 姜友华 on 2019/10/16.
//  Copyright © 2019 姜友华. All rights reserved.
//

#include "lexer.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// 记号流。
FILE *fp;                             // 读取的文件流。
struct Token tokens[1024];            // 保存所有记号。
int len = 0;                          // 保存记号数组实际长度。
void nextToken(char a);

/**
 2. 判断各种关键词。
 */
// Bool
int isBool(char *s){
    return (s[0] == 'b' && s[1] == 'o' && s[2] == 'o' && s[3] == 'l' && s[4] == '\0');
}

// int
int isInt(char *s){
    return (s[0] == 'i' && s[1] == 'n' && s[2] == 't' && s[3] == '\0');
}

// string
int isString(char *s){
    return (s[0] == 's' && s[1] == 't' && s[2] == 'r' && s[3] == 'i' && s[4] == 'n' && s[5] == 'g' && s[6] == '\0');
}

// true
int isTrue(char *s){
    return (s[0] == 't' && s[1] == 'r' && s[2] == 'u' && s[3] == 'e' && s[4] == '\0');
}

// false
int isFalse(char *s){
    return (s[0] == 'f' && s[1] == 'a' && s[2] == 'l' && s[3] == 's' && s[4] == 'e' && s[5] == '\0');
}

// return
int isReturn(char *s){
    return (s[0] == 'r' && s[1] == 'e' && s[2] == 't' && s[3] == 'u' && s[4] == 'r' && s[5] == 'n' && s[6] == '\0');
}

// if
int isIf(char *s){
    return (s[0] == 'i' && s[1] == 'f' && s[2] == '\0');
}

// for
int isFor(char *s){
    return (s[0] == 'f' && s[1] == 'o' && s[2] == 'r' && s[3] == '\0');
}

// in
int isIn(char *s){
    return (s[0] == 'i' && s[1] == 'n' && s[2] == '\0');
}

/**
 3. 判断Char属性。
 */
// 是否为空字符
int isEmpty(char a){
    return a == ' ' || a == '\t' || a == '\n' || a == '\r';
}

// 是否为数字
int isDigit(char a){
    return a >= '0' && a <= '9';
}

// 是否为字母
int isLetter(char a){
    return (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z');
}

/**
 4. 词法分析的实现。
 */

// 添加Token.
struct Token* addToken(enum Kind key, char *value){
    int i = len;
    tokens[len].key = key;
    strcpy(tokens[len].value, value);
    len++;
    return &tokens[i];
}

//  数字字面量。
void keyWithDigit(int *i, struct Token* token){
    char c = fgetc(fp);
    if (!isDigit(c)){
        return nextToken(c);
    }
    (*i)++;
    token->value[*i] = (char)c;
    return keyWithDigit(i, token);
}

// 字符串字面量。
void keyWithCharacters(int *i, struct Token* token){
    char c = fgetc(fp);
    if (c == '\n' || c == EOF){
        printf("Quotes without ending quotes.");
        exit(0);
    }
    (*i)++;
    token->value[*i] = (char)c;
    if (c == '"'){
        return;
    }
    return keyWithCharacters(i, token);
}

// 变量名与保留字。
void keyWithIdentityOrWord(int *i, struct Token* token){
    char c = fgetc(fp);
    if (!isDigit(c) && !isLetter(c) && c != '_'){
        // 函数。
        if (c == '('){
            token->key = FUNCTION;
        }
        nextToken(c);
        return;
    }
    (*i)++;
    token->value[*i] = (char)c;
    return keyWithIdentityOrWord(i, token);
}

// 对变量名与字符串字面量进行解析。
void singleOrMultipleCharacter(char a){
    int i = 0;
    
    // 数字。
    if (isDigit(a)){
        struct Token *token = addToken(0, &a);
        token->key = DIGIT;
        keyWithDigit(&i, token);
        token->value[i + 1] = '\0';
        return;
    }
    
    // 字符串。
    if (a == '"'){
        struct Token *token = addToken(0, &a);
        token->key = CHARACTERS;
        keyWithCharacters(&i, token);
        token->value[i + 1] = '\0';
        return;
    }
    
    // 变量名或保留字。
    if (isLetter(a) || a == '_'){
        struct Token *token = addToken(0, &a);
        token->key = IDENTITY;
        keyWithIdentityOrWord(&i, token);
        token->value[i + 1] = '\0';
        
        // 判断是否为保留字。
        // bool.
        if (isBool(token->value)){
            token->key = BOOLEAN;
            return;
        }
        // int.
        if (isInt(token->value)){
            token->key = INT;
            return;
        }
        // string.
        if (isString(token->value)){
            token->key = STRING;
            return;
        }
        // return.
        if (isReturn(token->value)){
            token->key = RETURN;
            return;
        }
        // true.
        if (isTrue(token->value)){
            token->key = ISTRUE;
            return;
        }
        // false.
        if (isFalse(token->value)){
            token->key = ISFALSE;
            return;
        }
        // if.
        if (isIf(token->value)){
            token->key = IF;
            return;
        }
        // for.
        if (isFor(token->value)){
            token->key = FOR;
            return;
        }
        // in.
        if (isIn(token->value)){
            token->key = IN;
            return;
        }
    }
}

// 读字符串。
void nextToken(char a){
    // 如果读到空字符串，重新读；
    if (isEmpty(a)){
        return;
    }
    
    // 获取具体值。
    switch (a) {
            //  Single character。
        case '=':
            addToken(ASSIGN, "=");
            break;
        case ',':
            addToken(COMMA, ",");
            break;
        case ';':
            addToken(SEMICOLON, ";");
            break;
        case ':':
            addToken(COLON, ":");
            break;
        case '(':
            addToken(LPARENTHESES, "(");
            break;
        case ')':
            addToken(RPARENTHESES, ")");
            break;
        case '[':
            addToken(LBRACKETS, "[");
            break;
        case ']':
            addToken(RBRACKETS, "]");
            break;
        case '{':
            addToken(LBRACES, "{");
            break;
        case '}':
            addToken(RBRACES, "}");
            break;
        case '+':
            addToken(ADD, "+");
            break;
            // Single or double character。
        case '<':
            addToken(LTHAN, "<");
            // Single or Multiple character。
        default:
            singleOrMultipleCharacter(a);
            break;
    }
}

// 获取字节流。
struct Token* getTokens(){
    return tokens;
}

// 获取字节流长度。
int getTokensLen(){
    return len;
}

// 从文件读。
void lexerRun(char *file){
    fp = fopen(file, "r");
    while(!feof(fp)){
        char a = fgetc(fp);
        nextToken(a);
    }
    fclose(fp);
}
