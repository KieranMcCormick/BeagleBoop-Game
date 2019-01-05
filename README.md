- note - The latest version can only be run from the root directory

<span>1</span>. Build the project on your host to your public folder by running 

`$ make`

<span>2</span>. The game is now available to the target via `# mnt/remote/myApps`. 

From `# mnt/remote/myApps` copy the executable `gameTest`, audio folder `audio`, and config file `name.txt` to the targets root `~/`


<span>3</span>. To make the service shell script:

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

<span>5</span>. To enable the service on boot

`systemctl enable beagleboop.service`

Once your service is running, and if you have not changed the `name.txt` config file, you can start the game from the web page by selecting the board `Eddie` and pressing the update button.

https://beagle-boop.github.io/

