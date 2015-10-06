# Control-flow-paths
From a source code it generate node to node control flow graph and then generate all possible path between two given nodes.

Before running batch file...

1. Put the source C file in lib folder and rename it "test.c"

***remember all opening brackets should be at end of the statement.
for example:
*****correct****
void main(){
}
****************
*****Wrong****
void main()
{
}
****************


2. open "funcStrtEnd.txt" and add function_name start_node end_node(line no) (for example: decision_algo 208 237).
You can add more functions.

3. RUN the batch file.
At some point it need input as "Enter root node and leave node". It means insert start and end node(i.e. line no) where you want the generate paths.
for example: If you input 208 237 then it will compute all path between 208 and 237.


4. A text file will show all paths.

