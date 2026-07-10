# Hevel+

## Vision

Hevel+ is an infinite canvas window manager.

The keyboard creates windows.
The mouse manipulates windows and the canvas.

The compositor assists with initial placement only. After a window is created,
its position is entirely controlled by the user.

---

# Design Principles

- Infinite world
- No workspaces
- No layouts
- Predictable spawning
- Manual organization
- Small independent modules
- Extend Hevel, don't rewrite it

---

# Responsibilities

## Keyboard

Responsible for requesting new applications.

Examples:

- Terminal
- Browser
- Launcher
- File Manager

Keyboard never computes placement.

---

## Spawn

Responsible for application launch requests.

Stores:

- command
- width
- height

Spawn does not know where the window will appear.

---

## Placement

Responsible only for coordinates.

Input:

Current viewport

Output:

x
y

Rules:

1. Spawn at viewport center.
2. If center overlaps another window,
   offset slightly.
3. If no nearby position is available,
   spawn centered on top.

Placement never launches applications.

---

## Window

Responsible for applying geometry.

Geometry =

x
y
width
height

Window never computes placement.

---

## Mouse

Responsible for manipulating the world.

- Move windows
- Resize windows
- Pan canvas
- Zoom canvas

Mouse never launches applications.

---

# Spawn Pipeline

Keyboard
    ↓
Spawn Request
(command, width, height)
    ↓
Placement
(x, y)
    ↓
Window
(x, y, width, height)
    ↓
Application appears

---

# Module Layout

src/

hevel.c
    Startup

input.c
    Mouse input

spawn.c
    Spawn requests

placement.c
    Initial placement

window.c
    Window lifecycle

scroll.c
    Canvas movement

zoom.c
    Zoom

select.c
    Existing selection system

---

# Current Plan

Phase 1

- Generic spawn request
- Placement engine
- Keyboard spawning

Phase 2

- Fuzzel integration
- Automatic placement
- Fullscreen improvements

Phase 3

- Application rules
- Session startup
- Wallpaper
- Notifications

---

# Important Decisions

✓ Keep Hevel architecture.

✓ Preserve existing terminal spawning.

✓ Preserve rectangle selection until replacement is complete.

✓ Placement returns only coordinates.

✓ Width and height belong to Spawn Request.

✓ New windows spawn near viewport center.

✓ Mouse is reserved for world manipulation.

✓ Keyboard is reserved for actions.




[Prateek@NullVoid CanvasWM]$ swc-launch hevel
running on /dev/tty2
[swc:../libswc/drm.c:163] DEBUG: /dev/dri/card1 is the primary GPU
# find_driver: Trying DRM driver `dumb'
screen 1920x1200
wayland-0
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
(WW) Option "-listen" for file descriptors is deprecated
Please use "-listenfd" instead.
(WW) Option "-listen" for file descriptors is deprecated
Please use "-listenfd" instead.
Xwayland glamor: GBM Wayland interfaces not available
Failed to initialize glamor, falling back to sw
_amdgpu_device_initialize: amdgpu_query_info(ACCEL_WORKING) failed (-13)
The XKEYBOARD keymap compiler (xkbcomp) reports:
> Warning:          Multiple symbols for level 1/group 1 on key <FK23>
>                   Using F23, ignoring XF86TouchpadOff
> Warning:          Symbol map for key <FK23> redefined
>                   Using last definition for conflicting fields
> Warning:          Symbol map for key <FK24> redefined
>                   Using last definition for conflicting fields
Errors from xkbcomp are not fatal to the X server
The XKEYBOARD keymap compiler (xkbcomp) reports:
> Warning:          Unsupported maximum keycode 709, clipping.
>                   X11 cannot support keycodes above 255.
> Warning:          Virtual modifier Hyper multiply defined
>                   Using 0, ignoring 0
> Warning:          Virtual modifier ScrollLock multiply defined
>                   Using 0, ignoring 0
Errors from xkbcomp are not fatal to the X server
button left (272) pressed
button right (273) pressed
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
button right (273) released
Trying #0: (460,200 1000x800)
fork returned 1935918
saved spawn pid = 1935918
spawned terminal at 596,427 435x594
fork returned 0
child executing kitty
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
button left (272) released
[0.090] Ignoring unknown config key: background_shader
[swc:../libswc/window.c:514] DEBUG: Initializing window, 0x55920551bee0
mode=0 width=1000 height=800 
pending=0 stored=0X0
window ''
focus (nil) ('') -> 0x55920551bee0 ('') (new_window)
libEGL warning: failed to get driver name for fd -1

libEGL warning: MESA-LOADER: failed to retrieve device information

libEGL warning: failed to get driver name for fd -1

[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[0.340] [glfw error 65544]: process_desktop_settings: failed with error: org.freedesktop.DBus.Error.NameHasNoOwner: Could not activate remote peer 'org.freedesktop.portal.Desktop': startup job failed
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
button left (272) pressed
button right (273) pressed
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
button right (273) released
Trying #0: (460,200 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #1: (500,240 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #2: (420,160 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #3: (500,160 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #4: (420,240 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #5: (540,280 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #6: (380,120 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #7: (540,120 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #8: (380,280 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #9: (580,320 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #10: (580,80 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #11: (340,320 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
fork returned 1936122
saved spawn pid = 1936122
spawned terminal at 143,433 405x274
fork returned 0
child executing kitty
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
button left (272) released
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[0.090] Ignoring unknown config key: background_shader
[swc:../libswc/window.c:514] DEBUG: Initializing window, 0x55920551bd60
mode=0 width=1000 height=800 
pending=0 stored=0X0
window ''
focus 0x55920551bee0 ('~/Documents/Projects/CanvasWM') -> 0x55920551bd60 ('') (new_window)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
libEGL warning: failed to get driver name for fd -1

libEGL warning: MESA-LOADER: failed to retrieve device information

libEGL warning: failed to get driver name for fd -1

[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[0.342] [glfw error 65544]: process_desktop_settings: failed with error: org.freedesktop.DBus.Error.NameHasNoOwner: Could not activate remote peer 'org.freedesktop.portal.Desktop': startup job failed
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
button left (272) pressed
button right (273) pressed
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
button right (273) released
Trying #0: (460,200 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #1: (500,240 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #2: (420,160 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #3: (500,160 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #4: (420,240 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #5: (540,280 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #6: (380,120 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #7: (540,120 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #8: (380,280 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #9: (580,320 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #10: (580,80 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
Trying #11: (340,320 1000x800)
Against: (460,200 1000x800)
Checking window: ~/Documents/Projects/CanvasWM
OVERLAP
fork returned 1936270
saved spawn pid = 1936270
spawned terminal at 137,505 367x248
fork returned 0
child executing kitty
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
button left (272) released
[0.086] Ignoring unknown config key: background_shader
[swc:../libswc/window.c:514] DEBUG: Initializing window, 0x559205517bb0
mode=0 width=1000 height=800 
pending=0 stored=0X0
window ''
focus 0x55920551bd60 ('~/Documents/Projects/CanvasWM') -> 0x559205517bb0 ('') (new_window)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
libEGL warning: failed to get driver name for fd -1

libEGL warning: MESA-LOADER: failed to retrieve device information

libEGL warning: failed to get driver name for fd -1

[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[0.331] [glfw error 65544]: process_desktop_settings: failed with error: org.freedesktop.DBus.Error.NameHasNoOwner: Could not activate remote peer 'org.freedesktop.portal.Desktop': startup job failed
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
button left (272) pressed
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
button left (272) released
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/window.c:567] DEBUG: Finalizing window, 0x559205517bb0
focus 0x559205517bb0 ('exit') -> (nil) ('') (destroy)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
button left (272) pressed
focus (nil) ('') -> 0x55920551bd60 ('~/Documents/Projects/CanvasWM') (click)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
button left (272) released
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/window.c:567] DEBUG: Finalizing window, 0x55920551bd60
focus 0x55920551bd60 ('exit') -> (nil) ('') (destroy)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
button left (272) pressed
focus (nil) ('') -> 0x55920551bee0 ('~/Documents/Projects/CanvasWM') (click)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
button left (272) released
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[swc:../libswc/window.c:567] DEBUG: Finalizing window, 0x55920551bee0
focus 0x55920551bee0 ('~/Documents/Projects/CanvasWM') -> (nil) ('') (destroy)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[Prateek@NullVoid CanvasWM]$ 
