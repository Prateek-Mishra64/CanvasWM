PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
PKG_CONFIG = pkg-config

CC = cc
CXX = c++

WAYFIRE_SRC = $(HOME)/Projects/Canvas-wayfire

CFLAGS = -O2 -std=c99 -Wall -Wextra -I$(PREFIX)/include
CXXFLAGS = -O2 -Wall -Wextra -std=c++20 -I$(PREFIX)/include

LDFLAGS = -L$(PREFIX)/lib -Wl,-rpath,$(PREFIX)/lib

PKGS = wayfire wlroots-0.20
CFLAGS   += $(shell $(PKG_CONFIG) --cflags $(PKGS))

CXXFLAGS += $(shell $(PKG_CONFIG) --cflags $(PKGS))
CXXFLAGS += \
	-I$(WAYFIRE_SRC) \
	-I$(WAYFIRE_SRC)/src \
	-I$(HOME)/Projects/Canvas-wayfire/build/src/libwayfire.so.p

LDLIBS += $(shell $(PKG_CONFIG) --libs $(PKGS))
C_SRC = src/canvas.c src/input.c src/scroll.c src/select.c src/window.c src/zoom.c src/spawn.c src/placement.c src/action.c src/binding.c src/viewport.c

CPP_SRC = src/host.cpp 

OBJ = $(C_SRC:.c=.o) $(CPP_SRC:.cpp=.o)

all: canvas


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

canvas: $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

clean:
	rm -f canvas $(OBJ)

confclean: clean
	rm -f config.h

install: canvas
	install -D -m 755 canvas $(DESTDIR)$(BINDIR)/canvas

.PHONY: all clean confclean install

