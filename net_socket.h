/*
 * net_socket.h
 *
 *  Created on: 2013��11��4��
 *      Author: jimm
 */

#ifndef NET_SOCKET_H_
#define NET_SOCKET_H_

#include "net_api.h"
#include "net_typedef.h"
#include "net_errordef.h"
#include "net_namespace.h"
#include "net_impl.h"
#include "net_timer.h"
#include "../common/common_cyclebuffer.h"

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

NETEVENT_NAMESPACE_BEGIN

class CNetHandler;

class CSocket: public IIOSession
{
public:
	CSocket();
	CSocket(CNetHandler *pNetHandler);
	virtual ~CSocket();

	virtual void Reset()
	{
		Clear();
	}

	//����socket�����Ҫ�������ݳ�Ա����������˺���
	virtual void Clear()
	{
		m_nSessionID = 0;
		m_nSocketFD = enmInvalidSocketFD;
		m_nSessionType = enmSessionType_Communicate;
		m_nSessionStatus = enmSessionStatus_Closed;
		memset(m_szClientAddress, 0, sizeof(m_szClientAddress));
		m_nPeerAddress = 0;
		m_nPeerPort = 0;
		m_nCreateTime = 0;
		m_nTotalRecvBytes = 0;
		m_nLastRecvTime = 0;
		m_nTotalSendBytes = 0;
		m_nLastSendTime = 0;
		m_pConnectTimer = NULL;
		m_pNetHandler = NULL;
		m_nIOEvents = 0;
	}
	//���ûỰID,ȫ��Ψһ
	virtual void SetSessionID(SessionID nID);
	//��ȡ�ỰID,ȫ��Ψһ
	virtual SessionID GetSessionID();
	//�����׽���������
	virtual void SetSocketFD(SocketFD nSocketFD);
	//��ȡ�׽���������
	virtual SocketFD GetSocketFD() const;
	//�����׽���״̬
	virtual void SetSessionStatus(SessionStatus nSocketStatus);
	//��ȡ�׽���״̬
	virtual SessionStatus GetSessionStatus() const;
	//���ûỰ����
	virtual void SetSessionType(SessionType nSocketType);
	//��ȡ�Ự����
	virtual SessionType GetSessionType();
	//����Զ�˵�ַ
	virtual void SetPeerAddress(const char *szAddress);
	//����Զ�˵�ַ
	virtual void SetPeerAddress(uint32_t nAddress);
	//��ȡԶ�˵�ַ
	virtual char *GetPeerAddressStr();
	//��ȡԶ�˵�ַ
	virtual uint32_t GetPeerAddress();
	//����Զ�˶˿�
	virtual void SetPeerPort(uint16_t nPort);
	//��ȡԶ�˶˿�
	virtual uint16_t GetPeerPort();
	//��ȡ���ص�ַ
	virtual uint32_t GetLocalAddress();
	//��ȡ���ض˿�
	virtual uint16_t GetLocalPort();

	virtual int32_t ReadEvent();

	virtual int32_t WriteEvent();

	virtual int32_t ErrorEvent();

	virtual int32_t Write(uint8_t *pBuf, int32_t nBufSize);

	virtual int32_t OpenSocket();
	//�ر��׽���
	virtual void CloseSocket(int32_t nCloseCode = 0);
	//�����˽�������
	virtual int32_t Connect(const char* szRemoteIP, uint16_t nPort);
	//��������
	virtual int32_t Recv(uint8_t *pBuffer, int32_t nSize, int32_t& nRecvBytes);
	//��������
	virtual int32_t Send(const uint8_t *pBuffer, const int32_t nLength, int32_t& nSendBytes);

	void SetCreateTime(time_t nCreateTime);

	void SetNetHandler(CNetHandler *pNetHandler);

	void SetConnectTimer(CConnectTimer *pTimer);

	CConnectTimer *GetSessionTimer();

	uint32_t GetIOEvents();

	void SetIOEvents(uint32_t nEvents);

	//��ʱ���¼�
	int32_t OnTimerEvent(CConnectTimer *pTimer);
protected:
	//���¼��ص�
	virtual int32_t OnRead(int32_t nErrorCode) = 0;
	//д�¼��ص�
	virtual int32_t OnWrite(int32_t nErrorCode) = 0;
	//�쳣�¼��ص�
	virtual int32_t OnError(int32_t nErrorCode) = 0;
	//�������ӻص�(���ش˺�����������������Щ������Ϣ��ʼ���Ĺ���)
	virtual int32_t OnAccept(SocketFD nAcceptFD, struct sockaddr_in stPeerAddress,
			socklen_t nPeerAddressLen);
	//���ӳɹ��ص�(���ش˺�����������������Щ������Ϣ��ʼ���Ĺ���)
	virtual int32_t OnConnected();
	//���ӳ�ʱ�ص�
	virtual int32_t OnConnectTimeout();
	//�Ͽ����ӻص�(���ش˺�����������������Щ��Դ���յĹ���)
	virtual int32_t OnDisconnect(int32_t nCloseCode);
	//���ͻ�����ʣ�������
	virtual int32_t SendRestData();
	//���Ծ����Ŭ����ȡ���������
	int32_t nRead(uint8_t *pBuffer, const int32_t nLength,bool &isConnClose);
	//���Ծ����Ŭ��д����д������
	int32_t nWrite(const uint8_t *pBuffer, const int32_t nLength);
	//�ı�epoll�¼�
	void ChangeWriteEvent();

private:
	//���ӳ�ʱ
	int32_t ConnectTimeout();
	//���ӳɹ�
	int32_t Connected();
	//���ӹر�
	int32_t Disconnected(int32_t nCloseCode);

protected:
	SessionID		m_nSessionID;				//ȫ��ΨһID
	SocketFD		m_nSocketFD;				//�׽���������
	SessionType		m_nSessionType;				//��ʾ�Ự�Ǽ������ͻ���ͨ������
	SessionStatus	m_nSessionStatus;			//�Ự״̬
	char			m_szClientAddress[enmMaxIPAddressLength];
	uint32_t		m_nPeerAddress;				//�Զ�ip
	uint16_t		m_nPeerPort;				//�Զ�port
	uint32_t		m_nLocalAddress;			//������ַ
	uint16_t		m_nLocalPort;				//�����˿�
	time_t			m_nCreateTime;				//���Ӵ���ʱ��
	time_t			m_nLastRecvTime;			//���һ�δӶԶ˽������ݵ�ʱ��
	time_t			m_nLastSendTime;			//���һ�η����Զ����ݵ�ʱ��
	int32_t			m_nTotalRecvBytes;			//�յ������ݴ�С
	int32_t			m_nTotalSendBytes;			//���͵����ݴ�С
//	CycleBuffer<enmRecvBufferSize>	m_stRecvBuffer;	//���ջ�����
	CycleBuffer<enmSendBufferSize>	m_stSendBuffer;	//���ͻ��棬���׽����쳣����ϵͳ���ͻ�����������£����ݽ��ᱻ�Ž��˻���

	CConnectTimer	*m_pConnectTimer;
	CNetHandler		*m_pNetHandler;
	uint32_t		m_nIOEvents;
};

NETEVENT_NAMESPACE_END


#endif /* NET_SOCKET_H_ */
