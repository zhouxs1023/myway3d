#pragma once

#include "MWString.h"
#include "MWDataStream.h"
#include "MWList.h"
#include "MWHashMap.h"
#include "MWMemory.h"

namespace Myway
{

enum enVerType
{
	NULL_VAR = 0,
	INT_VAR,
	FLOAT_VAR,
	STRING_VAR,
	VECTOR_VAR
};

struct MW_ENTRY stVector
{
	stVector() : x(0), y(0), z(0) {}

	float x, y, z;
};

//variable
class MW_ENTRY CVariable
{
public:
	CVariable();
	~CVariable();

	void SetData(int type, char * szName, void * pData);
	void SetData(int type, void * pData);
	
	char *	GetName()			{ return m_szName; };
	int		GetType()			{ return m_type; };

	int		GetDataAsInt()		{ return mntVal; };
	float	GetDataAsFloat()	{ return mloatVal; };
	char *	GetDataAsString()   { return m_stringVal; };
	stVector GetDataAsVector()  { return m_vecVal; }; 

protected:
	char		m_szName[128];
	int			m_type;
	int			mntVal;
	float		mloatVal;
	char *		m_stringVal;
	stVector    m_vecVal;

public:
	friend void CpyVariable(CVariable * pDst, CVariable * pSrc, unsigned int nTotals);
};

//Property Script
class MW_ENTRY CPropertyScript
{
public:
	CPropertyScript();
	~CPropertyScript();

	bool LoadScriptFile(const String & sFileName);

private:
	bool IncreaseVariableList();
	void ParseNext(char * pTmpLine, char * pOutData);

public:
	bool AddVariable(char * szName, int type, void * val);
	bool SetVariable(char * szName, int type, void * val);
	
	int		GetVariableAsInt(char * szName);
	float   GetVariableAsFloat(char * szName);
	char *	GetVariableAsString(char * szName);
	stVector GetVariableAsVector(char * szName);
	void	Shutdown();

private:
	CVariable * mVarList;
	int m_nTotalVars;
	int m_nCurrentLineChar;
};


#define MAX_CMD_DEF_SIZE    128
#define MAX_PRM_DEF_SIZE    896

class MW_ENTRY CommandScript
{
    struct command_param
    {
        char command[MAX_CMD_DEF_SIZE];
        char param[MAX_PRM_DEF_SIZE];

        inline command_param() { command[0] = 0; param[0] = 0; }
        inline ~command_param() {}

        inline command_param(const command_param & rk) 
        {
            Memcpy(this, &rk, sizeof(command_param));
        }

        inline command_param & operator =(const command_param & rk)
        {
            Memcpy(this, &rk, sizeof(command_param));
            return *this;
        }
    };

public:
	CommandScript();
	~CommandScript();

	bool LoadScriptFile(const String & sFileName);
    void LoadStream(DataStreamPtr stream);
    void LoadMemory(const void * data, const int size);

	String ParseCommand();
	String ParseStringParam();
	bool ParseboolParam();
	int ParseIntParam();
	float ParseFloatParam();

	void Shutdown();

	void MoveToStart();
    bool MoveToNextLine();
    bool MoveToPreLine();

	int GetTotalLines();
	bool IsLineComment();

private:
    void RemoveSpace();

private:
    bool                            m_start;
    int                             m_curlen;
	int                             m_curchar;
    List<command_param>::Iterator   m_cursor;
    List<command_param>::Iterator   m_beg;
    List<command_param>::Iterator   m_end;
    List<command_param>             m_script;
};

//token
class MW_ENTRY CToken
{
public:
	CToken();
	~CToken();

	void Reset();
	void SetTokenStream(char * data);
	bool GetNextToken(char * buffer);
	bool GetNextToken(char * token, char * buffer);
	bool MovToNextline(char * buffer);

	void Shutdown();

private:
	int m_length;
	int m_startIndex, m_endIndex;
	char * mData;
};


// config
class MW_ENTRY ConfigFile
{
    struct key
    {
        char k[MAX_CMD_DEF_SIZE];

        key() { k[0] = 0; }
        ~key() {}

        key(const char * rk)
        {
           Strcpy(k, MAX_CMD_DEF_SIZE, rk);
        }

        key(key & k)
        {
            *this = k;
        }

        key & operator =(const key & rk)
        {
            Strcpy(k, MAX_CMD_DEF_SIZE, rk.k);
            return *this;
        }

        struct hash_t
        {
            unsigned int operator ()(const key & rk) const
            {
                const char * str = rk.k;
                unsigned int hash = 0;

                while (*str)
                {
                    // equivalent to: hash = 65599*hash + (*str++);
                    hash = (*str++) + (hash << 6) + (hash << 16) - hash;
                }

                return (hash & 0x7FFFFFFF);
            }
        };

        struct equal_t
        {
            bool operator ()(const key & lk, const key & rk) const
            {
                return Strcmp(lk.k, rk.k) == 0;
            }
        };
    };

    struct pair
    {
        char command[MAX_CMD_DEF_SIZE];
        char param[MAX_PRM_DEF_SIZE];

        pair() { command[0] = 0; param[0] = 0; }
        ~pair() {}

        pair(const pair & rk) 
        {
            Memcpy(this, &rk, sizeof(pair));
        }

        pair & operator =(const pair & rk)
        {
            Memcpy(this, &rk, sizeof(pair));
            return *this;
        }
    };

    typedef HashMap<key, pair, key::hash_t, key::equal_t> PairMap;

public:
    ConfigFile();
    ~ConfigFile();

    bool LoadFile(const char * sFileName);
    void LoadMemory(const void * data, int size);

    const char * GetValue(const char * param);

private:
    void RemoveSpace();

private:
    PairMap mMPairs;
};

}
