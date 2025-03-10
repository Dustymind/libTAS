/*
    Copyright 2015-2024 Clément Gallet <clement.gallet@ens-lyon.org>

    This file is part of libTAS.

    libTAS is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libTAS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libTAS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Callbacks.h"
#include "Main.h"
#include "NamedLuaFunction.h"
#include "LuaFunctionList.h"

#include "Context.h"

#include <iostream>
extern "C" {
#include <lua5.4/lua.h>
#include <lua5.4/lauxlib.h>
}

namespace Lua {

/* List of functions to register */
static const luaL_Reg callback_functions[] =
{
    { "onStartup", Callbacks::onStartup},
    { "onInput", Callbacks::onInput},
    { "onFrame", Callbacks::onFrame},
    { "onPaint", Callbacks::onPaint},
    { NULL, NULL }
};

void Callbacks::registerFunctions(lua_State *L)
{
    luaL_newlib(L, callback_functions);
    lua_setglobal(L, "callback");
}

void Callbacks::clear()
{
    getList().clear();
}

int Callbacks::onStartup(lua_State *L)
{
    getList().add(L, NamedLuaFunction::CallbackStartup);
    return 0;
}

int Callbacks::onInput(lua_State *L)
{
    getList().add(L, NamedLuaFunction::CallbackInput);
    return 0;
}

int Callbacks::onFrame(lua_State *L)
{
    getList().add(L, NamedLuaFunction::CallbackFrame);
    return 0;
}

int Callbacks::onPaint(lua_State *L)
{
    getList().add(L, NamedLuaFunction::CallbackPaint);
    return 0;
}

bool Callbacks::call(NamedLuaFunction::CallbackType type)
{
    return getList().call(type);
}

LuaFunctionList& Callbacks::getList()
{
    static LuaFunctionList lfl;
    return lfl;
}

}
