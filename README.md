- note - The latest version can only be ran from root directory

1. Build project to your public folder

2. Copy the executable `gameTest`, audio folder `audio`, and config file `name.txt` to the targets ~/


3. Make the service shell script:

`# cd ~/`
`# nano beagleboop.sh`

```
#!/bin/bash
sleep 10
/root/gameTest
```

save and exit nano

`# chmod +x beagleboop.sh`

<span>4</span>. Setup the service to run your shell script

`# cd /lib/systemd/system`

`# nano beagleboop.service`

```
[Unit]
Description=BeagleBoop game

[Service]
WorkingDirectory=/
ExecStart=/root/beagleboop.sh
SyslogIdentifier=BeagleBoop

[Install]
WantedBy=multi-user.target
```

save and exit nano

<span>5</span>. Enable service on boot

`systemctl enable beagleboop.service`