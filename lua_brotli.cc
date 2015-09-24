#include <lua.hpp>

#include "brotli/dec/decode.h"
#include "brotli/enc/encode.h"
#include "brotli/enc/streams.h"

class MemReader : public brotli::BrotliIn
{
public:
  MemReader(const void *data, size_t len) : data_(data), len_(len) { }
  virtual ~MemReader() { }

  virtual const void* Read(size_t n, size_t* nread)
  {
    const void *d = data_;
    *nread = len_;
    data_ = NULL;
    len_ = 0;
    return d;
  }
private:
  const void *data_;
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

int lb_compress(lua_State *L)
{
  size_t in_len;
  const char *in = luaL_checklstring(L, 1, &in_len);

  luaL_Buffer b;
#if LUA_VERSION_NUM >= 502
  luaL_buffinitsize(L, &b, in_len / 2);
#else
  luaL_buffinit(L, &b);
#endif

  brotli::BrotliParams params;
  MemReader input(in, in_len);
  LuaWriter output(&b);
  if (!brotli::BrotliCompress(params, &input, &output))
    return luaL_error(L, "compression failed");

  luaL_pushresult(&b);

  return 1;
}

int BrotliLuaOutputFunction(void *data, const uint8_t *buf, size_t count)
{
  luaL_Buffer *b = (luaL_Buffer *)data;
  luaL_addlstring(b, (const char *)buf, count);
  return (int)count;
}

BrotliOutput BrotliInitLuaOutput(luaL_Buffer *b)
{
  BrotliOutput output;
  output.cb_ = &BrotliLuaOutputFunction;
  output.data_ = b;
  return output;
}

int lb_decompress(lua_State *L)
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
  return 1;
}

}
