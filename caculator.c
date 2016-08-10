#include<stdio.h>
#include<math.h>
#include<stdlib.h>

/*
 calc_func: 操作函数结构体
    - compute：处理计算任务的函数指针
    - key：操作字符
    - name：操作的名字描述
*/
struct calc_func {
    int (*compute)();
    char key;
    char *name;
};

/*
 加法处理函数
*/
int add() {
    
    // 初始化返回值
    int res=0;
    
    // 存储读取数字
    char cmd[20];
    
    // 提示输入要计算的数字
    printf("Enter numbers to add\n");
    
    // 读取所有输入的数字并相加，直到输入空行时返回结果
    while( fgets(cmd, 20, stdin) && cmd[0] != '\n' ) {
        res += atoi(cmd);
    }
    
    // 打印计算结果
    printf("Result is %d\n", res);
}

/*
 减法处理函数
*/
int subtract() {
    
    // 初始化返回值
    int num, res=0;
    
    // 存储读取数字
    char cmd[20];
    
    // 提示输入要计算的数字
    printf("Enter numbers to subtract\n");
    
    // 读取所有输入的数字并相减，直到输入空行时返回结果
    while( fgets(cmd, 20, stdin) && cmd[0] != '\n' ) {
        num = atoi(cmd);
        if ( res == 0 )
            res = num;
        else
            res -= atoi(cmd);
    }

    // 打印计算结果
    printf("Result is %d\n", res);
}

/*
 程序退出函数
*/
int quit() {
    printf("Good Bye\n");
    exit(0);
}

/*
 初始化结构体数组，注册所有的处理函数
 该数组为全局变量，所有函数里可以直接访问
*/
struct calc_func functions[] = {
    {
        .key = 'q',
        .compute = quit,
        .name = "quit",
    },
    {
        .key = '+',
        .compute = add,
        .name = "addition",
    },
    {
        .key = '-',
        .compute = subtract,
        .name = "subtraction",
    }

};

// 结构体数组中的元素数量
#define NB_FUNCS (sizeof(functions)/sizeof(struct calc_func))

// 打印所有已经注册的函数
void print_all_functions() {

    int i;

    // 循环打印结构体数组中的所有元素名称及操作字符
    for (i = 0 ; i < NB_FUNCS ; i++) {
        printf("Press key %c for %s\n", functions[i].key, functions[i].name);
    }
}

// 主函数
int main() {
    int i;
    char cmd[20];
    
    // 打印提示信息及支持的所有命令
    printf("Welcome to Calculator \n");
    printf("========================= \n\n");
    print_all_functions();
    printf("========================= \n\n");

    // 进入命令处理主循环
    while(1) {
        // 输入操作命令
        printf("Enter a command\n");
        fgets(cmd, 20, stdin);

        // 根据操作命令循环查找对应的结构体元素
        for (i = 0 ; i < NB_FUNCS ; i++) {
            
            if (functions[i].key == cmd[0]) {
                
                // 找到正确的处理函数
                printf("Calling command: %s\n", functions[i].name);
                
                // 调用处理函数
                functions[i].compute();
                
                // 跳出for循环，继续while循环
                break;
            }
        }
    }
}
