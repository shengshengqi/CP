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
//�������ݣ�����Ԥ����
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
//�ж��Ƿ��ǹؼ���
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
//�ж��Ƿ��ǲ�������ֽ��
int isop(char *c)
{
    int p[18]= {0},j,n=0,t=0;
    //��һ��Ѱ��
    for(j=0; j<18; j++)
    {
        if(c[0]==opOrde[j][0])
        {
            p[n++]=j;
        }
    }
//    printf("%s %d\n",c,n);
    //δ�ҵ�
    if(n==0)
        return -1;
    //���������
    if((c[1]>='a'&&c[1]<='z')||(c[1]>='A'&&c[1]<'Z'))
    {
        return p[0];
    }
    //˫�������
    for(j=0; j<n; j++)
    {
        if(c[1]==opOrde[p[j]][1])
        {
//            printf("%s %d\n",c,p[j]);
            return p[j];
        }
    }
    //δ�ҵ�ƥ���˫������������Ǵ���ƥ��ĵ��������
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
//�ж�����ĸ�����֡��»��ߡ����������ַ�
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
//������
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
    int u=0,OP; //��¼���ֻ�id����
    while(txt[i]!='$')  //��ȡ����
    {
        j=0;
        if(txt[i]==' ')
            i++;
        while(txt[i]!=' ')
        {
            c[j++] = txt[i];
            i++;
        }
        c[j]='\0';  //���տո��ȡ�ַ���
        printf("%s\n",c);
        t=0;//�α�
        f=isC(c[0]);
        //Сд��ĸ�����»��߿�ͷ
        if(f==0||f==2)
        {
            u=0;
            int k=iskey(c);
            //�ؼ���
            if(k>0)
            {
                fprintf(outfile,"<%s,-->\n",key[k]);
            }
            else
            {
                while(t<j)
                {
                    //������������߷ֽ��
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
                            op[1]='a'; //ռλ��
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
                            //��")"��";"֮��Ĳ������������
                            if(OP<15&&isC(c[t])==1)
                            {
                                while(isC(c[t])==1)
                                {
                                    num[u++]=c[t];
                                    t++;
                                }
                                //����֮�������ĸ�����»��ߣ�����
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
                        //��¼id
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
        //���ֿ�ͷ
        else if(f==1)
        {
            u=0;
            while(t<j)
            {
                //����֮�������ĸ
                if(isC(c[t])==0||isC(c[t])==2)
                {
                    fprintf(outfile,"err: id can not follow with number\n");
                    break;
                }
                else if(isC(c[t])==-1)
                {
                    //������ĸ�����֡��»��ߣ���������������߷ֽ��
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
                        op[1]='a'; //ռλ��
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
        //�������ͷ
        else
        {
            u=0;
//            printf("%s\n",c);
            while(t<j)
            {
                //�����֮������ĸ�����»���
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
                    //������ĸ�����»���
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
                        //��������
                        while(isC(c[t])==1)
                        {
                            num[u++]=c[t];
                            t++;
                        }
                        //����֮�������ĸ�����»��ߣ�����
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
    //д����������ر��ļ���
    fprintf(outfile,"$");
    fclose(fread);
    fclose(outfile);
    return 0;
}
