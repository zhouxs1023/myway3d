#include "MWOption.h"

namespace Myway
{
    Option::Option()
    {
        Clear();
    }

    Option::~Option()
    {

    }
   
    void Option::Clear()
    {
        Memzero(mItems, sizeof(Op) * MAX_OPTIONS);
    }

    Option::ptr & Option::operator [] (const char * name)
    {
        int i = 0;

        while (i < MAX_OPTIONS)
        {
            if (mItems[i].name[0] == '\0')
            {
                Strcpy(mItems[i].name, Op::len, name);
                break;
            }

            if (Strcmp(name, mItems[i].name) == 0)
                break;

            ++i;
        }

        assert (i < MAX_OPTIONS);

        return mItems[i].val;
    }

    const Option::ptr & Option::operator [] (const char * name) const
    {
        int i = 0;

        while (i < MAX_OPTIONS)
        {
            if (Strcmp(name, mItems[i].name) == 0)
                break;

            ++i;
        }

        assert (i < MAX_OPTIONS);

        return mItems[i].val;
    }

}