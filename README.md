# Problem description
The algorithm constructs a virtual representation of a highway composed of multiple stations (given in input as a sequence of add-station commands).
Each station is identified by its location along the highway (in km) and contains multiple cars, each with their own "autonomy" value that indicates how many km they can travel from their station.
It also elaborates the shortest path between two arbitrary stations (requested by the input with plan-path command).
- The shortest path is defined as the path with the least number of stops
- At each stop, the algorithm must change car
- For multiple paths with the same number of stops, the algorithm chooses the path with stations closer to km 0.
