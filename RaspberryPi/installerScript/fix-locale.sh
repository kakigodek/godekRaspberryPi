#!/bin/bash

echo "Fixing locale issues..."

# Generate en_GB.UTF-8 locale
sudo locale-gen en_GB.UTF-8

# Set default locale system-wide
sudo bash -c 'cat > /etc/default/locale <<EOF
LANG="en_GB.UTF-8"
LC_ALL="en_GB.UTF-8"
LC_CTYPE="en_GB.UTF-8"
EOF'

# Update locale settings
sudo update-locale

# Source new locale settings
source /etc/default/locale

# Confirm result
echo
echo "Current locale settings:"
locale

echo
echo "Locale fix complete."
