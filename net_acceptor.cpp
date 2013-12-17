/*
 * net_acceptor.cpp
 *
 *  Created on: 2013��12��16��
 *      Author: jimm
 */

#include "net_acceptor.h"
#include "net_handler.h"
#include "net_connection.h"

NETEVENT_NAMESPACE_BEGIN

CAcceptor::CAcceptor(CNetHandler *pNetHandler, IPacketParserFactory *pFactory, IIOHandler *pIOHandler)
{
	m_pNetHandler = pNetHandler;
	m_pPacketParserFactory = pFactory;
	m_pIOHandler = pIOHandler;
}

int32_t CAcceptor::Bind(const char *szLocalIP, uint16_t nPort)
{
	OpenSocket();

	if((m_nSocketType == enmSocketType_Listen) || (nPort > 0))
	{
		//�����׽�������
		int32_t op = 1;
		int32_t ret = setsockopt(m_nSocketFD, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op));
		if (0 != ret)
		{
			CloseSocket();
			return E_SOCKETOPTION;
		}
		//����������ַ&�˿���Ϣ
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		if((NULL == szLocalIP) || (strlen(szLocalIP) == 0))
		{
			addr.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else
		{
			addr.sin_addr.s_addr = inet_addr(szLocalIP);
		}
		addr.sin_port = htons(nPort);

		ret = bind(m_nSocketFD, (struct sockaddr *)&addr, sizeof(addr));
		if (0 != ret)
		{
			CloseSocket();
			return E_SOCKETBIND;
		}

		//��ʼ����
		ret = listen(m_nSocketFD, SOMAXCONN);
		if (0 != ret)
		{
			CloseSocket();
			return E_SOCKETLISTEN;
		}

		m_nListenAddress = addr.sin_addr.s_addr;
		m_nListenPort = nPort;

		m_pNetHandler->RegistEvent(this, mask_read);
	}

	//����Ϊ������
	set_non_block(m_nSocketFD);

	//�����׽������ͺ�״̬
	m_nSocketType = enmSocketType_Listen;
	m_nSocketStatus = enmSocketStatus_Opened;

	return S_OK;
}

int32_t CAcceptor::Bind(uint16_t nPort)
{
	return Bind(NULL, nPort);
}

//���¼��ص�
int32_t CAcceptor::OnRead(int32_t nErrorCode)
{
	return S_OK;
}

//д�¼��ص�
int32_t CAcceptor::OnWrite(int32_t nErrorCode)
{
	return S_OK;
}

//�쳣�¼��ص�
int32_t CAcceptor::OnError(int32_t nErrorCode)
{
	return S_OK;
}

//�������ӻص�(���ش˺�����������������Щ������Ϣ��ʼ���Ĺ���)
int32_t CAcceptor::OnAccept(SocketFD nAcceptFD, struct sockaddr_in stPeerAddress,
		socklen_t nPeerAddressLen)
{
	IPacketParser *pPacketParser = m_pPacketParserFactory->Create();
	CSocket *pSocket = new CConnection(m_pNetHandler, pPacketParser, m_pIOHandler);

	pSocket->Clear();
	pSocket->SetSocketFD(nAcceptFD);
	pSocket->SetSocketStatus(enmSocketStatus_Connected);
	pSocket->SetPeerAddress((uint32_t)stPeerAddress.sin_addr.s_addr);
	pSocket->SetPeerPort(stPeerAddress.sin_port);
	pSocket->SetCreateTime(time(NULL));

	set_non_block(nAcceptFD);

	m_pNetHandler->RegistEvent(pSocket, mask_read | mask_write);
	m_pIOHandler->OnOpened(pSocket);

	return S_OK;
}


NETEVENT_NAMESPACE_END
