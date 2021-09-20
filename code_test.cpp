// 2021-2022秋季学期软工实践第一次个人编程作业
// 目标：实现一个程序功能，它可以对读入的C或C++代码文件进行不同等级的关键字提取
// 基础要求：输出关键字统计信息
// 进阶要求：输出有几组switch case结构，同时输出每组对应的case个数
// 拔高要求：输出有几组if else结构
// 终极要求：输出有几组if，else if，else结构
#include <bits/stdc++.h>
using namespace std;

// 定义关键词字典作为全局变量，用于识别代码中出现的关键词
const string kKeyWordName[32]={"auto","break","case","char","const","continue","default","do",\
	"double","else","enum","extern","float","for","goto","if","int","long","register","return",\
	"short","signed","sizeof","static","struct","switch","typedef","union","unsigned","void",\
	"volatile","while"};

// CodeTest类封装了能够对c/c++代码进行检测的函数
// 调用类对象的 TestFile 文件可以实现对代码的检测
class CodeTest
{
    public:
		// 构造函数
        CodeTest(){}
		// Divide 函数用于检测当前字符是否是切割标志
        bool Divide(int &index);
		// Test1 将检测经过 Divide 函数处理后的单元，判断是否是关键词并实现进阶要求
		// 此外，Test1 通过改变布尔变量 is_previous_else_ ，表明上一个单元是else
        void Test1(string divided_unit);
		// 当 is_previous_else_ 为 true，调用 Test2，判断else的下一个单词是否是if，若不是则可以判断属于if-else结构还是if-else if-else结构
        void Test2(int &index);
		// 向 TestFile 传递需要检测的代码文件名，该函数可以通过调用 Test1、Test2、Divide 函数完成对代码的检测。
        void TestFile(string filename);
		// 根据输入的完成等级，进行输出
        void Display(int level);
    private:
		// str_line_是当前读出的行，word_是从行上切割下的单元
        string str_line_,word_;
		// 读写文件声明
        ifstream infile_;
        ofstream outfile_;
		// 栈 ifelse_stack_ 用于判断ifelse属于何种结构
        stack <int> ifelse_stack_;
		// 队列 switch_queue_ 用于存储case num
        queue <int> switch_queue_;
		// 指明上一个单元是否是else
        bool is_previous_else_;
		// 关键词计数器
        int keyword_count_;
		// if-else 结构计数器
        int if_else_count_;
		// if-else if-else 结构计数器
        int if_elseif_else_count_;
		// switch中的case 计数器
        int switch_count_;
}; 

// 判断是否对str_line_的当前字符位置进行切割
bool CodeTest::Divide(int &index)
{
    if(str_line_[index] == '"')
	{
		int j = index;
		for(index = index + 1; index < str_line_.size(); index++)
		{
			if(str_line_[index] == '"') return true;
		}
		index = j;
		return true;
	}
	if(str_line_[index] == '/' && str_line_[++index] == '*')
	{
		for(index = index + 1; index < str_line_.size(); index++)
		{
			if(str_line_[index] == '*' && str_line_[++index] == '/') return true;
		}
		while(getline(infile_, str_line_))
		{
			for(index = 0; index < str_line_.size(); index++)
			{ 
				if(str_line_[index] == '*' && str_line_[++index] == '/') return true;
			}		
		}
	}
    if(str_line_[index] == '(' || str_line_[index] == ')') return true;  // 遇到圆括号切割
    else if(str_line_[index] == '{' || str_line_[index] == '}') return true;  // 遇到大括号切割
    else if(str_line_[index] == '<' || str_line_[index] == '>') return true;  // 遇到尖括号切割
	else if(str_line_[index] == '&' || str_line_[index] == '*') return true;  // 遇到指针或引用切割
    else if(str_line_[index] == '\n'|| str_line_[index] == '\0' || str_line_[index] == '\t') return true;  // 遇到换行符、指标符、字符串结尾标识符切割
    else if(str_line_[index] == ' ' || str_line_[index] == ',' || str_line_[index] == ';' || str_line_[index] == ':') return true;  // 遇到标点符号或空格切割
}

// 对被切下来的单元进行关键词检测
void CodeTest::Test1(string divided_unit)
{
	for(int i = 0; i < 32; i++) if(divided_unit == kKeyWordName[i]) keyword_count_++;
	if(divided_unit == "switch" && switch_count_)  // 出现了switch结构，将之前计数的case num压入队列
	{
		switch_queue_.push(switch_count_);
		switch_count_ = 0;
	}
	if(divided_unit == "case") switch_count_++;
	if(divided_unit == "if")  // 出现 if 压入-1标识出现了新的if结构，再压入1
	{
		ifelse_stack_.push(-1);
		ifelse_stack_.push(1);
	} 
	if(divided_unit == "else") is_previous_else_ = true;  //检测到else
}

// 出现else时调用该函数，实现跳过所以空格，并从第一个非空格字符开始判断下一个单词是不是if
// 若判断是if，则ifelse_stack_压入1
// 若判断为不是if，则使用ifelse_stack_进行判断，将栈顶的连续的1弹出计算和，等于1是if-else结构。大于1是if-else if-else结构
void CodeTest::Test2(int &index)
{
	for(index; index < str_line_.size(); index++) if (!(str_line_[index] == ' ' || str_line_[index] == '\t')) break;
	if(index == str_line_.size())
	{
		int flag = 0;
		while (getline(infile_, str_line_))
		{
			for(index = 0; index < str_line_.size(); index++)
			{
				if (!(str_line_[index] == ' ' || str_line_[index] == '\t'))
				{
					flag = 1;
					break;
				}
			}
			if(flag) break;
		}	
	} 
	if(str_line_[index] == 'i' && str_line_[index+1] == 'f') 
	{
		ifelse_stack_.push(1);
		index++;
		keyword_count_++;
	}
	else
	{
		int ifelse_stack_count_ = 0;
		while(ifelse_stack_.top() > 0 && !ifelse_stack_.empty())
		{
			ifelse_stack_count_ += ifelse_stack_.top();
			ifelse_stack_.pop();
		}
		if(ifelse_stack_count_ > 1) if_elseif_else_count_++;
		else if(ifelse_stack_count_ == 1) if_else_count_++;
	}				
}

// 对输入的文件进行代码检测
void CodeTest::TestFile(string filename)
{
    is_previous_else_ = false;
    keyword_count_ = 0;
    if_else_count_ = 0;
    if_elseif_else_count_ = 0;
    switch_count_ = 0; 
    vector <string> outfilename_;
    infile_.open(filename, ios::in) ;
    outfile_.open("1out.txt", ios::trunc);
	if (!infile_.is_open())
	{
		cout << "文件不存在" << endl;
		exit (0);
	}
    while (getline(infile_, str_line_))
    {
        word_.clear();
        for (int i = 0; i <= str_line_.size(); ++i)
        {
			if (Divide(i))
            {
                if (word_.size() > 0)
                {
					Test1(word_);
					if(is_previous_else_)
					{
						Test2(i);
						is_previous_else_ = false;
					}
					outfilename_.push_back(word_);
                    word_.clear();
                }
                if(str_line_[i] == '{') ifelse_stack_.push(0); 
				else if(str_line_[i] == '}')
				{
					while(ifelse_stack_.top() != 0) ifelse_stack_.pop();
					ifelse_stack_.pop();	
				}
            }
            else word_ += str_line_[i];
        }
    }
    if(switch_count_)
	{
		switch_queue_.push(switch_count_);
		switch_count_ = 0;
	}
    for (auto it:outfilename_) outfile_<<it<<endl;
}

// 按输入的完成等级进行输出
void CodeTest::Display(int level)
{
    if(level >= 1) cout<<"total num: "<<keyword_count_<<endl;
    if(level >= 2)
    {
        cout<<"switch num: "<<switch_queue_.size()<<endl;
        cout<<"case num:";
		while(!switch_queue_.empty())
		{
			cout<<" "<<switch_queue_.front();
	        switch_queue_.pop();
		}
	    cout<<endl;
    }
    if(level >= 3) cout<<"if-else num: "<<if_else_count_<<endl;
    if(level >= 4) cout<<"if-elseif-else num: "<<if_elseif_else_count_<<endl;
}

int main()
{
    CodeTest *t = new CodeTest();  // 创建CodeTest对象准备进行代码检测
    string infilename;  // 待检测文件名
    int level;  // 完成等级
    cin>>infilename>>level;
    t->TestFile(infilename); // 调用检测函数
    t->Display(level);  // 按完成等级输出
    return 0;
}
