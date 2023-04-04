# Robot Vacuum Cleaner - Project implementation

## Summary of the project
This project aims to implement a real time software for a Robot vacuum cleaner. The robot is able to go around a room and automatically do some actions. <br>
The system offers the following functionalities: <br>
  - clean the floor (actioning brushes)
  - adjust speed and direction of movements 
  - check status of internal resources 
  - check the presence of obstacles (using proximity sensors)

## Task 1: obstacle_control
Two proximity sensors monitor the occurrence of obstacles during the movements. They can be objects or walls in front of the robot, and in the second case holes under it. In both cases this task needs to stop the robot, with an interrupt to the engine, and upgrades needed flags and the correspondent position in the matrix

## Task 2: movements_control
This task handles the control of movements and the writing of an internal map. The task will start from a corner of the room and the known dimension of the room, it decides where the robot should go and controls the direction and rotation, whenever an obstacle is found it updates the internal map. After stopping, because of an obstacle it decides the next direction it should follow. It also periodically checks the flags of the resources, in particular if the battery is low it can decide to go back to the recharge station. In order to do that, it applies an algorithm on the internal map to find the shorter path.
## Task 3: check_resources 
This task periodically starts and does the following checks: <br>
 - control the battery status
 - control if the garbage bag is full
Then it can display information to the user, communicate to the other tasks when the garbage bag is full and/or battery is low.

## Task 4: acoustic_signals
This task continuously checks the value of some flags (information sent from Task3): if they are active the task initiates a sound sequence to notify to the outside the internal status of the system (battery and garbage bag).

## Task 5: brushes_speed
Brushes’ speed changes based on characteristics of the surface. An increase in speed may be caused by a dirtier floor or a different kind of surface (i.e. a rug). This demands more power to reach a more efficient cleaning. On the other hand, when there is a decrease in speed, we need less energy, or we can set a medium value.

## Tasks properties
|Task |Priority |Period ms |Duration |Constraints |Preemptable | Deadline ms |
| ----- | ----- | ----- | ----- | ----- | ----- | ----- |
|Task1 |Max |500 |Low |Hard R.T. |no  |100 |
|Task2 |Med |500 |Med |Hard R.T. |yes |100 |
|Task3 |Med |250 |Med |Soft R.T. |yes |200 |
|Task4 |Low |- | |Low |Soft R.T. |yes |- |
|Task5 |Med |250 |Med |Soft R.T. |yes |200 |
|Tsystem |Low |- |- |- |yes |- |