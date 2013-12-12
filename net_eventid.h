/*
 * net_eventid.h
 *
 *  Created on: 2013��11��4��
 *      Author: jimm
 */

#ifndef NET_EVENTID_H_
#define NET_EVENTID_H_

#include "net_namespace.h"

NETEVENT_NAMESPACE_BEGIN

//���ӹر��¼����ͻ���������
#define SYS_EVENT_CONN_CLIENT_CLOSED				0x0001

//���ӹر��¼���������������
#define SYS_EVENT_CONN_SERVER_CLOSED				0x0002

//���ӳ����¼�
#define SYS_EVENT_CONN_ERROR						0x0003

//���ӳ�ͻ�¼�
#define SYS_EVENT_CONN_CONFLICT						0x0004

//ע��ʧ�ܹر������¼�
#define SYS_EVENT_CONN_REGISTFAILED					0x0005

//ע����������ͺͷ�����id�¼�
#define SYS_EVENT_REGIST_SERVER						0x0006

//�������ڲ����ӶϿ��¼�
#define SYS_EVENT_DISCONNECT_SERVER					0x0007

//�������ӳɹ��¼�
#define SYS_EVENT_INITIATIVE_CONNECT_SUCCESS		0x0008

//�������ӳɹ��¼�
#define SYS_EVENT_PASSIVE_CONNECT_SUCCESS			0x0009

NETEVENT_NAMESPACE_END


#endif /* NET_EVENTID_H_ */
