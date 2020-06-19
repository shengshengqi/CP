#include <iostream>
#include <string>
#include <string.h>
#include <cstdio>
#include <map>
using namespace std;

const int maxcp = 100;  //文法最大条数
const int maxf = 1000;  //文法中包含的最大字符数量
int length=0;       //文法长度
int lengths=0;      //select集长度

//文法、First集、Follow集、select集存储结构
struct cp
{
    string key;
    int len;
    string value[10];
    cp()
    {
        key = '0';
        value[0] = 'null';
        len=0;
    }
} lang[maxcp],first[maxf],follow[maxf],select[maxf];
//预测分析表存储结构
struct chart
{
    string key;
    int len;
    string in[10];
    string out[10];
} forecast[maxf];
//模拟堆栈
struct stacks
{
    int len;
    string value[100];
} stac;

//函数提前声明
void print(cp *a);
bool isup(string s);
int replace_a();
int replace_b();
void print_select(cp *a);
void find_follow(int i,int tlen);
int find_select(int t3,string t2,int i,int j,int tlen);

//消除左递归
void remove_left_recursion()
{
    int i,j,k,t;
    int fk;
    for(i = 1; i<=length; i++)
    {
        fk=0;
        for(j = 0; j<=i-1; j++)
        {
            /*将每个形如 Ai → Ajγ 的产生式替换为产生式组 Ａi → ξ1γ｜ξ2γ｜……｜ξkγ ，
            其中，Aj→a1｜a2｜……｜ak是所有的当前Aj产生式*/
            for(k=0; k<lang[i].len; k++)
            {
                if(lang[j].key[0]==lang[i].value[k][0])     //代入
                {
                    string tempc = lang[i].value[k].substr(1);
                    lang[i].value[k] = lang[j].value[0]+ tempc;
                    for(t=1; t<lang[j].len; t++)
                    {
                        lang[i].value[lang[i].len++] = lang[j].value[t]+tempc;
                    }
                }
            }
        }
//        print(lang);
        /*消除关于Ai产生式中的直接左递归性*/
        //判断是否存在左递归
        for(int k=0; k<lang[i].len; k++)
        {
            if(lang[i].key[0]==lang[i].value[k][0])
            {
                fk=1;
            }
        }
        if(fk==1)
        {
            lang[length].key=lang[i].key+"~";
            int tlen=0;
            //消除递归，新创建~，#代表空
            for(int k=0; k<lang[i].len; k++)
            {
                if(lang[i].key[0]==lang[i].value[k][0])
                {
                    lang[length].value[lang[length].len++] = lang[i].value[k].substr(1)+lang[i].key+"~";
                }
                else
                {
                    lang[i].value[tlen++]=lang[i].value[k]+lang[i].key+"~";
                }
            }
            lang[i].len=tlen;
            lang[length].value[lang[length].len++]="#";
            length++;
        }
    }
}
//提取左公共因子
void extract_left_common_factor()
{
    int i,j;
    string t;
    map<char, int> mapfac;
    for(i = 0; i<=length; i++)
    {
        //第一遍循环，判断是否存在首字母重复
        for(j=0; j<lang[i].len; j++)
        {
            t = lang[i].value[j];
            mapfac[t[0]] = mapfac[t[0]] + 1;
        }
        for(map<char,int>::iterator it = mapfac.begin(); it!=mapfac.end(); it++)
        {
            if(it->second>1)
            {
                int temp[10]= {0};
                int l=0;
                int p=1;    //最长公因子位置
                //获取存在公因子的位置数组
                for(j=0; j<lang[i].len; j++)
                {
                    t = lang[i].value[j];
                    if(t[0]==it->first)
                        temp[l++] = j;
                }
                int f=0;    //退出标志
                //获取最长公因子
                while(1)
                {
                    char c = lang[i].value[temp[0]][p];
                    //字符判断问题？
                    for(j=1; j<l+1; j++)
                    {
                        t = lang[i].value[temp[j]];
                        if(t[p]!=c||t.length()<=p)
                        {
                            f=1;
                            break;
                        }
                    }
                    if(f==1)
                        break;
                    p++;
                }
                //更新文法映射
                //创建新的非终结符
                lang[length].key = lang[i].key+"~";
                for(j=0; j<l; j++)
                {
                    if(lang[i].value[temp[j]].length()==p)
                        lang[length].value[lang[length].len++] = "#";
                    else
                        lang[length].value[lang[length].len++] = lang[i].value[temp[j]].substr(p);
                }
                //修改旧的文法
                lang[i].value[temp[0]] = lang[i].value[temp[0]].substr(0,p) + lang[i].key+"~";
                int tlen=temp[0]+1;
                for(int k=temp[0]+1; k<lang[i].len; k++)
                {
                    if(lang[i].value[k][0]!=it->first)
                    {
                        lang[i].value[tlen++]=lang[i].value[k];
                    }
                }
                lang[i].len = tlen;
                length++;
            }
        }
        //完成一个式子的统计，将数据归零。
        for(j=0; j<lang[i].len; j++)
        {
            t = lang[i].value[j];
            mapfac[t[0]] = 0;
        }
    }
}
//创建First集
void create_first()
{
    int i,j,tlen=1;
    //获取所有首字母
    for(i=0; i<length; i++)
    {
        first[i].key = lang[i].key;
        for(j=0; j<lang[i].len; j++)
        {
            tlen=1;
            first[i].value[first[i].len] = lang[i].value[j][0];
            while(lang[i].value[j][tlen]=='~')
            {
                first[i].value[first[i].len] = first[i].value[first[i].len] + lang[i].value[j][tlen++];
            }
            first[i].len++;
        }
    }
//    print(first);
    int t = replace_a();
    while(t==1)
    {
        t = replace_a();
    }
}
//替代First集中的非终结符
int replace_a()
{
    int i,j,k,m,n,f,f2,f3=0,tlen=1;
    //对所有非终结符进行替代，深层嵌套遍历，c++对于字符串的操作不太好使
    for(i=0; i<length; i++)
    {
        for(j=0; j<first[i].len; j++)
        {
            string t =  first[i].value[j];
//            print(first);
//            printf("\n");
//            cout<<first[i].key<<t<<endl;
            if(isup(t))
            {
                f = 0;//填充标记
                tlen = 0;
                for(k=0; k<length; k++)
                {
                    //找到对应的非终结符的First集
//                    cout<<"here "<<first[k].key<<endl;
                    if(first[k].key == t)
                    {
                        for(m=0; m<first[k].len; m++)
                        {
                            f2 = 0;//重复标记
                            for(n=0; n<first[i].len; n++)
                            {
                                //如果子集中存在非终结符，标记。
                                if(isup(first[k].value[m]))
                                    f3=1;
                                if(first[k].value[m]==first[i].value[n])
                                    f2=1;
                            }
                            if(f2!=1)
                            {
//                                cout<<first[k].value[m]<<endl;
                                //不重复，开始替代。
                                if(f==0)
                                {
                                    first[i].value[j] = first[k].value[m];
                                    f=1;
                                    tlen++;
                                }
                                else
                                {
                                    first[i].value[first[i].len++] = first[k].value[m];
                                }
//                                cout<<first[i].value[first[i].len]<<endl;
                            }
                        }
                        if(tlen==0)
                        {
//                            cout<<"here  "<<first[i].key<<endl;
                            //没有新的字符添加进入，直接删除对应的非终结符
                            for(n=j; n<first[i].len-1; n++)
                            {
                                first[i].value[n]=first[i].value[n+1];
                            }
//                            cout<<"here  "<<first[i].value[j]<<endl;
                            first[i].len--;
                        }
                    }
                }
            }
        }
    }
    return f3;
}
//创建Follow集
void create_follow()
{
    int i,j,k,m,n,e,e2,f2,tlen=0;
    follow[tlen++].key = lang[0].key;
    follow[0].value[follow[0].len++] = "$";
    for(i=0; i<length; i++)
    {
        for(j=0; j<lang[i].len; j++)
        {
            for(k=0; k<lang[i].value[j].length(); k++)
            {
                string t(1,lang[i].value[j][k]);
                while(lang[i].value[j][k+1]=='~')
                {
                    t = t+"~";
                    k++;
                }
                if(isup(t))
                {
                    for(m=0; m<tlen; m++)
                    {
                        if(t==follow[m].key)
                            break;
                    }
                    if(m==tlen)
                    {
                        //如果该非终结符未找到，则创建。
                        follow[tlen++].key = t;
                    }
                    if(k==lang[i].value[j].length()-1)
                    {
                        //如果是最后一个字符
                        if(t!= lang[i].key)
                        {
                            follow[m].value[follow[m].len++] = lang[i].key;
                        }
                    }
                    else
                    {
                        //后跟非终结符，jia加入其first
                        string t2(1,lang[i].value[j][k+1]);
                        int t3=k;
                        while(t3+2<lang[i].value[j].length()&&lang[i].value[j][t3+2]=='~')
                        {
                            t2 = t2+"~";
                            t3++;
                        }
                        if(isup(t2))
                        {
                            for(n=0; n<length; n++)
                            {
                                if(first[n].key == t2)
                                {
                                    for(e=0; e<first[n].len; e++)
                                    {
                                        f2 = 0; //重复标记
                                        for(e2=0; e2<follow[m].len; e2++)
                                        {
                                            if(first[n].value[e]==follow[m].value[e2])
                                            {
                                                f2=1;
                                                break;
                                            }
                                        }
                                        if(f2==0)
                                        {
//                                            cout<<first[n].key<<" "<<t2<<"  "<<first[n].value[e]<<endl;
                                            if(first[n].value[e]=="#")
                                            {
                                                follow[m].value[follow[m].len++] = lang[i].key;
                                            }
                                            else
                                            {
                                                follow[m].value[follow[m].len++] = first[n].value[e];
                                            }

                                        }
                                    }
                                    break;
                                }
                            }
                        }
                        else
                        {
                            follow[m].value[follow[m].len++] = lang[i].value[j][k+1];
                        }
                    }
                }
            }
        }
    }
    printf("\n");
    int f = replace_b();
    while(f==1)
    {
        f = replace_b();
    }
}
//替代Follow集中的非终结符
int replace_b()
{
    int i,j,k,m,n,f,f2,f3=0,tlen=1;
    //对所有非终结符进行替代
    for(i=0; i<length; i++)
    {
        for(j=0; j<follow[i].len; j++)
        {
            string t =  follow[i].value[j];
            if(isup(t))
            {
                f = 0;//填充标记
                tlen = 0;
                for(k=0; k<length; k++)
                {
                    //找到对应的非终结符的Follow集
                    if(follow[k].key == t)
                    {
                        for(m=0; m<follow[k].len; m++)
                        {
                            f2 = 0;//重复标记
                            for(n=0; n<follow[i].len; n++)
                            {
                                //如果子集中存在非终结符，标记。
                                if(isup(follow[k].value[m]))
                                    f3=1;
                                if(follow[k].value[m]==follow[i].value[n])
                                {
                                    f2=1;
                                    break;
                                }
                            }
                            if(f2!=1)
                            {
                                //不重复，开始替代。
                                if(f==0)
                                {
                                    follow[i].value[j] = follow[k].value[m];
                                    f=1;
                                    tlen++;
                                }
                                else
                                {
                                    follow[i].value[follow[i].len++] = follow[k].value[m];
                                };
                            }
                        }
                        if(tlen==0)
                        {
                            //没有新的字符添加进入，直接删除对应的非终结符
                            for(n=j; n<follow[i].len-1; n++)
                            {
                                follow[i].value[n]=follow[i].value[n+1];
                            }
                            follow[i].len--;
                        }
                    }
                }
            }
        }
    }
    return f3;
}
//创建select集
void create_select()
{
    int i,j,k,m,tlen=0;
    for(i=0; i<length; i++)
    {
        for(j=0; j<lang[i].len; j++)
        {
            string t = lang[i].key+"->"+lang[i].value[j];
            select[tlen].key =  t;
            if(isup(lang[i].value[j]))
            {
                string t2(1,lang[i].value[j][0]);
                int t3=1;
                while(lang[i].value[j][0]=='~')
                {
                    t2 =t2+"~";
                    t3++;
                }
                find_select(t3,t2,i,j,tlen);
            }
            else if(lang[i].value[j]=="#")
            {
                find_follow(i,tlen);
            }
            else
            {
                select[tlen].value[select[tlen].len++] =  lang[i].value[j][0];
            }
            tlen++;
        }
    }
    lengths = tlen;
}
//查找应该加入select集的元素,将其加入集合中
int find_select(int t3,string t2,int i,int j,int tlen)
{
    int m,n,e,e2,f2,k;
    for(k=0; k<length; k++)
    {
        if(first[k].key==t2)
            break;
    }
    for(m=0; m<first[k].len; m++)
    {
        if(first[k].value[m]=="#")
        {
            if(t3==lang[i].value[j].length())
            {
                //当前是最后一个字符，加入follow集
                find_follow(i,tlen);
            }
            else
            {
                string tt2(1,lang[i].value[j][t3]);
                int tt3=t3+1;
                while(lang[i].value[j][0]=='~')
                {
                    tt2 =tt2+"~";
                    tt3++;
                }
                find_select(tt3,tt2,i,j,tlen);
            }
        }
        else
        {
            //判重
            f2=0;
            for(e2=0; e2<select[tlen].len; e2++)
            {
                if(select[tlen].value[e2]==first[k].value[m])
                {
                    f2=1;
                    break;
                }
            }
            if(f2==0)
            {
                select[tlen].value[select[tlen].len++] = first[k].value[m];
            }

        }
    }
    return 0;
}
//查找follow集
void find_follow(int i,int tlen)
{
    int e,n,f2,e2;
    for(e=0; e<length; e++)
    {
        if(follow[e].key==lang[i].key)
            break;
    }
    for(n=0; n<follow[e].len; n++)
    {
        f2=0;
        for(e2=0; e2<select[tlen].len; e2++)
        {
            if(select[tlen].value[e2]==follow[e].value[n])
            {
                f2=1;
                break;
            }
        }
        if(f2==0)
        {
            select[tlen].value[select[tlen].len++] = follow[e].value[n];
        }
    }
}
//判断是否是LL1文法
bool judge()
{
    int i,j=0,k,m;
    map <string,int>s;
    for(i=0; i<length; i++)
    {
        while(1)
        {
            for(k=0; k<select[j].key.length(); k++)
            {
                if(select[j].key[k]=='-')
                    break;
            }
            if(lang[i].key==select[j].key.substr(0,k))
            {
                for(m=0; m<select[j].len; m++)
                {
                    s[select[j].value[m]] = s[select[j].value[m]]+1;
                }
            }
            else
            {
                break;
            }
            j++;
        }
        for(map<string,int>::iterator it=s.begin(); it !=s.end(); it++)
        {
            if(it->second>1)
                return false;
        }
        s.clear();
    }
    return true;
}
//创建预测分析表
void create_chart()
{
    int i,j=0,k,m,tlen=0;;
    for(i=0; i<length; i++)
    {
        forecast[tlen].key = lang[i].key;
        while(1)
        {
            for(k=0; k<select[j].key.length(); k++)
            {
                if(select[j].key[k]=='-')
                    break;
            }
            if(lang[i].key==select[j].key.substr(0,k))
            {
                for(m=0; m<select[j].len; m++)
                {
                    forecast[tlen].in[forecast[tlen].len] = select[j].value[m];
                    forecast[tlen].out[forecast[tlen].len] = select[j].key;
                    forecast[tlen].len++;
                }
            }
            else
            {
                break;
            }
            j++;
        }
        tlen++;
    }
}
//判断非终结符
bool isup(string s)
{
    if(s[0]>='A'&&s[0]<='Z')
        return true;
    else
        return false;
}
//打印出文法、First集、Follow集
void print(cp *a)
{
    for(int i=0; i<length; i++)
    {
        cout<<a[i].key<<"->";
        for(int j=0; j<a[i].len; j++)
        {
            cout<<a[i].value[j]<<'|';
        }
        cout<<endl;
    }
}
//打印出select集
void print_select(cp *a)
{
    for(int i=0; i<lengths; i++)
    {
        cout<<a[i].key<<"   ";
        for(int j=0; j<a[i].len; j++)
        {
            cout<<a[i].value[j]<<',';
        }
        cout<<endl;
    }
}
//打印出预测分析表
void print_chart(chart *a)
{
    for(int i=0; i<length; i++)
    {
        int j=0;
        cout<<"     ";
        for(j=0; j<a[i].len; j++)
        {
            cout<<a[i].in[j]<<"     ";
        }
        cout<<endl;
        cout<<a[i].key<<"   ";
        for(j=0; j<a[i].len; j++)
        {
            cout<<a[i].out[j]<<"     ";
        }
        cout<<endl;
    }
}
//主函数
int main()
{
    string v;
    int i=0,j=0,k,k2;
    //读入文法进行预处理
    while(1)
    {
        cin>>v;
        if(v[0]=='$')
            break;
        for(k=0; k<v.length(); k++)
        {
            if(v[k]=='-')
                break;
        }
        int pos=k+2;
        lang[i].key = v.substr(0,k);
        for(j=pos; j<v.length(); j++)
        {
            if(v[j]=='|')
            {
                lang[i].value[lang[i].len++] = v.substr(pos, j-pos);
                pos = j+1;
            }
        }
        lang[i].value[lang[i].len++] = v.substr(pos, j-pos);
        i++;
//单行输入
//        i=0;
//        while(1){
//            if(cp[i].key==c){
//                cp[i].value[cp[i].len++] = v;
//                break;
//            }else if{
//                cp[i].key=='0';
//                cp[i].key=c;
//                cp[i].value[cp[i].len++] = v;
//                break;
//            }
//            i++;
//        }
    }
    length = i;
    remove_left_recursion();
    cout<<"消除左递归"<<endl;
    print(lang);
    printf("\n");
    extract_left_common_factor();
    cout<<"提取左公共因子"<<endl;
    print(lang);
    printf("\n");
    create_first();
    cout<<"first集"<<endl;
    print(first);
    printf("\n");
    create_follow();
    cout<<"follow集"<<endl;
    print(follow);
    printf("\n");
    create_select();
    cout<<"select集"<<endl;
    print_select(select);
    printf("\n");
    if(judge())
    {
        printf("true LL1\n");
        create_chart();
        cout<<"预测分析表"<<endl;
        print_chart(forecast);
        printf("\n");
        cout<<"语法分析"<<endl;
        //初始化栈
        stac.value[stac.len++] ="$";
        stac.value[stac.len++] =lang[0].key;
        FILE *fread = fopen("C:\\Users\\shengqiongyi\\Desktop\\test\\final.txt", "rt");
        if(fread==NULL)
        {
            printf("no input language\n");
            return -1;
        }
        char linetxt[128];
        while(1)
        {
            fgets(linetxt,128,fread);
            //获取输入的元素，将输入元素转化为string类型
            char t;
            if(linetxt[0]=='$')
            {
                t='$';
            }
            else if(linetxt[1]=='3')
            {
                t = linetxt[4];
            }
            else
            {
                t = linetxt[1];
            }
            string ts(1,t);
            while(1)
            {
                //取栈顶元素
                for(i=0; i<length; i++)
                {
                    if(stac.value[stac.len-1]==forecast[i].key)
                        break;
                }
                //输出对应的产生式，更新栈，如果无法找到则报错。
                for(j=0; j<forecast[i].len; j++)
                {
                    //根据预测分析表找到输入ts时的产生式
                    if(forecast[i].in[j]==ts)
                    {
                        cout<<forecast[i].out[j]<<endl;
                        for(k=0; k<forecast[i].out[j].length(); k++)
                        {
                            if(forecast[i].out[j][k]=='>')
                                break;
                        }
                        string news[10];
                        int nlent=0;
                        int pos=k+1;
                        //如果产生式为空，直接退栈
                        if(forecast[i].out[j][pos]=='#')
                        {
                            stac.len--;
                            break;
                        }
                        //将结果转换成单个string更新栈
                        for(k2=k+1; k2<forecast[i].out[j].length(); k2++)
                        {
                            if(forecast[i].out[j][k2+1]!='~')
                            {
                                news[nlent++] = forecast[i].out[j].substr(pos,k2-pos+1);
                                pos=k2+1;
                            }
                        }
                        stac.len--;
                        for(k=nlent-1; k>=0; k--)
                        {
                            stac.value[stac.len++] = news[k];
                        }
                        break;
                    }
                }
                //如果栈顶元素和输入元素相同，退栈，进行下一位输入
                if(stac.value[stac.len-1]==ts)
                {
                    if(stac.value[stac.len-1]!="$")
                        stac.len--;
                    break;
                }
                //预测分析表中未找到对应的分析，则报错
                if(j==forecast[i].len)
                {
                    printf("err language\n");
                    return -1;
                }
            }
            //如果栈为空，判断是否成功完成语法分析
            if(stac.value[stac.len-1]=="$")
            {
                if(ts=="$")
                {
                    printf("correct language\n");
                    break;
                }
                else
                {
                    printf("err language\n");
                    return -1;
                }

            }
        }
    }
    else
    {
        printf("false LL1\n");
    }
    return 0;
}
