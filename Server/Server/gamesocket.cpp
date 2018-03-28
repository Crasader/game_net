#include "gamesocket.h"
#include "networkmgr.h"

gamesocket::gamesocket()
{
	port = 6000;
	this->init();
}


gamesocket::~gamesocket()
{
}


bool gamesocket::init()
{
	//��ʼ��WSA  
	sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return false;
	}

	//�����׽���  
	slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return false;
	}
	return true;
}

void gamesocket::DO()
{
	//��IP�Ͷ˿�  
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}

	//��ʼ����  
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return;
	}

	//ѭ����������  
	int nAddrlen = sizeof(remoteAddr);
	char revData[max_buff];
	while (true)
	{
		printf("\n�ȴ�����...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		//��������  
		int ret = recv(sClient, revData, max_buff, 0);
		if (ret > 0)
		{
			revData[ret] = 0x00;
			
			char* pb = ((char*)revData + len_head);

			unsigned char head[len_head];
			memset(head, 0, sizeof(head));
			for (int i = 0; i < len_head; i++)
			{
				head[i] = revData[i];
			}
			ushort uid = head[1] * 256 + head[0];
			process(uid, pb, ret - len_head);
		}
	}

	closesocket(slisten);
	WSACleanup();
}

//��������  
void gamesocket::Send(ushort uid, char* data,int len)
{
	char* pid = (char*)(&uid);
	char buff[max_buff];
	memset(buff, 0, sizeof(buff));
	memcpy(buff, pid, len_head);
	memcpy(buff + 2, data, len);
	send(sClient, buff, strlen(buff), 0);
	closesocket(sClient);
}


void gamesocket::process(ushort uid, char* pb,int len)
{
	cout << "uid is: " << uid << endl;
	networkmgr::Instance()->process(uid, pb, len);
}