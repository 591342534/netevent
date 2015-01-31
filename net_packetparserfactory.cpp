/*
 * net_packetparserfactory.h
 *
 *  Created on: 2014��12��24��
 *      Author: jimm
 */

#include "net_packetparserfactory.h"
#include "net_packetparser.h"
#include "../common/common_memmgt.h"

NETEVENT_NAMESPACE_BEGIN

CNetPacketParserFactory::CNetPacketParserFactory()
{

}

IPacketParser *CNetPacketParserFactory::Create()
{
	return NEW(CNetPacketParser);
}

void CNetPacketParserFactory::Destory(IPacketParser *pParser)
{
	DELETE(pParser);
}

NETEVENT_NAMESPACE_END
