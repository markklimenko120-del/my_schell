#include <stddef.h>
#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <stdlib.h>
#include <string.h>
#include "config_pars.h"

static config *
parsing_config(const char *cfg_path)
{
    config *cfg = malloc(sizeof(config));
    lua_State *L = luaL_newstate();
    luaopen_base(L);
    luaopen_table(L);
    int error = luaL_dofile(L, cfg_path);
    if (error)
    {
        fprintf(stderr,  "LUA: %s\n", lua_tostring(L, -1));
        return NULL;
    }
    lua_getglobal(L, "config");

    /*Buffers*/
    lua_getfield(L, -1, "buff_sizes");

    /*Pwd Buff*/
    lua_getfield(L, -1, "pwd_buff");
    cfg->pwd_buff = lua_isnumber(L, -1)  ? (size_t)lua_tonumber(L, -1) : 50;
    lua_pop(L, 1);

    /*Prompt Buff*/
    lua_getfield(L, -1, "prompt_buff");
    cfg->prompt_buff = lua_isnumber(L, -1) ? (size_t)lua_tonumber(L, -1) : 100;
    lua_pop(L, 1);

    lua_close(L);
    return cfg;
}

void
search_config(config *cfg)
{ 
    char *path = getenv("HOME");
    char *def_config = getenv("XDG_CONFIG_HOME");
    if (!def_config || *def_config == '\0')
    {
        strcat(path, "/.config/my_sh/config.lua");
    }
    else  
    {
        strcat(path, def_config);
        strcat(path, "my_sh/config");
    }
    cfg = parsing_config(path);
    if (cfg == NULL)
    {
        return ;
    }
    return ;
}
