/*
 * net_connector.h
 *
 *  Created on: 2013��12��16��
 *      Author: jimm
 */

#ifndef NET_CONNECTOR_H_
#define NET_CONNECTOR_H_

#include "net_namespace.h"
#include "net_impl.h"
#include "net_socket.h"

NETEVENT_NAMESPACE_BEGIN

class CSocket;
class CNetHandler;

class CConnector : public CSocket
{
public:
	CConnector(CNetHandler *pNetHandler, IPacketParserFactory *pFactory, IIOHandler *pIOHandler);

	int32_t Connect(const char *szRemoteIP, uint16_t nPort, uint32_t nTimeout = 3000);

	CNetHandler *GetNetHandler();
protected:
	//���¼��ص�
	virtual int32_t OnRead(int32_t nErrorCode);
	//д�¼��ص�
	virtual int32_t OnWrite(int32_t nErrorCode);
	//�쳣�¼��ص�
	virtual int32_t OnError(int32_t nErrorCode);
protected:
	CNetHandler				*m_pNetHandler;
	IPacketParserFactory	*m_pPacketParserFactory;
	IIOHandler				*m_pIOHandler;
};

NETEVENT_NAMESPACE_END

#endif /* NET_CONNECTOR_H_ */
