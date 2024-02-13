#!/bin/bash

# File paths
file_a="menu.js" //new menu function to add.
file_b="bot.js" //bot file

# Find the line number after which to add content from file_a in file_b
line_number=$(grep -n '//Add new function menu here.' "$file_b" | cut -d':' -f1)

# If the line number is found, insert the content of file_a after that line in file_b
if [ -n "$line_number" ]; then
    sed -i "${line_number}r $file_a" "$file_b"
    echo "Content from $file_a inserted into $file_b after line $line_number."
else
    echo "Error: Line '//Add new function menu here.' not found in $file_b."
fi
