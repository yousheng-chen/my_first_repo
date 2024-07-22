#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <conio.h>
#pragma warning(disable : 4996)
using namespace std;

struct Timetable		//车次信息结构体
{
	char no[10];//车次
	int hour;//发车时间（时）
	int minute;//发车时间（分）
	char Starting_station[10];//始发站
	char Last_station[10];//终点站
	float time;//行车时间
	int fare;  //票价
	int max_number;//最大载客数
	int sold_number;//已售票数
};

//函数原型声明
void GetPwd(char* pwd);  //读取口令
void Signin();//登录
int AddBus(Timetable* timetables, int n);  //增加车次
int find(Timetable* timetables, int n, char* no);  //按车次查找
int find_Last(Timetable* timetables, int n, char* Last_station, int* b);  //按终点站查找
int find_Start(Timetable* timetables, int n, char* Last_station, int* b);  //按终点站查找
int DelBus(Timetable* timetables, int n);  //删除车次
int AdminWindowSelect();  //管理员选择窗口
int MainWindowSelect();  //主选择窗口
int PassagerWindowSelect();  //顾客选择窗口
void TicketDelete(Timetable* timetables, int n);  //退票
void TicketOrder(Timetable* timetables, int n);   //售票
int StopSerive(Timetable* timetables, int n, char* no);  //停止服务
void ShowTimetable(Timetable* timetables, int n);   //显示所有车次
void ShowTitle();   //显示标题
void ShowBusInfo(Timetable* timetables, int n, int idx);   //显示一个车次信息
int ReadFromFile(Timetable* timetables, char* filename);   //从文件filename读到结构体数组
void inttostr(int n, char* str);   //整数转换成字符串
void GenerateLogFileName(char* filename);   //产生日志文件名
int WritetoFile(Timetable* timetables, int n, char* filename);  //将n个元素的结构体数组timetables写入文件filename
int QueryWindowSelect();   //查询选择窗口
void Query(Timetable* timetables, int n);   //按车次或按目的地查询并显示
void AdminMode(Timetable* timetables, char* filename);  //管理员模式
void SortbyTime(Timetable* timetables, int n);   //按发车时间升序排序
void PassagerMode(Timetable* timetables, char* filename);  //顾客模式
void SortbyIdx(Timetable* timetables, int n);

void GetPwd(char* pwd)  //读取口令，并将输入的字符串转换为*显示
{
	cin.sync();//清空cin数据流里的内容
	int i = 0;
	char ch;

	while (1)
	{
		ch = getch();//在头文件conio.h中，从键盘上读取一个字符
		if (ch == '\r')  break; //'\r' 表示回车键
		else if (ch == '\b') { cout << "\b \b";   i--; }  //'\b'表示退格键
		else { pwd[i++] = ch;	cout << '*'; }
	}
	pwd[i] = '\0';
	cout << '\n';
}

void Signin()      //管理员登录
{
	char ID[20];   //存放管理员名字
	char pwd[20];  //存放密码
	cin.sync();  //清空cin数据流里的内容

	do
	{
		cout << "输入管理员名字：";
		cin >> ID;
		cout << "输入密码：";//输入提示
		GetPwd(pwd);//输入密码并转换成*号	
		if (strcmp(ID, "admin") == 0 && strcmp(pwd, "123") == 0)
			break;

		cout << "用户或密码错误，重新输入：\n";
	} while (1);

	cout << "登录成功！\n";
	//system("pause");
}

int AddBus(Timetable* timetables, int n)     //增加车次，将更改的信息添加到文件中     
{
	cout << "增加车次，将更改的信息添加到文件中\n";
	int i;
	char num[10];
	system("cls");

	cout << "请输入增加的车次号：";
	cin >> num;
	while ((i = find(timetables, n, num)) != -1) //输入不存在的车次
	{
		cout << "该车次已存在，请重新输入";
		cin >> num;
	}
	strcpy(timetables[n].no, num);
	int hour, minute, max, fare;
	float time;
	char start_station[10], last_station[10];
	cout << "请依次输入车次时间，起点站，终点站，车次时长，票价，最大载客人数；" << endl;
	cin >> hour >> minute >> start_station >> last_station >> time >> fare >> max;
	timetables[n].hour = hour;   //依次逐个赋值
	timetables[n].minute = minute;
	timetables[n].max_number = max;
	timetables[n].fare = fare;
	timetables[n].time = time;
	strcpy(timetables[n].Starting_station, start_station);
	strcpy(timetables[n].Last_station, last_station);
	timetables[n].sold_number = 0;
	cout << "添加车次成功！\n";
	n++;
	//SortbyTime(timetables, n);
	//WritetoFile(timetables, n, "bus.txt");
	return n;
}

int find(Timetable* timetables, int n, char* no)	  //按车次查找
{
	//cout << "按车次查找\n";
	for (int i = 0; i < n; i++)
		if (strcmp(timetables[i].no, no) == 0)
			return i;      //返回下标
	return -1;   //没有找到相应车次时返回值-1
}

int find_Last(Timetable* timetables, int n, char* Last_station, int* b)  //按终点站查找  
{
	int j = 0;  /*存放同一终点站的车次数
	数组b的作用是记录下标
	 cout << "将同一终点站的多个车次在结构体数组中的下标写入b数组\n";*/
	for (int i = 0; i < n; i++)
		if (strcmp(timetables[i].Last_station, Last_station) == 0)
			b[j++] = i;
	if (j > 0) return j;
	return -1; //没有查到相应终点站的车次时返回值-1
}

int find_Start(Timetable* timetables, int n, char* Start_station, int* b)  //按终点站查找  
{
	int j = 0;  /*存放同一终点站的车次数
	数组b的作用是记录下标
	 cout << "将同一终点站的多个车次在结构体数组中的下标写入b数组\n";*/
	for (int i = 0; i < n; i++)
		if (strcmp(timetables[i].Starting_station, Start_station) == 0)
			b[j++] = i;
	if (j > 0) return j;
	return -1; //没有查到相应终点站的车次时返回值-1
}

int DelBus(Timetable* timetables, int n)   //删除车次,并更新车次文件
{
	//cout << "删除车次,并更新车次文件\n";
	cout << "输入需要删除的车次：";
	char num[7]; int i;
	cin >> num;
	while ((i = find(timetables, n, num)) == -1) //未找到相应的车次
	{
		cout << "输入错误，未找到该车次";
		return 0;
	}
	//找到相应的车次，完成删除车次并更新文件
	ShowTitle();
	int idx = find(timetables, n, num);
	ShowBusInfo(timetables, n,idx);
	cout << "确定？(y/n):";
	char ensure; cin >> ensure;
	switch (ensure)  //确定删除
	{
	case 'y': 
		timetables[idx] = timetables[n - 1];
		n -= 1; //数组个数减一
		cout << "删除车次成功！\n";
		//WritetoFile(timetables, n, "bus.txt"); break;
	case 'n': break;
	default: break;
	}
	//system("pause");
	return n;
}

int AdminWindowSelect() 		 	//管理员菜单界面
{
	int num;
	while (1)
	{
		system("cls");
		cout << "\n                *********** 管理员功能 ***************\n\n";
		cout << "                           1、增加车次信息\n";
		cout << "                           2、车次信息浏览\n";
		cout << "                           3、车次信息查询\n";
		cout << "                           4、注销车次信息\n";
		cout << "                           5、返回\n";
		cout << "\n                **************************************\n";
		cout << "                  请选择(1-5): ";
		cin.sync();
		cin >> num;
		if (num >= 1 && num <= 5)
			break;
	}
	return num;
}

int MainWindowSelect()  //主窗口菜单 
{
	int num;
	while (1)
	{
		system("cls");
		cout << "\n               *********** 欢迎使用车票管理系统 ***********\n\n";
		cout << "                              1.管理员功能\n";
		cout << "                              2.顾客功能\n";
		cout << "                              3.退出\n";
		cout << "\n               ********************************************\n";
		cout << "               请选择(1-3): ";
		cin >> num;
		if (num >= 1 && num <= 3)
			break;
	}
	return num;
}

int PassagerWindowSelect()     //顾客功能
{
	int num;
	while (1)
	{
		system("cls");
		cout << "\n                ************** 顾客功能 **************\n\n";
		cout << "                            1、车次信息查询\n";
		cout << "                            2、购        票\n";
		cout << "                            3、退        票\n";
		cout << "                            4、返        回\n";
		cout << "\n                **************************************\n";
		cout << "                  请选择(1-4): ";
		cin >> num;
		if (num >= 1 && num <= 4)
			break;
	}
	return num;
}

void TicketDelete(Timetable* timetables, int n)   //退票
{
	//cout << "实现退票功能\n";
	system("cls");
	cout << "输入要退票的车次(输入0返回上一层):";
	char num[10];
	cin >> num;
	if (strcmp(num, "0") == 0) return;
	int idx = find(timetables, n, num);
	if (idx != -1)
	{
		ShowTitle();
		ShowBusInfo(timetables, n, idx);
		if (!StopSerive(timetables, n, timetables[idx].no)) // 没有停止退票
		{   // 0可以，1不可以
			cout << "请输入退票数：";
			int tickets_num;
			cin >> tickets_num;
			if (tickets_num <= timetables[idx].sold_number)
			{
				timetables[idx].sold_number -= tickets_num;
				cout << "退票成功！";
			}
			else
				cout << "退票数大于购票数，退票失败！";
		}
		else
		{
			cout << "该班车已停止退票,无法退票!\n";
			return;
		}
	}
	else
		cout << "输入错误，没有该车次！\n";
	//system("pause");
}

void TicketOrder(Timetable* timetables, int n)  //购票        
{
	//cout << "实现购票功能\n";
	int idx;
	char num[10];
	system("cls");
	cin.sync();
	cout << "输入要订票的车次（输入0返回上一层）：\n";
	cin >> num;
	if (strcmp(num, "0") == 0)return;
	if ((idx = find(timetables, n, num)) != -1)//找得到对应的车次
	{
		ShowTitle();
		ShowBusInfo(timetables, n, idx);
		if (timetables[idx].sold_number < timetables[idx].max_number && !StopSerive(timetables, n, timetables[idx].no))
		{
			//当已售票数小于最大载客量 并且 没有停止售票服务时
			cout << "输入您要购买的票数：";
			int tickets_num;
			cin >> tickets_num;
			if (tickets_num <= (timetables[idx].max_number - timetables[idx].sold_number))
			{
				timetables[idx].sold_number += tickets_num;
				cout << "订票成功！\n";
			}
			else cout << "剩余票数不足！\n";
		}
		else
		{
			cout << "该车次票已售完或已停止售票！\n";
			//system("pasuse");
		}
	}
	else
	{
		cout << "没有该车次！\n";
		return;
		//system("pause");
	}
	//system("pause");
}

int StopSerive(Timetable* timetables, int n, char* no)  //判断是否停止售票或退票，0:可以，1：不可以   
{
	struct tm* local;       //struct tm 时间结构体类型
	time_t t;				//把当前时间给t
	t = time(NULL);
	local = localtime(&t);      //获取当前系统时间 
	int i = find(timetables, n, no);

	if ((local->tm_hour * 60 + local->tm_min) + 10 < (timetables[i].hour * 60 + timetables[i].minute))
		return 0;
	return 1;
}

void ShowTimetable(Timetable* timetables, int n)    //显示全部车次信息
{
	//cout << "显示全部车次信息\n";
	cin.sync();
	int i;
	system("cls");
	cout << "\n                   ********** 车次信息浏览 **********\n\n";
	cout << "                             1、按车次号排序\n";
	cout << "                             2、按发车时间查询\n";
	cout << "\n                   **********************************\n";
	cout << "                     请选择(1-2):";
	int num; cin >> num;
	switch (num)
	{
	case 1:SortbyIdx(timetables, n); break;
	case 2:SortbyTime(timetables, n); break;
	default:break;
	}
	ShowTitle();
	for (i = 0; i < n; i++)
		ShowBusInfo(timetables, n, i);
	//system("pause");
}

void ShowTitle()  //显示车次标题
{
	system("cls");
	cout << "\n\n***************************** 车次信息 *********************************\n";
	cout << "\n班次\t时间\t起点\t终点\t时长\t票价\t额定量\t已售票\t是否发车\n\n";
}

void ShowBusInfo(Timetable* timetables, int n, int idx)   //显示第idx车次信息   
{
	//cout << "显示第idx车次信息\n";
	Timetable tmp = timetables[idx];
	cout << tmp.no << '\t' << tmp.hour << ":" << tmp.minute << '\t' << tmp.Starting_station << '\t'
		<< tmp.Last_station << '\t' << tmp.time << '\t' << tmp.fare << '\t' << tmp.max_number << '\t'
		<< tmp.sold_number;
	if (StopSerive(timetables, n, timetables[idx].no) == 1)cout << "\t已发车\n";
	else cout << "\t未发车\n";
	//system("pause");
}

int ReadFromFile(Timetable* timetables, char* filename)
{
	int i = 0;  //车次数量
	//cout << "从文件filename读入数据到结构体数组timetables\n";
	ifstream bus; bus.open(filename);
	while (bus >> timetables[i].no)
	{
		//Timetable tmp = timetables[i];
		bus >> timetables[i].hour >> timetables[i].minute >> timetables[i].Starting_station >> timetables[i].Last_station
			>> timetables[i].time >> timetables[i].fare >> timetables[i].max_number >> timetables[i].sold_number;
		i++;
	}
	bus.close();
	//SortbyTime(timetables, i);
	//system("pause");
	return i;
}

void inttostr(int n, char* str) //将数字转换成字符串
{
	//cout << "将整数转换成字符串\n";
	int i = 0, j;
	while (n != 0)
	{
		str[i++] = n % 10 + '0';
		n = n / 10;
	}
	str[i] = '\0';
	for (j = 0, i = i - 1; j < i; j++, i--)
	{
		char t = str[i];
		str[i] = str[j];
		str[j] = t;
	}
	//system("pause");
}

void GenerateLogFileName(char* filename)   //生成日志文件的文件名
{
	filename[0] = '\0';
	struct tm* local;				//时间结构体 
	time_t t;                       //把当前时间给t
	t = time(NULL);					//NULL在stdio.h中定义为0
	local = localtime(&t);			//获取当前系统时间 

	char year[5], month[3], day[3];	//将年月日分别转换成字符串型
	inttostr(1900 + local->tm_year, year);
	inttostr(1 + local->tm_mon, month);
	inttostr(local->tm_mday, day);

	strcat(filename, year);			//生成日志文件名
	strcat(filename, "-");
	strcat(filename, month);
	strcat(filename, "-");
	strcat(filename, day);
	strcat(filename, ".log");
}

int WritetoFile(Timetable* timetables, int n, char* filename)
{
	//cout << "将具有n个变量的结构体数组timetables写入文件filename\n";
	//SortbyTime(timetables, n);    // 每次写入文件均排序一次
	ofstream out;out.open(filename);
	int i;     //控制循环次数
	for (i=0; i < n; i++)
	{
		//写入文件
		Timetable tmp = timetables[i];
		out << tmp.no << '\t' << tmp.hour << "\t" << tmp.minute << '\t' << tmp.Starting_station << '\t' << tmp.Last_station << '\t'
			<< tmp.time << '\t' << tmp.fare << '\t' << tmp.max_number << '\t' << tmp.sold_number << '\n';
	}
	out.close();
	return 0;
}

int QueryWindowSelect() 
{
	int num;
	while (1)
	{
		system("cls");
		cout << "\n                   ********** 车次信息查询 **********\n\n";
		cout << "                             1、按车次号查询\n";
		cout << "                             2、按终点站查询\n";
		cout << "                             3、按起点站查询\n";
		cout << "                             4、退 出  查 询\n";
		cout << "\n                   **********************************\n";
		cout << "                     请选择(1-3):";
		cin >> num;
		if (num >= 1 && num <= 3)
			break;
	}
	return num;
}

void Query(Timetable* timetables, int n)  //车次查询
{
	//cout << "完成按车次或按终点站查询并显示\n";
	int num,j=-1,b[10]; char no[10],L_station[5], S_station[5];
	int no_find;
	switch (QueryWindowSelect())
	{
	case 1: cout << "输入车次:";  cin >> no,no_find = find(timetables,n,no); 
		if (no_find != -1) { system("cls"); ShowTitle();  ShowBusInfo(timetables, n, no_find); }
		else { cout << "没有该车次！"; }break;
	case 2: cout << "输入终点站:";	cin >> L_station; j = find_Last(timetables, n, L_station, b);
		if (j == -1) { cout << "没有该终点站的车次！" << endl; break; }
		system("cls"); ShowTitle();
		for (int i = 0; i < j; i++) ShowBusInfo(timetables, n, b[i]); break;
	case 3: cout << "输入起点站:";	cin >> S_station; j = find_Start(timetables, n, S_station, b);
		if (j == -1) { cout << "没有该终点站的车次！" << endl; break; }
		system("cls"); ShowTitle();
		for (int i = 0; i < j; i++) ShowBusInfo(timetables, n, b[i]); break;
	case 4:return; break;// 返回上级菜单;
	//default:cout << "输入错误!\n"; system("pause"); break;
	}
}

int InitializaionPassageMode(Timetable* timetables, char* LogFileName, char* filename)
{
	int n = 0; //车次数量
	//cout << "如果日志文件不存在，则建立日志文件\n";
	if (!LogFileName)
	{
		GenerateLogFileName(LogFileName);
	}
	n = ReadFromFile(timetables, filename);
	WritetoFile(timetables, n, LogFileName);
	//system("pause");
	return n;
}

void SortbyTime(Timetable* timetables, int n)
{
	//cout << "对具有n个变量的结构体数组按发车时间排成升序\n";
	//冒泡排序法
	int i,j;
	Timetable tmp;
	for (i = 0; i < n - 1; i++)
	{
		for (j = 0; j < n - 1 - i; j++)
		{
			if ((timetables[j].hour * 60 + timetables[j].minute) > (timetables[j + 1].hour * 60 + timetables[j + 1].minute))
			{
			    tmp = timetables[j], timetables[j] = timetables[j + 1], timetables[j + 1] = tmp;
			}
		}
	}
	//system("pause");
}
void SortbyIdx(Timetable* timetables, int n)
{
	//按照车次序号排序
	int i, j;
	Timetable tmp;
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n - 1 - i; j++)
		{
			if (strcmp(timetables[j].no, timetables[j + 1].no) == 1)
			{
				tmp = timetables[j]; timetables[j] = timetables[j + 1]; timetables[j + 1] = tmp;
			}
		}
	}
}

void PassagerMode(Timetable* timetables, char* filename) //顾客功能
{
	char LogFileName[200];    //存放日志文件名。
	int n;
	GenerateLogFileName(LogFileName);//根据当前日期生成日志文件名

	n = InitializaionPassageMode(timetables, LogFileName, filename);

	int PassagerChoice;
	while (1)
	{
		switch (PassagerChoice = PassagerWindowSelect())
		{
		case 1: Query(timetables, n); system("pause"); break;//车次信息查询
		case 2: TicketOrder(timetables, n); WritetoFile(timetables, n, LogFileName); WritetoFile(timetables, n, filename); system("pause"); break;  //购票，并更改文件信息
		case 3: TicketDelete(timetables, n); WritetoFile(timetables, n, LogFileName); WritetoFile(timetables, n, filename); system("pause"); break;//退票，并更改文件信息
		case 4: WritetoFile(timetables, n, LogFileName); 	return;//返回上级菜单
		}
	}
}

void AdminMode(Timetable* timetables, char* filename)  //管理员功能
{
	int n;
	Signin();  //登录

	n = ReadFromFile(timetables, "bus.txt");

	while (1)
	{
		int AdminChoice = AdminWindowSelect();
		switch (AdminChoice)
		{
		case 1:	n = AddBus(timetables, n); WritetoFile(timetables, n, filename); system("pause"); break;	//增加车次信息,并及时写到文件中
		case 2:	ShowTimetable(timetables, n); system("pause"); break;	//浏览时刻表
		case 3:	Query(timetables, n); system("pause"); break;	//车次信息查询	   			
		case 4:	n = DelBus(timetables, n); system("pause"); WritetoFile(timetables, n, filename); break;//注销车次,并及时写到文件中
		case 5: WritetoFile(timetables, n, filename); return;//返回上级菜单
		}
	}
}

int Quit()
{
	cin.sync(); //清空输入缓冲区
	cout << "退出系统吗(y/n)?";
	char ans;
	cin >> ans;
	if (ans == 'Y' || ans == 'y')
		return 1;
	return 0;
}

int main()
{
	int UserChoice;
	Timetable timetables[100];
	while (1)
	{
		switch (UserChoice = MainWindowSelect())
		{
		case 1:  AdminMode(timetables, "bus.txt"); break;	    //管理员模式
		case 2:  PassagerMode(timetables, "bus.txt"); break;	//顾客模式	
		case 3:  if (Quit() != 1) continue;				    //退出
		}
		if (UserChoice == 3) break;
	}
	return 0;
}
