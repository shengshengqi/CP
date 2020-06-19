#include<cstdio>
char s[200][20];
int k=0,n=0;
void E();
//从文件读入二元组
int readFile()
{
    FILE *fread = fopen("C:\\Users\\shengqiongyi\\Desktop\\test\\final.txt", "rt");
    if(fread==NULL)
        return -1;
    char linetxt[128];
    fgets(linetxt,128,fread);
    int i,j;
    while(linetxt[0]!='$')
    {
        i=0,j=0;
        for(i=0; linetxt[i]!='\0';)
        {
            s[n][j++]=linetxt[i++];
//            printf("%s\n",s[n]);
        }
        s[n][j]='\0';
        n++;
        printf("%s\n",linetxt);
        fgets(linetxt,128,fread);
    }
    fclose(fread);
    //返回二元组数量
    return 0;
}

//A->+||-
void A()
{
    if(s[k][1]=='+')
        printf("A::+\n");
    else if(s[k][1]=='-')
        printf("A::-\n");
    else
        printf("A error\n");
    k++;
}
//M->*||/
void M()
{
    if(s[k][1]=='*')
        printf("M::*\n");
    else if(s[k][1]=='/')
        printf("M::/\n");
    else
        printf("M error\n");
    k++;
}
//F->(E)||i
void F()
{
    if(s[k][1]=='(')
    {
        k++;
        E();
        if(s[k][1]!=')')
            printf("F error need )\n");
        else{
            printf("F::(E)\n");
            k++;
        }
    }
    else if(s[k][1]=='3')
    {
        printf("F::i\n");
        k++;
    }
    else
    {
        printf("F error\n");
    }
}
//Tp->MFTp
void Tp()
{
    if(s[k][1]=='*'||s[k][1]=='/'){
        M();
        F();
        Tp();
        printf("Tp::MFTp\n");
    }
    printf("Tp:: \n");
}
//T->FTp
void T()
{
    F();
    Tp();
    printf("T::FTp\n");
}
//Ep->ATEp
void Ep()
{
    if(s[k][1]=='+'||s[k][1]=='-'){
        A();
        T();
        Ep();
        printf("Ep::ATEp\n");
    }
    printf("Ep:: \n");
}
//E->TE'
void E()
{
    T();
    Ep();
    printf("E::TEp\n");
}
//主函数
int main()
{
    int f=-2;
    f = readFile();
    if(f!=0) printf("err");
    E();
    return 0;
}
