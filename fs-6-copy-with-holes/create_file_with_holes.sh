#!/bin/sh

string=$(mktemp)

echo "123456789ABCDEF" > "$string"

rm file_with_holes

# the file will have 3 16-byte chunks with 2 16-byte holes in between 
dd if="$string" bs=16 seek=0 count=1 of=file_with_holes
dd if="$string" bs=16 seek=2 count=1 of=file_with_holes
dd if="$string" bs=16 seek=4 count=1 of=file_with_holes

rm "$string"

# you can look at file and its holes with `xxd file_with_holes`
