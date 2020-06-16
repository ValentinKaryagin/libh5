#pragma once

#include "commons.h"

namespace h5
{
	struct colorf_rgba
	{
		float r, g, b, a;

		__forceinline colorf_rgba()
			: r(0.0f)
			, g(0.0f)
			, b(0.0f)
			, a(1.0f)
		{}

		__forceinline colorf_rgba(float r, float g, float b, float a = 1.0f)
			: r(r)
			, g(g)
			, b(b)
			, a(a)
		{}

		__forceinline ~colorf_rgba() {}

		static const colorf_rgba white;
		static const colorf_rgba red;
		static const colorf_rgba green;
		static const colorf_rgba blue;
		static const colorf_rgba cyan;
		static const colorf_rgba magenta;
		static const colorf_rgba yellow;
		static const colorf_rgba black;
	};
}
