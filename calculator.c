#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


typedef struct {
	int type;//1 is unary 2 is binary
	double number;
	char operator[11];
} ElemenType;


typedef struct SNode{
	ElemenType date;
	struct SNode * next;
} SNode ,*Stack;



Stack NUM, OPER;//数栈和运算符栈

//堆栈变量初始化以上

void Push(ElemenType in,Stack S)
{
	SNode *TmpCell;
	TmpCell = (SNode *)malloc(sizeof(SNode));
	strcpy(TmpCell->date.operator,in.operator);
	TmpCell->date.number = in.number;
	TmpCell->date.type = in.type;
	TmpCell->next = S->next;
	S->next = TmpCell;
}

void Pop(Stack S, ElemenType *out)
{
	SNode *FirstCell;
	FirstCell = S->next;
	if(!FirstCell){//堆栈为空
		printf("ERROR:101 \n");//错误代码101
		exit(0);//终止程序，错误原因可能是表达式违法
	}
	S->next = FirstCell->next;
	strcpy(out->operator,FirstCell->date.operator);
	out->number = FirstCell->date.number;
	out->type = FirstCell->date.type;
	free(FirstCell);
}

Stack CreateStack(Stack *S)
{
	*S = (Stack)malloc(sizeof(struct SNode));
	(*S)->next = NULL;
}	


void DestroyStack(Stack *S)
{
	SNode *p;
	while(p = *S){
		*S = p->next;
		free(p);
	}
}

//ElemenType GeTOP(Stack S)
//{
//	ElemenType TopElem;
//	SNode *FirstCell;
//	FirstCell = S->next;
//	TopElem = FirstCell->date;
//	return TopElem;
//}



//堆栈操作以上

struct{
	char str[100];
	int kind;//0 is number, 1 is operator
} OneChar;//判断每一个独立单位是数字或运算符

char *expression;
int i = 0;//字符串数组脚标，记录当前位置

//表达式变量初始化 以上

void getexpression()
{
	 char *p;
	 int limit;
	 int size = 100;
	 expression = (char*)malloc(size * sizeof(char));
	 p = expression;
	 while((*p = getchar()) != '\n'){
	 	if(*p != ' '){
	 		if((*p >= 'A')&&(*p <= 'Z')){
	 			*p += 'a'-'A';//将大写转化为小写
	 		}
	 	p++;
	 	limit++;
	 	}
	 }
	 *p++ = '=';//用=标记表达式开头和结尾，因文件操作输出优雅 改标识符为=
	 *p = '\0';
}

int judge(char ch){
	if((ch >= '0')&&(ch <= '9')||(ch == '.'))
		return 1;
	else if((ch == '+')&&(ch == '-')){
		if((i == 0)||(expression[i-1] == '('))//若+/-在字符串头或'('后则为正负号，作为一个有效字符读入
			return 1;
	}
	return 0;
}

void getOneChar()//用'\0'分割字符串
{
	char *p = OneChar.str;
	*p = expression[i];
	if(judge(*p)){
		while(judge(*++p = expression[++i]));//读完一整个有效字符
		*p = '\0';//插入'\0'使其成为一个独立字符串
		OneChar.kind = 0;	
		return;
	}
	else if((*p >= 'a')&&(*p <= 'z')){
		while((expression[i+1] >= 'a')&&(expression[i+1] <= 'z'))
			*++p = expression[++i];
	}
	
	i++;
	*++p = '\0';
//	if(!strcmp(OneChar.str,"pi")){
//		strcpy(OneChar.str,"3.1415926535897932");
////		sprintf(OneChar.str,"%.16g",acos(-1.0));
//		OneChar.kind = 0;
//		return;
//	}
//	else if(!strcmp(OneChar.str,"e")){
//		sprintf(OneChar.str,"%.16g",exp(1));
//		OneChar.kind = 0;
//		return;
//	}
	OneChar.kind = 1;
	return;
}//从表达式中截取一个有效的字符串

char Prioritize(ElemenType *operator1, ElemenType *operator2)
{
	char *oper1,*oper2;
	oper1 = operator1->operator;
	oper2 = operator2->operator;
	if(!strcmp(oper1,"ln")||!strcmp(oper1,"lg")||
		!strcmp(oper1,"sin")||!strcmp(oper1,"cos")||!strcmp(oper1,"tan")){
		operator1->type = 1;
		return (!strcmp(oper2,"!")||!strcmp(oper2,"(")||!strcmp(oper2,"^"))? '<' : '>' ;
	}
	else if(!strcmp(oper1,"!")){
		operator1->type = 1;
		return '>';//
	}
	else if(!strcmp(oper1,"+")||!strcmp(oper1,"-")||!strcmp(oper1,"*")||!strcmp(oper1,"/")||
			!strcmp(oper1,"(")||!strcmp(oper1,")")||!strcmp(oper1,"^")||!strcmp(oper1,"=")){
		operator1->type = 2;
		switch(oper1[0]){
			case '+': case '-': return (!strcmp(oper2,"+")||!strcmp(oper2,"-")||
										!strcmp(oper2,")")||!strcmp(oper2,"=")) ? '>' : '<';
			case '*': case '/': return (strcmp(oper2,"(")||strcmp(oper2,"^")) ? '>' : '<';
			case '(': return !strcmp(oper2,")") ? '=' : '<';
			case ')': return '>';
			case '^': return (strcmp(oper2,"(")||strcmp(oper2,"!")||strcmp(oper2,"^")) ? '>' : '<';
			case '=': return !strcmp(oper2,"=") ? '=' : '<';
		}
	}else {printf("ERROR:102");exit(0);}//出现不可识别的运算符
}
//优先级返回结束

int factorial(n)
{
	if(n <= 1) return 1;
	else return n * factorial(n-1);
}//阶乘运算 未测试

ElemenType Calculate(ElemenType num1, ElemenType oper, ElemenType num2)
{
	ElemenType result;
	if(oper.type == 1){
		if(!strcmp(oper.operator,"!")) result.number = factorial(num1.number);
		else if(!strcmp(oper.operator,"lg")) result.number = log10(num1.number);
		else if(!strcmp(oper.operator,"ln")) result.number = log(num1.number);
		else if(!strcmp(oper.operator,"sin")) result.number = sin(num1.number);
		else if(!strcmp(oper.operator,"cos")) result.number = cos(num1.number);
		else if(!strcmp(oper.operator,"tan")) result.number = tan(num1.number);
	}else{
		switch(*oper.operator) {
			case '+': result.number = num1.number + num2.number; break;
			case '-': result.number = num1.number - num2.number; break;
			case '*': result.number = num1.number * num2.number; break;
			case '/': result.number = num1.number / num2.number; break;
			case '^': result.number = pow(num1.number,num2.number);break;
		}
	}
	return result;
}

int main()
{
	ElemenType num1, num2, oper;

	int mode;
	char s[1024];

	printf("简易计算器v0.1.0 \n\n");
	printf("支持的运算符:\n| + | - | * | / | \n|sin|cos|tan|   | \n| lg| ln| ^ | ! |\n\n");
	printf("错误代码:\n101: 表达式不合法，请检查表达式输入是否正确 \n102: 有违法运算符\n\n");
	printf("输入2进入文件读取模式\n");
	scanf("%d",&mode);
	fflush(stdin);
	if(mode == 2) freopen("input","r",stdin);
//		printf("输入表达式，回车求值 \n");
//		printf("输入quit结束程序\n\n\n");
		
	while(1){
		i = 0;
		getexpression();
		if(!strcmp(expression,"quit=")) return 0;

		CreateStack(&NUM);
		CreateStack(&OPER);
		strcpy(oper.operator, "=");
		Push(oper, OPER);
		getOneChar();
		while(strcmp(OneChar.str,"=")||strcmp(OPER->next->date.operator,"=")){
			if(OneChar.kind){
				strcpy(oper.operator,OneChar.str);
				switch(Prioritize(&(OPER->next->date), &oper)){
					case '<':
						strcpy(oper.operator,OneChar.str);
						Push(oper,OPER);
						getOneChar();
						break;
					case '=':
						Pop(OPER,&oper);
						getOneChar();
						break;
					case '>':
						Pop(OPER,&oper);
						Pop(NUM,&num2);
						if(oper.type == 2) Pop(NUM,&num1);
						Push(Calculate(num1,oper,num2), NUM);
				}
			}
			else{
				num1.number = atof(OneChar.str);//将字符串转化为浮点数计算
				Push(num1,NUM);
				getOneChar();
			}
		}
		if(mode == 2) printf("%s",expression);
		printf("%.16g\n\n",NUM->next->date.number);
		free(expression);
		DestroyStack(&NUM);
		DestroyStack(&OPER);
	}
	
	if(mode == 2) fclose(stdin);
	
	free(expression);
	return 0;
}
