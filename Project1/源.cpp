#pragma warning(disable:4996)
#include <iostream>
#include <iomanip>
# include <stdio.h>
# include <string.h>
# include <malloc.h>
# include <windows.h>
# include <fstream>
using namespace std;
#define OK 1
typedef int status;


//定义一个航班类型结构体
typedef struct flightnode
{
	char flight_num[10]; //航班号
	char start_time[10]; //起飞时间
	char end_time[10];   //抵达时间
	char start_place[20];//起飞城市
	char end_place[20];  //抵达城市
	int left;            //空座数
	float price;         //票价
	float price_discount;//票价折扣
	int isFull;          //航班是否满仓
	struct flightnode *next;
}flightnode, *flightlinklist;
//定义一个订单类型结构体
typedef struct passengernode
{
	char name[20];      //姓名
	char ID_num[20];    //证件号
	char flight_num[10];//航班号
	int order_num;      //订单号
	int ticket_num;     //订票数量
	struct passengernode *next;
	passengernode *head;
	passengernode *rear;
}passengernode, *passengerlinklist;


//初始化一个空的航班链表
status Init_flight(flightlinklist &F)
{
	F = new flightnode;
	if (!F)  exit(0);
	F->next = NULL;
	return OK;
}
//初始化一个空的订单链表
status Init_passenger(passengerlinklist &P)
{
	P = new passengernode;
	P->head = new passengernode;
	P->rear = P->head;
	P->rear->order_num = 0;
	P->head->next = NULL;
	return OK;
}


//保存航班链表
void save_flight(flightlinklist F)
{
	FILE* fp = fopen("flightList.txt", "wb");
	if (fp == NULL)
		return;
	flightnode *temp = F->next;
	while (temp != NULL)
	{
		fwrite(temp, sizeof(flightnode), 1, fp);
		temp = temp->next;
	}
	fclose(fp);
}
//加载航班链表
void load_flight(flightlinklist &F)
{
	flightlinklist pt = F;
	FILE* fp = fopen("flightList.txt", "rb");
	if (fp == NULL)
		return;
	flightnode *q;
	int n;
	while (!feof(fp))
	{
		q = new flightnode;
		n = fread(q, sizeof(flightnode), 1, fp);
		if (n != 1)
			break;
		pt->next = q;
		pt = q;
	}
	pt->next = NULL;
	fclose(fp);
}
//保存订单信息链表
void save_passenger(passengerlinklist PList)
{
	FILE* fp = fopen("passenger.txt", "wb");
	if (fp == NULL)
		return;
	passengernode *temp = PList->head->next;
	while (temp != NULL)
	{
		fwrite(temp, sizeof(passengernode), 1, fp);
		temp = temp->next;
	}
	fclose(fp);
}
//加载订单信息链表
void load_passenger(passengerlinklist PList)
{
	FILE* fp = fopen("passenger.txt", "rb");
	if (fp == NULL)
		return;
	passengerlinklist q;
	int n;
	while (!feof(fp))
	{
		q = new passengernode;
		n = fread(q, sizeof(passengernode), 1, fp);
		if (n != 1)
			break;
		PList->rear->next = q;
		PList->rear = q;
	}
	PList->rear->next = NULL;
	fclose(fp);
}


//插入一个航班类的结点
void insert_flight(flightlinklist &F, char* flight_num, char* start_place, char* end_place, char* start_time, char* end_time, int left, float price, float price_discount, int isFull)
{
	flightnode *q;
	flightnode *p = F;
	q = new flightnode;
	strcpy(q->flight_num, flight_num);
	strcpy(q->start_place, start_place);
	strcpy(q->end_place, end_place);
	strcpy(q->start_time, start_time);
	strcpy(q->end_time, end_time);
	q->left = left;
	q->price = price;
	q->price_discount = price_discount;
	q->isFull = isFull;

	q->next = p->next;//
	p->next = q;
	p = p->next;
}
//插入一个订单结点
void insert_passengerList(flightlinklist &F, passengerlinklist &PList, char *name, char *ID_num, char *flight_num, int ticket_num)
{
	flightnode *p = F->next;
	for (; p != NULL; p = p->next)
		if (strcmp(p->flight_num, flight_num) == 0)
		{
			p->left = p->left - ticket_num;
			if (p->left == 0)
				p->isFull = 1;
		}
	passengernode *q = new passengernode;
	strcpy(q->name, name);
	strcpy(q->ID_num, ID_num);
	strcpy(q->flight_num, flight_num);
	q->ticket_num = ticket_num;
	q->next = NULL;

	PList->rear->next = q;
	q->order_num = PList->rear->order_num + 1;
	PList->rear = q;
}


//录入航班信息
void add_flight(flightlinklist &F)
{
	flightnode *p = F;
	char flight_num[10], start_time[10], end_time[10], start_place[20], end_place[20];
	int left, isFull, mark = 1;
	float price, price_discount;

	for (; p->next != NULL; p = p->next) {}
	while (mark == 1)
	{
		cout << "\t\t请输入你要增加的航班号:";
		cin >> flight_num;
		cout << "\t\t请输入出发地:";
		cin >> start_place;
		cout << "\t\t请输入目的地:";
		cin >> end_place;
		cout << "\t\t请输入起飞时间:";
		cin >> start_time;
		cout << "\t\t请输入抵达时间:";
		cin >> end_time;
		cout << "\t\t请输入票价:";
		cin >> price;
		cout << "\t\t请输入折扣:";
		cin >> price_discount;
		cout << "\t\t请输入剩余座位数:";
		cin >> left;
		cout << "\t\t请输入是否满仓(0表示没有满仓1以满仓):";
		cin >> isFull;
		insert_flight(p, flight_num, start_place, end_place, start_time, end_time, left, price, price_discount, isFull);
		cout << "\t\t增加航班" << flight_num << "成功!\n";
		cout << "\t\t是否继续录入航班信息(1表示继续录入;0表示停止录入).\n";
		cout << "\t\t请输入:";
		cin >> mark;
	}

}


//通过起始地址和抵达地址查找航班信息
int place_check(flightlinklist F, char *start_place, char *end_place)
{
	flightnode *p = F;
	int mark = 0;
	cout << setw(8) << "航班号" << setw(12) << "起飞城市" << setw(12) << "抵达城市" << setw(10) << "起飞时间" << setw(10) << "抵达时间" << setw(8) << "价格" << setw(6) << "折扣" << setw(10) << "是否满仓" << setw(8) << "空座数" << endl;
	for (; p != NULL; p = p->next)
	{
		if (strcmp(p->start_place, start_place) == 0 && strcmp(p->end_place, end_place) == 0)
		{
			cout << setw(8) << p->flight_num << setw(12) << p->start_place << setw(12) << p->end_place << setw(10) << p->start_time << setw(10) << p->end_time << setw(8) << p->price << setw(6) << p->price_discount << setw(10) << p->isFull << setw(8) << p->left << endl;
			mark = 1;
		}
	}
	if (mark == 0)
	{
		cout << "\t\t抱歉,没有从" << start_place << "到" << end_place << "的航班信息!\n";
		return 0;
	}
	return 1;
}
//通过航班号查找航班信息
int find_same_flight(flightlinklist F, char *flight_num)
{
	flightnode *t = F->next, *p = F->next;
	int mark = 0;
	cout << setw(8) << "航班号" << setw(12) << "起飞城市" << setw(12) << "抵达城市" << setw(10) << "起飞时间" << setw(10) << "抵达时间" << setw(8) << "价格" << setw(6) << "折扣" << setw(10) << "是否满仓" << setw(8) << "空座数" << endl;
	while (t != NULL && strcmp(t->flight_num, flight_num) != 0) t = t->next;
	while (p != NULL) {
		if ((strcmp(t->start_place, p->start_place) == 0) && (strcmp(t->end_place, p->end_place) == 0) && (strcmp(t->flight_num, p->flight_num) != 0))
		{
			cout << setw(8) << p->flight_num << setw(12) << p->start_place << setw(12) << p->end_place << setw(10) << p->start_time << setw(10) << p->end_time << setw(8) << p->price << setw(6) << p->price_discount << setw(10) << p->isFull << setw(8) << p->left << endl;
			mark = 1;
		}
		p = p->next;
	}
	if (mark == 0)
	{
		cout << "\t\t抱歉，没有可选的航班!\n";
		return 0;
	}
	return 1;
}
//订票
int book(flightlinklist &F, passengerlinklist &PList)
{
	char name[20];       //姓名
	char ID_num[20];     //证件号
	char flight_num[10]; //航班号
	char start_place[20];//起飞城市
	char end_place[20];  //抵达城市
	int ticket_num;      //订票数量
	int k;
	flightnode *p = F->next;
	cout << "\t\t请输入信息:\n";
	cout << "\t\t请输入起飞城市:";
	cin >> start_place;
	cout << "\t\t请输入抵达城市:";
	cin >> end_place;
	if (place_check(F, start_place, end_place) == 1)
	{
		cout << "\t\t航班号:";
		cin >> flight_num;
		while (flight_num == NULL) {
			cout << "航班号不能为空!\n";
			cout << "\t\t航班号:";
			cin >> flight_num;
		}
		while (p != NULL) {
			if (strcmp(p->flight_num, flight_num) == 0)
			{
				cout << "\t\t姓名:";
				cin >> name;
				cout << "\t\t证件号码:";
				cin >> ID_num;
				cout << "\t\t订票数量:";
				cin >> ticket_num;
				while (name == NULL) {
					cout << "客户姓名不能为空!\n";
					cout << "\t\t姓名:";
					cin >> name;
				}
				while (ID_num == NULL) {
					cout << "客户证件号码不能为空!\n";
					cout << "\t\t证件号码:";
					cin >> ID_num;
				}
				while (ticket_num == 0) {
					cout << "客户订票数量不能为空!\n";
					cout << "\t\t订票数量:";
					cin >> ticket_num;
				}
				if (p->left > 0 && p->left >= ticket_num)
				{
					insert_passengerList(F, PList, name, ID_num, flight_num, ticket_num);
					cout << "\t\t你应付金额￥" << p->price*p->price_discount* ticket_num << endl;
					getchar();
					cout << "\t\t恭喜您,订票成功!\n";
					return 1;
				}
				else
				{
					cout << "\t\t***很遗憾，该航班已满!***\n";
					cout << "\t\t***如选择其他航班请输入-----1\n\t\t***不选则输入-----0***\n";
					cout << "\t\t输入数字进行选择:";
					cin >> k;
					if (k == 1)
					{
						cout << "\t\t此航线上的其他航班有:\n";
						if (find_same_flight(F, flight_num) == 1)
						{
							cout << "\t\t请输入您选的航班:";
							cin >> flight_num;
							insert_passengerList(F, PList, name, ID_num, flight_num, ticket_num);
							cout << "\t\t恭喜您,订票成功!\n";
							return 1;
						}
					}
				}
			}
			else
				p = p->next;
		}
		if (p == NULL)
			cout << "\t\t对不起,您输入的航班不存在!\n";
	}
	return 0;
}


//通过名字和身份证号，删除订单
int delete_passenger(passengerlinklist &PList, flightlinklist &F, char *name, char *ID_num)
{
	passengernode *p, *pr = PList->head;
	p = pr->next;
	while (p != NULL)
	{
		if (strcmp(name, p->name) == 0 && strcmp(ID_num, p->ID_num) == 0)
		{
			for (flightnode *f = F->next; f != NULL; f = f->next)
				if (strcmp(p->flight_num, f->flight_num) == 0)
				{
					f->left = f->left + p->ticket_num;
					f->isFull = 0;
					break;
				}
			pr->next = p->next;
			free(p);
			cout << "\t\t顾客" << name << "," << ID_num << "退票成功!\n";
			return 1;
		}
		pr = pr->next;
		p = pr->next;
	}
	cout << "\t\t无此顾客,无法退票!\n";
	return 0;
}
//退票
void cancel(passengerlinklist &PList, flightlinklist &F)
{
	char name[20], ID_num[20];
	cout << "\t\t请输入你的姓名:";
	cin >> name;
	cout << "\t\t请输入你的证件号:";
	cin >> ID_num;
	delete_passenger(PList, F, name, ID_num);
}


//通过航班号判断该航班是否存在
int flight_num_check(flightlinklist F, char *flight_num)
{
	flightnode *p = F;
	cout << setw(8) << "航班号" << setw(12) << "起飞城市" << setw(12) << "抵达城市" << setw(10) << "起飞时间" << setw(10) << "抵达时间" << setw(8) << "价格" << setw(6) << "折扣" << setw(10) << "是否满仓" << setw(8) << "空座数" << endl;
	for (; p != NULL; p = p->next)
	{
		if (strcmp(p->flight_num, flight_num) == 0)
		{
			cout << setw(8) << p->flight_num << setw(12) << p->start_place << setw(12) << p->end_place << setw(10) << p->start_time << setw(10) << p->end_time << setw(8) << p->price << setw(6) << p->price_discount << setw(10) << p->isFull << setw(8) << p->left << endl;
			return 1;
		}
	}
	cout << "\t\t抱歉,没有航班号为" << flight_num << "的航班信息!\n";
	return 0;
}
//输出所有航班信息
void check_all_flight(flightlinklist F)
{
	flightnode *p = F;
	int m = 0;
	cout << setw(8) << "航班号" << setw(12) << "起飞城市" << setw(12) << "抵达城市" << setw(10) << "起飞时间" << setw(10) << "抵达时间" << setw(8) << "价格" << setw(6) << "折扣" << setw(10) << "是否满仓" << setw(8) << "空座数" << endl;
	for (; p != NULL; p = p->next)
	{
		cout << setw(8) << p->flight_num << setw(12) << p->start_place << setw(12) << p->end_place << setw(10) << p->start_time << setw(10) << p->end_time << setw(8) << p->price << setw(6) << p->price_discount << setw(10) << p->isFull << setw(8) << p->left << endl;
		m = 1;
	}
	if (m == 0)
		cout << "\t\t航班信息为空!\n";
}
//查找
void flight_check(flightlinklist F)//选择使用哪一个函数进行查询
{
	flightnode *p = F->next;
	char flight_num[10], start_place[20], end_place[20];
	char a;
	cout << "\t\t请选择航班查询方式:\n";
	cout << "\t\t1表示按航班号进行查询;\n";
	cout << "\t\t2表示按起飞抵达城市进行查询;\n";
	cout << "\t\t3表示浏览全部航班信息.\n\t\t请选择输入:";
	cin >> a;
	if (a == '1')
	{
		cout << "\t\t请输入航班号:";
		cin >> flight_num;
		flight_num_check(p, flight_num);
	}
	else if (a == '2')
	{
		cout << "\t\t请输入起飞城市:";
		cin >> start_place;
		cout << "\t\t请输入抵达城市:";
		cin >> end_place;
		place_check(p, start_place, end_place);
	}
	else if (a == '3')
		check_all_flight(p);
	else
		return;
}


//查找订单，通过名字与身份证号查找
int ID_name_check(passengerlinklist PList, char *name, char *ID_num)
{
	passengernode *p = PList->head->next;
	int mark = 0;
	cout << setw(8) << "订单号" << setw(10) << "姓名" << setw(10) << "证件号" << setw(10) << "航班号" << setw(10) << "订票数量" << endl;
	for (; p != NULL; p = p->next)
	{
		if (strcmp(p->ID_num, ID_num) == 0 && strcmp(p->name, name) == 0)
		{
			cout << setw(8) << p->order_num << setw(10) << p->name << setw(10) << p->ID_num << setw(10) << p->flight_num << setw(10) << p->ticket_num << endl;
			mark = 1;
		}
	}
	if (mark == 0)
	{
		cout << "\t\t抱歉,没有" << name << ID_num << "的订单信息!\n";
		return 0;
	}
	return 1;
}
//查找订单，通过订单号查询
int order_num_check(passengerlinklist PList, int order_num)
{
	passengernode *p = PList->head->next;
	cout << setw(8) << "订单号" << setw(10) << "姓名" << setw(10) << "证件号" << setw(10) << "航班号" << setw(10) << "订票数量" << endl;
	for (; p != NULL; p = p->next)
	{
		if (p->order_num == order_num)
		{
			cout << setw(8) << p->order_num << setw(10) << p->name << setw(10) << p->ID_num << setw(10) << p->flight_num << setw(10) << p->ticket_num << endl;
			return 1;
		}
	}
	cout << "\t\t抱歉,没有订单号为" << order_num << "的订单信息!\n";
	return 0;
}
//查询订单
void passenger_check(passengerlinklist PList)
{
	char name[20], ID_num[15];
	int order_num;
	char a;
	cout << "\t\t请选择订单查询方式:\n";
	cout << "\t\t1表示按客户姓名和证件号进行查询;\n";
	cout << "\t\t2表示按订单号进行查询;\n";

	cin >> a;
	if (a == '1')
	{
		cout << "\t\t请输入客户姓名:";
		cin >> name;
		cout << "\t\t请输入证件号:";
		cin >> ID_num;
		ID_name_check(PList, name, ID_num);
	}
	else if (a == '2')
	{
		cout << "\t\t请输入订单号:";
		cin >> order_num;
		order_num_check(PList, order_num);
	}

	else
		return;
}
//查询所有订单
void check_all_passenger(passengerlinklist PList)
{
	passengernode *p = PList->head->next;
	int m = 0;
	cout << setw(8) << "订单号" << setw(10) << "姓名" << setw(10) << "证件号" << setw(10) << "航班号" << setw(10) << "订票数量" << endl;
	for (; p != NULL; p = p->next)
	{
		cout << setw(8) << p->order_num << setw(10) << p->name << setw(10) << p->ID_num << setw(10) << p->flight_num << setw(10) << p->ticket_num << endl;
		m = 1;
	}
	if (m == 0)
		cout << "\t\t订单信息为空!\n";
}


//删除航班信息，通过航班号
void delete_flight(flightlinklist &F, passengerlinklist &PList)
{
	flightnode *p, *pr;
	passengernode *q, *qr;
	char flight_num[10];
	int mark = 1;
	qr = PList->head;
	q = qr->next;
	pr = F;
	p = pr->next;
	cout << "\t\t请输入你要删除的航班号:";
	cin >> flight_num;
	while (p != NULL)
	{
		if (strcmp(flight_num, p->flight_num) == 0)
		{
			pr->next = p->next;
			free(p);
			cout << "\t\t删除" << flight_num << "航班成功!\n";
			mark = 0;
			p = NULL;
		}
		if (pr->next != NULL)
		{
			pr = pr->next;
			p = pr->next;
		}
	}
	if (mark == 1)
		cout << "\t\t无此航班,无法删除!\n";
	else
	{
		while (q != NULL)
		{
			if (strcmp(flight_num, q->flight_num) == 0)
			{
				qr->next = q->next;
				free(q);
				q = NULL;
			}
			if (qr->next != NULL)
			{
				qr = qr->next;
				q = qr->next;
			}
		}
	}
}


//修改航班信息
void modify_flight(flightlinklist &F, passengerlinklist &PList)
{
	flightlinklist p = F->next;
	char flight_num[10], start_time[10], end_time[10];
	char a;
	cout << "\t\t************航线信息修改*************\n";
	cout << "\t\t*************************************\n";
	cout << "\t\t*         增加航班-------1          *\n";
	cout << "\t\t*         删除航班-------2          *\n";
	cout << "\t\t*         修改航班时间---3          *\n";
	cout << "\t\t*************************************\n";
	cout << "\t\t请选择:";
	cin >> a;
	if (a == '1')
		add_flight(F);
	else if (a == '2')
		delete_flight(F, PList);
	else if (a == '3')
	{
		cout << "\t\t请输入要修改的航班的航班号:";
		cin >> flight_num;
		if (flight_num_check(p, flight_num) == 1)
		{
			cout << "\t\t请输入修改后的起飞时间:";
			cin >> start_time;
			cout << "\t\t请输入修改后的抵达时间:";
			cin >> end_time;
			for (; p != NULL; p = p->next)
				if (strcmp(flight_num, p->flight_num) == 0)
				{
					strcpy(p->start_time, start_time);
					strcpy(p->end_time, end_time);
					cout << "\t\t航班%s时间修改成功!\n", flight_num;
				}
		}
	}
	else
		return;
}


//菜单界面
void menu()
{
	cout << "\t\t************飞机订票系统菜单*************\n";
	cout << "\t\t*****************************************\n";
	cout << "\t\t*         录入信息----------1           *\n";
	cout << "\t\t*         订    票----------2           *\n";
	cout << "\t\t*         退    票----------3           *\n";
	cout << "\t\t*         查询航班----------4           *\n";
	cout << "\t\t*         查询订单----------5           *\n";
	cout << "\t\t*         修改航线----------6           *\n";
	cout << "\t\t*         查询所有----------7           *\n";
	cout << "\t\t*         保存退出----------0           *\n";
	cout << "\t\t*****************************************\n";
	cout << "\t\t请选择服务:";
}

//系统运行
void run(flightlinklist flight, passengerlinklist PList)
{
	char choice;
	while (1)
	{
		menu();
		choice = getchar();
		cout << choice << endl;
		system("cls");
		switch (choice)
		{
		case '1':
			add_flight(flight);
			//add_flighten(flight);
			system("pause");
			getchar();
			system("cls");
			break;
		case '2':
			book(flight, PList);
			system("pause");
			getchar();
			system("cls");
			break;
		case '3':
			cancel(PList, flight);
			system("pause");
			getchar();
			system("cls");
			break;
		case '4':
			flight_check(flight);
			system("pause");
			getchar();
			system("cls");
			break;
		case '5':
			passenger_check(PList);
			system("pause");
			getchar();
			system("cls");
			break;
		case '6':
			modify_flight(flight, PList);
			system("pause");
			getchar();
			system("cls");
			break;
		case '7':
			check_all_passenger(PList);
			system("pause");
			break;
		case '0':
			cout << "\t\t再见!\n";
			break;
		}
		if (choice == '0')
		{
			break;
		}
	}
}

//主函数
int  main()
{
	flightlinklist flight;
	passengerlinklist PList;
	char choice;
	if (!Init_flight(flight))
	{
		cout << "航班信息初始化失败！";
	}
	if (!Init_passenger(PList))
	{
		cout << "订单信息初始化失败！";
	}

	load_flight(flight);
	load_passenger(PList);

	run(flight, PList);

	save_flight(flight);
	save_passenger(PList);
	return 0;
}