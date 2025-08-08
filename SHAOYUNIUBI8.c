#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_VARS 100
#define MAX_INPUT_LEN 1024
#define MAX_VARS_NAME_LEN 50

// 变量存储结构
typedef struct {
    char name[MAX_VARS_NAME_LEN];
    int value;
} Variable;

Variable variables[MAX_VARS];
int var_count = 0;

// 查找变量
Variable* find_var(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return &variables[i];
        }
    }
    return NULL;
}

// 添加或更新变量
void set_var(const char* name, int value) {
    Variable* var = find_var(name);
    if (var) {
        var->value = value;
    } else {
        if (var_count < MAX_VARS) {
            strncpy(variables[var_count].name, name, MAX_VARS_NAME_LEN - 1);
            variables[var_count].name[MAX_VARS_NAME_LEN - 1] = '\0';
            variables[var_count].value = value;
            var_count++;
        }
    }
}

// 获取变量值
int get_var(const char* name) {
    Variable* var = find_var(name);
    if (var) {
        return var->value;
    }
    printf("错误：变量 '%s' 未定义\n", name);
    return 0;
}

// 简单表达式求值（仅支持整数和变量 + 加减乘除，简化实现）
int eval_expr(const char* expr) {
    // 这里为了简单，只处理单个变量或数字（可扩展为完整表达式解析）
    // 假设表达式是 "x" 或 "123" 或 "x+1" （简化处理，仅演示用）
    char* end;
    int num = strtol(expr, &end, 10);
    if (end != expr) {
        return num; // 是数字
    }
    return get_var(expr); // 是变量
}

// 执行单行命令（简化版解析，支持您的各种命令）
void execute_line(const char* line) {
    char cmd[32];
    sscanf(line, "%s", cmd);

    if (strcmp(cmd, "exit") == 0) {
        exit(0);
    } else if (strcmp(cmd, "7891") == 0) {
        char content[256];
        sscanf(line + 5, "%s", content); // 简单处理，不完美
        // 尝试是否是 %%%变量
        if (strncmp(content, "%%%", 3) == 0) {
            char var_name[MAX_VARS_NAME_LEN];
            sscanf(content + 3, "%s", var_name);
            int val = get_var(var_name);
            printf("%d\n", val);
        } else {
            printf("%s\n", content);
        }
    } else if (strncmp(cmd, "少羽牛逼@", 6) == 0) {
        char rest[MAX_INPUT_LEN];
        strcpy(rest, line + 6);
        char var_name[MAX_VARS_NAME_LEN];
        int value;
        if (sscanf(rest, "%[^=]=%d", var_name, &value) == 2) {
            set_var(var_name, value);
        } else if (sscanf(rest, "%[^=]=%[^\n]", var_name, rest) == 2) {
            // 简单处理表达式（这里仅支持变量，可以后续扩展表达式解析器）
            int val = eval_expr(rest);
            set_var(var_name, val);
        }
    } else if (strcmp(cmd, "灭火器") == 0) {
        int seconds;
        sscanf(line + 4, "%d", &seconds);
        sleep(seconds);
    } else if (strncmp(line, "UZI ", 4) == 0) {
        // 简化实现：UZI x > 78 跳(...)???(...)
        char var_name[MAX_VARS_NAME_LEN];
        int threshold;
        char jump_cmd[MAX_INPUT_LEN];
        char else_cmd[MAX_INPUT_LEN];

        // 格式：UZI x > 78 跳(...)???(...)
        if (sscanf(line, "UZI %[^ ] > %d 跳(%[^)] )??? (%[^)] )",
                  var_name, &threshold, jump_cmd, else_cmd) == 4) {

            int val = get_var(var_name);
            if (val > threshold) {
                // 执行跳(...)中的命令
                execute_line(jump_cmd);
            } else {
                // 执行 ??? (...)中的命令
                execute_line(else_cmd);
            }
        } else {
            printf("语法错误：条件语句格式应为 → UZI 变量 > 数字 跳(命令)???(命令)\n");
        }
    } else if (strncmp(line, "循环当 ", 5) == 0) {
        // 简化实现：循环当 x > 0 ... 结束循环
        // 我们采用“模拟循环”的方式：逐行读取多行，直到遇到“结束循环”
        char var_name[MAX_VARS_NAME_LEN];
        int threshold;
        if (sscanf(line, "循环当 %[^ ] > %d", var_name, &threshold) == 2) {
            while (1) {
                // 检查条件
                int val = get_var(var_name);
                if (val <= threshold) break;

                // TODO：这里应该进入一个循环体输入模式，但目前为简化，暂时不支持交互式多行循环体
                // 为了演示，我们假设下一行就是循环体（实际应该更复杂，比如进入缓冲模式）
                // 这里只是一个示意，真实实现需要更复杂的解析，比如收集多行直到“结束循环”

                // 演示：我们只执行一次循环体内容（简化实现）
                // 真实项目需要：读取多行直到 “结束循环”
                printf("[循环体执行一次]（当前 %s=%d）\n", var_name, val);
                break; // 只执行一次，简化
            }
        } else {
            printf("语法错误：循环语句格式应为 → 循环当 变量 > 数值\n");
        }
    } else {
        printf("未知命令: %s\n", cmd);
    }
}

// 多行输入模式
void multi_line_mode() {
    printf("进入多行编辑模式，输入代码，以 9178 结束输入：\n");
    char buffer[MAX_INPUT_LEN];
    while (1) {
        fgets(buffer, sizeof(buffer), stdin);
        // 去掉换行符
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "9178") == 0) {
            printf("执行代码块...\n");
            break;
        } else {
            execute_line(buffer);
        }
    }
}

// 主函数
int main() {
    printf("=== SHAOYUNIUBI ===\n");
    printf("支持命令：\n");
    printf("  少羽牛逼@变量=值     --> 定义变量\n");
    printf("  7891 内容           --> 打印内容\n");
    printf("  7891%%%变量         --> 打印变量值\n");
    printf("  灭火器 秒数         --> 延时\n");
    printf("  UZI x > 数字 跳(...)???(...)  --> 条件判断（自定义语法）\n");
    printf("  循环当 x > 数字      --> 循环（简化版，后续完善）\n");
    printf("  骂少羽死浮木         --> 进入多行编辑模式\n");
    printf("  9178                 --> 执行多行代码（在骂少羽死浮木后使用）\n");
    printf("  exit                 --> 退出\n");

    char input[MAX_INPUT_LEN];

    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        // 去掉换行符
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "骂少羽死浮木") == 0) {
            multi_line_mode();
        } else {
            execute_line(input);
        }
    }

    return 0;
}
