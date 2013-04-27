#include "MWPath.h"

namespace Myway
{

const char white = 0;
const char gray = 1;
const char black = 2;

#define _DEFAULT_MAX_STEP 5000


Path::Path(int width, int height)
: mWidth(width),
  mHeight(height)
{
    mData = (char *)alloc_mem(width * height * sizeof(char));
    mColor = (char *)alloc_mem(width * height * sizeof(char));
    Reset();

    SetMaxStep(_DEFAULT_MAX_STEP);
}


Path::~Path()
{
    if (mData)
        free_mem(mData);

    if (mColor)
        free_mem(mColor);
}


void Path::Set(int x, int y, char flag)
{
    assert (x < mWidth && y < mHeight);

    int i = y * mWidth + x;
    mData[i] |= flag;
}


void Path::Clear(int x, int y, char flag)
{
    assert (x < mWidth && y < mHeight);

    int i = y * mWidth + x;
    mData[i] &= ~flag;
}


char Path::Get(int x, int y) const
{
    assert (x < mWidth && y < mHeight);

    int i = y * mWidth + x;
    return mData[i];
}

bool Path::IsSet(int x, int y, char flag) const
{
    return (Get(x, y) & flag) != 0;
}

void Path::Reset()
{
    Memset8(mData, 0, GetSize());
}

void Path::ResetColor()
{
    Memset8(mColor, white, mWidth * mHeight);
    m_alloc.Shutdown();
    m_qOpen.beg = m_qOpen.end = NULL;
    m_qClose.beg = m_qClose.end = NULL;
}

void Path::SetColor(int x, int y, char color)
{
    int i = y * mWidth + x;
    mColor[i] = color;
}

char Path::GetColor(int x, int y) const
{
    int i = y * mWidth + x;
    return mColor[i];
}

char * Path::GetData()
{
    return mData;
}

const char * Path::GetData() const
{
    return mData;
}


int Path::GetWidth() const
{
    return mWidth;
}

int Path::GetHeight() const
{
    return mHeight;
}

int Path::GetSize() const
{
    return mWidth * mHeight;
}

void Path::SetMaxStep(int num)
{
    mMaxStep = num;
}

int Path::GetMaxStep() const
{
    return mMaxStep;
}

void Path::FindPath(int x, int y, int dx, int dy, char stop, Result & result)
{
    ResetColor();

    // check param valid
	if (x < 0 || x >= mWidth ||
		y < 0 || y >= mHeight)
	{
		result.flag = PR_INVALID;
		result.x = -1;
		result.y = -1;
		result.dx = -1;
		result.dy = -1;
		return;
	}

    // current position is invalid.
    if (IsSet(x, y, stop))
    {
        // left
        if (x - 1 >= 0 && !IsSet(x - 1, y, stop))
        {
            x = x - 1;
        }
        // right
        else if (x + 1 < mWidth && !IsSet(x + 1, y, stop))
        {
            x = x + 1;
        }
        // top
        else if (y - 1 >= 0 && !IsSet(x, y - 1, stop))
        {
            y = y - 1;
        }
        // bottom
        else if (y + 1 < mHeight && !IsSet(x, y + 1, stop))
        {
            y = y + 1;
        }
        else
        {
            result.flag = PR_INVALID;
            result.x = -1;
            result.y = -1;
            result.dx = -1;
            result.dy = -1;
            return;
        }
    }

    // dest position out of our area.
    if(dx < 0 || dx >= mWidth ||
        dy < 0 || dy >= mHeight)
    {
        CalcuDest(x, y, dx, dy, dx, dy);
    }

    // adjust dest failed.
    if(dx < 0 || dx >= mWidth ||
        dy < 0 || dy >= mHeight)
    {
        result.flag = PR_INVALID;
        result.x = -1;
        result.y = -1;
        result.dx = -1;
        result.dy = -1;
        return;
    }

    // is reached ?
	if(x == dx && y == dy)
	{
		result.flag = PR_REACHED;
		result.x = dx;
		result.y = dy;
		result.dx = dx;
		result.dy = dy;
		return;
	}

    // adjust dest position if the dest is a stop cell.
    if (IsSet(dx, dy, stop))
    {
        if (!TestLine(dx, dy, x, y, stop, result.dx, result.dy))
        {
            result.flag = PR_NOPATH;
            result.x = -1;
            result.y = -1;
            result.dx = -1;
            result.dy = -1;
            return;
        }
    }
    else
    {
        result.dx = dx;
        result.dy = dy;
    }

    dx = result.dx;
    dy = result.dy;

    int sx = x, sy = y;

    // find path
    PushOpen(x, y, NULL);
    int count = 0;
    bool found = false;

    while(count < mMaxStep && EmptyOpen())
    {
        Node* n = PopOpen();
        x = n->x;
        y = n->y;

        PushClose(n);

        if(x == dx && y == dy)
        {
            found = true;
            break;
        }

        int rx, ry;
        if (CalcuNext(x, y, dx, dy, stop, rx, ry))
        {
            PushOpen(rx, ry, n);
        }
        else
        {
            // left
			if(x - 1 >= 0 && !IsSet(x - 1, y, stop) && GetColor(x - 1, y) == white)
			{
				PushOpen(x - 1, y, n);
			}

            // right
            if(x + 1 < mWidth && !IsSet(x + 1, y, stop) && GetColor(x + 1, y) == white)
            {
                PushOpen(x + 1, y, n);
            }

            // top
            if(y - 1 >= 0 && !IsSet(x, y - 1, stop) && GetColor(x, y - 1) == white)
            {
                PushOpen(x, y - 1, n);
            }

            // bottom
            if(y + 1 < mHeight && !IsSet(x, y + 1, stop) && GetColor(x, y + 1) == white)
            {
                PushOpen(x, y + 1, n);
            }
        }

        ++count;
    }

#if 1
    d_printf ("Steps: %d\n", count);
#endif 

    if(!found)
    {
        result.flag = PR_NOPATH;
        result.x = -1;
        result.y = -1;
        result.dx = -1;
        result.dy = -1;
        return;
    }

    for (Node * n = m_qClose.end; n != NULL; n = n->parent)
    {
        if (DrawLine(sx, sy, n->x, n->y, stop))
        {
            if (n->x == dx && n->y == dy)
                result.flag = PR_FINISHED;
            else
                result.flag = PR_UNFINISHED;

            result.x = n->x;
            result.y = n->y;

            return ;
        }
    }

    result.flag = PR_NOPATH;
    result.x = -1;
    result.y = -1;
}

bool Path::DrawLine(int sx, int sy, int ex, int ey, int stop)
{
    // test dir
    if (sx > ex)
    {
        Math::Swap(sx, ex);
        Math::Swap(sy, ey);
    }

    int dx = ex - sx;
    int dy = ey - sy;

    if (dx > Math::Abs(dy))
    {
        float k = (float)dy / dx;
        int x = sx, y = sy;

        if (k > 0)
        {
            float e = -0.5f;

            for (int i = 0; i < dx; ++i)
            {
                x = x + 1;
                e = e + k;
                if (e > 0)
                {
                    y = y + 1;
                    e = e - 1.0f;
                }

                if (IsSet(x, y, stop))
                  return false;
            }
        }
        else if (k == 0)
        {
            for (int i = 0; i < dx; ++i)
            {
                x = x + 1;

                if (IsSet(x, y, stop))
                    return false;
            }
        }
        else
        {
            float e = -0.5f;

            for (int i = 0; i < dx; ++i)
            {
                x = x + 1;
                e = e - k;
                if (e > 0)
                {
                    y = y - 1;
                    e = e - 1.0f;
                }

                if (IsSet(x, y, stop))
                    return false;
            }
        }
    }
    else
    {
        float k = (float)dx / dy;
        int x = sx, y = sy;

        if (k > 0)
        {
            float e = -0.5f;

            for (int i = 0; i < dy; ++i)
            {
                y = y + 1;
                e = e + k;
                if (e > 0)
                {
                    x = x + 1;
                    e = e - 1.0f;
                }

                if (IsSet(x, y, stop))
                    return false;
            }
        }
        else if (k == 0)
        {
            if (dy > 0)
            {
                for (int i = 0; i < dy; ++i)
                {
                    y = y + 1;

                    if (IsSet(x, y, stop))
                        return false;
                }
            }
            else
            {
                for (int i = 0; i < -dy; ++i)
                {
                    y = y - 1;

                    if (IsSet(x, y, stop))
                        return false;
                }
            }
        }
        else
        {
            float e = -0.5f;

            for (int i = 0; i < -dy; ++i)
            {
                y = y - 1;
                e = e - k;
                if (e > 0)
                {
                    x = x + 1;
                    e = e - 1.0f;
                }

                if (IsSet(x, y, stop))
                    return false;
            }
        }
    }

    return true;
}

bool Path::TestLine(int sx, int sy, int ex, int ey, int stop, int & _dx, int & _dy)
{
    _dx = -1;
    _dy = -1;

    // test dir
    if(ex > sx && ey > sy)
    {
        int dx = ex - sx;
        int dy = ey - sy;
        int x = sx, y = sy;

        if(dx > dy)
        {
            float k = (float)dy / dx;
            float e = -0.5f;

            for(int i = 0; i < dx; ++i)
            {
                x = x + 1;
                e = e + k;
                if(e > 0)
                {
                    y = y + 1;
                    e = e - 1.0f;
                }

                if(!IsSet(x, y, stop))
                {
                    _dx = x;
                    _dy = y;
                    return true;
                }
            }
        }
        else
        {
            float k = (float)dx / dy;
            float e = -0.5f;

            for(int i = 0; i < dy; ++i)
            {
                y = y + 1;
                e = e + k;
                if(e > 0)
                {
                    x = x + 1;
                    e = e - 1.0f;
                }

                if(!IsSet(x, y, stop))
                {
                    _dx = x;
                    _dy = y;
                    return true;
                }
            }
        }
    }
    else if (ey == sy)
    {
        int dx = ex - sx;
        int dy = ey - sy;
        int x = sx, y = sy;

        if (dx > 0)
        {
            for(int i = 0; i < dx; ++i)
            {
                x = x + 1;

                if(!IsSet(x, y, stop))
                {
                    _dx = x;
                    _dy = y;
                    return true;
                }
            }
        }
        else
        {
            for(int i = 0; i < -dx; ++i)
            {
                x = x - 1;

                if(!IsSet(x, y, stop))
                {
                    _dx = x;
                    _dy = y;
                    return true;
                }
            }
        }

    }
    else if (ex == sx)
    {
        int dx = ex - sx;
        int dy = ey - sy;
        int x = sx, y = sy;

        if(dy > 0)
        {
            for(int i = 0; i < dy; ++i)
            {
                y = y + 1;

                if(!IsSet(x, y, stop))
                {
                    _dx = x;
                    _dy = y;
                    return true;
                }
            }
        }
        else
        {
            for(int i = 0; i < -dy; ++i)
            {
                y = y - 1;

                if(!IsSet(x, y, stop))
                {
                    _dx = x;
                    _dy = y;
                    return true;
                }
            }
        }
    }
    else if (ex > sx && ey < sy)
    {
        int dx = ex - sx;
        int dy = ey - sy; // < 0
        int x = sx, y = sy;

        if(dx > -dy)
        {
            float k = (float)dy / dx;
            float e = -0.5f;

            for(int i = 0; i < dx; ++i)
            {
                x = x + 1;
                e = e - k;
                if(e > 0)
                {
                    y = y - 1;
                    e = e - 1.0f;
                }

                if(!IsSet(x, y, stop))
                {
                    _dx = x;
                    _dy = y;
                    return true;
                }
            }
        }
        else
        {
            float k = (float)dx / dy;
            float e = -0.5f;

            for(int i = 0; i < -dy; ++i)
            {
                y = y - 1;
                e = e - k;
                if(e > 0)
                {
                    x = x + 1;
                    e = e - 1.0f;
                }

                if(!IsSet(x, y, stop))
                {
                    _dx = x;
                    _dy = y;
                    return true;
                }
            }

        }
    }
    else if (ex < sx && ey > sy)
    {
        int dx = ex - sx;  // < 0
        int dy = ey - sy;
        int x = sx, y = sy;

        if(-dx > dy)
        {
            float k = (float)dy / dx;
            float e = -0.5f;

            for(int i = 0; i < -dx; ++i)
            {
                x = x - 1;
                e = e - k;
                if(e > 0)
                {
                    y = y + 1;
                    e = e - 1.0f;
                }

                if(!IsSet(x, y, stop))
                {
                    _dx = x;
                    _dy = y;
                    return true;
                }
            }
        }
        else
        {
            float k = (float)dx / dy;
            float e = -0.5f;

            for(int i = 0; i < dy; ++i)
            {
                y = y + 1;
                e = e - k;
                if(e > 0)
                {
                    x = x - 1;
                    e = e - 1.0f;
                }

                if(!IsSet(x, y, stop))
                {
                    _dx = x;
                    _dy = y;
                    return true;
                }
            }
        }
    }
    else
    {
        int dx = ex - sx;  // < 0
        int dy = ey - sy;
        int x = sx, y = sy;

        if(-dx > -dy)
        {
            float k = (float)dy / dx;
            float e = -0.5f;

            for(int i = 0; i < -dx; ++i)
            {
                x = x - 1;
                e = e + k;
                if(e > 0)
                {
                    y = y - 1;
                    e = e - 1.0f;
                }

                if(!IsSet(x, y, stop))
                {
                    _dx = x;
                    _dy = y;
                    return true;
                }
            }

        }
        else
        {
            float k = (float)dx / dy;
            float e = -0.5f;

            for(int i = 0; i < -dy; ++i)
            {
                y = y - 1;
                e = e + k;
                if(e > 0)
                {
                    x = x - 1;
                    e = e - 1.0f;
                }

                if(!IsSet(x, y, stop))
                {
                    _dx = x;
                    _dy = y;
                    return true;
                }
            }
        }
    }

    return false;
}

void Path::CalcuDest(int sx, int sy, int ex, int ey, int & ox, int & oy)
{
    if (ex == sx)
    {
        if (ey > sy)
        {
            ox = sx;
            oy = mHeight - 1;

            return ;
        }
        else
        {
            ox = sx;
            oy = 0;

            return ;
        }
    }
    else if (ey == sy)
    {
        if (ex > sx)
        {
            ox = mWidth - 1;
            oy = sy;

            return ;
        }
        else
        {
            ox = 0;
            oy = sy;

            return ;
        }
    }
    else
    {
        int dx = ex - sx;
        int dy = ey - sy;

        float k = (float)dy / dx;
        float b = sy - k * sx;

        int x = -1, y = -1;

        if (ex > sx)
        {
            y = (int)(k * (mWidth - 1) + b);

            if (y >= 0 && y < mHeight)
            {
                ox = mWidth - 1;
                oy = y;
                return ;
            }
        }
        else
        {
            y = (int)(k * 0 + b);

            if (y >= 0 && y < mHeight)
            {
                ox = 0;
                oy = y;
                return ;
            }
        }

        if (ey > sy)
        {
            int x = (int)((mHeight - 1 - b) / k);

            if (x >= 0 && x < mWidth)
            {
                ox = x;
                oy = mHeight - 1;
                return ;
            }
        }
        else
        {
            int x = (int)((0 - b) / k);

            if (x >= 0 && x < mWidth)
            {
                ox = x;
                oy = 0;
                return ;
            }
        }
    }

    ox = -1;
    oy = -1;
}

bool Path::CalcuNext(int x, int y, int ex, int ey, int stop, int & ox, int & oy)
{
    int dx = abs(ex - x);
    int dy = abs(ey - y);

    if (dx > dy)
    {
        if (ex < x)
        {
            // left
            if(x - 1 >= 0 && !IsSet(x - 1, y, stop) && GetColor(x - 1, y) == white)
            {
                ox = x - 1;
                oy = y;

                return true;
            }
        }
        else if (ex > x)
        {
            // right
            if(x + 1 < mWidth && !IsSet(x + 1, y, stop) && GetColor(x + 1, y) == white)
            {
                ox = x + 1;
                oy = y;

                return true;
            }
        }

        if (ey < y)
        {
            // top
            if(y - 1 >= 0 && !IsSet(x, y - 1, stop) && GetColor(x, y - 1) == white)
            {
                ox = x;
                oy = y - 1;

                return true;
            }
        }
        else if (ey > y)
        {
            // bottom
            if(y + 1 < mHeight && !IsSet(x, y + 1, stop) && GetColor(x, y + 1) == white)
            {
                ox = x;
                oy = y + 1;

                return true;
            }
        }
    }
    else
    {
        if (ey < y)
        {
            // top
            if(y - 1 >= 0 && !IsSet(x, y - 1, stop) && GetColor(x, y - 1) == white)
            {
                ox = x;
                oy = y - 1;

                return true;
            }
        }
        else if (ey > y)
        {
            // bottom
            if(y + 1 < mHeight && !IsSet(x, y + 1, stop) && GetColor(x, y + 1) == white)
            {
                ox = x;
                oy = y + 1;

                return true;
            }
        }

        if (ex < x)
        {
            // left
            if(x - 1 >= 0 && !IsSet(x - 1, y, stop) && GetColor(x - 1, y) == white)
            {
                ox = x - 1;
                oy = y;

                return true;
            }
        }
        else if (ex > x)
        {
            // right
            if(x + 1 < mWidth && !IsSet(x + 1, y, stop) && GetColor(x + 1, y) == white)
            {
                ox = x + 1;
                oy = y;

                return true;
            }
        }
    }

    return false;
}

void Path::PushOpen(int x, int y, Path::Node * parent)
{
    Node * n = AllocNode();
    n->x = x;
    n->y = y;
    n->parent = parent;
    SetColor(x, y, gray);

    if (EmptyOpen())
    {
        m_qOpen.beg = m_qOpen.end = n;
    }
    else
    {
        m_qOpen.end->next = n;
        m_qOpen.end = n;
    }
}

Path::Node * Path::PopOpen()
{
    Node * n = NULL;

    if (m_qOpen.beg)
    {
        n = m_qOpen.beg;
        m_qOpen.beg = n->next;

        SetColor(n->x, n->y, black);

        if (EmptyOpen())
        {
            m_qOpen.beg = m_qOpen.end = NULL;
        }
    }

    return n;
}

bool Path::EmptyOpen() const
{
    return m_qOpen.beg == NULL;
}



void Path::PushClose(Path::Node * n)
{
    if (EmptyClose())
    {
        m_qClose.beg = m_qClose.end = n;
    }
    else
    {
        m_qClose.end->next = n;
        m_qClose.end = n;
    }
}

Path::Node * Path::PopClose()
{
    Node * n = NULL;

    if (m_qOpen.beg)
    {
        n = m_qOpen.beg;
        m_qOpen.beg = n->next;

        if (EmptyClose())
        {
            m_qClose.beg = m_qClose.end = NULL;
        }
    }

    return NULL;
}

bool Path::EmptyClose() const
{
    return m_qClose.beg == NULL;
}


Path::Node * Path::AllocNode()
{
    return m_alloc.Alloc();
}

void Path::FreeNode(Path::Node * n)
{
    m_alloc.Free(n);
}

}
