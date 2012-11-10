

inline bool File::eof()
{
    return mEof;
}

inline const TString128 & File::FileName()
{
    return mFile;
}

/*
operator >>
*/
inline File & File::operator >>(bool & data)
{
    Read(&data, sizeof(bool), 1);
    return (*this);
}

inline File & File::operator >>(char & data)
{
    Read(&data, sizeof(bool), 1);
    return (*this);
}

inline File & File::operator >>(int & data)
{
    Read(&data, sizeof(int), 1);
    return (*this);
}

inline File & File::operator >>(float & data)
{
    Read(&data, sizeof(float), 1);
    return (*this);
}

inline File & File::operator >>(double & data)
{
    Read(&data, sizeof(double), 1);
    return (*this);
}

inline File & File::operator >>(String & data)
{
    ReadString(data);
    return *this;
}



/*
operator <<
*/
inline File & File::operator <<(bool data)
{
    Write(&data, sizeof(bool), 1);
    return *this;
}

inline File & File::operator <<(char data)
{
    Write(&data, sizeof(char), 1);
    return *this;
}

inline File & File::operator <<(int data)
{
    Write(&data, sizeof(int), 1);
    return *this;
}

inline File & File::operator <<(float data)
{
    Write(&data, sizeof(float), 1);
    return *this;
}

inline File & File::operator <<(double data)
{
    Write(&data, sizeof(double), 1);
    return *this;
}

inline File & File::operator <<(const String & data)
{
    Write(data.c_str(), data.Length(), 1);
    return *this;
}

inline File & File::operator <<(const char * data)
{
    Write(data, Strlen(data), 1);
    return *this;
}
