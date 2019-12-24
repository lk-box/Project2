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


//����һ���������ͽṹ��
typedef struct flightnode
{
	char flight_num[10]; //�����
	char start_time[10]; //���ʱ��
	char end_time[10];   //�ִ�ʱ��
	char start_place[20];//��ɳ���
	char end_place[20];  //�ִ����
	int left;            //������
	float price;         //Ʊ��
	float price_discount;//Ʊ���ۿ�
	int isFull;          //�����Ƿ�����
	struct flightnode *next;
}flightnode, *flightlinklist;
//����һ���������ͽṹ��
typedef struct passengernode
{
	char name[20];      //����
	char ID_num[20];    //֤����
	char flight_num[10];//�����
	int order_num;      //������
	int ticket_num;     //��Ʊ����
	struct passengernode *next;
	passengernode *head;
	passengernode *rear;
}passengernode, *passengerlinklist;


//��ʼ��һ���յĺ�������
status Init_flight(flightlinklist &F)
{
	F = new flightnode;
	if (!F)  exit(0);
	F->next = NULL;
	return OK;
}
//��ʼ��һ���յĶ�������
status Init_passenger(passengerlinklist &P)
{
	P = new passengernode;
	P->head = new passengernode;
	P->rear = P->head;
	P->rear->order_num = 0;
	P->head->next = NULL;
	return OK;
}


//���溽������
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
//���غ�������
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
//���涩����Ϣ����
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
//���ض�����Ϣ����
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


//����һ��������Ľ��
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
//����һ���������
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


//¼�뺽����Ϣ
void add_flight(flightlinklist &F)
{
	flightnode *p = F;
	char flight_num[10], start_time[10], end_time[10], start_place[20], end_place[20];
	int left, isFull, mark = 1;
	float price, price_discount;

	for (; p->next != NULL; p = p->next) {}
	while (mark == 1)
	{
		cout << "\t\t��������Ҫ���ӵĺ����:";
		cin >> flight_num;
		cout << "\t\t�����������:";
		cin >> start_place;
		cout << "\t\t������Ŀ�ĵ�:";
		cin >> end_place;
		cout << "\t\t���������ʱ��:";
		cin >> start_time;
		cout << "\t\t������ִ�ʱ��:";
		cin >> end_time;
		cout << "\t\t������Ʊ��:";
		cin >> price;
		cout << "\t\t�������ۿ�:";
		cin >> price_discount;
		cout << "\t\t������ʣ����λ��:";
		cin >> left;
		cout << "\t\t�������Ƿ�����(0��ʾû������1������):";
		cin >> isFull;
		insert_flight(p, flight_num, start_place, end_place, start_time, end_time, left, price, price_discount, isFull);
		cout << "\t\t���Ӻ���" << flight_num << "�ɹ�!\n";
		cout << "\t\t�Ƿ����¼�뺽����Ϣ(1��ʾ����¼��;0��ʾֹͣ¼��).\n";
		cout << "\t\t������:";
		cin >> mark;
	}

}


//ͨ����ʼ��ַ�͵ִ��ַ���Һ�����Ϣ
int place_check(flightlinklist F, char *start_place, char *end_place)
{
	flightnode *p = F;
	int mark = 0;
	cout << setw(8) << "�����" << setw(12) << "��ɳ���" << setw(12) << "�ִ����" << setw(10) << "���ʱ��" << setw(10) << "�ִ�ʱ��" << setw(8) << "�۸�" << setw(6) << "�ۿ�" << setw(10) << "�Ƿ�����" << setw(8) << "������" << endl;
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
		cout << "\t\t��Ǹ,û�д�" << start_place << "��" << end_place << "�ĺ�����Ϣ!\n";
		return 0;
	}
	return 1;
}
//ͨ������Ų��Һ�����Ϣ
int find_same_flight(flightlinklist F, char *flight_num)
{
	flightnode *t = F->next, *p = F->next;
	int mark = 0;
	cout << setw(8) << "�����" << setw(12) << "��ɳ���" << setw(12) << "�ִ����" << setw(10) << "���ʱ��" << setw(10) << "�ִ�ʱ��" << setw(8) << "�۸�" << setw(6) << "�ۿ�" << setw(10) << "�Ƿ�����" << setw(8) << "������" << endl;
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
		cout << "\t\t��Ǹ��û�п�ѡ�ĺ���!\n";
		return 0;
	}
	return 1;
}
//��Ʊ
int book(flightlinklist &F, passengerlinklist &PList)
{
	char name[20];       //����
	char ID_num[20];     //֤����
	char flight_num[10]; //�����
	char start_place[20];//��ɳ���
	char end_place[20];  //�ִ����
	int ticket_num;      //��Ʊ����
	int k;
	flightnode *p = F->next;
	cout << "\t\t��������Ϣ:\n";
	cout << "\t\t��������ɳ���:";
	cin >> start_place;
	cout << "\t\t������ִ����:";
	cin >> end_place;
	if (place_check(F, start_place, end_place) == 1)
	{
		cout << "\t\t�����:";
		cin >> flight_num;
		while (flight_num == NULL) {
			cout << "����Ų���Ϊ��!\n";
			cout << "\t\t�����:";
			cin >> flight_num;
		}
		while (p != NULL) {
			if (strcmp(p->flight_num, flight_num) == 0)
			{
				cout << "\t\t����:";
				cin >> name;
				cout << "\t\t֤������:";
				cin >> ID_num;
				cout << "\t\t��Ʊ����:";
				cin >> ticket_num;
				while (name == NULL) {
					cout << "�ͻ���������Ϊ��!\n";
					cout << "\t\t����:";
					cin >> name;
				}
				while (ID_num == NULL) {
					cout << "�ͻ�֤�����벻��Ϊ��!\n";
					cout << "\t\t֤������:";
					cin >> ID_num;
				}
				while (ticket_num == 0) {
					cout << "�ͻ���Ʊ��������Ϊ��!\n";
					cout << "\t\t��Ʊ����:";
					cin >> ticket_num;
				}
				if (p->left > 0 && p->left >= ticket_num)
				{
					insert_passengerList(F, PList, name, ID_num, flight_num, ticket_num);
					cout << "\t\t��Ӧ����" << p->price*p->price_discount* ticket_num << endl;
					getchar();
					cout << "\t\t��ϲ��,��Ʊ�ɹ�!\n";
					return 1;
				}
				else
				{
					cout << "\t\t***���ź����ú�������!***\n";
					cout << "\t\t***��ѡ����������������-----1\n\t\t***��ѡ������-----0***\n";
					cout << "\t\t�������ֽ���ѡ��:";
					cin >> k;
					if (k == 1)
					{
						cout << "\t\t�˺����ϵ�����������:\n";
						if (find_same_flight(F, flight_num) == 1)
						{
							cout << "\t\t��������ѡ�ĺ���:";
							cin >> flight_num;
							insert_passengerList(F, PList, name, ID_num, flight_num, ticket_num);
							cout << "\t\t��ϲ��,��Ʊ�ɹ�!\n";
							return 1;
						}
					}
				}
			}
			else
				p = p->next;
		}
		if (p == NULL)
			cout << "\t\t�Բ���,������ĺ��಻����!\n";
	}
	return 0;
}


//ͨ�����ֺ����֤�ţ�ɾ������
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
			cout << "\t\t�˿�" << name << "," << ID_num << "��Ʊ�ɹ�!\n";
			return 1;
		}
		pr = pr->next;
		p = pr->next;
	}
	cout << "\t\t�޴˹˿�,�޷���Ʊ!\n";
	return 0;
}
//��Ʊ
void cancel(passengerlinklist &PList, flightlinklist &F)
{
	char name[20], ID_num[20];
	cout << "\t\t�������������:";
	cin >> name;
	cout << "\t\t���������֤����:";
	cin >> ID_num;
	delete_passenger(PList, F, name, ID_num);
}


//ͨ��������жϸú����Ƿ����
int flight_num_check(flightlinklist F, char *flight_num)
{
	flightnode *p = F;
	cout << setw(8) << "�����" << setw(12) << "��ɳ���" << setw(12) << "�ִ����" << setw(10) << "���ʱ��" << setw(10) << "�ִ�ʱ��" << setw(8) << "�۸�" << setw(6) << "�ۿ�" << setw(10) << "�Ƿ�����" << setw(8) << "������" << endl;
	for (; p != NULL; p = p->next)
	{
		if (strcmp(p->flight_num, flight_num) == 0)
		{
			cout << setw(8) << p->flight_num << setw(12) << p->start_place << setw(12) << p->end_place << setw(10) << p->start_time << setw(10) << p->end_time << setw(8) << p->price << setw(6) << p->price_discount << setw(10) << p->isFull << setw(8) << p->left << endl;
			return 1;
		}
	}
	cout << "\t\t��Ǹ,û�к����Ϊ" << flight_num << "�ĺ�����Ϣ!\n";
	return 0;
}
//������к�����Ϣ
void check_all_flight(flightlinklist F)
{
	flightnode *p = F;
	int m = 0;
	cout << setw(8) << "�����" << setw(12) << "��ɳ���" << setw(12) << "�ִ����" << setw(10) << "���ʱ��" << setw(10) << "�ִ�ʱ��" << setw(8) << "�۸�" << setw(6) << "�ۿ�" << setw(10) << "�Ƿ�����" << setw(8) << "������" << endl;
	for (; p != NULL; p = p->next)
	{
		cout << setw(8) << p->flight_num << setw(12) << p->start_place << setw(12) << p->end_place << setw(10) << p->start_time << setw(10) << p->end_time << setw(8) << p->price << setw(6) << p->price_discount << setw(10) << p->isFull << setw(8) << p->left << endl;
		m = 1;
	}
	if (m == 0)
		cout << "\t\t������ϢΪ��!\n";
}
//����
void flight_check(flightlinklist F)//ѡ��ʹ����һ���������в�ѯ
{
	flightnode *p = F->next;
	char flight_num[10], start_place[20], end_place[20];
	char a;
	cout << "\t\t��ѡ�񺽰��ѯ��ʽ:\n";
	cout << "\t\t1��ʾ������Ž��в�ѯ;\n";
	cout << "\t\t2��ʾ����ɵִ���н��в�ѯ;\n";
	cout << "\t\t3��ʾ���ȫ��������Ϣ.\n\t\t��ѡ������:";
	cin >> a;
	if (a == '1')
	{
		cout << "\t\t�����뺽���:";
		cin >> flight_num;
		flight_num_check(p, flight_num);
	}
	else if (a == '2')
	{
		cout << "\t\t��������ɳ���:";
		cin >> start_place;
		cout << "\t\t������ִ����:";
		cin >> end_place;
		place_check(p, start_place, end_place);
	}
	else if (a == '3')
		check_all_flight(p);
	else
		return;
}


//���Ҷ�����ͨ�����������֤�Ų���
int ID_name_check(passengerlinklist PList, char *name, char *ID_num)
{
	passengernode *p = PList->head->next;
	int mark = 0;
	cout << setw(8) << "������" << setw(10) << "����" << setw(10) << "֤����" << setw(10) << "�����" << setw(10) << "��Ʊ����" << endl;
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
		cout << "\t\t��Ǹ,û��" << name << ID_num << "�Ķ�����Ϣ!\n";
		return 0;
	}
	return 1;
}
//���Ҷ�����ͨ�������Ų�ѯ
int order_num_check(passengerlinklist PList, int order_num)
{
	passengernode *p = PList->head->next;
	cout << setw(8) << "������" << setw(10) << "����" << setw(10) << "֤����" << setw(10) << "�����" << setw(10) << "��Ʊ����" << endl;
	for (; p != NULL; p = p->next)
	{
		if (p->order_num == order_num)
		{
			cout << setw(8) << p->order_num << setw(10) << p->name << setw(10) << p->ID_num << setw(10) << p->flight_num << setw(10) << p->ticket_num << endl;
			return 1;
		}
	}
	cout << "\t\t��Ǹ,û�ж�����Ϊ" << order_num << "�Ķ�����Ϣ!\n";
	return 0;
}
//��ѯ����
void passenger_check(passengerlinklist PList)
{
	char name[20], ID_num[15];
	int order_num;
	char a;
	cout << "\t\t��ѡ�񶩵���ѯ��ʽ:\n";
	cout << "\t\t1��ʾ���ͻ�������֤���Ž��в�ѯ;\n";
	cout << "\t\t2��ʾ�������Ž��в�ѯ;\n";

	cin >> a;
	if (a == '1')
	{
		cout << "\t\t������ͻ�����:";
		cin >> name;
		cout << "\t\t������֤����:";
		cin >> ID_num;
		ID_name_check(PList, name, ID_num);
	}
	else if (a == '2')
	{
		cout << "\t\t�����붩����:";
		cin >> order_num;
		order_num_check(PList, order_num);
	}

	else
		return;
}
//��ѯ���ж���
void check_all_passenger(passengerlinklist PList)
{
	passengernode *p = PList->head->next;
	int m = 0;
	cout << setw(8) << "������" << setw(10) << "����" << setw(10) << "֤����" << setw(10) << "�����" << setw(10) << "��Ʊ����" << endl;
	for (; p != NULL; p = p->next)
	{
		cout << setw(8) << p->order_num << setw(10) << p->name << setw(10) << p->ID_num << setw(10) << p->flight_num << setw(10) << p->ticket_num << endl;
		m = 1;
	}
	if (m == 0)
		cout << "\t\t������ϢΪ��!\n";
}


//ɾ��������Ϣ��ͨ�������
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
	cout << "\t\t��������Ҫɾ���ĺ����:";
	cin >> flight_num;
	while (p != NULL)
	{
		if (strcmp(flight_num, p->flight_num) == 0)
		{
			pr->next = p->next;
			free(p);
			cout << "\t\tɾ��" << flight_num << "����ɹ�!\n";
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
		cout << "\t\t�޴˺���,�޷�ɾ��!\n";
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


//�޸ĺ�����Ϣ
void modify_flight(flightlinklist &F, passengerlinklist &PList)
{
	flightlinklist p = F->next;
	char flight_num[10], start_time[10], end_time[10];
	char a;
	cout << "\t\t************������Ϣ�޸�*************\n";
	cout << "\t\t*************************************\n";
	cout << "\t\t*         ���Ӻ���-------1          *\n";
	cout << "\t\t*         ɾ������-------2          *\n";
	cout << "\t\t*         �޸ĺ���ʱ��---3          *\n";
	cout << "\t\t*************************************\n";
	cout << "\t\t��ѡ��:";
	cin >> a;
	if (a == '1')
		add_flight(F);
	else if (a == '2')
		delete_flight(F, PList);
	else if (a == '3')
	{
		cout << "\t\t������Ҫ�޸ĵĺ���ĺ����:";
		cin >> flight_num;
		if (flight_num_check(p, flight_num) == 1)
		{
			cout << "\t\t�������޸ĺ�����ʱ��:";
			cin >> start_time;
			cout << "\t\t�������޸ĺ�ĵִ�ʱ��:";
			cin >> end_time;
			for (; p != NULL; p = p->next)
				if (strcmp(flight_num, p->flight_num) == 0)
				{
					strcpy(p->start_time, start_time);
					strcpy(p->end_time, end_time);
					cout << "\t\t����%sʱ���޸ĳɹ�!\n", flight_num;
				}
		}
	}
	else
		return;
}


//�˵�����
void menu()
{
	cout << "\t\t************�ɻ���Ʊϵͳ�˵�*************\n";
	cout << "\t\t*****************************************\n";
	cout << "\t\t*         ¼����Ϣ----------1           *\n";
	cout << "\t\t*         ��    Ʊ----------2           *\n";
	cout << "\t\t*         ��    Ʊ----------3           *\n";
	cout << "\t\t*         ��ѯ����----------4           *\n";
	cout << "\t\t*         ��ѯ����----------5           *\n";
	cout << "\t\t*         �޸ĺ���----------6           *\n";
	cout << "\t\t*         ��ѯ����----------7           *\n";
	cout << "\t\t*         �����˳�----------0           *\n";
	cout << "\t\t*****************************************\n";
	cout << "\t\t��ѡ�����:";
}

//ϵͳ����
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
			cout << "\t\t�ټ�!\n";
			break;
		}
		if (choice == '0')
		{
			break;
		}
	}
}

//������
int  main()
{
	flightlinklist flight;
	passengerlinklist PList;
	char choice;
	if (!Init_flight(flight))
	{
		cout << "������Ϣ��ʼ��ʧ�ܣ�";
	}
	if (!Init_passenger(PList))
	{
		cout << "������Ϣ��ʼ��ʧ�ܣ�";
	}

	load_flight(flight);
	load_passenger(PList);

	run(flight, PList);

	save_flight(flight);
	save_passenger(PList);
	return 0;
}