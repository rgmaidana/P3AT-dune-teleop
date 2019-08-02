# Pioneer 3-AT Teleoperation client
Client for driving the P3AT mobile base around (i.e., teleoperation), using LSTS' Dune/IMC framework.

This DUNE task sends DesiredVelocity messages over UDP to a Pioneer 3-AT robot, ideally running the [P3AT DUNE driver](https://github.com/rgmaidana/P3AT-dune-driver).

The task's code was based on the [rosaria_client](https://github.com/pengtang/rosaria_client) ROS package.

# Installation/Build

* Follow the DUNE framework install/build instructions: [Obtaining the Source Code](https://github.com/LSTS/dune/wiki/Obtaining-the-Source-Code); [Building: Unix Command Line](https://github.com/LSTS/dune/wiki/Building%3A-Unix-Command-Line);

* Clone this repository to some directory (e.g., your home directory)

```
git clone https://github.com/rgmaidana/P3AT-dune-teleop $HOME/P3AT-dune-teleop
```

* Go to DUNE's root directory (for example, *$HOME/dune/dune*, considering DUNE was built in your home directory) and copy the P3AT Teleop task to DUNE's src directory

```
cd $HOME/dune/dune
cp -r $HOME/P3AT-dune-driver/src/P3AT $HOME/dune/dune/src
```

* Copy the P3AT Teleop configuration file ([P3AT_Teleop.ini](https://github.com/rgmaidana/P3AT-dune-teleop/blob/master/etc/P3AT_Teleop.ini)) to DUNE's etc directory

```
cp -r $HOME/P3AT-dune-driver/etc/P3AT_Teleop.ini $HOME/dune/dune/etc
```

* Clone the IMC repository to DUNE's build directory (for example, *$HOME/dune/build*, considering DUNE was built in your home directory)

```
git clone https://github.com/lsts/imc $HOME/dune/build/IMC
```

* Go to DUNE's build directory, build the IMC messages and rebuild the project

```
cd $HOME/dune/build
make imc
make rebuild_cache
make
```

## Dependencies

* [DUNE](https://lsts.fe.up.pt/toolchain/dune)
* [IMC](https://lsts.fe.up.pt/toolchain/imc)

# Usage

* Set up a P3AT mobile base, build and run the [P3AT DUNE driver](https://github.com/rgmaidana/P3AT-dune-driver) on the robot
* As the DesiredVelocity messages are sent over UDP to a static IP address, modify the P3AT_Teleop.ini configuration file to point to your robot computer's IP address and port (port 6002 by default in the P3AT driver)
* On your host computer, go to DUNE's build folder (*$HOME/dune/build*) and run the DUNE application, indicating the P3AT_Teleop configuration file:

```
cd $HOME/dune/build
./dune -c P3AT_Teleop
```

* Use your keyboard's arrow keys to move the robot around (a/z to increase/decrease linear velocity, s/x to increase/decrease angular velocity)

# Acknowledgements

* [Laboratório de Sistemas e Tecnologia Subaquática (LSTS)](https://lsts.fe.up.pt/)
* [Laboratório de Sistemas Autônomos (LSA)](https://lsa-pucrs.github.io/)
* [rosaria_client package](https://github.com/pengtang/rosaria_client)

# Contributors

* [Renan Maidana](https://github.com/rgmaidana)
