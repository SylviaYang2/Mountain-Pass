# Mountain-Pass
When people plan roads, rail roads or other routes over mountain ranges they spend a lot of time working with elevation data to chart a good path. A good path is one that has a minimal change in elevation. When traveling over a mountain range it is impossible to avoid a bunch of elevation gain and then fall but the more that can be avoided the better.
This program reads in a CSV file of topographic (land elevation) data into a 2D array, find the highest point, chart a low elevation change path through the land shown and output an image of the path.

## Algorithm
Implemented a "greedy" algorithm to find the path. 
Since our map is in a 2D grid, we can envision a "walk" as starting in some cell at the left-most edge of the map (column 0) and proceeding forward by taking a "step" into one of the 3 adjacent cells in the next column over (column 1). Our "greedy walk" will assume that you will choose the cell whose elevation is closest to the elevation of the cell you're standing in. (NOTE: this might mean walking uphill or downhill).

<img width="279" alt="image" src="https://user-images.githubusercontent.com/83314726/184468644-b809bf3c-1714-41d8-ab14-882a30e0bc4c.png">

In the case of a tie with the forward position, we should always choose to go straight forward. In the case of a tie between the two non- forward locations, we should flip a coin to choose where to go.

<img width="742" alt="image" src="https://user-images.githubusercontent.com/83314726/184468684-66e46223-eadb-4a43-affd-92352b01419c.png">
