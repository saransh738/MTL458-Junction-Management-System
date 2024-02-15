# Implemented a Junction Management System
## Problem Statement
We are required to implement a Junction management system. There are 4 lanes which intersect with each other. Trains can enter a lane from the directions as shown in the figure. 
North is top of the figure. Each lane can allow only one train to pass through it at a time. Each train is represented by an independent thread and it performs the following 3 functions, in order:
* Arrive Lane - arriveLane(): The front of the train arrives at the first intersection point in the lane.
* Cross Lane - crosslane (): The train crosses the lane.
* Exit Lane - exitlane(): The complete train exits the lane.
  ![image](https://github.com/saransh738/MTL458-Junction-Management-System/assets/74806993/184efeaa-1d4a-47c6-bd0c-42260cb85e57)

To prevent crashes and multiple trains entering a lane at the same time, the trains do the following:
* To arrive at a lane, a train blocks the first intersection on the lane in advance.
* To cross a lane it must block the next intersection on the lane.
* Since the size of the train is large, it is safe to unblock the intersection points it has blocked earlier only after the train has exited the lane.
  
Note that following the above conditions can cause a deadlock. You are required to detect and resolve any deadlock. For this, a separate thread will be running, which periodically checks if there is a deadlock and signals one of the trains to go. You are free to use any synchronization primitives for implementing controllers, and other critical sections/signaling in your code.
Your submission will be judged on 3 properties:
* Safety: No crash can occur, and only one train can use a lane at a time.
* Liveness: The system is not stuck forever.
* Starvation: No train should be starved from entering the lane.
