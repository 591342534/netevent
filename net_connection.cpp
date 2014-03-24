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

void CConnection::SetPacketParser(IPacketParser *pPacketParser)
{
	m_pPacketParser = pPacketParser;
}

IPacketParser *CConnection::GetPacketParser()
{
	return m_pPacketParser;
}

void CConnection::SetIOHandler(IIOHandler *pIOHandler)
{
	m_pIOHandler = pIOHandler;
}

IIOHandler *CConnection::GetIOHandler()
{
	return m_pIOHandler;
}

void CConnection::Close(int32_t nCloseCode)
{
	if(g_ConnMgt.UnregistConnection(this))
	{
		g_ConnMgt.DestroyConnection(this);

		CSocket::Close(nCloseCode);

		m_pNetHandler = NULL;
		m_pPacketParser = NULL;
		m_pIOHandler = NULL;
	}
}

int32_t CConnection::Write(uint8_t *pBuf, int32_t nBufSize)
{
	uint8_t *pMem = MALLOC(sizeof(NetPacket) + nBufSize + 1);

	NetPacket *pPacket = new(pMem) NetPacket();
	pPacket->m_nNetPacketLen = nBufSize + 1;
	pPacket->m_nSessionID = m_nSessionID;
	memcpy(pPacket->m_pNetPacket, pBuf, nBufSize);
	pPacket->m_pNetPacket[nBufSize] = '\0';//��Ҫ�������Ϣ���ַ����İ������Ӱ�ȫ��

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

	if(nRecvBytes > 0)
	{
		m_pPacketParser->InputData(arrBuf, nRecvBytes);
		//��ȡ��Ϣ��
		do
		{
			int32_t nPacketSize = 0;
			uint8_t arrPacket[enmMaxMessageSize];
			m_pPacketParser->Parser(arrPacket, nPacketSize);
			if(nPacketSize <= 0)
			{
				break;
			}

			m_pIOHandler->OnRecved(this, arrPacket, nPacketSize);
			if(m_nSessionStatus != enmSessionStatus_Connected)
			{
				return S_OK;
			}
		}while(true);
	}

	if(nCloseCode != 0)
	{
		Close(nCloseCode);
		return E_SOCKETERROR;
	}

	return S_OK;
}

//д�¼��ص�
int32_t CConnection::OnWrite(int32_t nErrorCode)
{
	if(nErrorCode != 0)
	{
		Close(SYS_EVENT_CONN_ERROR);
		return E_SOCKETERROR;
	}

	int32_t nRet = SendRestData();
	if(nRet < 0)
	{
		Close(SYS_EVENT_CONN_ERROR);
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
	m_pNetHandler->DeleteEvent(this);
	m_pIOHandler->OnClosed(this);

	return S_OK;
}

NETEVENT_NAMESPACE_END