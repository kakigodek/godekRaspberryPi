sudo nano /etc/systemd/system/oled-display.service

[Unit]
Description=OLED System Display
After=network.target

[Service]
Type=simple
Restart=on-failure
RestartSec=5
ExecStart=/usr/bin/python3 /opt/oled/info_1.py
#Restart=always
User=root

[Install]
WantedBy=multi-user.target


sudo systemctl enable oled-display
sudo systemctl start oled-display
sudo journalctl -u oled-display -f

sudo pip3 install -r /opt/oled/requirements.txt

