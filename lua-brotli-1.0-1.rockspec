package = "lua-brotli"
  version = "1.0-1"
  source = {
    url = "..." -- We don't have one yet
  }
  description = {
    summary = "A simple brotli lua binding.",
    detailed = [[
      Consists of two functions: compress and decompress.
      Both functions take an input string and return an output string.
    ]],
    homepage = "http://...", -- We don't have one yet
    license = "Apache 2.0"
  }
  dependencies = {
    "lua >= 5.1"
  }
  build = {
    type = "make",
    build_variables = {
      LUA_CFLAGS="$(CFLAGS)",
      LIBFLAG="$(LIBFLAG)",
      LUA_LIBDIR="$(LUA_LIBDIR)",
      LUA_BINDIR="$(LUA_BINDIR)",
      LUA_INCDIR="$(LUA_INCDIR)",
      LUA="$(LUA)",
    },
    install_variables = {
      INST_PREFIX="$(PREFIX)",
      INST_BINDIR="$(BINDIR)",
      INST_LIBDIR="$(LIBDIR)",
      INST_LUADIR="$(LUADIR)",
      INST_CONFDIR="$(CONFDIR)",
    },
  }
