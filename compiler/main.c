//
//  main.c
//  compiler
//
//  Created by 姜友华 on 2019/10/16.
//  Copyright © 2019 姜友华. All rights reserved.
//

#include "lexer/lexer.h"

int main()
{
    // 解析器。
    printf("Input King Lang:\n");
    
    // 读文原代码文件。
    lexerRun("/Users/jiangyouhua/Documents/code/c/compiler/compiler/king.txt");
    // 输出解析结果；
    int len = getTokensLen();
    if (!len) {
        return 0;
    }
    struct Token* p = getTokens();
    for (int i = 0; i < len; i++){
        printf("%d: %u, %s.\n", i, p->key, p->value);
        p++;
    }
    return 0;
}
