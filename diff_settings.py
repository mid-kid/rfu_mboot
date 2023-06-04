# Usage:
# Copy expected file to expected/file.o
# Run diff.py -mwo -f file.o symbol

def apply(config, args):
    config["source_directories"] = ["."]
    config["arch"] = "arm32"
    config["objdump_executable"] = "arm-none-eabi-objdump"
    config["makeflags"] = ["-f", "MakefileDemo"]
