
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH

Refactor 2 – Paint Brush
Objective

Decouple Canvas input handling from compositor logic by introducing a configurable, action-driven input architecture.

The goal of this refactor was to make every keyboard interaction pass through a common abstraction layer while exposing a clean public API for both Canvas System Actions and user-defined application launching.

Architecture Before
Keyboard
    ↓
input.c / hevel.c
    ↓
Direct compositor logic
    ↓
Window manipulation

Problems:

Hardcoded keybindings.
Logic duplicated across input handlers.
Users had to modify compositor source to change bindings.
Input and compositor logic were tightly coupled.
External application spawning and Canvas actions were mixed together.
Architecture After
Canvas System Pipeline
Keyboard
        ↓
Binding
        ↓
Action Dispatcher
        ↓
Canvas Subsystems
        ↓
Window / Viewport / Compositor
External Application Pipeline
Keyboard
        ↓
Binding
        ↓
Spawn API
        ↓
Placement Engine
        ↓
fork()
        ↓
execvp()

Canvas System Actions and user application spawning are now independent systems.

Public Input API

Users now configure bindings exclusively through config.h.

Canvas exposes backend-independent modifier constants:

MOD_SUPER
MOD_SHIFT
MOD_CTRL
MOD_ALT

instead of exposing SWC-specific modifier masks.

Backend translation is handled internally by the binding subsystem.

Binding Subsystem

Introduced:

binding.h
binding.c
binding_initialize()

Responsibilities:

Register all keyboard bindings.
Translate Canvas modifier masks into backend modifier masks.
Dispatch bindings into either Canvas Actions or application spawning.

All keyboard registration has been centralized.

hevel.c no longer contains compositor-specific keyboard bindings.

Action System

Introduced:

action.h
action.c
action_execute()

Responsibilities:

Execute compositor functionality.
Dispatch requests into subsystem implementations.
Serve as the central API for every Canvas operation.

The action system contains only compositor behavior.

External applications are intentionally excluded.

Window Subsystem Extraction

Moved window-specific behavior out of input.c.

Extracted:

window_toggle_sticky()
window_toggle_fullscreen()

Mouse gestures and keyboard actions now reuse the same subsystem implementation.

Spawn API

Introduced a generic spawn pipeline:

spawn_launch()
        ↓
spawn_request_prepare()
        ↓
placement_compute()
        ↓
spawn_execute()

Responsibilities:

Process spawning requests.
Compute initial placement.
Execute the application.

The spawn subsystem is now independent of the input system.

Placement Integration

The spawn subsystem now automatically integrates with the placement engine.

New applications inherit Canvas placement behavior without requiring input-specific logic.

Modifier Abstraction

Removed public dependency on SWC modifier constants.

Instead of:

SWC_MOD_LOGO

users configure:

MOD_SUPER

Modifier translation occurs exclusively inside the binding subsystem.

Input Decoupling

Input devices no longer implement compositor behavior.

They only request actions.

Current direction:

Keyboard
Mouse
Trackpad
IPC
Search Palette

↓

Action Dispatcher

Every future input method will invoke the same Canvas API.

Bug Fixes During Refactor

Resolved:

Duplicate spawn execution caused by both key press and key release triggering bindings.
Backend modifier leakage into public configuration.
Binding registration centralization.
Generic spawn pipeline integration.
Window subsystem extraction.
Current Public API
Canvas System Actions

Current action categories:

Viewport

Window

Focus

Zoom

Compositor

These actions manipulate Canvas itself.

User Actions

Application launching is handled separately through the Spawn API.

This distinction separates:

Canvas behavior

from

User applications
Design Principles Established
Input does not implement behavior.

Input requests actions.

Actions dispatch.

Subsystems implement.

Canvas owns compositor behavior.

Spawn owns application launching.

Public API is backend independent.

Users should never need to know SWC exists.

Future Compatibility

This architecture provides the foundation for:

Mouse bindings
Trackpad gestures
Search Palette
Window search/jump
IPC
Plugins
External automation
Multiple spawn clients

All future features will integrate through the existing Action and Spawn APIs.

Refactor Status
Completed
Binding subsystem
Action dispatcher
Backend-independent modifier API
Window subsystem extraction
Spawn pipeline
Placement integration
Configurable keybindings
Fullscreen action
Sticky action
Quit action
Keyboard architecture
Next Milestone
Major Core Features 1 — Behaviour Defining

This milestone focuses on defining Canvas' unique interaction model rather than refactoring its internals.

Primary areas:

Navigation
Keyboard viewport navigation
Hybrid keyboard + mouse navigation
Trackpad gesture navigation
Directional focus movement
Window jump/search
Smooth viewport movement
Application State
Fullscreen persistence independent of navigation
Preserve application presentation state while moving through the canvas
Focus loss without presentation changes
Stable window state across viewport movement
Spawning
EXEC_BIND
Multiple Spawn API clients
Service-initiated spawning
Multiple window support
Rich spawn requests
Future launcher and IPC integration














swc=(1332,753)
input=(1332,753)
swc=(1331,753)
input=(1331,753)
swc=(1330,753)
input=(1330,753)
swc=(1327,753)
input=(1327,753)
swc=(1324,752)
input=(1324,752)
swc=(1320,751)
input=(1320,751)
swc=(1318,751)
input=(1318,751)
swc=(1318,751)
input=(1318,751)
swc=(1316,750)
input=(1316,750)
swc=(1314,750)
input=(1314,750)
swc=(1314,750)
input=(1314,750)
swc=(1313,749)
input=(1313,749)
swc=(1313,749)
input=(1313,749)
swc=(1313,749)
input=(1313,749)
swc=(1313,749)
input=(1313,749)
[BUTTON] button=272 state=1 held=1 input.held=1 mods=12
[MOVE] BEGIN
[MOVE] begin
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1313,749)
input=(1313,749)
[MOVE] tick
cursor 1313 749
cursor=(1313,749)
screen=(0,0 1920x1200)
swc=(1313,749)
input=(1313,749)
[MOVE] tick
cursor 1313 749
cursor=(1313,749)
screen=(0,0 1920x1200)
swc=(1313,749)
input=(1313,749)
[MOVE] tick
cursor 1313 749
cursor=(1313,749)
screen=(0,0 1920x1200)
swc=(1326,750)
input=(1326,750)
[MOVE] tick
cursor 1326 750
cursor=(1326,750)
screen=(0,0 1920x1200)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1339,753)
input=(1339,753)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[MOVE] tick
cursor 1339 753
cursor=(1339,753)
screen=(0,0 1920x1200)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1378,756)
input=(1378,756)
[MOVE] tick
cursor 1378 756
cursor=(1378,756)
screen=(0,0 1920x1200)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1413,756)
input=(1413,756)
[MOVE] tick
cursor 1413 756
cursor=(1413,756)
screen=(0,0 1920x1200)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1455,756)
input=(1455,756)
[MOVE] tick
cursor 1455 756
cursor=(1455,756)
screen=(0,0 1920x1200)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1494,756)
input=(1494,756)
[MOVE] tick
cursor 1494 756
cursor=(1494,756)
screen=(0,0 1920x1200)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1536,756)
input=(1536,756)
[MOVE] tick
cursor 1536 756
cursor=(1536,756)
screen=(0,0 1920x1200)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1555,756)
input=(1555,756)
[MOVE] tick
cursor 1555 756
cursor=(1555,756)
screen=(0,0 1920x1200)
swc=(1617,756)
input=(1617,756)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[MOVE] tick
cursor 1617 756
cursor=(1617,756)
screen=(0,0 1920x1200)
swc=(1659,756)
input=(1659,756)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[MOVE] tick
cursor 1659 756
cursor=(1659,756)
screen=(0,0 1920x1200)
swc=(1699,756)
input=(1699,756)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[MOVE] tick
cursor 1699 756
cursor=(1699,756)
screen=(0,0 1920x1200)
swc=(1736,756)
input=(1736,756)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[MOVE] tick
cursor 1736 756
cursor=(1736,756)
screen=(0,0 1920x1200)
swc=(1770,756)
input=(1770,756)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[MOVE] tick
cursor 1770 756
cursor=(1770,756)
screen=(0,0 1920x1200)
swc=(1804,756)
input=(1804,756)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[MOVE] tick
cursor 1804 756
cursor=(1804,756)
screen=(0,0 1920x1200)
swc=(1850,756)
input=(1850,756)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[MOVE] tick
cursor 1850 756
cursor=(1850,756)
screen=(0,0 1920x1200)
swc=(1875,756)
input=(1875,756)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[MOVE] tick
cursor 1875 756
cursor=(1875,756)
screen=(0,0 1920x1200)
swc=(1886,756)
input=(1886,756)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[MOVE] tick
cursor 1886 756
cursor=(1886,756)
screen=(0,0 1920x1200)
swc=(1907,756)
input=(1907,756)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[MOVE] tick
cursor 1907 756
cursor=(1907,756)
screen=(0,0 1920x1200)
swc=(1919,756)
input=(1919,756)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
[MOVE] tick
cursor 1919 756
cursor=(1919,756)
screen=(0,0 1920x1200)
swc=(1919,756)
input=(1919,756)
[MOVE] tick
cursor 1919 756
cursor=(1919,756)
screen=(0,0 1920x1200)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1919,756)
input=(1919,756)
[MOVE] tick
cursor 1919 756
cursor=(1919,756)
screen=(0,0 1920x1200)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1919,756)
input=(1919,756)
[MOVE] tick
cursor 1919 756
cursor=(1919,756)
screen=(0,0 1920x1200)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1919,756)
input=(1919,756)
[MOVE] tick
cursor 1919 756
cursor=(1919,756)
screen=(0,0 1920x1200)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1919,756)
input=(1919,756)
[MOVE] tick
cursor 1919 756
cursor=(1919,756)
screen=(0,0 1920x1200)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1919,756)
input=(1919,756)
[MOVE] tick
cursor 1919 756
cursor=(1919,756)
screen=(0,0 1920x1200)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1919,757)
input=(1919,757)
[MOVE] tick
cursor 1919 757
cursor=(1919,757)
screen=(0,0 1920x1200)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1919,758)
input=(1919,758)
[MOVE] tick
cursor 1919 758
cursor=(1919,758)
screen=(0,0 1920x1200)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1919,758)
input=(1919,758)
[MOVE] tick
cursor 1919 758
cursor=(1919,758)
screen=(0,0 1920x1200)
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
swc=(1919,758)
input=(1919,758)
[MOVE] tick
cursor 1919 758
cursor=(1919,758)
screen=(0,0 1920x1200)
swc=(1919,758)
input=(1919,758)
[MOVE] tick
cursor 1919 758
cursor=(1919,758)
screen=(0,0 1920x1200)
swc=(1919,758)
input=(1919,758)
