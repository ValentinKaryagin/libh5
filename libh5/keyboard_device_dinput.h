#pragma once

#include "input_device_dinput.h"
#include "keyboard_keys_dinput.h"

namespace h5
{
	class application_window_windows;

	class keyboard_device_dinput
		: public input_device_dinput
	{

	public:
		enum class key
			: size_t
		{
			escape = DINPUT_ESCAPE,
			num1 = DINPUT_1,
			num2 = DINPUT_2,
			num3 = DINPUT_3,
			num4 = DINPUT_4,
			num5 = DINPUT_5,
			num6 = DINPUT_6,
			num7 = DINPUT_7,
			num8 = DINPUT_8,
			num9 = DINPUT_9,
			num0 = DINPUT_0,
			minus = DINPUT_MINUS,
			equals = DINPUT_EQUALS,
			backspace = DINPUT_BACK,
			tab = DINPUT_TAB,
			q = DINPUT_Q,
			w = DINPUT_W,
			e = DINPUT_E,
			r = DINPUT_R,
			t = DINPUT_T,
			y = DINPUT_Y,
			u = DINPUT_U,
			i = DINPUT_I,
			o = DINPUT_O,
			p = DINPUT_P,
			lbracket = DINPUT_LBRACKET,
			rbtacket = DINPUT_RBRACKET,
			enter = DINPUT_RETURN,
			lcontrol = DINPUT_LCONTROL,
			a = DINPUT_A,
			s = DINPUT_S,
			d = DINPUT_D,
			f = DINPUT_F,
			g = DINPUT_G,
			h = DINPUT_H,
			j = DINPUT_J,
			k = DINPUT_K,
			l = DINPUT_L,
			semicolon = DINPUT_SEMICOLON,
			apostrophe = DINPUT_APOSTROPHE,
			grave = DINPUT_GRAVE,
			lshift = DINPUT_LSHIFT,
			backslash = DINPUT_BACKSLASH,
			z = DINPUT_Z,
			x = DINPUT_X,
			c = DINPUT_C,
			v = DINPUT_V,
			b = DINPUT_B,
			n = DINPUT_N,
			m = DINPUT_M,
			comma = DINPUT_COMMA,
			period = DINPUT_PERIOD,
			slash = DINPUT_SLASH,
			rshift = DINPUT_RSHIFT,
			numpadstar = DINPUT_MULTIPLY,
			lalt = DINPUT_LMENU,
			space = DINPUT_SPACE,
			capslock = DINPUT_CAPITAL,
			f1 = DINPUT_F1,
			f2 = DINPUT_F2,
			f3 = DINPUT_F3,
			f4 = DINPUT_F4,
			f5 = DINPUT_F5,
			f6 = DINPUT_F6,
			f7 = DINPUT_F7,
			f8 = DINPUT_F8,
			f9 = DINPUT_F9,
			f10 = DINPUT_F10,
			numlock = DINPUT_NUMLOCK,
			scrolllock = DINPUT_SCROLL,
			numpad7 = DINPUT_NUMPAD7,
			numpad8 = DINPUT_NUMPAD8,
			numpad9 = DINPUT_NUMPAD9,
			numpadminus = DINPUT_SUBTRACT,
			numpad4 = DINPUT_NUMPAD4,
			numpad5 = DINPUT_NUMPAD5,
			numpad6 = DINPUT_NUMPAD6,
			numpadplus = DINPUT_ADD,
			numpad1 = DINPUT_NUMPAD1,
			numpad2 = DINPUT_NUMPAD2,
			numpad3 = DINPUT_NUMPAD3,
			numpad0 = DINPUT_NUMPAD0,
			numpadperiod = DINPUT_DECIMAL,
			oem102 = DINPUT_OEM_102,
			f11 = DINPUT_F11,
			f12 = DINPUT_F12,
			f13 = DINPUT_F13,
			f14 = DINPUT_F14,
			f15 = DINPUT_F15,
			kana = DINPUT_KANA,
			abntc1 = DINPUT_ABNT_C1,
			convert = DINPUT_CONVERT,
			noconvert = DINPUT_NOCONVERT,
			yen = DINPUT_YEN,
			abntc2 = DINPUT_ABNT_C2,
			numpadequals = DINPUT_NUMPADEQUALS,
			prevtrack = DINPUT_PREVTRACK,
			at = DINPUT_AT,
			colon = DINPUT_COLON,
			underline = DINPUT_UNDERLINE,
			kanji = DINPUT_KANJI,
			stop = DINPUT_STOP,
			ax = DINPUT_AX,
			unlabeled = DINPUT_UNLABELED,
			nexttrack = DINPUT_NEXTTRACK,
			numpadenter = DINPUT_NUMPADENTER,
			rcontrol = DINPUT_RCONTROL,
			mute = DINPUT_MUTE,
			calculator = DINPUT_CALCULATOR,
			playpause = DINPUT_PLAYPAUSE,
			mediastop = DINPUT_MEDIASTOP,
			volumedown = DINPUT_VOLUMEDOWN,
			volumeup = DINPUT_VOLUMEUP,
			webhome = DINPUT_WEBHOME,
			numpadcomma = DINPUT_NUMPADCOMMA,
			numpadslash = DINPUT_DIVIDE,
			sysrq = DINPUT_SYSRQ,
			ralt = DINPUT_RMENU,
			pause = DINPUT_PAUSE,
			home = DINPUT_HOME,
			up = DINPUT_UP,
			pageup = DINPUT_PRIOR,
			left = DINPUT_LEFT,
			right = DINPUT_RIGHT,
			end = DINPUT_END,
			down = DINPUT_DOWN,
			pagedown = DINPUT_NEXT,
			insert = DINPUT_INSERT,
			del = DINPUT_DELETE,
			lcommand = DINPUT_LWIN,
			rcommand = DINPUT_RWIN,
			apps = DINPUT_APPS,
			power = DINPUT_POWER,
			sleep = DINPUT_SLEEP,
			wake = DINPUT_WAKE,
			websearch = DINPUT_WEBSEARCH,
			webfavorites = DINPUT_WEBFAVORITES,
			webrefresh = DINPUT_WEBREFRESH,
			webstop = DINPUT_WEBSTOP,
			webforward = DINPUT_WEBFORWARD,
			webback = DINPUT_WEBBACK,
			mycomputer = DINPUT_MYCOMPUTER,
			mail = DINPUT_MAIL,
			mediaselect = DINPUT_MEDIASELECT,
			reserved238,
			reserved239,
			reserved240,
			reserved241,
			reserved242,
			reserved243,
			reserved244,
			reserved245,
			reserved246,
			reserved247,
			reserved248,
			reserved249,
			reserved250,
			reserved251,
			reserved252,
			reserved253,
			reserved254,
			reserved255,
			max
		};

	protected:
		uint8_t state[(size_t)key::max] = {};
		event events[(size_t)key::max] = {};

	public:
		keyboard_device_dinput();
		virtual ~keyboard_device_dinput();

	public:
		int32_t init(const application_window_windows *window);
		int32_t init(const application_window_windows *window, const guid &id, const std::wstring &name);

	public:
		int32_t read() override;
		void clear_state() override;

	public:
		bool is_event(key key, event event);
	};	
}
