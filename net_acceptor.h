/*
 * net_acceptor.h
 *
 *  Created on: 2013��12��16��
 *      Author: jimm
 */

#ifndef NET_ACCEPTOR_H_
#define NET_ACCEPTOR_H_

#include "net_namespace.h"
#include "net_socket.h"

NETEVENT_NAMESPACE_BEGIN

class CAcceptor : public CSocket
{
public:
	CAcceptor(CNetHandler *pNetHandler, IPacketParserFactory *pFactory, IIOHandler *pIOHandler);

	int32_t Bind(const char *szLocalIP, uint16_t nPort);

	int32_t Bind(uint16_t nPort);

protected:
	//���¼��ص�
	virtual int32_t OnRead(int32_t nErrorCode);
	//д�¼��ص�
	virtual int32_t OnWrite(int32_t nErrorCode);
	//�쳣�¼��ص�
	virtual int32_t OnError(int32_t nErrorCode);
	//�������ӻص�(���ش˺�����������������Щ������Ϣ��ʼ���Ĺ���)
	virtual int32_t OnAccept(SocketFD nAcceptFD, struct sockaddr_in stPeerAddress,
			socklen_t nPeerAddressLen);

protected:
	CNetHandler				*m_pNetHandler;
	IPacketParserFactory	*m_pPacketParserFactory;
	IIOHandler				*m_pIOHandler;
};

NETEVENT_NAMESPACE_END

#endif /* NET_ACCEPTOR_H_ */
