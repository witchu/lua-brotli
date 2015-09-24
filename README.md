# lua-brotli
Brotli compression library binding for Lua.

Consists of two functions: compress and decompress.
Both functions take an input string and return an output string.

```lua
local brotli = require("brotli")
local s = "Hello, World"
local e = brotli.compress(s)
local d = brotli.decompress(e)
assert(s == d)
```
