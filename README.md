# Printer3D

Printer3D is a 3D printing mod for DayZ. It includes the client content, in-game UI, server logic, printer validation, queue processing, persistence, configuration, and logging needed to run the full printing system from a single mod package.

This project is open source under **GPLv3**.

## What it does

Printer3D lets players place 3D printers, install filament and blueprint storage, manage print queues, and fabricate configurable items in-game.

The mod currently provides:

* Placeable 3D printer kits
* Standard and large `1000_Slots` printer variants
* Static printer variants
* Placeable filing cabinets for blueprint folder storage
* USB media items that hold blueprint folders
* Filament items with multiple tiers
* Blueprint folders that define printable items
* In-game printer UI and print queue
* Server-side validation for printer state, USB data, filament, queue state, power, and output space
* Notifications for printer status and errors
* Optional generator or power requirements
* Optional folder degradation
* Optional queue auto-resume after stop or restart conditions
* Optional prevention of printer pickup
* Queue persistence through save and load hooks
* Logging through RusticModsCore

## Included gameplay objects

### Printers

* `Printer3D_Printer`
* `Printer3D_Printer_static`
* `Printer3D_Printer_1000_Slots`
* `Printer3D_Printer_static_1000_Slots`

### Kits

* `Printer3D_Printer_Kit`
* `Printer3D_Printer_1000S_Kit`
* `Printer3D_FilingCabinet_Kit`

### Support items

* `Printer3D_USB`
* `Printer3D_FilingCabinet`
* `Printer3D_Filament_T1_Blue`
* `Printer3D_Filament_T2_Red`
* `Printer3D_Filament_T3_Gold`
* Blueprint folder classes derived from `Printer3D_FolderBase`

## How it works

1. A player places a printer kit.
2. The printer accepts filament and a `Printer3D_USB`.
3. The USB can hold up to 4 blueprint folders.
4. Each folder describes what item can be printed and what it costs.
5. The player opens the printer menu and adds jobs to the queue.
6. The server validates power, filament, selected blueprint, cargo space, and queue state.
7. The server progresses the print job.
8. The printed item is created when the job completes.

The filing cabinet provides **24 folder slots** for storing and organizing blueprint folders.

## Blueprint folder data

Blueprint folders drive printable item behavior through custom config values on the folder classname.

Supported values include:

* `printerPrintTypename`
* `printerDisplayname`
* `printerTimeToPrint`
* `printerFilamentAmount`
* `printerFilamentTier`
* `printerPrintQuantity`

This allows server owners and content creators to extend the system by defining new folder classes for additional printable items.

For an example of how to create custom blueprint folders, see:

```text
https://github.com/RusticMods/Printer3D_Data_Example
```

## Configuration

On first start, the mod creates:

```text
profiles/RusticMods/Printer3D.json
```

Current config fields:

* `EnableDebugMode`
* `DropItemsIfNoSpace`
* `RequireGeneratorForStatic`
* `RequireGeneratorForNonStatic`
* `DegradeFolders`
* `AutoResumeQueueAfterStop`
* `DisablePrinterPickup`

## Required mods

Install these alongside Printer3D:

* `RusticModsCore`
* `CF`

`RusticModsCore` is required for shared Rustic systems such as permissions, logging, and common support functionality.

## Installation

1. Click `Code`, then `Download ZIP`.
2. Extract the folder inside the ZIP file into your P drive.
3. Remove `-main` from the folder name.

Example:

```text
Printer3D-main
```

should become:

```text
Printer3D
```

4. Pack the folder into a PBO using AddonBuilder or another packing tool.
5. Sign the PBO.
6. Upload the mod to your server pack or Steam Workshop.
7. Load the mod on both the server and clients.
8. Make sure `RusticModsCore` and `CF` are also installed.

No separate `Printer3D_Server` PBO is required for current versions.

## Notes for content creators

The printing system is designed to be extended through blueprint folder classes. If you define a new folder class with valid printer config values, the server can validate and execute that print job.

This makes it possible to add new printable items without rewriting the queue or printer logic.

Example custom blueprint repository:

```text
https://github.com/RusticMods/Printer3D_Data_Example
```

## License

This project is licensed under the **GNU General Public License v3.0**.

See `LICENSE-GPL-3.0.txt` for the full license text.

## Steam Workshop redistribution

This project is licensed under the GNU General Public License v3.0.

You may upload, repost, fork, modify, or redistribute this project on Steam Workshop only if you follow the GPLv3 license terms.

Any Steam Workshop upload or derivative version must:

* Credit the original author: Rustic
* Link to the original repository: [Printer3D](https://github.com/RusticMods/Printer3D/)
* Include a clear GPLv3 license notice
* Provide access to the complete corresponding source code for the uploaded version
* Clearly state whether changes were made
* Preserve copyright and license notices
* Not imply endorsement by the original author unless permission was explicitly granted

Suggested attribution:

> Based on Printer3D by Rustic, licensed under GPLv3. Original source: [https://github.com/RusticMods/Printer3D/](https://github.com/RusticMods/Printer3D/). Modified version source: [Your Modified Source URL].