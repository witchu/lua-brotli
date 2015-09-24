solution "lua_brotli"
	language "C++"
	location "build"
	targetdir "bin"
	objdir "tmp"

	configurations { "Release" }
	flags { "OptimizeSize" }
	buildoptions { "-fno-omit-frame-pointer", "-no-canonical-prefixes" }
	buildoptions { "-fno-tree-vrp" }

	project "lua_brotli"
		kind "SharedLib"
		targetprefix ""
		targetname "brotli"
		files {
			"lua_brotli.cc",
			"brotli/enc/*.cc",
		}
		includedirs {
			"/usr/include/lua5.1",
			"brotli/enc",
			"brotli/dec",
		}
		buildoptions { "-std=c++11" }
		links "brotli_dec"

	project "brotli_dec"
		kind "StaticLib"
		files { "brotli/dec/*.c" }
		buildoptions { "-fPIC" }
