#include <bits/stdc++.h>
using namespace std;
unordered_map <string,int> dict;
vector <string> vec;
string s,subs;
stack <int> sta;
int totalNum;
int switchNum;
vector <int> caseNum;
int if_else_num;
int double_if_else_num;
bool checkDiv(string& s,int& i)   //用部分符号切割单词
{
	if (s[i]=='"')   //忽略字符串
	{
		for (i=i+1;i<s.size();i++)
		{
			if (s[i]=='"') return true;
		}
		while (getline(cin,s))
		{
			for (i=0;i<s.size();i++)
			{
				if (s[i]=='"') return true;
			}
		}
	}
	if (s[i]=='/' && i+1<s.size() && s[++i]=='*')  //忽略注释
	{
		for (i=i+1;i<s.size();i++)
		{
			if (s[i]=='*' && s[++i]=='/') return true;
		}
		while (getline(cin,s))
		{
			for (i=0;i<s.size();i++)
			{
				if (s[i]=='*' && i+1<s.size() && s[++i]=='/') return true;
			}
		}
	}
    return s[i]==' ' || s[i]=='\n' || s[i]==';' || s[i]=='(' || s[i]==')' || s[i]=='{' || s[i]=='}' || s[i]=='<' || s[i]=='>' || s[i]==',' || s[i]==':';
}
void calc()
{
    dict.clear();
    dict["auto"]=1; dict["break"]=1; dict["case"]=1; dict["char"]=1; dict["const"]=1; dict["continue"]=1; dict["default"]=1; dict["do"]=1;
    dict["double"]=1; dict["else"]=1; dict["enum"]=1; dict["extern"]=1; dict["float"]=1; dict["for"]=1; dict["goto"]=1; dict["if"]=1;
    dict["int"]=1; dict["long"]=1; dict["register"]=1; dict["return"]=1; dict["short"]=1; dict["signed"]=1; dict["sizeof"]=1; dict["static"]=1;
    dict["struct"]=1; dict["switch"]=1; dict["typedef"]=1; dict["union"]=1; dict["unsigned"]=1; dict["void"]=1; dict["volatile"]=1; dict["while"]=1;

    bool hasPreSwitch=false; //防止出现一个switch都没有的情况而误统计
    int curCaseNum=0;
    for (string it:vec)
    {
        if (dict[it]==1) ++totalNum;
        if (it=="case") ++curCaseNum;
        else if (it=="switch")
        {
            ++switchNum;
            if (hasPreSwitch) caseNum.push_back(curCaseNum);
            curCaseNum=0;
            hasPreSwitch=true;
        }
    }
    if (hasPreSwitch) caseNum.push_back(curCaseNum);
}
int main()
{
    ios::sync_with_stdio(false);

    freopen("in.txt","r",stdin);
	freopen("1out.txt","w",stdout);

    bool hasPreElse=false;  //记录上一个单词是否为else且与现单词之间均为空字符
    bool hasPreIf=false;    //记录上一个if/else单词是否是if
    int cnt=0;  //记录有几个连续的if-else if-else
    while (getline(cin,s))
    {
        s=' '+s+' ';
        subs.clear();
        for (int i=1;i<s.size();++i)
        {
            if (checkDiv(s,i))
            {
                if (subs=="if")
                {
                    if (hasPreElse)
                    {
                        hasPreElse=false;
                    }
                    else
                    {
                        //出现if且前面没有紧跟着的else，说明已经进入新的if-else段中
                        if (!hasPreIf)  //上个if-else段不以if作结尾
                        {
                            if (cnt>1) ++double_if_else_num;
                            else if (cnt==1) ++if_else_num;
                        }
                        cnt=0;
                    }
                    hasPreIf=true;
                }
                else if (subs=="else")
                {
                    ++cnt;
                    hasPreIf=false;
                    hasPreElse=true;
                }

                if (s[i]!=' ' && s[i]!='\n') hasPreElse=false;

                if (subs.size()>0)
                {
                    vec.push_back(subs);
                    subs.clear();
                }

                if (s[i]=='{')
                {
                    sta.push(hasPreElse);
                    sta.push(hasPreIf);
                    sta.push(cnt);
                    hasPreElse=false;
                    hasPreIf=false;
                    cnt=0;
                }
                else if (s[i]=='}')
                {
                    if (!hasPreIf)  //遇到右括号时直接先统计当前的if-else if-else段
                    {
                        if (cnt>1) ++double_if_else_num;
                        else if (cnt==1) ++if_else_num;
                    }
                    cnt=sta.top(); sta.pop();
                    hasPreIf=sta.top(); sta.pop();
                    hasPreElse=sta.top(); sta.pop();
                }
            }
            else subs+=s[i];
        }
    }
    calc();
    cout<<"total num: "<<totalNum<<endl;
    cout<<"swich num: "<<switchNum<<endl;
    cout<<"case num:";
    for (int i:caseNum) cout<<" "<<i;
    cout<<endl;
    cout<<"if-else num: "<<if_else_num<<endl;
    cout<<"if-else if-else num: "<<double_if_else_num<<endl;
    fclose(stdin);
    fclose(stdout);
}
