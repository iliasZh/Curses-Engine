#pragma once

#include "Expression.h"

class Global
{
public:
	static constexpr float frametime = 16.67f;
	Lang lang = Lang::EN;
};

inline Global global;