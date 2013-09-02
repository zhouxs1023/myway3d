#pragma once

/*
*   网络数据包
*
*       Format:
*           
*           id(2 bytes) size(2bytes) tick(4 bytes) data(...)
*
*/

#define PACK_COMPACK        "$-$"
#define PACK_COMPACK_SIZE   3
#define PACK_HEAD_KEY       "0123456789"
#define PACK_BODY_KEY       "9876543210"
#define PACK_HEAD_SPACER    0
#define PACK_BODY_SPACER    1
#define MAX_PACK_SIZE       2048

#define NET_ENCRYPT(x, xlen, k, place)	    \
    if( (x)!=NULL )                         \
    {                                       \
                                            \
        char * buf = (char *)(x);           \
        char * key = { k };                 \
        int klen = (int)strlen(key);        \
        for (int i = 0; i < (xlen); i++)    \
        {                                   \
            buf[i] ^= key[(i + place) % klen]; \
        } \
    }

struct NetPacketHead
{
    static const int SIZE = sizeof(short) + sizeof(short) + sizeof(int);

    short id;
    short size;
    int tick;
};


class PkOStream
{
	static const int PK_SIZE = MAX_PACK_SIZE - NetPacketHead::SIZE - PACK_COMPACK_SIZE;

public:
	PkOStream();
	~PkOStream();

	void Write(const void * buffer, int size);
	int Size() const { return mSize; }
	const void * Data() const { return mBuffer; }

protected:
	int mSize;
	char mBuffer[PK_SIZE];
};

class PkIStream
{
	static const int PK_SIZE = MAX_PACK_SIZE - NetPacketHead::SIZE;

public:
	PkIStream(const char * buffer, int size);
	~PkIStream();

	void Read(void * buffer, int size);

	bool eof() { return mPointer == mSize; }

protected:
	int mPointer;
	int mSize;
	char mBuffer[PK_SIZE];
};


class NetPacket
{
public:
    NetPacket() {}
    virtual ~NetPacket() {}

	virtual short GetPacketId() = 0;

	virtual void In(PkIStream & stream) {}
	virtual void Out(PkOStream & stream) {}

	virtual void Execute(void * param) {} ;
};



class NetPacketManager
{
    typedef HashMap<int, NetPacket*> NetPacketMap;

    DECLARE_SINGLETON(NetPacketManager);

public:
    NetPacketManager();
    ~NetPacketManager();

	void AddPacket(NetPacket * factory);
	NetPacket * GetPacket(int id);

protected:
	NetPacketMap mFactorys;
};

void RegistNetPacket();

#define SERVER_PORT 10086

#define MAX_ACCOUNT_SIZE 16
#define MAX_PASSWORD_SIZE 16




enum NetPacketId {

	NPI_LOGIN_CL,               // 客户端发送登陆消息
	NPI_LOGIN_SV,               // 服务器发送登陆验证返回消息

	NPI_PLAYER_ENTER_CL,		// 玩家进入游戏

	NPI_ADD_NPC_SV,				// 添加NPC

	NPI_MAX,
};

enum eLogin {
	eLoginFail,
	eLoginOk,
	eLoginPasswardError,
	eLoginAccountError,
};