#include "machineplayer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int MachinePlayer::judge(int chessboard1[15][15],int x,int y)//找到要检测的位置
{
    int i,j;
    int s=0;
    for(i=x-1;i<x+2;i++)
        for(j=y-1;j<y+2;j++)
            if((i>0&&j>0&&i<15&&j<15)&&(chessboard1[i][j]!=0))
                s=1;
    for(i=x-2;i<x+3;i+=2)
        for(j=y-2;j<y+3;j+=2)
            if((i>0&&j>0&&i<15&&j<15)&&(chessboard1[i][j]!=0))
                s=1;
    return s;
}

int MachinePlayer::score_table(int number, int empty)
{
    if (number >= 5) return 10000;
    else return (int)pow(10, number+empty-3);
}

int MachinePlayer::count_score(int n[], int chessColor)
{
    int i = 1;
    int empty = 0;
    int number = 0;
    int scoretmp = 0;
    if (n[0] == 0)
    {
        empty++;
    }
    else if (n[0] == chessColor)
    {
        number++;
    }
    while (i < 15)
    {
        if (n[i] == chessColor)
        {
            number++;
        }
        else if (n[i] == 0)
        {
            if (number == 0)
            {
                empty = 1;
            }
            else
            {
                scoretmp += score_table(number, empty+1);
                empty = 1;
                number = 0;
            }
        }
        else
        {
            scoretmp += score_table(number, empty);
            empty = 0;
            number = 0;
        }
        i++;
    }
    scoretmp += score_table(number, empty);
    return scoretmp;
}

int MachinePlayer::score(int a,int chessboard1[15][15]){//局面评分
    int AIScore=0;
    int humScore=0;
    int i, j;
    int x, y;
    int n[15];
    memset(n, 0, sizeof(n));
    for (i=0; i<15; i++)
    {
        for(j=0; j<15; j++)
        {
            n[j] = chessboard1[i][j];
        }
        AIScore += count_score(n, a);
        humScore += count_score(n, 3-a);
        memset(n, 0, sizeof(n));
    }
    for (j=0; j<15; j++)
    {
        for(i=0; i<15; i++)
        {
            n[i] = chessboard1[i][j];
        }
        AIScore += count_score(n, a);
        humScore += count_score(n, 3-a);
        memset(n, 0, sizeof(n));
    }
    for (i=0; i<15; i++)
    {
        for(x=i,y=0; x<15&&y<15; x++,y++)
        {
            n[y] = chessboard1[x][y];
        }
        AIScore += count_score(n, a);
        humScore += count_score(n, 3-a);
        memset(n, 0, sizeof(n));
    }
    for (j=1; j<15; j++)
    {
        for(x=0,y=j; y<15&&x<15; x++,y++)
        {
            n[x] = chessboard1[x][y];
        }
        AIScore += count_score(n, a);
        humScore += count_score(n, 3-a);
        memset(n, 0, sizeof(n));
    }
    for (i=0; i<15; i++)
    {
        for(y=i,x=0; y>=0&&x<15; y--,x++)
        {
            n[x] = chessboard1[x][y];
        }
        AIScore += count_score(n, a);
        humScore += count_score(n, 3-a);
        memset(n, 0, sizeof(n));
    }
    for (j=1; j<15; j++)
    {
        for (y=j,x=14; y<15&&x>=0; y++,x--)
        {
            n[y-j] = chessboard1[x][y];
        }
        AIScore += count_score(n,a);
        humScore += count_score(n, 3-a);
        memset(n, 0, sizeof(n));
    }
    return AIScore-humScore;
}

int MachinePlayer::minMax(int chessboard1[15][15],int deep,int a,int Alpha,int Beta)//多层搜索，剪枝算法
{
    int i,j,n=1;
    int minmax;
    int data;
    int Alpha1=Alpha, Beta1=Beta, u1=0, v1=0;
    if(deep>0)
    {
        for(i=0;i<15;i++)
            for(j=0;j<15;j++)
            {
                if(chessboard1[i][j]==0&&judge(chessboard1,i,j)&&Alpha1<Beta1)
                {
                    chessboard1[i][j]=a;
                    minmax=minMax(chessboard1,deep-1,3-a,Alpha1,Beta1);
                    chessboard1[i][j]=0;
                    if(deep%2==0)
                    {
                        if(n==1)
                        {
                            data=minmax;
                            n++;
                        }
                        if(Alpha1<minmax)
                        {
                            Alpha1=minmax;
                            data=minmax;
                            u1=i;
                            v1=j;
                        }
                    }
                    else
                    {
                        if(n==1)
                        {
                            data=minmax;
                            n++;
                        }
                        if(Beta1>minmax)
                        {
                            Beta1=minmax;
                            data=minmax;
                            u1=i;
                            v1=j;
                        }
                    }
                }
            }
        u=u1;
        v=v1;
        return data;
    }
    else
    {
        return score(a,chessboard1);
    }
}
/*
 * info: 字符串，由 * O（大写字母O) X组成，共225个字符，表示15*15棋盘上的225个位置（横数），*为空位，O为白，X为黑，形如"*********XOOOOX****..."
 * 返回方式: 使用emit resultReady(result); result的类型是QVector<int>，共三个int，分别为颜色（与传入的color一致）、行和列（行列均为1-15）
 */

void MachinePlayer::goNext(const QString& info, int color){
    // 函数功能：传入当前棋盘状态和即将落子的颜色，算出下一步应落的位置，打包成结构体step返回
    int i,j,k=0;
    for(i = 0; i <= 14; i++)
    for(j = 0; j <= 14; j++){
        if(info[i*15+j] == 'X') {chessboard[i][j] = 1; k=1;}
        if(info[i*15+j] == 'O') chessboard[i][j] = 2;
    }
    if(k==0){
        QVector<int> result;
        result << color << 8 << 8;
        emit resultReady(result);
        return;
        //黑棋先下第一步
        }
    if(color == 1) minMax(chessboard,2,1,-2147483648,2147483647);
    else minMax(chessboard,2,2,-2147483648,2147483647);
    u += 1; v += 1;//u,v代表行数和列数
    QVector<int> result;
    result << color << u << v;
    emit resultReady(result);
}
