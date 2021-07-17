#pragma once

#include "Expression.h"

class Global
{
public:
	static constexpr float frametime = 20.0f;
	Lang lang = Lang::EN;
};

inline Global global;