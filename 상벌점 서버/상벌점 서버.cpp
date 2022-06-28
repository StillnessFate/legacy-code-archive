// 상벌점 서버.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <windows.h>
#include <process.h> 
#include <Shlwapi.h> 
#include <io.h>

#pragma warning(disable:4996)

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Shlwapi.lib")

#define MAX_CLNT 256
#define BUF_SIZE 100

void SendMsg(char* msg, int len);
unsigned WINAPI HandleClnt(void* arg);
BOOL disconnect_(SOCKET sock, FILE* fp = NULL);
BOOL d_save();
BOOL d_load();
BOOL u_load();
BOOL find_f(int Year, int Month, SOCKET sock);

HANDLE hMutex;
SOCKET clntSocks[MAX_CLNT];
int clntCnt = 0;

typedef struct node {
	//struct GAH_Client* previous;
	char name[20];
	int num;
	int score;
	node* next;
} node;
node* nodes = NULL;

typedef struct u_node {
	//struct GAH_Client* previous;
	char name[50];
	char id[50];
	char pw[50];
	u_node* next;
} u_node;
u_node* u_nodes = NULL;

BOOL new_node(char* name, int num, int score)
{
	WaitForSingleObject(hMutex, INFINITE);

	if (nodes == NULL)
	{
		nodes = new node;
		ZeroMemory(nodes, sizeof(node));
		strcpy(nodes->name, name);
		nodes->num = num;
		nodes->score = score;
	}
	else
	{
		node* temp = nodes;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = new node;
		ZeroMemory(temp->next, sizeof(node));
		strcpy(temp->next->name, name);
		temp->next->num = num;
		temp->next->score = score;
	}
	ReleaseMutex(hMutex);
	return TRUE;
}

BOOL new_u_node(char* name, char* id, char* pw)
{
	WaitForSingleObject(hMutex, INFINITE);

	if (u_nodes == NULL)
	{
		u_nodes = new u_node;
		ZeroMemory(u_nodes, sizeof(u_node));
		strcpy(u_nodes->name, name);
		strcpy(u_nodes->id, id);
		strcpy(u_nodes->pw, pw);
	}
	else
	{
		u_node* temp = u_nodes;
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = new u_node;
		ZeroMemory(temp->next, sizeof(u_node));
		strcpy(temp->next->name, name);
		strcpy(temp->next->id, id);
		strcpy(temp->next->pw, pw);
	}
	ReleaseMutex(hMutex);
	return TRUE;
}

node* Find_node(int num)
{
	node* temp = nodes;
	WaitForSingleObject(hMutex, INFINITE);
	if (nodes == NULL)
	{
		goto ERROR_GO;
	}
	else
	{
		while (temp)
		{
			if (temp->num == num)
				break;
			temp = temp->next;
		}
		if (!temp)
			goto ERROR_GO;
	}
	ReleaseMutex(hMutex);
	return temp;

ERROR_GO:
	ReleaseMutex(hMutex);
	return NULL;
}
node* Find_node(char* name)
{
	node* temp = nodes;
	//WaitForSingleObject(hMutex, INFINITE);
	if (nodes == NULL)
	{
		goto ERROR_GO;
	}
	else
	{
		while (temp)
		{
			if (!strcmp(temp->name, name))
				break;
			temp = temp->next;
		}
		if (!temp)
			goto ERROR_GO;
	}
	//ReleaseMutex(hMutex);
	return temp;

ERROR_GO:
	//ReleaseMutex(hMutex);
	return NULL;
}

u_node* Find_u_node(char* id, char* pw)
{
	u_node* temp = u_nodes;
	//WaitForSingleObject(hMutex, INFINITE);
	if (u_nodes == NULL)
	{
		goto ERROR_GO;
	}
	else
	{
		while (temp)
		{
			if (!strcmp(temp->id, id) && !strcmp(temp->pw, pw))
				break;
			temp = temp->next;
		}
		if (!temp)
			goto ERROR_GO;
	}
	//ReleaseMutex(hMutex);
	return temp;

ERROR_GO:
	//ReleaseMutex(hMutex);
	return NULL;
}

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSz;
	HANDLE  hThread;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup() error!");
		exit(1);
	}

	hMutex = CreateMutex(NULL, FALSE, NULL);
	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(3388);

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		printf("bind() error");
		exit(1);
	}
	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		printf("listen() error");
		exit(1);
	}

	printf("서버 작동중...\n");
	new_u_node("관리자", "admin", "1234");
	//new_node("강민석", 1333,0);
	//new_node("강문석", 1633,0);
	d_load();
	u_load();
	while (1)
	{
		clntAdrSz = sizeof(clntAdr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz);

		WaitForSingleObject(hMutex, INFINITE);
		clntSocks[clntCnt++] = hClntSock;
		ReleaseMutex(hMutex);

		hThread =
			(HANDLE)_beginthreadex(NULL, 0, HandleClnt, (void*)&hClntSock, 0, NULL);
		printf("Connected client IP: %s \n", inet_ntoa(clntAdr.sin_addr));
	}
	closesocket(hServSock);
	WSACleanup();

	return 0;
}

unsigned WINAPI HandleClnt(void* arg)
{
	SOCKET hClntSock = *((SOCKET*)arg);
	char temp = 0;
	char name[20] = { 0 };
	int num = 0;
	char msg[BUF_SIZE];

	int option = TRUE;               //네이글 알고리즘 on/off
	int rt = setsockopt(hClntSock,             //해당 소켓
		IPPROTO_TCP,          //소켓의 레벨
		TCP_NODELAY,          //설정 옵션
		(const char*)&option, // 옵션 포인터
		sizeof(option));      //옵션 크기
	if (rt != 0)
		printf("설정 오류!");
	while (true)
	{
		if (recv(hClntSock, &temp, 1, 0) < 0)
		{
			disconnect_(hClntSock);
			return 0;
		}
		if (temp == 0)
		{
			if (recv(hClntSock, (char*)&num, 4, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			if (Find_node(num) == NULL)
			{
				num = -1;
				if (send(hClntSock, (char*)&num, 4, 0) < 0)
				{
					disconnect_(hClntSock);
					return 0;
				}
				continue;
			}
			if (send(hClntSock, (char*)&num, 4, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			//printf("이름 %s", Find_node(num)->name);
			temp = strlen(Find_node(num)->name);

			if (send(hClntSock, &temp, 1, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			if (send(hClntSock, Find_node(num)->name, temp, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			if (send(hClntSock, (char*)&Find_node(num)->score, 4, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
		}
		else if (temp == 1)
		{
			if (recv(hClntSock, &temp, 1, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			if (recv(hClntSock, name, temp, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			name[temp] = 0;

			if (Find_node(name) == NULL)
			{
				num = -1;
				if (send(hClntSock, (char*)&num, 4, 0) < 0)
				{
					disconnect_(hClntSock);
					return 0;
				}
				continue;
			}
			if (send(hClntSock, (char*)&Find_node(name)->num, 4, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			temp = strlen(Find_node(name)->name);

			if (send(hClntSock, &temp, 1, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			if (send(hClntSock, Find_node(name)->name, temp, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			if (send(hClntSock, (char*)&Find_node(name)->score, 4, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
		}
		else if (temp == 2)
		{
			WaitForSingleObject(hMutex, INFINITE);
			int score = 0;
			if (recv(hClntSock, (char*)&num, 4, 0) < 0)
			{
				disconnect_(hClntSock);
				ReleaseMutex(hMutex);
				return 0;
			}
			if (Find_node(num) == NULL)
				continue;
			if (recv(hClntSock, (char*)&score, 4, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			Find_node(num)->score = score;
			ReleaseMutex(hMutex);
			d_save();
		}
		else if (temp == 3)
		{
			int score = 0;
			char r_type = 0;
			if (recv(hClntSock, &r_type, 1, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			if (r_type != 3)
				if (recv(hClntSock, (char*)&score, 4, 0) < 0)
				{
					disconnect_(hClntSock);
					return 0;
				}


			node* temp_n = nodes;
			if (nodes == NULL)
			{
				num = -1;
				if (send(hClntSock, (char*)&num, 4, 0) < 0)
				{
					disconnect_(hClntSock);
					return 0;
				}
				continue;
			}
			else
			{
				int check = 0;
				while (temp_n)
				{
					//printf("%s\n", temp_n->name);
					if ((r_type == 0 && score <= temp_n->score) || (r_type == 1 && score >= temp_n->score) || (r_type == 2 && score == temp_n->score) || r_type == 3)
					{

						if (send(hClntSock, (char*)&temp_n->num, 4, 0) < 0)
						{
							disconnect_(hClntSock);
							return 0;
						}
						temp = strlen(temp_n->name);

						if (send(hClntSock, &temp, 1, 0) < 0)
						{
							disconnect_(hClntSock);
							return 0;
						}
						if (send(hClntSock, temp_n->name, temp, 0) < 0)
						{
							disconnect_(hClntSock);
							return 0;
						}
						if (send(hClntSock, (char*)&temp_n->score, 4, 0) < 0)
						{
							disconnect_(hClntSock);
							return 0;
						}
						check = 1;
					}
					temp_n = temp_n->next;
				}
				if (check == 0)
				{
					num = -1;
					if (send(hClntSock, (char*)&num, 4, 0) < 0)
					{
						disconnect_(hClntSock);
						return 0;
					}
					continue;
				}
				else
				{
					num = -2;
					if (send(hClntSock, (char*)&num, 4, 0) < 0)
					{
						disconnect_(hClntSock);
						return 0;
					}
					continue;
				}
			}
		}
		else if (temp == 4)
		{
			char buf[30] = { 0 };
			char buff[1024] = { 0 };
			int Year = 0;
			int Month = 0;
			int Day = 0;
			int f_len = 0;

			if (recv(hClntSock, (char*)&Year, 4, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			if (recv(hClntSock, (char*)&Month, 4, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			if (recv(hClntSock, (char*)&Day, 4, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}


			sprintf(buf, "S\\%d-%d-%d.txt", Year, Month, Day);
			//printf("요청:%s\n", buf);
			FILE* fp;
			fp = fopen(buf, "r");
			if (fp == NULL)
			{
				//printf("없음:%s\n", buf);
				f_len = 0;
				if (send(hClntSock, (char*)&f_len, 4, 0) < 0)
				{
					disconnect_(hClntSock);
					return 0;
				}
				continue;
			}
			fseek(fp, 0, SEEK_END);
			f_len = ftell(fp);
			if (f_len > 1024)
				f_len = 0;
			//printf("길이:%d\n", f_len);
			//Sleep(2000);
			if (send(hClntSock, (char*)&f_len, 4, 0) < 0)
			{
				disconnect_(hClntSock, fp);
				return 0;
			}
			fseek(fp, 0, SEEK_SET);
			if (f_len)
			{

				fread(buff, 1, f_len, fp);
				if (send(hClntSock, buff, f_len, 0) < 0)
				{
					disconnect_(hClntSock, fp);
					return 0;
				}
				//printf("보냄:%s 길이:%d\n", buff, f_len);
			}
			fclose(fp);
		}
		else if (temp == 5)
		{
			int Year = 0;
			int Month = 0;

			if (recv(hClntSock, (char*)&Year, 4, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			if (recv(hClntSock, (char*)&Month, 4, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}

			if (PathFileExists("S\\") == FALSE)
			{
				CreateDirectory("S\\", NULL);
				temp = 0;
				if (send(hClntSock, &temp, 1, 0) < 0)
				{
					disconnect_(hClntSock);
					return 0;
				}
				continue;
			}
			if (find_f(Year, Month, hClntSock) == FALSE)
			{
				temp = 0;
				if (send(hClntSock, &temp, 1, 0) < 0)
				{
					disconnect_(hClntSock);
					return 0;
				}
				continue;
			}
		}
		else if (temp == 6)
		{
			char buf[30] = { 0 };
			char buff[1024] = { 0 };
			int Year = 0;
			int Month = 0;
			int Day = 0;
			int f_len = 0;

			if (recv(hClntSock, (char*)&Year, 4, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			if (recv(hClntSock, (char*)&Month, 4, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			if (recv(hClntSock, (char*)&Day, 4, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			if (recv(hClntSock, (char*)&f_len, 4, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			sprintf(buf, "S\\%d-%d-%d.txt", Year, Month, Day);
			if (1024 < f_len)
				continue;
			else if (f_len == 0)
			{
				remove(buf);
				continue;
			}

			if (recv(hClntSock, buff, f_len, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}

			if (PathFileExists("S\\") == FALSE)
				CreateDirectory("S\\", NULL);

			FILE* fp;
			fp = fopen(buf, "w");
			if (fp == NULL)
				continue;

			fwrite(buff, 1, f_len, fp);
			fclose(fp);
		}
		else if (temp == 7)
		{
			char id[50] = { 0 };
			char pw[50] = { 0 };
			if (recv(hClntSock, &temp, 1, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			if (recv(hClntSock, id, temp, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			id[temp] = 0;

			if (recv(hClntSock, &temp, 1, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			if (recv(hClntSock, pw, temp, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			pw[temp] = 0;
			if (Find_u_node(id, pw) == NULL)
			{
				temp = 0;
				if (send(hClntSock, &temp, 1, 0) < 0)
				{
					disconnect_(hClntSock);
					return 0;
				}
				continue;
			}
			printf("로그인 : %s\n", id);
			temp = strlen(Find_u_node(id, pw)->name);
			if (send(hClntSock, &temp, 1, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			if (send(hClntSock, Find_u_node(id, pw)->name, temp, 0) < 0)
			{
				disconnect_(hClntSock);
				return 0;
			}
			//printf("로그인 완료\n", id);
		}
	}

	/*WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i<clntCnt; i++)   // remove disconnected client
	{
		if (hClntSock == clntSocks[i])
		{
			while (i++<clntCnt - 1)
				clntSocks[i] = clntSocks[i + 1];
			break;
		}
	}
	clntCnt--;
	ReleaseMutex(hMutex);
	closesocket(hClntSock);*/
	return 0;
}

void SendMsg(char* msg, int len)   // send to all
{
	int i;
	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < clntCnt; i++)
		send(clntSocks[i], msg, len, 0);

	ReleaseMutex(hMutex);
}

BOOL disconnect_(SOCKET sock, FILE* fp)
{
	closesocket(sock);
	sock = NULL;
	if (fp)
		fclose(fp);
	printf("disconnect\n");
	return TRUE;
}

BOOL d_save()
{
	WaitForSingleObject(hMutex, INFINITE);
	FILE* fp;
	fp = fopen("D.txt", "w");
	node* temp_n = nodes;
	if (nodes == NULL)
	{
		ReleaseMutex(hMutex);
		return 0;
	}
	else
	{
		while (temp_n)
		{
			fprintf(fp, "%d %s %d\n", temp_n->num, temp_n->name, temp_n->score);
			temp_n = temp_n->next;
		}
	}
	fclose(fp);
	ReleaseMutex(hMutex);
	return TRUE;
}

BOOL d_load()
{
	WaitForSingleObject(hMutex, INFINITE);
	char buf[256];
	char name[20];
	int num;
	int score;

	FILE* fp;
	fp = fopen("D.txt", "r");
	if (fp)
	{
		while (true)
		{

			if (fgets(buf, sizeof(buf), fp) == NULL)
				break;
			if (*buf == 0)
				continue;
			sscanf(buf, "%d %s %d", &num, name, &score);
			new_node(name, num, score);
		}
	}
	fclose(fp);
	ReleaseMutex(hMutex);
	return TRUE;
}

BOOL u_load()
{
	WaitForSingleObject(hMutex, INFINITE);
	char buf[256];
	char name[50];
	char id[50];
	char pw[50];
	int num;
	int score;
	FILE* fp;
	fp = fopen("user.txt", "r");
	if (fp)
	{
		while (true)
		{

			if (fgets(buf, sizeof(buf), fp) == NULL)
				break;
			if (*buf == 0)
				continue;
			sscanf(buf, "%s %s %s", name, id, pw);
			//printf("%s %s %s\n", name, id, pw);
			new_u_node(name, id, pw);
		}
	}
	fclose(fp);
	ReleaseMutex(hMutex);
	return TRUE;
}

BOOL find_f(int Year, int Month, SOCKET sock)
{
	char temp = 0;
	char buf[20] = { 0 };
	char* chr;
	sprintf(buf, "%d-%d-", Year, Month);

	_finddata_t fd;
	long handle;
	int result = 1;
	handle = _findfirst("S\\*.txt", &fd);  //현재 폴더 내 모든 파일을 찾는다.

	if (handle == -1)
	{
		//printf("There were no files.\n");
		return FALSE;
	}

	while (result != -1)
	{
		if (!strncmp(buf, fd.name, strlen(buf)))
		{
			chr = strrchr(fd.name, '.');
			*chr = 0;

			temp = atoi(&fd.name[strlen(buf)]);
			if (send(sock, &temp, 1, 0) < 0)
			{
				disconnect_(sock);
				return 0;
			}
		}
		//printf("File: %s\n", fd.name);
		result = _findnext(handle, &fd);
	}
	temp = 0;
	if (send(sock, &temp, 1, 0) < 0)
	{
		disconnect_(sock);
		return 0;
	}
	_findclose(handle);
	return TRUE;
}