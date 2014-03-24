/*
 * net_connection.h
 *
 *  Created on: 2013��12��16��
 *      Author: jimm
 */

#ifndef NET_CONNECTION_H_
#define NET_CONNECTION_H_

#include "net_namespace.h"
#include "net_socket.h"
#include "net_impl.h"

NETEVENT_NAMESPACE_BEGIN

class CConnection : public CSocket
{
public:
	CConnection(CNetHandler *pNetHandler, IPacketParser *pPacketParser, IIOHandler *pIOHandler);
	//���ý����
	void SetPacketParser(IPacketParser *pPacketParser);
	IPacketParser *GetPacketParser();
	//����io������
	void SetIOHandler(IIOHandler *pIOHandler);
	IIOHandler *GetIOHandler();

	virtual void Close(int32_t nCloseCode = 0);
	//д������
	virtual int32_t Write(uint8_t *pBuf, int32_t nBufSize);
	//д�뵽�ײ�buf
	int32_t WritedToLowerBuf(uint8_t *pBuf, int32_t nBufSize);
protected:
	//���¼��ص�
	virtual int32_t OnRead(int32_t nErrorCode);
	//д�¼��ص�
	virtual int32_t OnWrite(int32_t nErrorCode);
	//�쳣�¼��ص�
	virtual int32_t OnError(int32_t nErrorCode);
	//���ӳɹ��ص�(���ش˺�����������������Щ������Ϣ��ʼ���Ĺ���)
	virtual int32_t OnConnected();
	//���ӳ�ʱ�ص�
	virtual int32_t OnConnectTimeout();
	//�Ͽ����ӻص�(���ش˺�����������������Щ��Դ���յĹ���)
	virtual int32_t OnDisconnect(int32_t nCloseCode);

protected:
	IPacketParser			*m_pPacketParser;
	IIOHandler				*m_pIOHandler;
};

NETEVENT_NAMESPACE_END

#endif /* NET_CONNECTION_H_ */
