#include "Precompiled.h"
#include "Session.h"
#include "SessionManager.h"
#include "Socket.h"
#include "Dispatcher.h"
#include "Stream.h"

Session::Session(Dispatcher* disp)
	: _disp(disp)
	, _recvBuf(nullptr)
{
	_recvBuf = Stream::create(MAX_RECV_BUFFER);
}

Session::~Session()
{
	Stream::destroy(_recvBuf);
}

void Session::onAccept()
{
	// can't reach
}

void Session::onError(int err)
{
	_disp->onDisconnect(err);
}

void Session::onRecv()
{
	int nrecv = _sock.recv(_recvBuf->writeptr(), _recvBuf->remain());
	if (nrecv < 0)
	{	
		onError(errno);
		return;	
	}
	else if (nrecv == 0)
	{
		onError(0);
		return;
	}

	_recvBuf->write(nrecv);

	if (_disp->parsing(_recvBuf) == false)
	{
		onError(EVENT_ERROR_PARSING);
		return;
	}
}

void Session::onSend()
{
//	int nsend = _sock.send(_sendBuf->readptr(), _sendBuf->length());
//	if (nsend < 0)
	{
		onError(errno);
		return;
	}

//	_sendBuf->read(nsend);
}

