/*
 * net_connection.cpp
 *
 *  Created on: 2013��12��16��
 *      Author: jimm
 */

#include "net_connection.h"
#include "net_handler.h"
#include "net_eventid.h"
#include "net_connmgt.h"
#include "../common/common_memmgt.h"

NETEVENT_NAMESPACE_BEGIN

CConnection::CConnection(CNetHandler *pNetHandler, IPacketParser *pPacketParser, IIOHandler *pIOHandler)
{
	m_pNetHandler = pNetHandler;
	m_pPacketParser = pPacketParser;
	m_pIOHandler = pIOHandler;
}

int32_t CConnection::Write(uint8_t *pBuf, int32_t nBufSize)
{
	uint8_t *pMem = MALLOC(sizeof(NetPacket) + nBufSize);

	NetPacket *pPacket = new(pMem) NetPacket();
	pPacket->m_nNetPacketLen = nBufSize;
	pPacket->m_nSessionID = m_nSessionID;
	memcpy(pPacket->m_pNetPacket, pBuf, nBufSize);

	m_pNetHandler->PushPacket(pPacket);

	return nBufSize;
}

int32_t CConnection::WritedToLowerBuf(uint8_t *pBuf, int32_t nBufSize)
{
	m_pIOHandler->OnSent(this, pBuf, nBufSize);

	return nBufSize;
}

//���¼��ص�
int32_t CConnection::OnRead(int32_t nErrorCode)
{
	uint8_t arrBuf[enmMaxMessageSize];

	int32_t nCloseCode = 0;
	int32_t nRecvBytes = 0;
	int32_t nRet = Recv(arrBuf, sizeof(arrBuf), nRecvBytes);
	if(nRet != S_OK)
	{
		nCloseCode = nRet;
	}

	m_pPacketParser->InputData(arrBuf, nRecvBytes);

	//��ȡ��Ϣ��
	NetPacket *pNetPacket = NULL;
	do
	{
		pNetPacket = m_pPacketParser->Parser();
		if(pNetPacket != NULL)
		{
			m_pIOHandler->OnRecved(this, pNetPacket->m_pNetPacket, pNetPacket->m_nNetPacketLen);
		}
	}while(pNetPacket != NULL);

	if(nCloseCode != 0)
	{
		CloseSocket(nCloseCode);
		return E_SOCKETERROR;
	}

	return S_OK;
}

//д�¼��ص�
int32_t CConnection::OnWrite(int32_t nErrorCode)
{
	if(nErrorCode != 0)
	{
		CloseSocket(SYS_EVENT_CONN_ERROR);
		return E_SOCKETERROR;
	}

	int32_t nRet = SendRestData();
	if(nRet < 0)
	{
		CloseSocket(SYS_EVENT_CONN_ERROR);
		return E_SOCKETERROR;
	}

	return S_OK;
}

//�쳣�¼��ص�
int32_t CConnection::OnError(int32_t nErrorCode)
{
	m_pIOHandler->OnError(this);

	return S_OK;
}

//���ӳɹ��ص�(���ش˺�����������������Щ������Ϣ��ʼ���Ĺ���)
int32_t CConnection::OnConnected()
{
	g_ConnMgt.RegistConnection(this);

	m_pIOHandler->OnOpened(this);

	return S_OK;
}
//���ӳ�ʱ�ص�
int32_t CConnection::OnConnectTimeout()
{
	m_pIOHandler->OnTimeout(this);

	return S_OK;
}

//�Ͽ����ӻص�(���ش˺�����������������Щ��Դ���յĹ���)
int32_t CConnection::OnDisconnect(int32_t nCloseCode)
{
	m_pIOHandler->OnClosed(this);

	g_ConnMgt.UnregistConnection(this);

	return S_OK;
}

NETEVENT_NAMESPACE_END
