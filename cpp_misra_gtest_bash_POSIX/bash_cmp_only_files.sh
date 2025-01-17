

#compare two folders but only for files in subfolders also, but ignore folder path

# Generate file lists (including subfolders) for both directories.
# Sort and normalize paths to ignore folder structure.
# Compare the results using diff or similar tools.

#!/bin/bash

# Define the two directories to compare
dir1="/path/to/first/folder"
dir2="/path/to/second/folder"

# Generate sorted file lists for each directory (ignore paths, consider only filenames)
find "$dir1" -type f -exec basename {} \; | sort > /tmp/files1.txt
find "$dir2" -type f -exec basename {} \; | sort > /tmp/files2.txt

# Compare the lists and show differences
diff /tmp/files1.txt /tmp/files2.txt

# Clean up temporary files
rm /tmp/files1.txt /tmp/files2.txt


