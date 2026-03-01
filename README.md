# Printer3D

Printer3D is the **client/content/UI half** of the Rustic 3D printing mod for DayZ. It lets players place printers, install filament and blueprint folders, manage a print queue, and fabricate configurable items in-game.

Need a bandage but do not want to make a pilgrimage to trader or loot thirty sheds like a goblin on overtime? Print it with filament and the right blueprint.

This project is now open source under **GPLv3**.

## What it does

Printer3D currently provides:

- Placeable 3D printer kits
- A standard printer and a larger `1000_Slots` printer variant
- Static printer variants
- Placeable filing cabinets for blueprint folder storage
- USB media items that hold blueprint folders
- Filament items with multiple tiers
- Blueprint folders that define:
  - target item classname
  - display name
  - print time
  - filament cost
  - filament tier requirement
  - output quantity
- An in-game printer UI and print queue
- Notifications for printer status and errors
- Optional generator/power requirements
- Optional folder degradation
- Optional auto-resume of queues after stop/restart conditions
- Optional prevention of printer pickup
- Queue persistence support through save/load hooks

## Client vs server

This repository splits the mod into two parts:

- **Printer3D** — client/content/UI side
- **Printer3D_Server** — server logic/config/RPC side

You need **both** pieces for a working deployment.

## Included gameplay objects

### Printers

- `Printer3D_Printer`
- `Printer3D_Printer_static`
- `Printer3D_Printer_1000_Slots`
- `Printer3D_Printer_static_1000_Slots`

### Kits

- `Printer3D_Printer_Kit`
- `Printer3D_Printer_1000S_Kit`
- `Printer3D_FilingCabinet_Kit`

### Support items

- `Printer3D_USB`
- `Printer3D_FilingCabinet`
- `Printer3D_Filament_T1_Blue`
- `Printer3D_Filament_T2_Red`
- `Printer3D_Filament_T3_Gold`
- blueprint folder classes derived from `Printer3D_FolderBase`

## How it works

At a high level:

1. A player places a printer kit.
2. The printer can accept:
   - **Filament**
   - a **Printer3D USB**
3. The USB can hold up to 4 blueprint folders.
4. Each folder describes what item can be printed and what it costs.
5. The player opens the printer menu and adds jobs to the queue.
6. The server validates power, filament, selected model, available space, and queue state.
7. The printed item is created when the job completes.

There is also a filing cabinet object with **24 folder slots** for storage/organization of blueprint folders.

## Configuration

See `Printer3D_Server` for server-side config details.

## Installation

Install `Printer3D` on:

- the **server**
- all **clients**

Also install:

- `RusticModsCore`
- `Printer3D_Server`
- `CF`

## Notes for content creators

Blueprint folder behavior is driven by custom config values read from the folder classname, including:

- `printerPrintTypename`
- `printerDisplayname`
- `printerTimeToPrint`
- `printerFilamentAmount`
- `printerFilamentTier`
- `printerPrintQuantity`

That means you can extend the system by defining additional folder classes for new printable items.

## License

This project is licensed under the **GNU General Public License v3.0**. See `LICENSE-GPL-3.0.txt` for the full license text.
