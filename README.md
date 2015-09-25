Brotli compression library binding for Lua.

[![Build Status](https://travis-ci.org/witchu/lua-brotli.svg)](https://travis-ci.org/witchu/lua-brotli)
[![Build status](https://ci.appveyor.com/api/projects/status/y5ov9v32rkn0o3na/branch/master?svg=true)](https://ci.appveyor.com/project/witchu/lua-brotli/branch/master)

Consists of two functions: compress and decompress.
Both functions take an input string and return an output string.

```lua
local brotli = require("brotli")
local s = "Hello, World"
local e = brotli.compress(s)
local d = brotli.decompress(e)
assert(s == d)
```

To install from luarocks
```
luarocks install lua-brotli
```
