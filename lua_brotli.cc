#include <lua.hpp>

#include "brotli/dec/decode.h"
#include "brotli/enc/encode.h"
#include "brotli/enc/streams.h"

static void luaL_fieldinteger(lua_State *L, int table_index, const char *field_name, int &value)
{
  lua_getfield(L, table_index, field_name);
  int type = lua_type(L, -1);
  if (type != LUA_TNIL)
  {
    if (type != LUA_TNUMBER) luaL_error(L, "field '%s' must be a number", field_name);
    value = lua_tointeger(L, -1);
  }
  lua_pop(L, 1);
}

class MemReader : public brotli::BrotliIn
{
public:
  MemReader(const void *data, size_t len) : data_((const uint8_t *)data), len_(len) { }
  virtual ~MemReader() { }

  virtual const void* Read(size_t n, size_t* nread)
  {
    const void *d = data_;
    if (n >= len_)
    {
      *nread = len_;
      data_ = NULL;
      len_ = 0;
    }
    else
    {
      *nread = n;
      data_ += n;
      len_ -= n;
    }
    return d;
  }
private:
  const uint8_t *data_;
  size_t len_;
};

class LuaWriter : public brotli::BrotliOut
{
public:
  LuaWriter(luaL_Buffer *b_) : b(b_) { }
  virtual ~LuaWriter() { }

  virtual bool Write(const void *buf, size_t n)
  {
    luaL_addlstring(b, (const char *)buf, n);
    return true;
  }
private:
  luaL_Buffer *b;
};

static int lb_compress(lua_State *L)
{
  size_t in_len;
  const char *in = luaL_checklstring(L, 1, &in_len);

  brotli::BrotliParams params;
  if (lua_isnumber(L, 2))
  {
    params.quality = lua_tointeger(L, 2);

  }
  else if (lua_istable(L, 2))
  {
    int mode = -1;
    luaL_fieldinteger(L, 2, "mode",    mode);
    luaL_fieldinteger(L, 2, "quality", params.quality);
    luaL_fieldinteger(L, 2, "lgwin",   params.lgwin);
    luaL_fieldinteger(L, 2, "lgblock", params.lgblock);
    if (mode >= 0) params.mode = (brotli::BrotliParams::Mode)mode;
  }

  luaL_Buffer b;
#if LUA_VERSION_NUM >= 502
  luaL_buffinitsize(L, &b, in_len / 2);
#else
  luaL_buffinit(L, &b);
#endif

  MemReader input(in, in_len);
  LuaWriter output(&b);
  if (!brotli::BrotliCompress(params, &input, &output))
    return luaL_error(L, "compression failed");

  luaL_pushresult(&b);

  return 1;
}

static int BrotliLuaOutputFunction(void *data, const uint8_t *buf, size_t count)
{
  luaL_Buffer *b = (luaL_Buffer *)data;
  luaL_addlstring(b, (const char *)buf, count);
  return (int)count;
}

static BrotliOutput BrotliInitLuaOutput(luaL_Buffer *b)
{
  BrotliOutput output;
  output.cb_ = &BrotliLuaOutputFunction;
  output.data_ = b;
  return output;
}

static int lb_decompress(lua_State *L)
{
  size_t in_len;
  const char *in = luaL_checklstring(L, 1, &in_len);

  luaL_Buffer b;
#if LUA_VERSION_NUM >= 502
  luaL_buffinitsize(L, &b, in_len);
#else
  luaL_buffinit(L, &b);
#endif

  BrotliMemInput mem_input;
  BrotliInput input = BrotliInitMemInput((const uint8_t *)in, in_len, &mem_input);
  BrotliOutput output = BrotliInitLuaOutput(&b);
  if (!BrotliDecompress(input, output))
    return luaL_error(L, "corrupt input");

  luaL_pushresult(&b);

  return 1;
}

extern "C"
{

static const luaL_Reg export_functions[] = {
  { "compress",   lb_compress   },
  { "decompress", lb_decompress },
  { NULL,         NULL          },
};


LUALIB_API int luaopen_brotli(lua_State *L)
{
#if LUA_VERSION_NUM >= 502
  luaL_newlib(L, export_functions);
#else
  lua_newtable(L);
  luaL_register(L, NULL, export_functions);
#endif

  int table = lua_gettop(L);
  lua_pushinteger(L, brotli::BrotliParams::MODE_GENERIC);
  lua_setfield(L, table, "MODE_GENERIC");
  lua_pushinteger(L, brotli::BrotliParams::MODE_TEXT);
  lua_setfield(L, table, "MODE_TEXT");
  lua_pushinteger(L, brotli::BrotliParams::MODE_FONT);
  lua_setfield(L, table, "MODE_FONT");

  return 1;
}

}
