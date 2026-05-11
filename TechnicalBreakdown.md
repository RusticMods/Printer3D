## Repository structure

Printer3D now contains both sides of the mod:

* Client content, UI, items, layouts, and stringtable data
* Server logic, RPC handlers, configuration, validation, queue execution, persistence, notifications, and logging

A separate `Printer3D_Server` repository or PBO is no longer required for current versions.

## Server-side behavior

The server side is responsible for making sure print jobs are valid and safe before they run.

Server logic handles:

* Creating and loading `Printer3D.json`
* Registering printer RPC handlers
* Adding items to the print queue
* Removing queue items
* Clearing the queue
* Starting, stopping, and progressing print jobs
* Validating printer state before printing
* Validating USB presence and blueprint folder data
* Validating filament presence, quantity, and required tier
* Validating available cargo space for output items
* Optionally dropping printed items to the ground when no printer space exists
* Sending translated printer notifications to players
* Locking relevant attachment slots while printing
* Restoring queue state during store and load persistence
* Logging printer activity through RusticModsCore

## Validation checks

Before a print starts, the server checks things such as:

* Printer has a USB attached
* USB has a valid folder or model entry
* Printer has filament installed
* Enough filament exists for the selected print
* Installed filament tier matches the folder requirement
* There is enough room in printer cargo, or dropping is allowed
* Queue state is valid
* Power requirements are satisfied when enabled

## Notifications

Printer3D sends localized notifications for common states, including:

* Queue added
* Queue removed
* Queue cleared
* Queue empty
* No USB installed
* Folder empty
* No filament installed
* Not enough filament
* Wrong filament tier
* No model selected
* Not enough output space
* Printing started
* Printing cancelled
* Printing finished

## Logging

Printer3D logs activity through:

```text
GetRusticModsCoreLogger()
```

Examples of logged activity include:

* Startup confirmation
* Queue and state issues
* Invalid queue restoration entries
* Runtime printer events
* Print start, stop, cancel, and completion events

Logs can be found under:

```text
profiles/RusticMods/Logs/
```