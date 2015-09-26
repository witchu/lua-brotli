local brotli = require("brotli")

local s = string.rep("0123456789", 100000) -- 1M
local e = brotli.compress(s) -- default quality = 11
local d = brotli.decompress(e)
assert(s == d)

local e1 = brotli.compress(s, 1)
local e9 = brotli.compress(s, {quality=9,lgwin=10,lgblock=16})

print("quality = 1, size = "..#e1)
print("quality = 9, size = "..#e9)
print("quality = 11, size = "..#e)

print("ok")
