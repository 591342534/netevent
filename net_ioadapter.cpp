/*
 * net_ioadapter.cpp
 *
 *  Created on: 2013��12��17��
 *      Author: jimm
 */

#include "net_ioadapter.h"

NETEVENT_NAMESPACE_BEGIN

int32_t CIOHandlerAdapter::OnOpened(IEventHandler *pHandler)
{
	return 0;
}

int32_t CIOHandlerAdapter::OnRecved(IEventHandler *pHandler, uint8_t *pBuf, uint32_t nBufSize)
{
	return 0;
}

int32_t CIOHandlerAdapter::OnSent(IEventHandler *pHandler, uint8_t *pBuf, uint32_t nBufSize)
{
	return 0;
}

int32_t CIOHandlerAdapter::OnClosed(IEventHandler *pHandler)
{
	return 0;
}

int32_t CIOHandlerAdapter::OnError(IEventHandler *pHandler)
{
	return 0;
}

NETEVENT_NAMESPACE_END
