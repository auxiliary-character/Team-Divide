# Team Divide

So Coach Andy and I were talking in the truck on our way home from Iowa, wondering about how we could do scouting better. I suggested
that perhaps instead of assigning particular slots of a match to a student (such as 3rd robot, blue alliance), maybe we could instead ask each student to follow a handful of teams throughout the day.

There was a problem with this approach, however. If a student has two teams in the same match, they would have to divert their attention to watch both of them.

Fortunately, we can solve this problem with graph theory. If we construct a graph where every team is a node, and there are edges between nodes, our problem becomes a [graph coloring](https://en.wikipedia.org/wiki/Graph_coloring) problem. After constructing our graph with match data from The Blue Alliance's API, we just need to apply a greedy algorithm to solve it.
