# Signaloid SoC application

This directory holds the firmware that runs on the Signaloid SoC inside the
compute module.

It receives two floating-point value pairs and parses them as uniform
distributions, adds the two distributions, and returns the mean and variance of
the resulting distribution.

The firmware is built in the Signaloid Cloud Developer Platform. Use the targets
in the top-level [Makefile](../Makefile) to build, download, and flash it.

This is a basic template firmware, used as a starting point for developing your
own applications.

## Files

| File                   | Purpose                                                                                                                    |
| ---------------------- | -------------------------------------------------------------------------------------------------------------------------- |
| [main.c](main.c)       | Entry point. Polls the command register, dispatches to the selected command, and packs the results into the output buffer. |
| [config.mk](config.mk) | Build configuration. Selects the target device sources.                                                                    |

## Configuration

`config.mk` sets the sources for the selected `DEVICE_TYPE` and the Signaloid
Compute Module Utilities path.

Add your own compiler flags through the `BUILD_FLAGS` variable, and add your
sources and include paths on the `SOURCES` and `INC` variables respectively of
`config.mk`.
