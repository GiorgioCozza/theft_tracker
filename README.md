# theft_tracker
A localization algorithm for a vehicle anti-theft system based on WSN.

## Prerequisites

Contiki OS is already included in the repository while Node-RED must be installed (https://nodered.org/docs/getting-started/local).
An additional node type is required to display positions in a world map (https://flows.nodered.org/node/node-red-contrib-web-worldmap).

## How to build the application?

The target platform is a Sky mote, from the compile procedure three different binaries should be generated, the simple mote and the two gateways required
to test the application. The gateways include two fake coordinates hard-coded in the source files to simulate GPS, these two motes must be static. 
At first it is required to go to `contiki_os/apps/theft_tracker/` and build the application library:

 `make libs`
 
 tben compile the mote application:
 
 `make TARGET=sky TYPE=mote`
 
 then the two gateways from the `theft_tracker_gateway.ctemp`:
 
 `make TARGET=sky TYPE=gtw1`
 
 `make TARGET=sky TYPE=gtw2`
 
 the application is ready to be simulated in Cooja.
 
 ## How to simulate the application in Cooja?
 Once the application is built:
 
1) Open Node-RED and import the flow contained in `./server_nr/tt_server_flow.json` and deploy
2) Open Cooja and create a new simulation
3) Set the simulation speed at 100%
4) Add 4 Sky motes and insert the firmware in `contiki_os/apps/theft_tracker/src/theft_tracker_mote.sky`, place the motes at a proper distance
5) Add the first gateway with the firmware in `contiki_os/apps/theft_tracker/src/theft_tracker_gateway_1.sky` 
6) Configure the gateway as TCP Server with port 60005
7) Add the second gateway with the firmware in `contiki_os/apps/theft_tracker/src/theft_tracker_gateway_2.sky` 
8) Configure the gateway as TCP Server with port 60006
9) Move on Node-RED and press `Ctrl+shift+m` to open the world map
10) Choose the mote that will be the stolen vehicle, press the mote button and move it near the motes/gateways
11) Observe how the position and distance estimation change in the world map 


