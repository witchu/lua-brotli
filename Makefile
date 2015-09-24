UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
# linux config
LUA_INCDIR ?= /usr/include/lua5.1
LUA_LIBDIR ?= /usr/lib
LIBFLAGS   ?= -shared
endif
ifeq ($(UNAME), Darwin)
# macosx config
LUA_INCDIR ?= /usr/local/opt/lua/include
LUA_LIBDIR ?= /usr/local/opt/lua/lib
LIBFLAGS   ?= -bundle -undefined dynamic_lookup -all_load
endif

LUALIB     ?= brotli.so
LUA_CFLAGS ?= -Os -fPIC

BROTLI      = brotli
ENCOBJ      = $(BROTLI)/enc/*.o
DECOBJ      = $(BROTLI)/dec/*.o

CMOD        = $(LUALIB)
OBJS        = lua_brotli.o

CFLAGS      = $(LUA_CFLAGS) -I$(LUA_INCDIR)
CXXFLAGS    = $(LUA_CFLAGS) -I$(LUA_INCDIR)
LDFLAGS     = $(LIBFLAGS) -L$(LUA_LIBDIR)

GCCVERSION := $(shell gcc --version | grep ^gcc | sed 's/^.* //g' | sed 's/^4\.6\..*/bad/')
ifeq ($(GCCVERSION), bad)
CXXFLAGS   += -std=c++0x
else
CXXFLAGS   += -std=c++11
endif


# rules

all: brotli

install: $(CMOD)
	cp $(CMOD) $(INST_LIBDIR)

clean:
	$(RM) $(CMOD) $(OBJS) $(ENCOBJ) $(DECOBJ)

brotli: $(OBJS) deps
	$(CXX) $(LDFLAGS) $(OBJS) $(ENCOBJ) $(DECOBJ) -o $(CMOD)

.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

deps:
	$(MAKE) -C $(BROTLI)/enc
	$(MAKE) -C $(BROTLI)/dec
