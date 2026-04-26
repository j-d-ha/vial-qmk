# Vial Pointer Configuration Plan for HolyKeebs

Status: draft / design note

This document describes what it would look like to expose the HolyKeebs pointer settings in Vial, ideally through the QMK tab or a keyboard-specific Vial menu.

## Goal

Expose runtime settings for the current HolyKeebs pointer stack so the user can tune:

- right trackpoint behavior
- left trackball scrolling behavior
- trackball LED behavior/color
- save/reset of HolyKeebs pointer settings

without rebuilding firmware.

## Current baseline

The repo now uses the original HolyKeebs `users/holykeebs` userspace as the working pointer baseline.

That baseline already supports runtime pointer settings in firmware via HolyKeebs keycodes and EEPROM-backed state.

Relevant files:

- `users/holykeebs/holykeebs.c`
- `users/holykeebs/holykeebs.h`
- `users/holykeebs/pointing.h`
- `keyboards/crkbd/keymaps/hk/keymap.c`
- `keyboards/crkbd/keymaps/hk/vial.json`

## Settings already supported by firmware

These are already present in HolyKeebs userspace:

- default movement multiplier
- sniping movement multiplier
- scroll buffer size
- sniping mode toggle
- drag-scroll mode toggle
- scroll lock mode
- scroll direction invert
- save settings
- reset settings
- dump settings

Important behavior:

- settings normally apply to the **main** pointer
- holding **Shift** applies them to the **peripheral** pointer

For this keyboard when plugged into the right side:

- main pointer = right trackpoint
- peripheral pointer = left trackball

Trackball LED status today:

- the Pimoroni trackball LED rainbow is currently compile-time controlled
- `keyboards/crkbd/keymaps/hk/rules.mk` sets `TRACKBALL_RGB_RAINBOW = yes`
- `users/holykeebs/holykeebs.c` runs the rainbow animation and writes colors with `pimoroni_trackball_set_rgbw(...)`
- this is **not** currently exposed as a Vial setting

## What should be exposed in Vial

Recommended first pass:

### Trackpoint / main pointer

- Default multiplier
- Sniping multiplier
- Sniping mode toggle

### Trackball / peripheral pointer

- Drag-scroll toggle
- Scroll direction invert
- Scroll buffer size
- Scroll lock mode
- Trackball LED mode
- Trackball LED color

### Global

- Save HolyKeebs settings
- Reset HolyKeebs settings

## Suggested Vial UI

Two reasonable options:

### Option A: Keyboard-specific Vial menu

Add a custom menu section in `keyboards/crkbd/keymaps/hk/vial.json`.

Suggested groups:

- `Trackpoint`
- `Trackball`
- `HolyKeebs`

Example conceptual layout:

- Trackpoint
  - Default Speed
  - Sniping Speed
  - Sniping Enabled
- Trackball
  - Drag Scroll
  - Invert Scroll
  - Scroll Buffer
  - Scroll Lock
  - LED Mode
  - LED Red
  - LED Green
  - LED Blue
- HolyKeebs
  - Save Settings
  - Reset Settings

This is probably the best UX.

### Option B: Reuse QMK tab style where possible

If Vial supports the needed control types cleanly, present these like QMK controls, but they would still be implemented as keyboard custom values underneath.

In practice, this is still custom keyboard firmware work.

## Firmware work needed

### 1. Add custom Vial menu definitions

Update:

- `keyboards/crkbd/keymaps/hk/vial.json`

Add:

- `menus`
- controls for toggles, ranges, enums, actions

Exact menu syntax should be based on existing Vial custom-menu examples already in the repo.

## 2. Add custom value command handler

Implement keyboard-level Vial custom value handling.

Likely entry point:

- `via_custom_value_command_kb(uint8_t *data, uint8_t length)`

This can live in either:

- `keyboards/crkbd/keymaps/hk/keymap.c`
- a new keyboard/keymap-specific source file
- or a small HolyKeebs integration file if we want to keep pointer logic centralized

Recommended approach:

- keep HolyKeebs pointer logic in `users/holykeebs`
- keep Vial UI glue in keyboard/keymap layer

That preserves the project rule:

- pointer logic in `users/holykeebs`
- board wiring in `keyboards/*`

### 3. Map Vial values to HolyKeebs state

The handler should read and write `g_hk_state` fields where those already exist.

Likely fields:

- `g_hk_state.main.pointer_default_multiplier`
- `g_hk_state.main.pointer_sniping_multiplier`
- `g_hk_state.main.cursor_mode`
- `g_hk_state.peripheral.drag_scroll`
- `g_hk_state.peripheral.scroll_direction_inverted`
- `g_hk_state.peripheral.pointer_scroll_buffer_size`
- `g_hk_state.peripheral.scroll_lock`

Trackball LED control is a little different:

- the current rainbow behavior is build-time only, not part of `g_hk_state`
- if we want Vial control, we should add explicit runtime state for:
  - LED mode: off / fixed / rainbow
  - LED color: red / green / blue
- runtime writes should call `pimoroni_trackball_set_rgbw(...)`
- persistence would likely require adding fields to the HolyKeebs EEPROM config

For save/reset actions, call the same logic used by HolyKeebs keycodes.

## 4. Persistence decision

Need to decide one of these models:

### Model A: live changes, explicit save

- Vial changes update runtime state immediately
- user must press `Save` in Vial menu to persist

Pros:

- matches current HolyKeebs behavior
- lower EEPROM churn
- easier mental model for existing HolyKeebs users

Cons:

- one more step for user

### Model B: auto-save on every change

- every Vial change writes EEPROM immediately

Pros:

- simple user experience

Cons:

- more EEPROM writes
- less aligned with current HolyKeebs model

Recommended:

- **Model A**

## Suggested value ranges

These should be constrained in firmware.

### Multipliers

Represent as integer tenths in Vial, convert to float in firmware.

Example:

- range: `1..80`
- display meaning: `0.1 .. 8.0`

### Scroll buffer

Suggested range:

- `0..32`

### Scroll lock enum

- `0 = Off`
- `1 = Horizontal`
- `2 = Vertical`

### Trackball LED mode enum

Recommended:

- `0 = Off`
- `1 = Fixed color`
- `2 = Rainbow`

### Trackball LED color channels

If Vial does not provide a convenient color-picker widget, expose separate integer sliders:

- `Red   = 0..255`
- `Green = 0..255`
- `Blue  = 0..255`

### Toggles

- `0 = Off`
- `1 = On`

## Suggested custom value IDs

Example only; actual IDs can be chosen later.

- `0x01` main default multiplier
- `0x02` main sniping multiplier
- `0x03` main sniping mode
- `0x10` peripheral drag-scroll
- `0x11` peripheral invert scroll
- `0x12` peripheral scroll buffer
- `0x13` peripheral scroll lock
- `0x14` trackball LED mode
- `0x15` trackball LED red
- `0x16` trackball LED green
- `0x17` trackball LED blue
- `0x20` save settings action
- `0x21` reset settings action

## Minimal implementation plan

Phase 1:

- expose peripheral trackball controls only
  - drag-scroll
  - invert scroll
  - scroll buffer
  - scroll lock
  - trackball LED mode
  - trackball LED color
- expose save/reset

This gives the highest value fastest.

Phase 2:

- expose main trackpoint controls
  - default multiplier
  - sniping multiplier
  - sniping mode

Phase 3:

- consider exposing both main/peripheral versions of more settings
- consider a more polished menu layout

## Rough effort estimate

For a first useful version:

- moderate effort
- likely a few hours

For a polished, fully organized Vial panel:

- closer to half a day to a day

## Risks / gotchas

- Vial menu JSON syntax must match the firmware handler exactly
- float values should be represented as integers over the wire
- save/reset should not fight existing HolyKeebs EEPROM logic
- side semantics must be explicit; Vial should ideally label main vs peripheral clearly
- if the keyboard is plugged into the opposite side, main/peripheral semantics change
- trackball LED control is separate from normal keyboard `rgblight`; users should not expect the existing Vial lighting tab to affect the Pimoroni LED automatically
- if rainbow remains available, runtime Vial control needs to override or replace the current compile-time-only `TRACKBALL_RGB_RAINBOW` behavior cleanly

## Recommended UX choice

For this keyboard specifically, it may be clearer to label controls by behavior rather than by abstract side:

When USB is on the right, show / think of them as:

- `Trackpoint (main)`
- `Trackball scroll (peripheral)`

But internally they are still `main` and `peripheral` HolyKeebs state.

## Future enhancement idea

If needed later, add a custom UI with both:

- main/peripheral names
- detected pointer kind
- live value readback

This would make it easier to understand what changes are affecting which device.

## Summary

This is very doable without changing the Vial app itself.

Most likely implementation path:

1. add custom menu items to `keyboards/crkbd/keymaps/hk/vial.json`
2. implement `via_custom_value_command_kb(...)`
3. map values to `g_hk_state`
4. keep explicit `Save` / `Reset`

Recommended first version:

- trackball drag-scroll
- trackball invert scroll
- trackball scroll buffer
- trackball scroll lock
- trackball LED mode/color
- save/reset
