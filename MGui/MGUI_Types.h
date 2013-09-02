//////////////////////////////////////////////////////////////////////////
//
// MGUI
//   GUI Engine For Myway
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//
#pragma once

#include "Myway.h"
#include "MGUI_Entry.h"

#pragma warning(push)
#pragma warning(disable : 4996)

namespace Myway {

	typedef Color4 MGUI_Color;

	struct MGUI_Size
	{
		int w, h;

		MGUI_Size() : w(0), h(0) {}
	};

	struct MGUI_ENTRY MGUI_Rect
	{
		int x0, y0;
		int x1, y1;

		MGUI_Rect() : x0(0), y0(0), x1(0), y1(0) {}
		MGUI_Rect(int _x0, int _y0, int _x1, int _y1)
		{
			x0 = _x0; y0 = _y0;
			x1 = _x1; y1 = _y1;
		}

		MGUI_Rect(bool xywh, int _x, int _y, int _w, int _h)
		{
			x0 = _x; y0 = _y;
			x1 = _x + _w - 1; y1 = _y + _h - 1;
		}

		void Set(int _x0, int _y0, int _x1, int _y1)
		{
			x0 = _x0; y0 = _y0;
			x1 = _x1; y1 = _y1;
		}

		int DX() const { return x1 - x0; }
		int DY() const { return y1 - y0; }

		int CenterX() const { return (x0 + x1) / 2; }
		int CenterY() const { return (y0 + y1) / 2; }

		TString128 ToString() const
		{
			TString128 str;

			str.Format("%d,%d,%d,%d", x0, y0, x1, y1);

			return str;
		}

		void FromString(const TString128 & _string)
		{
			sscanf(_string.c_str(), "%d,%d,%d,%d", x0, y0, x1, y1);
		}

		TString128 ToString_XYWH() const
		{
			TString128 str;

			str.Format("%d,%d,%d,%d", x0, y0, DX() + 1, DY() + 1);

			return str;
		}

		void FromString_XYWH(const TString128 & _string)
		{
			int w = 0, h = 0;

			sscanf(_string.c_str(), "%d,%d,%d,%d", &x0, &y0, &w, &h);

			x1 = x0 + w - 1;
			y1 = y0 + h - 1;
		}
	};

	struct MGUI_RectF
	{
		float x0, y0;
		float x1, y1;

		MGUI_RectF() : x0(0), y0(0), x1(0), y1(0) {}
		MGUI_RectF(float _x0, float _y0, float _x1, float _y1)
		{
			x0 = _x0; y0 = _y0;
			x1 = _x1; y1 = _y1;
		}

		void Set(float _x0, float _y0, float _x1, float _y1)
		{
			x0 = _x0; y0 = _y0;
			x1 = _x1; y1 = _y1;
		}

		float DX() const { return x1 - x0; }
		float DY() const { return y1 - y0; }

		float CenterX() const { return (x0 + x1) / 2; }
		float CenterY() const { return (y0 + y1) / 2; }
	};

	struct MGUI_Align
	{
		enum eEnum
		{
			None,

			HCenter = 1,
			VCenter = 2,
			Center = HCenter | VCenter,

			Left =  4,
			Right = 8,
			HStretch = 16,

			Top = 32,
			Bottom = 64,
			VStretch = 128,

			Stretch = HStretch | VStretch,
		};

		int _value;

		MGUI_Align() : _value(None) {}
		MGUI_Align(int val) : _value(val) {}

		MGUI_Align & operator =(int rk)
		{
			_value = rk;
			return *this;
		}

		bool operator ==(int rk) const
		{
			return _value == rk;
		}

		bool operator !=(int rk) const
		{
			return _value != rk;
		}

		bool operator ==(const MGUI_Align & rk) const
		{
			return _value == rk._value;
		}

		bool operator !=(const MGUI_Align & rk) const
		{
			return _value != rk._value;
		}
	};

	struct MGUI_Vertex 
	{
		float x, y;
		Color4 color;
		float u, v;
	};

	typedef int MGUI_Char;

	struct MGUI_ENTRY MGUI_MouseButton
	{
		enum Enum
		{
			None = -1,

			Left = 0,
			Right,
			Middle,

			MAX
		};

		MGUI_MouseButton(Enum _value = None) :
			value(_value)
			{
			}

			friend bool operator == (MGUI_MouseButton const& a, MGUI_MouseButton const& b)
			{
				return a.value == b.value;
			}

			friend bool operator != (MGUI_MouseButton const& a, MGUI_MouseButton const& b)
			{
				return a.value != b.value;
			}

			int ToValue() const
			{
				return (int)value;
			}

	private:
		Enum value;
	};


	struct MGUI_ENTRY MGUI_KeyCode
	{
		enum Enum
		{
			None          = 0x00,
			Escape          = 0x01,
			One             = 0x02,
			Two             = 0x03,
			Three           = 0x04,
			Four            = 0x05,
			Five            = 0x06,
			Six             = 0x07,
			Seven           = 0x08,
			Eight           = 0x09,
			Nine            = 0x0A,
			Zero            = 0x0B,
			Minus           = 0x0C,    /* - on main keyboard */
			Equals			= 0x0D,
			Backspace		= 0x0E,    /* Backspace */
			Tab				= 0x0F,
			Q               = 0x10,
			W               = 0x11,
			E               = 0x12,
			R               = 0x13,
			T               = 0x14,
			Y               = 0x15,
			U               = 0x16,
			I               = 0x17,
			O               = 0x18,
			P               = 0x19,
			LeftBracket     = 0x1A,
			RightBracket    = 0x1B,
			Return			= 0x1C,    /* Enter on main keyboard */
			LeftControl		= 0x1D,
			A               = 0x1E,
			S               = 0x1F,
			D               = 0x20,
			F               = 0x21,
			G               = 0x22,
			H               = 0x23,
			J               = 0x24,
			K               = 0x25,
			L               = 0x26,
			Semicolon       = 0x27,
			Apostrophe		= 0x28,
			Grave           = 0x29,    /* Accent grave ( ~ ) */
			LeftShift       = 0x2A,
			Backslash       = 0x2B,
			Z               = 0x2C,
			X               = 0x2D,
			C               = 0x2E,
			V               = 0x2F,
			B               = 0x30,
			N               = 0x31,
			M               = 0x32,
			Comma           = 0x33,
			Period          = 0x34,    /* . on main keyboard */
			Slash           = 0x35,    /* '/' on main keyboard */
			RightShift      = 0x36,
			Multiply        = 0x37,    /* * on numeric keypad */
			LeftAlt        = 0x38,    /* Left Alt */
			Space           = 0x39,
			Capital         = 0x3A,
			F1              = 0x3B,
			F2              = 0x3C,
			F3              = 0x3D,
			F4              = 0x3E,
			F5              = 0x3F,
			F6              = 0x40,
			F7              = 0x41,
			F8              = 0x42,
			F9              = 0x43,
			F10             = 0x44,
			NumLock         = 0x45,
			ScrollLock      = 0x46,    /* Scroll Lock */
			Numpad7         = 0x47,
			Numpad8         = 0x48,
			Numpad9         = 0x49,
			Subtract        = 0x4A,    /* - on numeric keypad */
			Numpad4         = 0x4B,
			Numpad5         = 0x4C,
			Numpad6         = 0x4D,
			Add				= 0x4E,    /* + on numeric keypad */
			Numpad1         = 0x4F,
			Numpad2         = 0x50,
			Numpad3         = 0x51,
			Numpad0         = 0x52,
			Decimal			= 0x53,    /* . on numeric keypad */
			OEM_102         = 0x56,    /* < > | on UK/Germany keyboards */
			F11             = 0x57,
			F12             = 0x58,
			F13             = 0x64,    /*                     (NEC PC98) */
			F14             = 0x65,    /*                     (NEC PC98) */
			F15             = 0x66,    /*                     (NEC PC98) */
			Kana            = 0x70,    /* (Japanese keyboard)            */
			ABNT_C1         = 0x73,    /* / ? on Portugese (Brazilian) keyboards */
			Convert         = 0x79,    /* (Japanese keyboard)            */
			NoConvert       = 0x7B,    /* (Japanese keyboard)            */
			Yen             = 0x7D,    /* (Japanese keyboard)            */
			ABNT_C2         = 0x7E,    /* Numpad . on Portugese (Brazilian) keyboards */
			NumpadEquals    = 0x8D,    /* = on numeric keypad (NEC PC98) */
			PrevTrack       = 0x90,    /* Previous Track (KC_CIRCUMFLEX on Japanese keyboard) */
			At              = 0x91,    /*                     (NEC PC98) */
			Colon           = 0x92,    /*                     (NEC PC98) */
			Underline       = 0x93,    /*                     (NEC PC98) */
			Kanji           = 0x94,    /* (Japanese keyboard)            */
			Stop            = 0x95,    /*                     (NEC PC98) */
			AX              = 0x96,    /*                     (Japan AX) */
			Unlabeled       = 0x97,    /*                        (J3100) */
			NextTrack       = 0x99,    /* Next Track */
			NumpadEnter     = 0x9C,    /* Enter on numeric keypad */
			RightControl    = 0x9D,
			Mute            = 0xA0,
			Calculator      = 0xA1,
			PlayPause       = 0xA2,    /* Play / Pause */
			MediaStop       = 0xA4,    /* Media Stop */
			VolumeDown      = 0xAE,    /* Volume - */
			VolumeUp        = 0xB0,    /* Volume + */
			WebHome         = 0xB2,    /* Web home */
			NumpadComma     = 0xB3,    /* , on numeric keypad (NEC PC98) */
			Divide          = 0xB5,    /* / on numeric keypad */
			SysRq           = 0xB7,
			RightAlt        = 0xB8,    /* Right Alt */
			Pause           = 0xC5,
			Home            = 0xC7,    /* Home on arrow keypad */
			ArrowUp         = 0xC8,    /* UpArrow on arrow keypad */
			PageUp          = 0xC9,    /* PgUp on arrow keypad */
			ArrowLeft       = 0xCB,    /* LeftArrow on arrow keypad */
			ArrowRight      = 0xCD,    /* RightArrow on arrow keypad */
			End             = 0xCF,    /* End on arrow keypad */
			ArrowDown       = 0xD0,    /* DownArrow on arrow keypad */
			PageDown		= 0xD1,    /* PgDn on arrow keypad */
			Insert          = 0xD2,    /* Insert on arrow keypad */
			Delete          = 0xD3,    /* Delete on arrow keypad */
			LeftWindows     = 0xDB,    /* Left Windows key */
			RightWindow     = 0xDC,    /* Right Windows key */
			RightWindows    = 0xDC,    /* Right Windows key - Correct spelling :) */
			AppMenu         = 0xDD,    /* AppMenu key */
			Power           = 0xDE,    /* System Power */
			Sleep           = 0xDF,    /* System Sleep */
			Wake			= 0xE3,    /* System Wake */
			WebSearch		= 0xE5,
			WebFavorites	= 0xE6,
			WebRefresh		= 0xE7,
			WebStop			= 0xE8,
			WebForward		= 0xE9,
			WebBack			= 0xEA,
			MyComputer		= 0xEB,
			Mail			= 0xEC,
			MediaSelect		= 0xED
		};

		MGUI_KeyCode(Enum _value = None) :
			value(_value)
			{
			}

			friend bool operator < (MGUI_KeyCode const& a, MGUI_KeyCode const& b)
			{
				return (a.value < b.value);
			}

			friend bool operator >= (MGUI_KeyCode const& a, MGUI_KeyCode const& b)
			{
				return !(a < b);
			}

			friend bool operator > (MGUI_KeyCode const& a, MGUI_KeyCode const& b)
			{
				return (b < a);
			}

			friend bool operator <= (MGUI_KeyCode const& a, MGUI_KeyCode const& b)
			{
				return !(a > b);
			}

			friend bool operator == (MGUI_KeyCode const& a, MGUI_KeyCode const& b)
			{
				return !(a < b) && !(a > b);
			}

			friend bool operator != (MGUI_KeyCode const& a, MGUI_KeyCode const& b)
			{
				return !(a == b);
			}

			int ToValue() const
			{
				return (int)value;
			}

	private:
		Enum value;
	};


	struct MGUI_ENTRY MGUI_WidgetState {

		enum {
			Normal,
			Disabled,
			Focused,
			Pressed,
			Selected,

			Max,
		};

		static const TString128 StateString[];

		static int GetState(const TString128 & _state);
		static const TString128 & GetStateString(int _state); 
	};


	struct MGUI_ENTRY MGUI_WidgetType {

		enum {
			Button,
			CheckBox,
			ComboBox,
			EditBox,
			ImageBox,
			Label,
			ListBox,
			Panel,
			ProgressBar,
			ScrollBar,
			Slider,

			Reserved0,
			Reserved1,
			Reserved2,
			Reserved3,
			Reserved4,
			Reserved5,

			User,

			Max,
		};

		static const TString128 TypeString[Max];

		static int GetType(const TString128 & _type);
		static const TString128 & GetTypeString(int _type);
	};
}

#pragma warning(pop)