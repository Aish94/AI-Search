This project implements 4 AI search techniques:
1. Breadth First Search
2. Depth First Search
3. Uniform Cost Search
4. A* Search

The input.txt file specifies the input in the following format:
<ALGO>
<START STATE>
<GOAL STATE>
<NUMBER OF LIVE TRAFFIC LINES>
<... LIVE TRAFFIC LINES ...>
<NUMBER OF SUNDAY TRAFFIC LINES>
<... SUNDAY TRAFFIC LINES ...>

The live traffic lines specifies the actual cost between two states
the sunday traffic lives specifies the heuristic cost from the intital state to that state

The output.txt file displays the output in the following format:
A list of states from start to goal-
<STATE> <ACCUMULATED TRAVEL TIME FROM START TO HERE>
