#include<cstdio>
#include <string.h>
using namespace std;

char key[12][6] =
{
    " ", "begin", "end", "if", "then", "else", "for", "while", "do", "and", "or", "not"
};
char opOrde[18][3] =
{
    " ","+","-","*","/",">","<","=",":=",">=","<=","<>","++","--","(",")",";","#"
};
//读入数据，进行预处理
int readFile()
{
    FILE *fread = fopen("helloworld.txt", "rt");
    FILE *outfile = fopen("pretreat.txt","w");
    if(fread==NULL||outfile==NULL)
        return -1;
    char linetxt[128];
    int i;
    fgets(linetxt,128,fread);
    while(linetxt[0]!='$')
    {
        if(linetxt[0]!='#')
        {
            i=0;
            while(linetxt[i]!='\n')
            {
                if(linetxt[i]=='#')
                    break;
                if(linetxt[i]==' '&&linetxt[i+1]!=' ')
                {
                    fprintf(outfile," ");
                }
                if(linetxt[i]!=' ')
                {
                    fprintf(outfile,"%c",linetxt[i]);
                }
                i++;
            }
            fprintf(outfile," ");
        }
        fgets(linetxt,128,fread);
    }
    fprintf(outfile,"$");
    fclose(fread);
    fclose(outfile);
    return 0;
}
//判断是否是关键词
int iskey(char *c)
{
    int k[12]= {0},i=0,j,n=0,t=0;
    for(j=0; j<12; j++)
    {
        if(c[i]==key[j][0])
        {
            k[n++]=j;
        }
    }
    if(n==0)
        return -1;
    i=1;
    while(c[i]!='\0')
    {
        t=0;
        for(j=0; j<n; j++)
        {
            if(c[i]==key[k[j]][i])
            {
                k[t++]=k[j];
            }
        }
        if(t==0)
            return -1;
        n=t;
        i++;
    }
    if(strlen(c)==strlen(key[k[0]]))
        return k[0];
    else
        return -1;
}
//判断是否是操作符或分界符
int isop(char *c)
{
    int p[18]= {0},j,n=0,t=0;
    //第一轮寻找
    for(j=0; j<18; j++)
    {
        if(c[0]==opOrde[j][0])
        {
            p[n++]=j;
        }
    }
//    printf("%s %d\n",c,n);
    //未找到
    if(n==0)
        return -1;
    //单个运算符
    if((c[1]>='a'&&c[1]<='z')||(c[1]>='A'&&c[1]<'Z'))
    {
        return p[0];
    }
    //双个运算符
    for(j=0; j<n; j++)
    {
        if(c[1]==opOrde[p[j]][1])
        {
//            printf("%s %d\n",c,p[j]);
            return p[j];
        }
    }
    //未找到匹配的双个运算符，但是存在匹配的单个运算符
    for(j=0; j<n; j++)
    {
        if(strlen(opOrde[p[j]])==1)
        {
//            printf("%s %d\n",c,p[j]);
            return p[j];
        }
    }
    return -1;
}
//判断是字母、数字、下划线、还是特殊字符
int isC(char c)
{
    if((c>='a'&&c<='z')||(c>='A'&&c<='Z'))
        return 0;
    else if (c>='0'&&c<='9')
        return 1;
    else if(c=='_')
        return 2;
    else
        return -1;
}
//主程序
int main()
{
    printf("%d\n",readFile());
    FILE *fread = fopen("pretreat.txt", "rt");
    FILE *outfile = fopen("C:\\Users\\shengqiongyi\\Desktop\\test\\final.txt","w");
    char txt[10000],op[3],c[100],num[32],id[100];
    fgets(txt,10000,fread);
    int i=0;
    int j;
    int t,f;
    int u=0,OP; //记录数字或id长度
    while(txt[i]!='$')  //读取数据
    {
        j=0;
        if(txt[i]==' ')
            i++;
        while(txt[i]!=' ')
        {
            c[j++] = txt[i];
            i++;
        }
        c[j]='\0';  //按照空格获取字符串
        printf("%s\n",c);
        t=0;//游标
        f=isC(c[0]);
        //小写字母或者下划线开头
        if(f==0||f==2)
        {
            u=0;
            int k=iskey(c);
            //关键词
            if(k>0)
            {
                fprintf(outfile,"<%s,-->\n",key[k]);
            }
            else
            {
                while(t<j)
                {
                    //出现运算符或者分界符
                    if(isC(c[t])==-1)
                    {
                        if(u!=0)
                        {
                            id[u]='\0';
                            fprintf(outfile,"<30,%s>\n",id);
                            u=0;
                        }
                        op[0]=c[t];
                        if(t+1<j&&isC(c[t+1])==-1)
                        {
                            op[1]=c[t+1];
                            t=t+2;
                        }
                        else
                        {
                            op[1]='a'; //占位符
                            t++;
                        }
                        op[2]='\0';
                        OP=isop(op);
                        if(op[1]!='a'&&(OP<8||OP>13))
                        {
                            t=t-1;
                        }
                        if(OP>0)
                        {
                            fprintf(outfile,"<%s,-->\n",opOrde[OP]);
                            //除")"和";"之外的操作符后跟数字
                            if(OP<15&&isC(c[t])==1)
                            {
                                while(isC(c[t])==1)
                                {
                                    num[u++]=c[t];
                                    t++;
                                }
                                //数字之后出现字母或者下划线，报错
                                if(isC(c[t])==0||isC(c[t])==2)
                                {
                                    fprintf(outfile,"err:id can not start with number\n");
                                }
                                else
                                {
                                    num[u]='\0';
                                    fprintf(outfile,"<29,%s>\n",num);
                                    u=0;
                                }
                            }
                        }
                        else
                        {
                            fprintf(outfile,"err: can not find this op\n");
                        }

                    }
                    else
                    {
                        //记录id
                        id[u++]=c[t];
                        t++;
                    }
                }
                if(u!=0)
                {
                    id[u]='\0';
                    fprintf(outfile,"<30,%s>\n",id);
                    u=0;
                }
            }

        }
        //数字开头
        else if(f==1)
        {
            u=0;
            while(t<j)
            {
                //数字之后出现字母
                if(isC(c[t])==0||isC(c[t])==2)
                {
                    fprintf(outfile,"err: id can not follow with number\n");
                    break;
                }
                else if(isC(c[t])==-1)
                {
                    //不是字母、数字、下划线，即出现运算符或者分界符
                    num[u]='\0';
                    fprintf(outfile,"<29,%s>\n",num);
                    u=0;
                    op[0]=c[t];
                    if(t+1<j&&isC(c[t+1])==-1)
                    {
                        op[1]=c[t+1];
                        t=t+2;
                    }
                    else
                    {
                        op[1]='a'; //占位符
                        t++;
                    }
                    op[2]='\0';
                    OP=isop(op);
                    if(op[1]!='a'&&(OP<8||OP>13))
                    {
                        t--;
                    }
                    if(OP>0)
                    {
                        fprintf(outfile,"<%s,-->\n",opOrde[OP]);
                    }
                    else
                    {
                        fprintf(outfile,"err: can not find this op\n");
                    }
                }
                else
                {
                    num[u++]=c[t];
                    t++;
                }
            }
            if(u!=0)
            {
                num[u]='\0';
                fprintf(outfile,"<29,%s>\n",num);
                u=0;
            }
        }
        //运算符开头
        else
        {
            u=0;
//            printf("%s\n",c);
            while(t<j)
            {
                //运算符之后是字母数字下划线
                if(isC(c[t])==0||isC(c[t])==2||isC(c[t])==1)
                {
                    if(u==1)
                        op[1]='a';
                    OP=isop(op);
                    if(op[1]!='a'&&(OP<8||OP>13))
                    {
                        t--;
                    }
                    u=0;
                    if(OP>0)
                    {
                        fprintf(outfile,"<%s,-->\n",opOrde[OP]);
                    }
                    else
                    {
                        fprintf(outfile,"err: can not find this op\n");
                    }
                    //出现字母或者下划线
                    if(isC(c[t])==0||isC(c[t])==2)
                    {
                        while(isC(c[t])!=-1)
                        {
                            id[u++]=c[t];
                            t++;
                        }
                        id[u]='\0';
                        fprintf(outfile,"<30,%s>\n",id);
                        u=0;
                    }
                    else
                    {
                        //出现数字
                        while(isC(c[t])==1)
                        {
                            num[u++]=c[t];
                            t++;
                        }
                        //数字之后出现字母或者下划线，报错
                        if(isC(c[t])==0||isC(c[t])==2)
                        {
                            fprintf(outfile,"err:id can not start with number\n");
                        }
                        else
                        {
                            num[u]='\0';
                            fprintf(outfile,"<29,%s>\n",num);
                            u=0;
                        }
                    }
                }
                else
                {
                    op[u++]=c[t];
                    if(u>2)
                    {
                        fprintf(outfile,"err:err op\n");
                        u=0;
                    }
                    t++;
                }
            }
            if(u!=0)
            {
                if(u==1)
                    op[1]='a';
                op[2]='\0';
                OP=isop(op);
                if(op[1]!='a'&&(OP<8||OP>13))
                {
                    t--;
                }
//                printf("op%d\n",OP);
                if(OP>0)
                {
                    fprintf(outfile,"<%s,-->\n",opOrde[OP]);
                }
                else
                {
                    fprintf(outfile,"err: can not find this op\n");
                }
            }
        }
        i++;
    }
    //写入结束符、关闭文件。
    fprintf(outfile,"$");
    fclose(fread);
    fclose(outfile);
    return 0;
}
