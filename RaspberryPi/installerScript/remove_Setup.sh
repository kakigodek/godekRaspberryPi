#!/bin/bash

# Stop the Apache service
sudo systemctl stop apache2

# Uninstall PHPMyAdmin and its configuration
sudo apt-get remove --purge phpmyadmin -y
sudo apt-get purge --auto-remove phpmyadmin -y

# Remove the symbolic link to PHPMyAdmin
sudo rm /var/www/html/phpmyadmin

# Uninstall PHP and related modules
sudo apt-get remove --purge php libapache2-mod-php php-mysql -y
sudo apt-get purge --auto-remove php libapache2-mod-php php-mysql -y

# Uninstall Apache and its configuration
sudo apt-get remove --purge apache2 -y
sudo apt-get purge --auto-remove apache2 -y

# Uninstall MySQL server and its configuration
sudo apt-get remove --purge mariadb-server -y
sudo apt-get purge --auto-remove mariadb-server -y

# Clean up any residual configuration files
sudo apt-get autoremove
sudo apt-get autoclean

# Uninstall MySQL secure installation package (if it was installed)
sudo apt-get remove --purge mysql-secure-installation -y

# Remove any residual MySQL configuration files
sudo rm -rf /etc/mysql

# Remove PHPMyAdmin configuration file
sudo rm /etc/apache2/conf-available/phpmyadmin.conf

# Reload the Apache service to apply changes
sudo systemctl reload apache2

# Your Apache, PHP, PHPMyAdmin, and MySQL setup is removed.

# You can verify the removal of packages and configurations.
echo "Packages and configurations removed. Please verify that everything is uninstalled successfully."
