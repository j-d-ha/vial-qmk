# crkbd/rev1_rp2040 hk keymap notes

## Vial combos

This keymap enables Vial-editable combos and seeds a default set of combos into Vial's dynamic combo storage.

Relevant files:

- `keyboards/crkbd/keymaps/hk/rules.mk`
- `keyboards/crkbd/keymaps/hk/config.h`
- `keyboards/crkbd/keymaps/hk/keymap.c`
- `keyboards/crkbd/keymaps/hk/vial.json`

### Default combo behavior

The firmware reserves `32` Vial combo slots with:

- `COMBO_ENABLE = yes`
- `#define VIAL_COMBO_ENTRIES 32`

On the master half, the firmware performs a one-time migration for combo defaults:

- if the migration marker has not been written yet
- and no Vial combos are configured yet
- then the default combos are written into Vial combo storage

After that, the migration marker is stored in keyboard EEPROM so later boots do not overwrite user-edited Vial combos.

### Seeded default combos

The default combos currently seeded are:

- `C + V -> Cmd+V`
- `X + C -> Cmd+C`
- `X + C + V -> Shift+Cmd+C`
- `Z + X -> Cmd+Z`
- `Z + X + C -> Shift+Cmd+Z`
- `Z + C -> Cmd+X`

### Notes

- These defaults are meant to appear once for blank combo storage.
- Later edits in Vial should persist normally.
- The migration/seeding runs on the master half only.
- For this split setup, make sure the correct left/right UF2 files are flashed to the matching halves.
