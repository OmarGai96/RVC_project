# RVC_Oses_Project

## Main Goal
Development of a Real Time Operating Systems, aims to control a Robot Vacuum Cleaner.  

## Authors 
Caviglia Alessio, Gai Omar, Gianino Fabio

## Summary of the project
This project aims to implement a real time software for a Robot Vacuum Cleaner (RVC). <br>
The robot is able to move into a room and automatically do some actions. <br>
The system offers the following functionalities: <br>
  - check the presence of obstacles (using proximity sensors)
  - moves into a room adjusting direction of movements (based on obstacles)
  - clean the floor (actioning brushes)
  - check status of internal resources (battery and garbage bag) 
  - notify to the extern some events (battery low or garbage bag full) 

## Target hardware
STM32F429I-DISCOVERY <br>
see next for more information. <br>
![STM32F4](doc/figures/board_image.PNG)

**Useful files**

- [RT-Thread Documentation](doc/README.md)
- [Board Documentation](RVC_Oses/README.md)
- [Project Implementation](RVC_Oses/implementation.md)
- [Simulations](benchmark_def_simulations/README.md)
