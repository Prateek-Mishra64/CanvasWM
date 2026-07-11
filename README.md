CanvasWm 
-----

"Make the user interface invisible"

CanvasWm is a custom fork or spinoff of Hevel a scrollable, floating window manager for Wayland that uses mouse
chords for all commands.

Its design is inspired by ideas from Rob Pike's 1988 paper, "Window Systems 
Should be Transparent", taken to their logical extremes. In this sense, hevel
is a modernization of mouse-driven Unix and Plan 9 window systems such as mux,
8½, and rio.

CanvasWM treats the canvas origin as a high-speed cache. New windows appear there by default because that's where the user's attention already is. When the origin is occupied, new windows cascade predictably from the occupying window rather than replacing it. Long-lived windows are expected to be moved by the user into their own locations on the infinite canvas.

Unlike those systems, hevel has no menus and is not limited to a single
screen of space. Instead, the desktop is an infinite plane:
windows can be created anywhere, and the view can be freely scrolled thru (vertically, or in all axis).

hevel is implemented using the [neuswc](https://git.sr.ht/~shrub900/neuswc) library.

hevel is the flagship window manager designed for use with 
[dérive linux](https://derivelinux.org).

**WARNING**: This is experimental software. Use at your own risk.

Commands 
--------

Commands are issued using mouse chords: combinations of mouse buttons pressed 
in sequence.

Mouse buttons are referred to as follows:
- 1: left click
- 2: middle click (scroll wheel)
- 3: right click.

Here are the commands:

- 1 → 3 → drag → release

  Create a new terminal in the dragged rectangle.
  
- 3 → 1 → move mouse over target window → release

  Kill the target window.
   
- 3 → 2 → release 2, keep holding the scroll wheel

  Scroll vertically in vertical mode, and drag the cursor in drag mode.

- 2 → 3 → release 2 over a window, and then drag with 3

  Resize the window.

- 2 → 1 → release 2 over a window, then drag with 1

  Move the window. Dragging to the top or bottom of the screen begins 
  scrolling.

- 1 → 2

  User-configurable (see config.h).

Building
----- 

To build hevel, you will need the [neuwld](https://git.sr.ht/~shrub900/neuwld) and [neuswc](https://git.sr.ht/~shrub900/neuswc)
library installed. Hevel requires the following development dependencies:

- A C99-compatible compiler
- some sort of make
- pkg-config
- wayland-scanner, wayland-server, wayland-client
- wayland-server, wayland-client
- libinput, libdrm, pixman, xkbcommon
- [neuwld](https://git.sr.ht/~shrub900/neuwld)
- [neuswc](https://git.sr.ht/~shrub900/neuswc)

```
make
make install 
```

To run:

```
swc-launch hevel
```
Linux is the primary supported platform. NetBSD and FreeBSD also work, but may
require minor Makefile adjustments. Depending on your setup, you may want to 
tweak neuswc itself via config.mk before compiling hevel.

hevel-specific configuration is done at compile time via config.h.
j



In file included from src/hevel.c:2:
src/binding.h:10:3: error: expected specifier-qualifier-list before ‘BIND_ACTION’
   10 |   BIND_ACTION,
      |   ^~~~~~~~~~~
src/binding.h:16:5: error: unknown type name ‘xkb_keysym_t’
   16 |     xkb_keysym_t key;
      |     ^~~~~~~~~~~~
src/binding.h:18:10: error: ‘binding_type’ defined as wrong kind of tag
   18 |     enum binding_type type;
      |          ^~~~~~~~~~~~
src/binding.h:18:23: error: field ‘type’ has incomplete type
   18 |     enum binding_type type;
      |                       ^~~~
src/hevel.c: In function ‘main’:
src/hevel.c:107:3: error: implicit declaration of function ‘binding_intialize’; did you mean ‘binding_initialize’? [-Wimplicit-function-declaration]
  107 |   binding_intialize();
      |   ^~~~~~~~~~~~~~~~~
      |   binding_initialize
In file included from src/binding.c:3:
src/binding.h:10:3: error: expected specifier-qualifier-list before ‘BIND_ACTION’
   10 |   BIND_ACTION,
      |   ^~~~~~~~~~~
src/binding.h:16:5: error: unknown type name ‘xkb_keysym_t’
   16 |     xkb_keysym_t key;
      |     ^~~~~~~~~~~~
src/binding.h:18:10: error: ‘binding_type’ defined as wrong kind of tag
   18 |     enum binding_type type;
      |          ^~~~~~~~~~~~
src/binding.h:18:23: error: field ‘type’ has incomplete type
   18 |     enum binding_type type;
      |                       ^~~~
src/binding.c: In function ‘binding_action’:
src/binding.c:25:12: error: ‘BIND_ACTION’ undeclared (first use in this function)
   25 |       case BIND_ACTION:
      |            ^~~~~~~~~~~
src/binding.c:25:12: note: each undeclared identifier is reported only once for each function it appears in
src/binding.c:28:12: error: ‘BIND_EXEC’ undeclared (first use in this function)
   28 |       case BIND_EXEC:
      |            ^~~~~~~~~
src/binding.c: In function ‘binding_initialize’:
src/binding.c:39:21: error: implicit declaration of function ‘LENGTH’ [-Wimplicit-function-declaration]
   39 |     for (i = 0; i < LENGTH(bindings); ++i) {
      |                     ^~~~~~
src/binding.c:39:28: error: ‘bindings’ undeclared (first use in this function); did you mean ‘binding’?
   39 |     for (i = 0; i < LENGTH(bindings); ++i) {
      |                            ^~~~~~~~
      |                            binding
make: *** [Makefile:21: hevel] Error 1
