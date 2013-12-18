/*
 * net_connector.cpp
 *
 *  Created on: 2013��12��16��
 *      Author: jimm
 */

#include "net_connector.h"
#include "net_typedef.h"
#include "net_errordef.h"
#include "net_timer.h"
#include "net_handler.h"
#include "net_connection.h"

#include <errno.h>

NETEVENT_NAMESPACE_BEGIN

CConnector::CConnector(CNetHandler *pNetHandler, IPacketParserFactory *pFactory, IIOHandler *pIOHandler)
{
	m_pNetHandler = pNetHandler;
	m_pPacketParserFactory = pFactory;
	m_pIOHandler = pIOHandler;
}

int32_t CConnector::Connect(const char *szRemoteIP, uint16_t nPort)
{
	if (NULL == szRemoteIP)
	{
		return E_REMOTEIP;
	}

	IPacketParser *pPacketParser = m_pPacketParserFactory->Create();
	CSocket *pSocket = new CConnection(m_pNetHandler, pPacketParser, m_pIOHandler);
	if(pSocket == NULL)
	{
		m_pPacketParserFactory->Destory(pPacketParser);
		return E_NULLPOINTER;
	}

	pSocket->OpenSocket();

	//�����׽������ͺ�״̬
	pSocket->SetSessionType(enmSessionType_Communicate);

	//�ж��׽�������
	if (enmSessionType_Communicate != pSocket->GetSessionType())
	{
		m_pPacketParserFactory->Destory(pPacketParser);
		return E_SOCKETTYPE;
	}

	//�׽����Ƿ��
	if ((enmInvalidSocketFD == pSocket->GetSocketFD()) || (enmSessionStatus_Opened != pSocket->GetSessionStatus()))
	{
		m_pPacketParserFactory->Destory(pPacketParser);
		return E_SOCKETNOTCREATED;
	}

	pSocket->SetPeerAddress(szRemoteIP);
	pSocket->SetPeerPort(nPort);
	pSocket->SetPeerAddress((uint32_t)inet_addr(szRemoteIP));

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(szRemoteIP);
	addr.sin_port = htons(nPort);

	CConnectTimer *pConnTimer = g_ConnectTimerMgt.CreateConnectTimer(pSocket,
			static_cast<ConnectTimerProc>(&CConnection::OnTimerEvent), 3);
	if(pConnTimer == NULL)
	{
		m_pPacketParserFactory->Destory(pPacketParser);
		return E_UNKNOWN;
	}

	pSocket->SetConnectTimer(pConnTimer);

	//��������˽�������
	int32_t ret = connect(pSocket->GetSocketFD(), (const struct sockaddr*)&addr, sizeof(addr));
	if (0 != ret)
	{
		if (errno != EINPROGRESS)
		{
			m_pPacketParserFactory->Destory(pPacketParser);
			pSocket->CloseSocket();
			return E_SOCKETCONNECT;
		}
		else
		{
			m_pNetHandler->RegistEvent(pSocket, mask_read | mask_write);
			pSocket->SetSessionStatus(enmSessionStatus_Connecting);
			return E_SOCKET_CONNECTING;
		}
	}

	m_pNetHandler->RegistEvent(pSocket, mask_read | mask_write);
	//�����׽���״̬
	pSocket->SetSessionStatus(enmSessionStatus_Connected);
	return S_OK;
}

//���¼��ص�
int32_t CConnector::OnRead(int32_t nErrorCode)
{
	return S_OK;
}

//д�¼��ص�
int32_t CConnector::OnWrite(int32_t nErrorCode)
{
	return S_OK;
}

//�쳣�¼��ص�
int32_t CConnector::OnError(int32_t nErrorCode)
{
	return S_OK;
}


NETEVENT_NAMESPACE_END
