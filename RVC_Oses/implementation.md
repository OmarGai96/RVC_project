# Robot Vacuum Cleaner - Project implementation

## Summary of the project
This project aims to implement a real time software for a Robot Vacuum Cleaner (RVC). <br>
The robot is able to move into a room and automatically do some actions. <br>
The system offers the following functionalities: <br>
  - check the presence of obstacles (using proximity sensors)
  - moves into a room adjusting direction of movements (based on obstacles)
  - clean the floor (actioning brushes)
  - check status of internal resources (battery and garbage bag) 
  - notify to the extern some events (battery low or garbage bag full) 

In the following is reported the implementation of each task.

## Task 1: obstacle_control
Two proximity sensors monitor the occurrence of obstacles during the movements. Impediments can be physical objects in front of the robot or walls under it. In both cases this task sends a soft interrupt to Task2 (movements_control), in order to signal an imminent stop. Furthermore Task1 upgrades needed flags and the correspondent position in the map.

## Task 2: movements_control
This task handles the control of movements and the writing of an internal map (modeled as a matrix). The robot will start from a corner of the room and, knowning the dimension of the room, it decides where the robot should go. It controls the direction and rotation of movements an whenever an obstacle is found, it updates the internal map. After stopping, it decides the next direction it should follow. <br>
If the battery is low (notification arrives from Task3) it can decide to go back to the recharging station. In order to do that, it applies an algorithm on the internal map to find the shorter path.

## Task 3: check_resources 
This task periodically checks: <br>
 - battery status
 - garbage bag status <br>
In the first case, it compares the actual battery status with some thresholds. If the battery is LOW, this task sends a mail to Task2
(movements_control) and set an event for Task4 (acoustic_signals). <br>
If the battery is TOTALLY LOW, it sends a soft interrupt to the internal system to force the turning off.<br>
In the second case, it notifies to Task2 and Task4 if the garbage bag is FULL, so it is necessary to go home (reach the charging station).

## Task 4: acoustic_signals
This task continuously checks if some events are set (information arriving from Task3): if they are active the task initiates a sound sequence to notify to the outside the internal status of the system (battery is low and/or garbage bag is full).

## Task 5: brushes_speed
Brushes’ speed changes based on characteristics of the surface. An increase in speed may be caused by a dirtier floor or a different kind of surface (i.e. a rug). This demands more power to reach a more efficient cleaning. On the other hand, when there is a decrease in speed, we need less energy, or we can set a medium value. <br>
If this task receives a mail from Task2 (the robot is going back to charging station), it turns off the brushes, because not needed.

## Tasks properties

In the following table are reported the main properties of the tasks. Note that Hard R.T. means that the task is Hard Real Time, so missing its
deadline may cause serious consequences on the environment under control.

|Task |Priority |Period ms |Duration |Constraints |Preemptable | Deadline ms |
| ----- | ----- | ----- | ----- | ----- | ----- | ----- |
|Task1 |Max |500 |Low |Hard R.T. |no  |100 |
|Task2 |Med |500 |Med |Hard R.T. |yes |100 |
|Task3 |Med |250 |Med |Soft R.T. |yes |200 |
|Task4 |Low |-   |Low |Soft R.T. |yes |-   |
|Task5 |Med |250 |Med |Soft R.T. |yes |200 |
|Tsystem |Low |- |-   |-         |yes |-   |

On the following figure you can see the predicted and actual behavior of the tasks.

![Timing_tasks](/doc/figures/timing_tasks.png)
