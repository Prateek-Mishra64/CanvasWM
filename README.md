# Canvas

> A spatial desktop built on an infinite world.

Canvas is an experimental Wayland desktop environment that replaces the traditional workspace model with a continuous, infinite canvas.

Instead of switching between workspaces, desktops, or virtual screens, every application exists somewhere in a single shared world that can be navigated freely.

The viewport moves.

Windows stay where they are.

---

## Philosophy

Modern desktop environments are built around temporary layouts.

Applications are tiled, floated, stacked, or assigned to numbered workspaces. As the number of running applications grows, users spend more time managing layouts than interacting with information.

Canvas approaches desktop interaction differently.

The desktop is treated as a persistent spatial environment rather than a collection of pages.

Applications have locations.

Locations have meaning.

Memory becomes spatial instead of symbolic.

Rather than remembering:

> "Firefox is on workspace 3."

you remember:

> "Firefox is north-east of my terminal."

Just as people naturally remember where objects are placed on a physical desk, Canvas allows applications to become part of a persistent mental map.

---

# Core Principles

## Infinite World

Canvas has no workspaces.

There is only one world.

The viewport moves through that world while windows remain fixed.

Applications are never "sent" to another workspace because no such concept exists.

---

## Spatial Memory

Canvas is designed around human spatial memory.

Every application occupies a physical location.

Instead of switching contexts, users navigate space.

---

## Viewport Navigation

The camera is the primary object that moves.

The world itself remains static.

Navigation is intended to feel closer to moving around a map than changing desktops.

---

## Windows Are Objects

Windows are not layout elements.

They are independent objects existing within the world.

Canvas intentionally avoids enforcing tiling or floating paradigms.

The user decides where objects belong.

---

## Minimal Compositor

Canvas is **not** another compositor implementation.

The compositor is considered infrastructure.

Its responsibility is simply to provide:

- window creation
- input
- rendering
- outputs
- protocol support

Everything else belongs to Canvas.

---

## Backend Agnostic

Canvas should not depend on a specific compositor implementation.

The compositor exists only to expose a small platform interface.

Any backend capable of providing:

- windows
- rendering
- input
- outputs

should be capable of running Canvas.

---

# Goals

- Infinite desktop
- Spatial interaction
- Persistent layouts
- Low latency
- Smooth viewport navigation
- Minimal compositor logic
- Clear separation between backend and desktop logic

---

# Non Goals

Canvas is **not** trying to become:

- another tiling window manager
- another floating window manager
- another desktop shell
- another compositor project

The focus is the spatial desktop experience.

---

# Architecture

```
                Applications
                      │
                      ▼
              Wayland Backend
                      │
      ┌───────────────┴───────────────┐
      │                               │
      │        Platform Interface      │
      │                               │
      └───────────────┬───────────────┘
                      │
                      ▼
                   Canvas
      ┌────────────────────────────────┐
      │ Infinite World                 │
      │ Viewport                       │
      │ Spatial Navigation             │
      │ Selection                      │
      │ Window Placement               │
      │ Interaction Model              │
      └────────────────────────────────┘
```

Canvas owns the world.

The backend owns the Wayland implementation.

---

# Why Another Desktop?

Because existing desktop environments optimize for window management.

Canvas optimizes for information management.

The objective is to reduce context switching by allowing users to build long-term spatial memory.

---

# Current Status

Canvas is currently experimental.

Development is focused on creating a stable foundation before introducing additional features.

Current priorities include:

- rendering pipeline
- focus system
- popup handling
- output management
- application startup
- viewport interaction

---

# Roadmap

## Phase 1

- Stable rendering
- Reliable focus
- Popup support
- Output management
- Playtest-ready desktop

## Phase 2

- Spatial search
- Persistent sessions
- Better viewport controls
- Multi-monitor support

## Phase 3

- Plugins
- Advanced gestures
- Spatial workflows
- Collaborative features

---

# Inspiration

Canvas draws inspiration from several systems and ideas rather than attempting to imitate any single project.

### User Interfaces

- Xerox Alto
- Smalltalk
- Andrew Window System
- Macintosh Finder (spatial navigation)
- Plan 9 (simplicity)
- Infinite whiteboards

### Window Managers

- Hyprland
- sway
- river
- dwm
- Hevel

### Research

- Spatial memory in HCI
- Zoomable User Interfaces (ZUIs)
- Information visualization
- Infinite canvas interfaces

---

# Design Values

Every feature should satisfy at least one of these principles.

- Simplicity
- Spatial consistency
- Predictability
- Performance
- Low latency
- Minimal abstraction leakage

If a feature increases complexity without improving the spatial workflow, it probably does not belong in Canvas.

---

# License

(TBD)

---

> The desktop shouldn't be a stack of workspaces.
>
> It should be a place.
