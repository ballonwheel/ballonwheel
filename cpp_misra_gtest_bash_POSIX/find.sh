


find /path/to/search_directory -type f -exec grep -H "win laptop" {} +



+: This terminator to the -exec option indicates that find should pass as many filenames as possible to each invocation of the command, rather than running the command once for each file.





