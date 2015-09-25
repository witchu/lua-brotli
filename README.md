Brotli compression library binding for Lua.

[![Build Status](https://travis-ci.org/witchu/lua-brotli.svg)](https://travis-ci.org/witchu/lua-brotli)
[![Build status](https://ci.appveyor.com/api/projects/status/y5ov9v32rkn0o3na/branch/master?svg=true)](https://ci.appveyor.com/project/witchu/lua-brotli/branch/master)

## Introduction

Brotli is a compression library from Google that is similar in speed with deflate but offers more dense compression. For more information visit https://github.com/google/brotli

This binding consists of two functions: `compress` and `decompress`.
Both functions take an input string and return an output string.

```lua
local brotli = require("brotli")
local s = "Hello, World"
local e = brotli.compress(s)
local d = brotli.decompress(e)
assert(s == d)
```

## Installation

Install from luarocks
```
luarocks install lua-brotli
```

## Compression options

```lua
local options = {
  mode = brotli.MODE_TEXT,
  quality = 1,
  lgwin = 10,
  lgblock = 16,
}
brotli.compress("Hello!!", options)
```

### mode
* `MODE_GENERIC` Default compression mode. The compressor does not know anything in advance about the properties of the input.
* `MODE_TEXT` Compression mode for UTF-8 format text input.
* `MODE_FONT` Compression mode used in WOFF 2.0.

### quality
Controls the compression-speed vs compression-density tradeoffs. The higher the quality, the slower the compression. Range is 0 to 11.

### lgwin
Base 2 logarithm of the sliding window size. Range is 10 to 24.

### lgblock
Base 2 logarithm of the maximum input block size. Range is 16 to 24. If set to 0, the value will be set based on the quality.
