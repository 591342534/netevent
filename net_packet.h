/*
 * net_packet.h
 *
 *  Created on: 2013��12��16��
 *      Author: Administrator
 */

#ifndef NET_PACKET_H_
#define NET_PACKET_H_

#include "net_namespace.h"
#include "net_typedef.h"

NETEVENT_NAMESPACE_BEGIN

class NetPacket
{
public:
	NetPacket()
	{
		m_nNetPacketLen = 0;
		m_nSessionID = 0;
	}

	uint32_t 		m_nNetPacketLen;	//����������4�ֽڵĳ���
	SessionID		m_nSessionID;		//���ӵ�Ψһ��ʾ
	uint8_t			m_pNetPacket[0];
};

NETEVENT_NAMESPACE_END

#endif /* NET_PACKET_H_ */
