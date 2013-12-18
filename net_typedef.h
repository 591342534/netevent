/*
 * net_typedef.h
 *
 *  Created on: 2013��11��4��
 *      Author: jimm
 */

#ifndef NET_TYPEDEF_H_
#define NET_TYPEDEF_H_

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#endif


#ifdef WIN32
typedef SOCKET		SocketFD;
enum
{
	enmInvalidSocketFD		= INVALID_SOCKET,		//��Ч�׽���
};
#else
typedef int32_t			SocketFD;
enum
{
	enmInvalidSocketFD		= -1,					//��Ч�׽���
};
#endif

typedef uint32_t		SessionID;

//�׽�������
typedef enum enmSessionType
{
	enmSessionType_Listen				= 0,		//�����׽���
	enmSessionType_Communicate			= 1,		//ͨ���׽���
} SessionType;

//�׽�������״̬
typedef enum enmSessionStatus
{
	enmSessionStatus_Closed				= 0,		//
	enmSessionStatus_Opened				= 1,
	enmSessionStatus_Connecting			= 2,
	enmSessionStatus_Connected			= 3,
	enmSessionStatus_Error				= 4,
	enmSessionStatus_Available			= 5,
} SessionStatus;

enum
{
	enmRecvBufferSize				= 0x00040000,		//���ջ�������С
	enmSendBufferSize				= 0x00040000,		//���ͻ�������С
};


enum
{
	enmMaxConnectCount					= 500,			//����������
	enmDefaultConnectionTimeout			= 1000,			//Ĭ�����ӳ�ʱʱ��
};


enum
{
	enmMaxMessageSize					= 0x40000,	//�����Ϣ����, 1024*256, 256k
	enmMaxIPAddressLength				= 20,	//IP��ַ��󳤶�
};


#endif /* NET_TYPEDEF_H_ */
