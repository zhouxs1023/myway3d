#include "MWScript.h"
#include "MWFile.h"
#include "MWMemory.h"

#pragma warning(push)
#pragma warning(disable : 4999)
#pragma warning(disable : 4996)
#pragma warning(disable : 4267)

using namespace Myway;

void Myway::CpyVariable(CVariable * pDst, CVariable * pSrc, unsigned int nTotals)
{
	for (unsigned int i = 0; i < nTotals; i++)
	{
		Memcpy(pDst[i].m_szName, pSrc[i].m_szName, 128);
		pDst[i].m_type =  pSrc[i].m_type;
		pDst[i].mntVal = pSrc[i].mntVal;
		pDst[i].mloatVal = pSrc[i].mloatVal;

		// cpy string
		if (pDst[i].m_stringVal)
		{
			delete[] pDst[i].m_stringVal;
			pDst[i].m_stringVal = NULL;
		}

		if (pSrc[i].m_stringVal)
		{
			size_t len = Strlen(pSrc[i].m_stringVal);
			pDst[i].m_stringVal = new char[len + 1];
			memcpy(pDst[i].m_stringVal, pSrc[i].m_stringVal, len);
			pDst[i].m_stringVal[len] = '\0';
		}
		
		pDst[i].m_vecVal = pSrc[i].m_vecVal;
	}
}

int DetermineType(int startIndex, char * buffer)
{
	int numComponents = 0;
	int type = NULL_VAR;
	bool decimalFound =false;
	bool charFound = false;
	int index = startIndex;

	while(index < (int)Strlen(buffer))
	{
		if (numComponents == 0)
			numComponents++;

		if (buffer[index] == ' ')
			numComponents++;

		if (buffer[index] == '.')
			decimalFound = true;

		if ((buffer[index] >= 'a' && buffer[index] <= 'z') || 
			(buffer[index] >= 'A' && buffer[index] >= 'B'))
		{
			charFound = true;
		}

		index++;
	}

	switch (numComponents)
	{
	case 1:
		if (charFound)
			type = STRING_VAR;
		else
			type = INT_VAR;

		if (decimalFound == true && charFound == false)
			type = FLOAT_VAR;
		
		break;

	case 3:
		if (charFound)
			type = STRING_VAR;
		else
			type = VECTOR_VAR;

		break;

	default:
		if (numComponents > 0)
			type = STRING_VAR;
		
		break;
	}

	return type;
}

CVariable::CVariable()
: m_type(0), mloatVal(0), mntVal(0), m_stringVal(0)
{
	m_szName[0] = '\0';
	m_vecVal.x = m_vecVal.y = m_vecVal.z = 0;
}

CVariable::~CVariable()
{
	if (m_stringVal) 
		delete[] m_stringVal;
	m_stringVal = NULL;
}

void CVariable::SetData(int type, char *szName, void *pData)
{
	if (!szName)
		return;

	memcpy(m_szName, szName, Strlen(szName));
	m_szName[Strlen(szName)] = '\0';
	SetData(type, pData);
}

void CVariable::SetData(int type, void *pData)
{
	stVector * vec = NULL;
	size_t len = 0;

	//depending on the type will depend where the value is stored
	switch (type)
	{
	case INT_VAR:
		mntVal = *(int*)pData;
		break;

	case FLOAT_VAR:
		mloatVal = *(float*)pData;
		break;

	case STRING_VAR:
		len = Strlen((char*)pData);
		m_stringVal = new char[len + 1];
		memcpy(m_stringVal, (char*)pData, len);
		m_stringVal[len] = '\0';
		break;

	case VECTOR_VAR:
		vec = (stVector*)pData;
		m_vecVal.x = vec->x;
		m_vecVal.y = vec->y;
		m_vecVal.z = vec->z;
		break;

	default:
		return;
		break;
	}

	m_type = type;
}

CPropertyScript::CPropertyScript()
: mVarList(NULL), m_nTotalVars(0), m_nCurrentLineChar(0)
{
}

CPropertyScript::~CPropertyScript()
{
	Shutdown();
}

bool CPropertyScript::IncreaseVariableList()
{
	if (!mVarList)
	{
		mVarList = new CVariable[1];
		if (!mVarList)
			return false;
	}
	else
	{
		CVariable * pTmp = new CVariable[m_nTotalVars + 1];
		if (!pTmp)
			return false;

		CpyVariable(pTmp, mVarList, m_nTotalVars);
		delete[] mVarList;

		mVarList = pTmp;
	}

	return true;
}

bool CPropertyScript::LoadScriptFile(const String & sFileName)
{
	int totalScriptLines = 0;
	char tmpLine[MAX_LINE_SIZE];
	char varName[MAX_CMD_SIZE];
	char param[MAX_PARAM_SIZE];
	int type = 0;
    File input;

	input.Open(sFileName, OM_READ);
	if (!input.IsOpen())
		return false;

	//clear all data
	Shutdown();

	//open and get number of lines
	while (input.ReadLine(tmpLine))
		totalScriptLines++;

    input.Seek(0);

	//open it this time to get the variables out
	for (int i = 0; i < totalScriptLines; i++)
	{
		m_nCurrentLineChar = 0;

		//read the entire line from file
		input.ReadLine(tmpLine);
		tmpLine[Strlen(tmpLine)] = '\0';

		//check if Check if this is a comment. if not keep going
		if (tmpLine[0] != '#')
		{
			//reader the name then determine the type
			ParseNext(tmpLine, varName);
			type = DetermineType(m_nCurrentLineChar, tmpLine);

			if (type == INT_VAR)
			{
				/*
				if (IncreaseVariableList())
				{
					int iVal = atoi(param);
					ParseNext(tmpLine, param);
					mVarList[m_nTotalVars].SetData(INT_VAR, 
						varName, &iVal);
					
					m_nTotalVars++;
				}*/
				int iVal = atoi(param);
				ParseNext(tmpLine, param);
				AddVariable(varName, type, (void*)&iVal);
			}
			else if (type == FLOAT_VAR)
			{
				/*if (IncreaseVariableList())
				{
					float fVal = 0;
					ParseNext(tmpLine, varName);
					fVal = (float)atof(param);
					mVarList[m_nTotalVars].SetData(FLOAT_VAR, 
						varName, &fVal);

					m_nTotalVars++;
				}*/
				float fVal = 0;
				ParseNext(tmpLine, param);
				fVal = (float)atof(param);
				AddVariable(varName, type, (void*)&fVal);
			}
			else if (type == STRING_VAR)
			{
				/*if (IncreaseVariableList())
				{
					ParseNext(tmpLine, param);
					mVarList[m_nTotalVars].SetData(STRING_VAR,
						varName, (void*)param);

					m_nTotalVars++;
				}*/
				ParseNext(tmpLine, param);
				AddVariable(varName, type, (void*)param);

			}
			else if (type == VECTOR_VAR)
			{
				/*
				if (IncreaseVariableList())
				{
					stVector vecVal;
					ParseNext(tmpLine, param);
					vecVal.x = (float)atof(param);
					ParseNext(tmpLine, param);
					vecVal.y = (float)atof(param);
					ParseNext(tmpLine, param);
					vecVal.z = (float)atof(param);

					mVarList[m_nTotalVars].SetData(VECTOR_VAR,
						varName, (void*)&vecVal);

					m_nTotalVars++;
				}*/
				stVector vecVal;
				ParseNext(tmpLine, param);
				vecVal.x = (float)atof(param);
				ParseNext(tmpLine, param);
				vecVal.y = (float)atof(param);
				ParseNext(tmpLine, param);
				vecVal.z = (float)atof(param);
				AddVariable(varName, type, (void*)&vecVal);
			}
		}
	}

	//close file
	input.Close();
	return true;
}

void CPropertyScript::ParseNext(char * pTmpLine, char * pOutData)
{
	int paramSize = 0;

	//error checking
	if (!pTmpLine || !pOutData)
		return;

	//initialize string
	pOutData[0] = '\0';

	//loop until that find a space or newline
	while (m_nCurrentLineChar < (int)Strlen(pTmpLine))
	{
		if (pTmpLine[m_nCurrentLineChar] == ' ' || 
			pTmpLine[m_nCurrentLineChar] == '\n')
			break;

		//save the text in the array
		pOutData[paramSize] = pTmpLine[m_nCurrentLineChar];

		paramSize++;
		m_nCurrentLineChar++;
	}

	pOutData[paramSize] = '\0';
	m_nCurrentLineChar++;
}



bool CPropertyScript::AddVariable(char *szName, int type, void *val)
{
	if (!SetVariable(szName, type, val))
	{
		if (!IncreaseVariableList())
		{
			return false;
		}
		
		mVarList[m_nTotalVars].SetData(type, szName, val);
		m_nTotalVars++;
	}

	return true;
}

bool CPropertyScript::SetVariable(char *szName, int type, void *val)
{
	for (int i = 0; i < m_nTotalVars; i++)
	{
		if (_stricmp(mVarList[i].GetName(), szName) == 0)
		{
			mVarList[i].SetData(type, val);
			return true;
		}
	}

	return false;
}

int	CPropertyScript::GetVariableAsInt(char * szName)
{
	for(int i = 0; i < m_nTotalVars; i++)
	{
		if (_stricmp(mVarList[i].GetName(), szName) == 0)
			return mVarList[i].GetDataAsInt();
	}

	return 0;
}

float CPropertyScript::GetVariableAsFloat(char * szName)
{
	for(int i = 0; i < m_nTotalVars; i++)
	{
		if (_stricmp(mVarList[i].GetName(), szName) == 0)
			return mVarList[i].GetDataAsFloat();
	}

	return 0;
}

char * CPropertyScript::GetVariableAsString(char * szName)
{
	for(int i = 0; i < m_nTotalVars; i++)
	{
		if (_stricmp(mVarList[i].GetName(), szName) == 0)
			return mVarList[i].GetDataAsString();
	}

	return NULL;
}

stVector CPropertyScript::GetVariableAsVector(char * szName)
{
	for(int i = 0; i < m_nTotalVars; i++)
	{
		if (_stricmp(mVarList[i].GetName(), szName) == 0)
			return mVarList[i].GetDataAsVector();
	}

	return stVector();
}

void CPropertyScript::Shutdown()
{
	if (mVarList)
		delete[] mVarList;

	mVarList = NULL;
}


//command script
CommandScript::CommandScript()
{
    m_curchar = m_curlen = 0;
    m_start = false;
    m_cursor = m_beg = m_end = m_script.End();
}

CommandScript::~CommandScript()
{
	Shutdown();
}

bool CommandScript::LoadScriptFile(const String & sFileName)
{
	File file;

	if (file.Open(sFileName, OM_READ_BINARY))
    {
        int size = file.Size();

        void * data = alloc_m(size);
        file.Read(data, sizeof(char), size);
        LoadMemory(data, size);
        free_m(data);

        return true;
    }

	return false;
}

void CommandScript::LoadStream(DataStreamPtr stream)
{
    int size = stream->Size();
    char * data = new char[size + 1];
    stream->Read(data, size);
    data[size] = 0;

    LoadMemory(data, size);

    delete[] data;
}

void CommandScript::LoadMemory(const void * data, const int size)
{
    Shutdown();

    command_param cp;

    const char * buff = (const char *)data;
    int pos = 0;
    int index = 0;

    while (pos < size)
    {
        cp.command[0] = '\0';
        cp.param[0] = '\0';

        //skip to command
        while (pos < size)
        {
            if (buff[pos] != ' ' && buff[pos] != '\t' &&
                buff[pos] != '\r' && buff[pos] != '\n')
            {
                break;
            }

            ++pos;
        }

        //parse command
        index = 0;
        while (pos < size && index < MAX_CMD_DEF_SIZE)
        {
            if (buff[pos] == ' ' || buff[pos] == '\t' || 
                buff[pos] == '\n' || buff[pos] == '\r' ||
                buff[pos] == '=')
            {
                break;
            }

            cp.command[index++] = buff[pos++];
        }

        //invalid
        if (index >= MAX_CMD_DEF_SIZE)
        {
            assert(0);
            //skip next line
            while (pos < size && buff[pos++] == '\n')
                break;

            continue;
        }

        cp.command[index] = 0;

        //skip to param
        while (pos < size)
        {
            if (buff[pos] != ' ' && buff[pos] != '\t' && buff[pos] != '=')
                break;

            ++pos;
        }


        //parse param
        index = 0;
        while (pos < size && index < MAX_PRM_DEF_SIZE)
        {
            if (buff[pos] == '\n' || buff[pos] == '\r')
            {
                break;
            }

            cp.param[index++] = buff[pos++];
        }

        //invalid
        if (index >= MAX_PRM_DEF_SIZE)
        {
            assert(0);
            //skip next line
            while (pos < size && buff[pos++] == '\n')
                break;

            continue;
        }

        cp.param[index] = '\0';

        m_script.PushBack(cp);
    }

    MoveToStart();
}

String CommandScript::ParseCommand()
{
    return m_cursor->command;
}

String CommandScript::ParseStringParam()
{
    char param[MAX_PARAM_SIZE];

    if (m_cursor == m_end && m_curchar >= m_curlen)
        return 0;

    RemoveSpace();

    param[0] = '\0';

    int index = 0;
    while (m_curchar < m_curlen &&
        index < MAX_PARAM_SIZE)
    {
        if (m_cursor->param[m_curchar] == ' ' || 
            m_cursor->param[m_curchar] == '\t')
            break;

        param[index++] = m_cursor->param[m_curchar++];
    }

    if (index == MAX_PARAM_SIZE)
    {
        return "";
    }

    param[index] = '\0';

    return param;
}

bool CommandScript::ParseboolParam()
{
    char param[MAX_PARAM_SIZE];

    if (m_cursor == m_end && m_curchar >= m_curlen)
        return 0;

    RemoveSpace();

    param[0] = '\0';

    int index = 0;
    while (m_curchar < m_curlen &&
           index < MAX_PARAM_SIZE)
    {
        if (m_cursor->param[m_curchar] == ' ' || 
            m_cursor->param[m_curchar] == '\t')
            break;

        param[index++] = m_cursor->param[m_curchar++];
    }

    if (index == MAX_PARAM_SIZE)
    {
        return false;
    }

    if (param[0] == 't' &&
        param[1] == 'r' && 
        param[2] == 'u' && 
        param[3] == 'e')
        return true;

	return false;
}

int CommandScript::ParseIntParam()
{
    char param[MAX_PARAM_SIZE];

    if (m_cursor == m_end && m_curchar >= m_curlen)
        return 0;

    RemoveSpace();

    param[0] = '\0';

    int index = 0;
    while (m_curchar < m_curlen &&
        index < MAX_PARAM_SIZE)
    {
        if (m_cursor->param[m_curchar] == ' ' || 
            m_cursor->param[m_curchar] == '\t')
            break;

        param[index++] = m_cursor->param[m_curchar++];
    }

    if (index == MAX_PARAM_SIZE)
    {
        return 0;
    }

    param[index] = '\0';

    return atoi(param);
}

float CommandScript::ParseFloatParam()
{
	char param[MAX_PARAM_SIZE];

	if (m_cursor == m_end && m_curchar >= m_curlen)
		return 0;

    RemoveSpace();

    param[0] = '\0';

    int index = 0;
    while (m_curchar < m_curlen &&
           index < MAX_PARAM_SIZE)
    {
        if (m_cursor->param[m_curchar] == ' ' || 
            m_cursor->param[m_curchar] == '\t')
            break;

        param[index++] = m_cursor->param[m_curchar++];
    }

    if (index == MAX_PARAM_SIZE)
    {
        return 0;
    }

	param[index] = '\0';

	return (float)atof(param);
}

void CommandScript::MoveToStart()
{
	m_curchar = 0;
	m_curlen = 0;
    m_start = TRUE;
    m_beg = m_script.Begin();
    m_cursor = m_end = m_script.End();
}

bool CommandScript::MoveToNextLine()
{
    m_curchar = 0;
    m_curlen = 0;

    if (m_start)
    {
        m_cursor = m_script.Begin();
        m_start = false;
    }
    else if (m_cursor == m_end)
    {
        return false;
    }
    else
    {
        ++m_cursor;
        if (m_cursor == m_end)
            return false;
    }

    while (m_cursor->param[m_curlen++] != '\0') ;

    return true;
}

bool CommandScript::MoveToPreLine()
{
    m_curchar = 0;
    m_curlen = 0;

    if (m_cursor == m_end)
    {
        return false;
    }
    else if (m_cursor == m_beg)
    {
        m_cursor = m_end;
        m_start = true;
        return true;
    }
    else
    {
        --m_cursor;
        while (m_cursor->param[m_curlen++] != '\0') ;

        return true;
    }
}

int CommandScript::GetTotalLines()
{
	return m_script.Size();
}

bool CommandScript::IsLineComment()
{
	if (m_cursor->command[0] == '/' &&
        m_cursor->command[1] == '/')
		return true;

	return false;
}

void CommandScript::Shutdown()
{
    m_script.Clear();
    m_cursor = m_end;
    m_curchar = 0;
    m_curlen = 0;
}

void CommandScript::RemoveSpace()
{
    char c;

    while (m_curchar < m_curlen)
    {
        c = m_cursor->param[m_curchar];

        if (c == ' ' || c == '\0'|| c == '\t')
            ++m_curchar;
        else
            break;
    }
}

/*
bool CommandScript::IsVaild(char c)
{
    if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
        (c == '/')      || (c == '(')   || (c == ')')   || (c == '-') || 
        (c == '.')      || (c == '\t')  || (c == '\0')  || (c == '=') ||
        (c == '!')      || (c == '_')   || (c == '#')   || (c == '$') ||
        (c == '%')      || (c == '^')   || (c == '&')   || (c == '(') ||
        (c == ')')      || (c == '-')   || (c == '+')   || (c == '[') ||
        (c == ']')      || (c == '{')   || (c == '}')   || (c == ';') ||
        (c == ':')      || (c == '"')   || (c == '\'')  || (c == '|') ||
        (c == '\\')     || (c == '<')   || (c == ',')   || (c == '>') ||
        (c == '.')      || (c == '?')   || (c == '~')   || (c == ' ') ||
        (c == '`'))
        return true;

    return false;
}
*/


//token
bool IsValidIdentifier(char c)
{
	if ((c >= '0' && c <= '9') || (c >= 'a' && c < 'z') ||
		(c >= 'A' && c <= 'Z') || (c == '_') || (c == '"') || 
		(c == '/') || (c == '(') || (c == ')') || (c == '-') || 
		(c == '.'))
		return true;

	return false;
}

CToken::CToken()
: m_length(0), m_startIndex(0), m_endIndex(0), mData(0)
{
}

CToken::~CToken()
{
	Shutdown();
}

void CToken::Reset()
{
	m_startIndex = m_endIndex = 0;
}

void CToken::SetTokenStream(char * data)
{
	Shutdown();

	m_length = (int)Strlen(data);
	mData = new char[(m_length + 1) * sizeof(char)];
	Strcpy(mData, m_length + 1, data);
	mData[m_length] = '\0';
}

bool CToken::GetNextToken(char * buffer)
{
	bool isString = false;
	m_startIndex = m_endIndex;

	while (m_startIndex < m_length && 
		(mData[m_startIndex] == ' ' || 
		mData[m_startIndex] == '\t' || mData[m_startIndex] == '"'/*!isString*/))
	{
		/*
		if (mData[m_startIndex] = '"') 
			isString = !isString;*/

		m_startIndex++;
	}

	m_endIndex = m_startIndex + 1;

	if (m_startIndex < m_length)
	{
		bool valid = true;

		if (IsValidIdentifier(mData[m_startIndex]))
		{
			while(IsValidIdentifier(mData[m_endIndex]) && 
				mData[m_endIndex] != '"')
				m_endIndex++;
		}
		else
			valid = false;

		if (buffer != NULL)
		{
			if (valid)
			{
				strncpy(buffer, mData + m_startIndex, 
					m_endIndex - m_startIndex);

				buffer[m_endIndex - m_startIndex] = '\0';

				if (Strcmp(buffer, "\n") == 0)
					buffer[0] = '\0';
			}
			else
				buffer[0] = '\0';
		}

		return true;
	}

	return false;
}

bool CToken::GetNextToken(char * token, char * buffer)
{
	char tok[256];

	while (GetNextToken(tok))
	{
		if (_stricmp(tok, token) == 0)
			return GetNextToken(buffer);
	}

	return false;
}

bool CToken::MovToNextline(char * buffer)
{
	if (m_startIndex < m_length && m_endIndex < m_length)
	{
		m_startIndex = m_endIndex;

		while (m_endIndex < m_length && (mData[m_endIndex] != '\n' &&
			mData[m_endIndex] != '\r' && mData[m_endIndex] != '\0'))
			m_endIndex++;

		if (m_endIndex - m_startIndex >= 511)
			return false;

		if (buffer != NULL)
		{
			strncpy(buffer, mData + m_startIndex, 
				m_endIndex - m_startIndex);

			buffer[m_endIndex - m_startIndex] = '\0';
		}
	}
	else
		return false;

	return true;
}

void CToken::Shutdown()
{
	if (mData) 
		delete[] mData;
	mData = NULL;

	m_length = m_startIndex = m_endIndex = 0;
}




ConfigFile::ConfigFile()
{
}

ConfigFile::~ConfigFile()
{
}

bool ConfigFile::LoadFile(const char * sFileName)
{
    File file;

    if (file.Open(sFileName, OM_READ_BINARY))
    {
        int size = file.Size();

        void * data = alloc_mem(size);
        file.Read(data, sizeof(char), size);
        LoadMemory(data, size);
        free_mem(data);

        return true;
    }

    return false;
}

void ConfigFile::LoadMemory(const void * data, int size)
{
    pair cp;

    const char * buff = (const char *)data;
    int pos = 0;
    int index = 0;

    while (pos < size)
    {
        cp.command[0] = '\0';
        cp.param[0] = '\0';

        //skip to command
        while (pos < size)
        {
            if (buff[pos] != ' ' && buff[pos] != '\t' &&
                buff[pos] != '\r' && buff[pos] != '\n')
            {
                break;
            }

            ++pos;
        }

        //parse command
        index = 0;
        while (pos < size && index < MAX_CMD_DEF_SIZE)
        {
            if (buff[pos] == ' ' || buff[pos] == '\t' || 
                buff[pos] == '\n' || buff[pos] == '\r' ||
                buff[pos] == '=')
            {
                break;
            }

            cp.command[index++] = buff[pos++];
        }

        //invalid
        if (index >= MAX_CMD_DEF_SIZE)
        {
            assert(0);
            //skip next line
            while (pos < size && buff[pos++] == '\n')
                break;

            continue;
        }

        cp.command[index] = 0;

        //skip to param
        while (pos < size)
        {
            if (buff[pos] != ' ' && buff[pos] != '\t' && buff[pos] != '=')
                break;

            ++pos;
        }


        //parse param
        index = 0;
        while (pos < size && index < MAX_PRM_DEF_SIZE)
        {
            if (buff[pos] == '\n' || buff[pos] == '\r')
            {
                break;
            }

            cp.param[index++] = buff[pos++];
        }

        //invalid
        if (index >= MAX_PRM_DEF_SIZE)
        {
            assert(0);
            //skip next line
            while (pos < size && buff[pos++] == '\n')
                break;

            continue;
        }

        cp.param[index] = '\0';

        mMPairs.Insert(cp.command, cp);
    }
}

const char * ConfigFile::GetValue(const char * param)
{
    PairMap::Iterator whr = mMPairs.Find(param);
    PairMap::Iterator end = mMPairs.End();

    if (whr != end)
    {
        return whr->second.param;
    }

    return NULL;
}

#pragma warning(pop)
