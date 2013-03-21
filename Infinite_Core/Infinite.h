//////////////////////////////////////////////////////////////////////////
//
// Infinite. Scene Editor For Myway3D
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
//
#pragma once

#ifdef INFI_EXPORT

#define INFI_ENTRY __declspec(dllexport)

#else

#define INFI_ENTRY __declspec(dllimport)

#endif


enum eTransformOperator
{
	eTO_Unknown,
	eTO_Move,
	eTO_Rotate,
	eTO_Scale,
};

enum ePick
{
	PICK_Flag = 0x01,
};

enum eAppMode
{
	eAM_Editor,
	eAM_Game,
};

namespace Infinite {

	struct Utils
	{
		static inline MyGUI::Colour Color4To(const Color4 & clr)
		{
			MyGUI::Colour result;

			result.red = clr.r;
			result.green = clr.g;
			result.blue = clr.b;
			result.alpha = clr.a;

			return result;
		}


		static inline Color4 ToColor4(const MyGUI::Colour & clr)
		{
			Color4 result;

			result.r = clr.red;
			result.g = clr.green;
			result.b = clr.blue;
			result.a = clr.alpha;

			return result;
		}

		static bool IsMouseDoubleClick(MouseKeyCode kc)
		{
			static int s_count = 0;
			static float s_time = 0;

			if (IMouse::Instance()->KeyDown(kc))
			{
				s_count++;

				float time  = Engine::Instance()->GetTime();
				if (time - s_time > 0.2f)
					s_count = 1;
			}

			if (s_count == 1)
			{
				s_time = Engine::Instance()->GetTime();
			}

			if (s_count == 2 && IMouse::Instance()->KeyUp(kc))
			{
				float time  = Engine::Instance()->GetTime();

				if (time - s_time < 0.2f)
				{
					s_count = 0;
					s_time = 0;
					return true;
				}
			}

			if (s_count > 2)
			{
				s_count = 0;
				s_time = 0;
			}

			return false;
		}


		static const char * ToString(float f)
		{
			static char buffer[32];
			
			sprintf_s(buffer, 32, "%2f", f);

			return buffer;
		}

		static const char * ToString(int i)
		{
			static char buffer[32];

			sprintf_s(buffer, 32, "%d", i);

			return buffer;
		}

		static float ToFloat(const char * str)
		{
			return (float)atof(str);
		}

		static int ToInt(const char * str)
		{
			return atoi(str);
		}
	};

}