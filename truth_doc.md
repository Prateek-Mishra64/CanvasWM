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

========== ORIGIN ==========
Origin : (460,200) 1000x800
Focused: '~/Documents/Projects/CanvasWM'
============================
Candidate before policy : (460,200) 1000x800

------ placement_decide ------
Window '~/Documents/Projects/CanvasWM'
    Geometry : (787,-305) 1000x800
    Overlap : YES
    ==> CASCADE PARENT
Placement policy : CASCADE
Cascade parent geometry : (787,-305) 1000x800
Final spawn geometry : (827,-265) 1000x800
=========================================

fork returned 44859
saved spawn pid = 44859
spawned terminal at 1031,689 164x174
fork returned 0
child executing kitty
button left (272) released
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[0.089] Ignoring unknown config key: background_shader
[swc:../libswc/window.c:511] DEBUG: Initializing window, 0x5561fb8dc400
window ''
focus 0x5561fb971a30 ('~/Documents/Projects/CanvasWM') -> 0x5561fb8dc400 ('') (new_window)
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
[0.332] [glfw error 65544]: process_desktop_settings: failed with error: org.freedesktop.DBus.Error.NameHasNoOwner: Could not activate remote peer 'org.freedesktop.portal.Desktop': startup job failed
[0.332] [glfw error 65544]: Notify: Failed to get server capabilities error: org.freedesktop.DBus.Error.NoReply: Did not receive a reply. Possible causes include: the remote application did not send a reply, the message bus security policy 
blocked the reply, the reply timeout expired, or the network connection was broken.
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
button right (273) pressed
button middle (274) pressed
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
