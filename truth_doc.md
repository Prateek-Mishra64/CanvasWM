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
