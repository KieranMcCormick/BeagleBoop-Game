`Beagle Boop`
=============

Beagle Boop - A Simon Says / Bop It inspired game for the Beagle Bone.
=============

Developed by Eddie Marchioni, Jamie Epp, and Kieran McCormick

Building on Unix with `CMake`
=============

Home page for Beagle-Boop: https://beagle-boop.github.io/

### Prerequisites:

This project requires static libraries from `curl` and `json-c`.

#### json-c:

`json-c` GitHub repo: https://github.com/json-c/json-c

```sh
$ git clone https://github.com/json-c/json-c.git
$ cd json-c
$ sh autogen.sh
```

followed by

```sh
$ ./configure --host=arm-linux-gnueabihf
$ make
$ make check # optional
$ make install # may need su
```

If you installed the library it will now be in `/usr/local/lib/`

otherwise, it can be found in `${PROJECT_BINARY_DIR}/.libs/`

You will need to copy this library to `${PROJECT_BINARY_DIR}/lib/json-c/` once you have cloned `Beagle Boop`

i.e.

```sh
$ cp /usr/local/lib/libjson-c.a ${PROJECT_BINARY_DIR}/lib/json-c/
```


#### curl:

`curl` releases: https://curl.haxx.se/download.html

```sh
$ ./configure --host=arm-linux-gnueabihf
$ make
$ make test # optional
$ make install # may need su
```

Once installed, CMake knows where to find it.

### Build instructions:


`Beagle Boop` GitLab repo: https://csil-git1.cs.surrey.sfu.ca/Beagle-Boop/Beagle-Boop-Game

```sh
$ git clone https://csil-git1.cs.surrey.sfu.ca/Beagle-Boop/Beagle-Boop-Game.git
$ cd Beagle-Boop-Game
$ mkdir build
$ cd build/
$ cmake .. /
$ make
```

The executable will be directly built to your NFS folder `/home/$USER/cmpt433/public/myApps` 