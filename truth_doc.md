
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



Dump of assembler code for function cursor_tick:
   0x000056111f6b1fe0 <+0>:     sub    $0x18,%rsp
   0x000056111f6b1fe4 <+4>:     mov    %fs:0x28,%rsi
   0x000056111f6b1fed <+13>:    mov    %rsi,0x8(%rsp)
   0x000056111f6b1ff2 <+18>:    lea    0x4(%rsp),%rsi
   0x000056111f6b1ff7 <+23>:    mov    %rsp,%rdi
   0x000056111f6b1ffa <+26>:    call   0x56111f6b12c0 <swc_cursor_position@plt>
   0x000056111f6b1fff <+31>:    mov    0x42a2(%rip),%r9        # 0x56111f6b62a8 <input+8>
   0x000056111f6b2006 <+38>:    test   %al,%al
   0x000056111f6b2008 <+40>:    je     0x56111f6b207d <cursor_tick+157>
   0x000056111f6b200a <+42>:    lea    0x426f(%rip),%rdi        # 0x56111f6b6280 <compositor+32>
   0x000056111f6b2011 <+49>:    mov    0x4270(%rip),%rcx        # 0x56111f6b6288 <compositor+40>
   0x000056111f6b2018 <+56>:    mov    (%rsp),%edx
   0x000056111f6b201b <+59>:    mov    0x4(%rsp),%eax
   0x000056111f6b201f <+63>:    cmp    %rdi,%rcx
   0x000056111f6b2022 <+66>:    je     0x56111f6b207d <cursor_tick+157>
   0x000056111f6b2024 <+68>:    test   %edx,%edx
   0x000056111f6b2026 <+70>:    lea    0xff(%rdx),%esi
   0x000056111f6b202c <+76>:    lea    0xff(%rax),%r8d
   0x000056111f6b2033 <+83>:    cmovns %edx,%esi
--Type <RET> for more, q to quit, c to continue without paging--



[swc:../libswc/drm.c:163] DEBUG: /dev/dri/card1 is the primary GPU
# find_driver: Trying DRM driver `dumb'
[spawn_launch] kitty
fork returned 2251
screen 1920x1200
fork returned 0
wayland-0
[DEBUG] BEFORE wl_display_run()
[swc:../libswc/compositor.c:1695] DEBUG: Performing update
[swc:../libswc/compositor.c:399] DEBUG: Rendering to target { x: 0, y: 0, w: 1920, h: 1200 }
(WW) Option "-listen" for file descriptors is deprecated
Please use "-listenfd" instead.
(WW) Option "-listen" for file descriptors is deprecated
Please use "-listenfd" instead.
Xwayland glamor: GBM Wayland interfaces not available
Failed to initialize glamor, falling back to sw
_amdgpu_device_initialize: amdgpu_query_info(ACCEL_WORKING) failed (-13)
[0.096] Ignoring unknown config key: background_shader
[0.100] [glfw error 65544]: X11: The DISPLAY environment variable is missing
GLFW initialization failed
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
[DEBUG] AFTER wl_display_run()
[DEBUG] BEFORE swc_finalize()
[DEBUG] AFTER swc_finalize()
[DEBUG] BEFORE wl_display_destroy()
[DEBUG] AFTER wl_display_destroy()
[DEBUG] RETURNING FROM MAIN
[DEBUG] atexit() called







For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from /usr/local/bin/hevel...

This GDB supports auto-downloading debuginfo from the following URLs:
  <https://debuginfod.archlinux.org>
Enable debuginfod for this session? (y or [n]) n
Debuginfod has been disabled.
To make this setting permanent, add 'set debuginfod enabled off' to .gdbinit.
--Type <RET> for more, q to quit, c to continue without paging--c
(No debugging symbols found in /usr/local/bin/hevel)
[New LWP 2278]
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/usr/lib/libthread_db.so.1".
Core was generated by `hevel'.
Program terminated with signal SIGSEGV, Segmentation fault.
#0  0x00007f64e929a29c in ?? () from /usr/lib/libc.so.6
(gdb) bt full
#0  0x00007f64e929a29c in ??? () at /usr/lib/libc.so.6
#1  0x00007f64e923e7d0 in raise () at /usr/lib/libc.so.6
#2  0x00007f64e923e8f0 in <signal handler called> () at /usr/lib/libc.so.6
#3  0x00007f64e96479df in wl_event_source_timer_update () at /usr/lib/libwayland-server.so.0
#4  0x000056111f6b208a in cursor_tick ()
#5  0x00007f64e964a677 in wl_event_loop_dispatch () at /usr/lib/libwayland-server.so.0
#6  0x00007f64e964c567 in wl_display_run () at /usr/lib/libwayland-server.so.0
#7  0x000056111f6b161c in main ()
(gdb) frame 0
#0  0x00007f64e929a29c in ?? () from /usr/lib/libc.so.6
(gdb) list
1       /* wld: buffered_surface.c
2        *
3        * Copyright (c) 2013, 2014 Michael Forney
4        *
5        * Permission is hereby granted, free of charge, to any person obtaining a copy
6        * of this software and associated documentation files (the "Software"), to deal
7        * in the Software without restriction, including without limitation the rights
8        * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
9        * copies of the Software, and to permit persons to whom the Software is
10       * furnished to do so, subject to the following conditions:
(gdb) 
