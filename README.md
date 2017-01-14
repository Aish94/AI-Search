This project implements 4 AI search techniques:  
<ol>
<li>Breadth First Search</li>
<li>Depth First Search</li>
<li>Uniform Cost Search</li>
<li>A* Search</li>
</ol>

**INPUT**</br>  
The input.txt file specifies the input in the following format:</br> 
\<ALGO\></br> 
\<START STATE\></br> 
\<GOAL STATE\></br>
\<NUMBER OF LIVE TRAFFIC LINES\></br> 
\<... LIVE TRAFFIC LINES ...\></br>
\<NUMBER OF SUNDAY TRAFFIC LINES\></br> 
\<... SUNDAY TRAFFIC LINES ...\></br>
</br>
The live traffic lines specifies the actual cost between two states</br>
the sunday traffic lives specifies the heuristic cost from the intital state to that state</br>
</br>
**OUTPUT**</br>
The output.txt file displays the output in the following format:</br>
A list of states from start to goal-</br>
\<STATE\> \<ACCUMULATED TRAVEL TIME FROM START TO HERE\>
