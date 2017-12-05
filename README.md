Starting the game automatically on boot:

1. Pull latest version of project from master
 - note - The latest version can only be ran from root directory

2. Build project to your public folder

3. Copy gameTest, audio folder, and name.txt config file to ~/

4. Make the service shell script:
cd ~/
nano beagleboop.sh

```
#!/bin/bash
sleep 10
/root/gameTest
```

save and exit nano

chmod +x beagleboop.sh

5. Setup the service to run your shell script
cd /lib/systemd/system
nano beagleboop.service

```
[Unit]
Description=BeagleBoop game

[Service]
WorkingDirectory=/
ExecStart=/root/beagleboop.sh
SyslogIdentifier=BeagleBoop
```

[Install]
WantedBy=multi-user.target

save and exit nano

6. Enable service on boot
systemctl enable beagleboop.service