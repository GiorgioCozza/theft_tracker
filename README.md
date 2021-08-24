# theft_tracker
A localization algorithm for vehicle anti-theft systems based on WSN.

## Prerequisites

Contiki OS is already included in the repository while Node-RED must be installed (https://nodered.org/docs/getting-started/local).
An additional node type is required to display positions in a world map (https://flows.nodered.org/node/node-red-contrib-web-worldmap).

## How to build the application?

The target platform is a Sky mote, from the compile procedure three different binaries should be generated, the simple mote and the two gateways required
to test the application. The gateways includes fake coordinates hard-coded in the source files to simulate GPS, these two motes must be static. 
At first it is required to go to `contiki_os/apps/theft_tracker/` and build the application library:

 `make libs`
 
 tben compile the mote application:
 
 `make TARGET=sky TYPE=mote`
 
 then the two gateways from the `theft_tracker_gateway.ctemp`:
 
 `make TARGET=sky TYPE=gtw1`
 
 `make TARGET=sky TYPE=gtw2`
 
 the application is ready to be simulated in Cooja.
 
 ## How to simulate the application in Cooja?
 
 All the procedure is explained in the last section of `report/report.pdf`

