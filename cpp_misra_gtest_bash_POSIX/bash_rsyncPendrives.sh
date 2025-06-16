#!/bin/bash

# Define mount points
SRC="/media/$USER/pen"
DEST="/media/$USER/pen1"

# Check if both drives are mounted
if [ ! -d "$SRC" ]; then
    echo "Source drive not mounted at $SRC"
    exit 1
fi

if [ ! -d "$DEST" ]; then
    echo "Destination drive not mounted at $DEST"
    exit 1
fi

# Perform rsync with options:
# -a: archive mode (preserves permissions, etc.)
# -v: verbose
# -h: human-readable sizes
# --progress: show progress
# --delete: remove files from DEST not in SRC (optional!)
rsync -avh --progress "$SRC"/ "$DEST"/

# Done
echo "Sync complete."
