#!/bin/bash

# Function to check if a package is installed and retrieve its version
check_installed() {
    package_name="$1"
    if dpkg -l | grep -q "ii  $package_name "; then
        version=$(dpkg -l | grep "ii  $package_name " | awk '{print $3}')
        echo "$package_name is installed (Version: $version)"
    else
        echo "$package_name is not installed."
    fi
}

# Check and display the versions of Apache, PHP, PHPMyAdmin, and MySQL
check_installed "apache2"
check_installed "php"
check_installed "phpmyadmin"
check_installed "mariadb-server"

