#   Copyright (c) 2026, Signaloid.
#
#   Permission is hereby granted, free of charge, to any person obtaining a
#   copy of this software and associated documentation files (the "Software"),
#   to deal in the Software without restriction, including without limitation
#   the rights to use, copy, modify, merge, publish, distribute, sublicense,
#   and/or sell copies of the Software, and to permit persons to whom the
#   Software is furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included in
#   all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#   DEALINGS IN THE SOFTWARE.


import argparse
from enum import IntEnum

from app_helpers import (
    compute_module_args,
    init_compute_module,
    pack_floats,
    run_and_get_results,
    unpack_floats,
)


# Command IDs supported by the SoC application.
# Keep these in sync with the SignaloidSoCCommand enum in
# signaloid-soc-application/main.c.
class Commands(IntEnum):
    CalculateNoCommand = 0
    CalculateAddition = 1


def parse_arguments(
    explicit_args: list[str] | None = None,
):
    parser = argparse.ArgumentParser(
        description="Host application for the Signaloid C0 compute modules \
            template application"
    )

    compute_module_args(parser=parser)

    subparsers = parser.add_subparsers(
        dest="command",
        help="Commands",
    )

    # Subparser for "add" command (requires two uncertainty values)
    parser_add = subparsers.add_parser(
        Commands.CalculateAddition.name,
        help="Add two uniform distributions X, Y",
    )

    parser_add.add_argument(
        "input_values",
        nargs=4,
        type=float,
        help="The two input uniform distributions X and Y, given as \
            [min, max] pairs: [X_min, X_max, Y_min, Y_max]",
    )

    args = parser.parse_args(explicit_args)
    return args


def main(explicit_args: list[str] | None = None):
    args = parse_arguments(explicit_args)

    compute_module = init_compute_module(
        device_path=args.device_path,
        variant=args.variant,
        reset_on_launch=args.reset_on_launch,
    )

    command_value = Commands[args.command]
    input_buffer = pack_floats(
        floats=args.input_values,
        size=compute_module.INPUT_BUFFER_SIZE_BYTES,
    )

    # Run the calculation and get the results
    result_buffer = run_and_get_results(
        compute_module=compute_module,
        command_value=command_value,
        input_buffer=input_buffer,
        stop_on_exit=args.stop_on_exit
    )

    # Unpack the device's response. The SoC returns two floats here (mean
    # and variance).
    # Adjust the unpacking to match what your command writes.
    mean, variance = unpack_floats(
        byte_buffer=result_buffer,
        count=2,
    )
    print(f"Mean: {mean}")
    print(f"Variance: {variance}")


if __name__ == "__main__":
    main()
