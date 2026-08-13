# Signaloid-Compute-Module-Application-Template

This is a template for building UxHw applications that run on the Signaloid
compute modules. This template uses a Python host application to interact with
the C application running on the Signaloid SoC of the selected compute module.

This is a minimal source code example. You can find more detailed application
examples that use the capabilities of the Signaloid compute modules in the
official Signaloid GitHub
[organization](https://github.com/signaloid?q=Signaloid-Compute-Module).

## Compatibility

This template currently supports:

- **Signaloid C0-microSD**
- **Signaloid C0-microSD+**
- **Signaloid C0-SD**

## Cloning this repository

Clone this repository recursively to get all its submodules:

```sh
git clone --recursive https://github.com/signaloid?q=Signaloid-Compute-Module-Application-Template
```

To update all submodules:

```sh
git pull --recurse-submodules
git submodule update --remote --recursive
```

If you did not clone with `--recursive` and ended up with empty submodule
directories, you can fetch them with:

```sh
git submodule update --init --recursive
```

## Getting started

### Configure the `Makefile`

1. Configure the `DEVICE` variable. This is the path where your compute module
   is located (e.g. /dev/disk4).
2. Configure the `DEVICE_TYPE` variable for your compute module. This is the
   compute module hardware variant you are using. The supported options are:
    - `SIGNALOID_C0_MICROSD`
    - `SIGNALOID_C0_MICROSD_PLUS`
    - `SIGNALOID_C0_SD`.
3. Configure the `CORE_ID` variable matching your compute module type. This
   controls the precision and correlation tracking for your application.
   default: `C0-*-N` core.

### Build the Compute Module application

The Makefile compiles the Signaloid SoC application on the Signaloid Cloud
Compute Engine using the
[Signaloid CLI](https://docs.signaloid.io/docs/api/signaloid-cli/intro/). The
`Makefile` (at the repository root) uses the CLI to connect this repository,
start a build in the Signaloid Cloud Compute Engine, and download the resulting
`main.bin`. The build inputs (source files and include paths) are defined in
`signaloid-soc-application/config.mk`.

#### Prerequisites:

- A supported Signaloid compute module (see compatibility above) and its device
  path on your host.
- A [Signaloid account](https://get.signaloid.io).
- A GitHub account connected to your Signaloid account, as shown in the
  [GitHub Login guide](https://docs.signaloid.io/docs/platform/user-interface/repositories/github-login/),
  so you can fork this template repository, push your changes, and build it on
  the [Signaloid Cloud Developer Platform](https://signaloid.io).
- An API key for authentication.
  [Create one here](https://signaloid.io/settings/api).
- The [Signaloid CLI](https://docs.signaloid.io/docs/api/signaloid-cli/intro/)
  installed and authenticated as shown in its
  [installation](https://docs.signaloid.io/docs/api/signaloid-cli/installation/)
  and
  [authentication](https://docs.signaloid.io/docs/api/signaloid-cli/authentication/)
  documentation.
- **Python 3.10 or later** for the host application and the flashing toolkit.
- Root privileges (`sudo`) for raw block-device access.

#### Build the firmware
To build, run `make`. This connects the repository (first run only), starts a
cloud build, waits for it to finish, and downloads `main.bin` into the
repository root.

### Flash the Compute Module application

1. Make sure you have correctly configured the `DEVICE` and `DEVICE_TYPE`
   variables in the `Makefile` as described above.
2. Run `make flash`. This flashes the `<build-id>.main.bin` (it builds and
   downloads it first, if needed).
3. If you are targeting a Signaloid C0-microSD, you will be asked to power cycle
   the device to switch modes (Bootloader, Signaloid SoC). The device will have
   finished flashing when the green LED is solid.

## Host application

The host application sends two floating-point number pairs to the compute
module, which encode the limits of two input uniform distributions. The compute
module adds them and returns the mean and variance of the result for the host to
print.

### Run the Python based host application

To run the Python-based host application you first need to install its
dependencies. To do that:

1. Create a virtual environment: `python3 -m venv .venv`
2. Activate the virtual environment: `source .venv/bin/activate`
3. Navigate to `./python-host-application`
4. Install the requirements: `pip install -r requirements.txt`

### Example command

> [!IMPORTANT]
>
> Root privileges are required for raw access to the block device.
>
> We invoke the virtual environment's interpreter directly (`.venv/bin/python3`)
> because a plain `sudo python3` would use the system Python without the
> packages installed in the virtual environment.

Assuming the C0-microSD device is located at `/dev/disk4`, run:

```sh
sudo .venv/bin/python3 host_application.py --device-path /dev/disk4 --variant C0-microSD CalculateAddition 5.0 6.0 4.0 7.5
```

### Usage

```
usage: host_application.py [-h] [-d DEVICE_PATH] [-v {C0-microSD,C0-microSD+,C0-SD}] [-r] [-s] {CalculateAddition} ...

Host application for the Signaloid C0 compute modules template application

positional arguments:
  {CalculateAddition}   Commands
    CalculateAddition   Add two uniform distributions X, Y

options:
  -h, --help            show this help message and exit
  -d, --device-path DEVICE_PATH
                        Path of the C0 compute module device (e.g., /dev/disk4)
  -v, --variant {C0-microSD,C0-microSD+,C0-SD}
                        Hardware variant (default: C0-microSD+)
  -r, --reset-on-launch
                        Reset the core on launch. Ignored on the C0-microSD.
  -s, --stop-on-exit    Stop the core on exit. Ignored on the C0-microSD.
```

## How to use this template:

You can use this template to base your own applications on. The recommended
workflow is the following:

1. Go to the
   [Signaloid Cloud Developer Platform Code Playground](https://signaloid.io/launch)
   and develop your computation algorithm. You can easily test your code there,
   so it helps speed up development.
2. Copy and paste your new C application's computation algorithm into the
   `main.c` file.
3. Add any new commands for each of your new computations.
4. Modify the input data parsing logic.
5. Modify the output data writing logic.
6. If you use any additional `.c` sources, add them to the `SOURCES` variable in
   the `signaloid-soc-application/config.mk` file.
7. Modify the Python host application to support your new C application (new
   commands, input data writing, output data parsing).
8. Build and flash your C application as shown above.
9. Run your Python host application as shown above.
