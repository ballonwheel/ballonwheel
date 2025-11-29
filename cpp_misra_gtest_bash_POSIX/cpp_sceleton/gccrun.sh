#!/usr/bin/env bash
# -------------------------------------------------------------
# gccrun.sh — run make using a specific GCC executable on Windows
# -------------------------------------------------------------

export PATH="/c/Users/dio2bp/cmakeTrain/Tools/mingw/8.2.0/bin:$PATH"

# Path to your specific GCC executable
GCC_PATH="/c/Users/dio2bp/cmakeTrain/Tools/mingw/8.2.0/bin/gcc.exe"

# Check if GCC exists
if [[ ! -f "$GCC_PATH" ]]; then
    echo "❌ Error: GCC not found at: $GCC_PATH"
    exit 1
fi

# Set environment variable so make uses this GCC
export CC="$GCC_PATH"
export CXX="${GCC_PATH%/gcc.exe}/g++.exe"

echo "✅ Using GCC: $CC"
echo "✅ Using G++: $CXX"

# Run make in the current directory
if [[ -f "Makefile" || -f "makefile" ]]; then
    echo "🛠  Running make in: $(pwd)"
    make "$@"
else
    echo "❌ No Makefile found in current directory: $(pwd)"
    exit 2
fi

