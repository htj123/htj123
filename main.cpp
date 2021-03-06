// sudoku.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>
#include<string.h>
int shuDu[10][10];
int numbers;
int num;//用于记录当前产生的数独终局数 
int op = 0;
FILE *fp;
//转换函数 
void change(char str[])
{
	for (int i = 0; i < strlen(str); i++)
	{
		numbers += numbers * 10 + (str[i] - '0');
	}
	return;
}
//九宫格判断 
int decide(int i, int j, int k)
{
	int flag = 1;
	int up = (i / 3) * 3 + 3;//特殊情况超出小宫格取模
	int up1 = (j / 3) * 3 + 3;
	if (i % 3 == 0) up = i;
	if (j % 3 == 0) up1 = j;
	for (int t = up - 2; t <= up; t++)  //检查小宫格中是否有相同的数字
	{
		for (int m = up1 - 2; m <= up1; m++)
		{
			if (shuDu[t][m] == k)
			{
				flag = 0;
				break;
			}
		}
	}
	return flag;
}
//行的判断 
int coldecide(int i, int j, int k)
{
	int flag = 1;
	for (int n = 1; n < j; n++) // 检查同一行是否出现过数字k
	{
		if (shuDu[i][n] == k)  //该行出现过数字k 
		{
			flag = 0;
			break;
		}
	}
	return flag;
}
//列的检查
int rowdecide(int i, int j, int k)
{
	int flag = 1;
	for (int m = 1; m < i; m++) // 检查同一列是否出现过数字k
	{
		if (shuDu[m][j] == k)  //判断是否在该列出现过数字k 
		{
			flag = 0;
			break;
		}
	}
	return flag;
}
//数独输出函数 
void Print_shuDu(int cout)
{
	op++;
	int i, j;
	for (i = 1; i <= 9; i++)
	{
		for (j = 1; j <= 9; j++)
		{
			fputc(shuDu[i][j] + '0', fp);
			if (j == 9 && i != 9) fprintf(fp, "\n");
			else if (i == 9 && j == 9)
			{
				if (cout < numbers)
				{
					fprintf(fp, "\n");
					fprintf(fp, "\n");
				}
			}
			else
				fputc(' ', fp);
		}
	}
	return;
}
//搜索第(i , j)位置处是否可以放置数字
int judge(int i, int j)
{
	if (i > 9 || j > 9) return 1; //若为最后一位就放置
	for (int k = 1; k <= 9; k++)
	{
		bool flag = 1;                // flag 变量用于判断数字k能否放在 ( i , j ) 处
		flag = rowdecide(i, j, k);
		if (flag == 1)  flag = coldecide(i, j, k);//行的检查 
		if (flag == 1)  flag = decide(i, j, k);         // 检查小宫格中是否有相同的数字
		if (flag == 1) //flag==1说明数字k可以放在该位置上 
		{
			shuDu[i][j] = k;
			if (j<9)
			{
				if (judge(i, j + 1) == 1) return 1;
			}
			else
			{
				if (i < 9)
				{
					if (judge(i + 1, 1) == 1)
						return 1;
				}
				else
				{
					num++;
					if (num < numbers) Print_shuDu(num);
					else
					{
						Print_shuDu(num);
						return 1;
					}
				}
			}
			shuDu[i][j] = 0;  //若是没有找到解则置为0
		}
		else continue;
	}
	return 0;
}
//数独生成
void Create_ShuDu(char s[])
{
	change(s);
	shuDu[1][1] = 4;//学号后面的两位相加模9加一
	judge(1, 2);//进行判断
}
//求解数独
int solve(int i, int j)
{
	if (i > 9 || j > 9) return 1;//搜索结束 
	if (shuDu[i][j] != 0)
	{
		if (j<9)
		{
			if (solve(i, j + 1) == 1) return 1;  //从同一行的下一位置开始判断 
		}
		else
		{
			if (i < 9)
			{
				if (solve(i + 1, 1) == 1) return 1;   // 从下一行的第一个空格开始判断 
			}
		}
	}
	for (int k = 1; k <= 9; k++) //依次寻找适合的数据 
	{
		int flag = 1;                // flag变量用于记录数字k能否放在 ( i , j ) 处
		flag = rowdecide(i, j, k);
		if (flag == 1)	flag = coldecide(i, j, k);
		if (flag == 1)   flag = decide(i, j, k);         //检查小宫格中是否有相同的数字 
		if (flag == 1) //flag==1说明数字k可以放在该位置上 
		{
			shuDu[i][j] = k;
			if (j<9)
			{
				if (solve(i, j + 1) == 1) return 1;
			}
			else
			{
				if (i < 9)
				{
					if (solve(i + 1, 1) == 1) return 1;
				}
				else return 1;
			}
		}
		else continue;
	}
	shuDu[i][j] = 0;  /* 关键这一步：找不到解就要回复原状，否则会对下面的搜索造成影响 */
	return 0;  /*  1到9都尝试过都不行，则返回递归的上一步 */
}
void Solve_shuDu(char F[])
{
	FILE *a;
	int i, j;
	numbers = 1;
	 fopen_s(&a,F, "r");
	if (a == NULL)
	{
		printf("无法打开此文件\n");
		return;
	}
	while (1)
	{
		for (i = 1; i <= 9; i++)
		{
			for (j = 1; j <= 9; j++)
			{
				fscanf_s(a, "%d", &shuDu[i][j]);
			}
		}
		solve(1, 1);
		fgetc(a);
		if (feof(a) == 1)
		{
			Print_shuDu(1);
			break;
		}
		else Print_shuDu(0);
	}
	fclose(a);
	return;
}
int main(int argc,char *argv[])
{
	char str[20] = "suduko.exe";
	fopen_s(&fp,"sudoku.txt", "w");
	if (fp == NULL)
	{
		printf("Can not open the file!\n");
		return 0;
	}
	if (strcmp("-c", argv[1]) == 0)//生成数独 
	{
		Create_ShuDu(argv[2]);
	}
	else if (strcmp("-s", argv[1]) == 0)//解数独
	{
		Solve_shuDu(argv[2]);
	}
	else if (strcmp("-s", argv[1]) != 0 || strcmp("-c", argv[1]) != 0)
	{
		if (strlen(argv[0]) != 10)
		{
			printf("输入的文件名有误！\n");
			return 0;
		}
		for (int i = 0; i < strlen(argv[0]); i++)
		{
			if (argv[0][i] != str[i])
			{
				printf("输入的文件名有误！\n");
				return 0;
			}
		}
		char s1[3] = "-c";
		char s2[3] = "-s";
		for (int i = 0; i < strlen(argv[1]); i++)
		{
			if (argv[1][i] != s1[i] || argv[1][i] != s2[i])
			{
				printf("Input error!\n");
			}
		}
	}
	fclose(fp);
	return 0;
}
