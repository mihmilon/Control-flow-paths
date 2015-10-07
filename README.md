# Control-flow-paths
From a source code it generate node to node control flow graph and then generate all possible path between two given nodes.

Before running batch file...

1. Put the source C file in lib folder and rename it "test.c"

***remember all opening brackets should be at end of the statement.

for example:

*****correct****

void main(){<br>
}
****************

*****Wrong****

void main()<br>
{<br>
}

****************


2. open "funcStrtEnd.txt" and add function_name start_node end_node(line no) (for example: decision_algo 208 237).
You can add more functions.

3. RUN the batch file.
At some point it need input as "Enter root node and leave node". It means insert start and end node(i.e. line no) where you want the generate paths.
for example: If you input 208 237 then it will compute all path between 208 and 237.


4. A text file will show all paths.

sample output:
208 209 210 211 212 213 214 215 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 216 217 218 219 181 182 183 184 185 186 187<br><br> 188 189 190 191 192 193 194 195 196 197 198 220 221 222 223 224 225 226 227 228 229 230 231 232 233 234 235 236 237 <br><br>
208 209 210 211 212 213 214 215 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 216 217 218 219 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 220 221 222 223 224 225 226 227 228 229 230 231 234 235 236 237 <br><br>
208 209 210 211 212 213 214 215 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 216 217 218 219 181 182 183 184 185 186 187 188 189 190 191 192 193 194 195 196 197 198 220 221 222 223 224 225 226 227 228 229 230 235 236 237 


