#include "StdAfx.h"

#include "Client.h"

Client * Client::msInstance = NULL;

Client::Client()
{
	msInstance = this;

    Socket::Init();

	mServerIp = "127.0.0.1";

    mIStream = new NetIStream(&mSock);
    mOStream = new NetOStream(&mSock);

	RegistNetPacket();
}

Client::~Client()
{
    delete mOStream;
    delete mIStream;

    Socket::Close(&mSock);

    Socket::Shutdown();

	msInstance = NULL;
}

void Client::Connect()
{
    Client::DeConnect();
    Socket::Open(&mSock);
    Socket::Connect(&mSock, mServerIp.c_str(), SERVER_PORT);

    if (!Socket::IsValid(&mSock))
    {
        //ISystem::Instance().ShowMessage("Connect to server error!");

		return ;
    }

	Socket::EnableNoBlock(&mSock, TRUE);
    Socket::SetLinger(&mSock, 0);
}

void Client::DeConnect()
{
    Socket::Close(&mSock);
}

bool Client::IsConnect()
{
    return Socket::IsValid(&mSock) == TRUE;
}

void Client::Update()
{
    if (!IsConnect())
    {
        return ;
    }

    if (!_Select() || !_Except() || !_Input() || !_Output())
    {
        // error.
        return ;
    }

    _Process();
}

bool Client::_Select()
{
    Socket::FD_Clear();
    Socket::FD_SetRD(&mSock);
    Socket::FD_SetWT(&mSock);
    Socket::FD_SetEX(&mSock);

    return Socket::FD_Select(0, 0) == TRUE;
}

bool Client::_Input()
{
    if (Socket::FD_IsSetRD(&mSock))
    {
        int ret = mIStream->Refresh();

        if (ret <= NET_ERROR)
        {
            Socket::Close(&mSock);
            return false;
        }
    }

    return true;
}

bool Client::_Output()
{
    if (Socket::FD_IsSetWT(&mSock))
    {
        int ret = mOStream->Refresh();

        if (ret <= NET_ERROR)
        {
            Socket::Close(&mSock);
            return false;
        }
    }

    return true;
}

bool Client::_Except()
{
    if (Socket::FD_IsSetEX(&mSock))
    {
        Socket::Close(&mSock);
        return false;
    }

    return true;
}

void Client::_Process()
{
	char buffer[MAX_PACK_SIZE] = { 0 };

	while (1)
	{
		NetPacketHead head;

		int skip = 0;

		// 跳过分割符
		if (!mIStream->Skip(PACK_COMPACK, PACK_COMPACK_SIZE))
		{  
			break;
		}

		skip += PACK_COMPACK_SIZE;

		// 读取消息头
		if (mIStream->Read(&head, NetPacketHead::SIZE) == 0)
		{
			// 没有接受完全
			mIStream->Skip(-skip);
			break;
		}

		skip += NetPacketHead::SIZE;

		// 解密
		NET_ENCRYPT(&head, NetPacketHead::SIZE, PACK_HEAD_KEY, PACK_HEAD_SPACER);

		if (head.id >= NPI_MAX)
		{
			printf("Warning: Net Packet Id Error, %d\n", head.id);
			break;
		}

		if (head.size < 0 || head.size > MAX_PACK_SIZE - NetPacketHead::SIZE - PACK_COMPACK_SIZE)
		{
			printf("Warning: Net Pack Size Error, id %d, size %d\n", head.id, head.size);
			break;
		}

		// 读取消息
		if (mIStream->Read(buffer, head.size) == 0)
		{
			// 尚未接受完全
			mIStream->Skip(-skip);
			break;
		}

		// 解密
		NET_ENCRYPT(buffer, head.size, PACK_BODY_KEY, PACK_BODY_SPACER);

		// 处理数据包
		NetPacket * pack = NetPacketManager::Instance()->GetPacket(head.id);

		d_assert (pack && "数据包处理程序尚未注册.");

		pack->In(PkIStream(buffer, head.size));
		pack->Execute(this);
	}
}

void Client::SendPacket(NetPacket * pack)
{
    if (Socket::IsValid(&mSock))
    {
		PkOStream stream;

		pack->Out(stream);

		char buffer[MAX_PACK_SIZE];

		NetPacketHead head;

		head.id = pack->GetPacketId();
		head.size = (short)stream.Size();
		head.tick = 0;

		int offset = 0;
		WriteBuffer(buffer, MAX_PACK_SIZE, 0, PACK_COMPACK, PACK_COMPACK_SIZE);
		offset += PACK_COMPACK_SIZE;
		WriteBuffer(buffer, MAX_PACK_SIZE, offset, &head, NetPacketHead::SIZE);
		offset += NetPacketHead::SIZE;

		WriteBuffer(buffer, MAX_PACK_SIZE, offset, stream.Data(), head.size);
		offset += head.size;

		// 加密消息头
		NET_ENCRYPT(buffer + PACK_COMPACK_SIZE, NetPacketHead::SIZE, PACK_HEAD_KEY, PACK_HEAD_SPACER);
		// 加密消息体
		NET_ENCRYPT(buffer + PACK_COMPACK_SIZE + NetPacketHead::SIZE, head.size, PACK_BODY_KEY, PACK_BODY_SPACER);

		mOStream->Write(buffer, offset);
    }
}
