# cs415-project02-topological-sorting
Topological Sorting

Soren Richenberg
Owen Mastropietro

Instructions To Run:
$ make
$ ./topological_sort.out
enter the name of the data file you wish to use

We were confused about the wording on canvas about the output file: 

"Output on the console as indicated above.
In addition, code should save the output in a file called "out" + input filename. For example, if input filename is "data3.txt", output filename is "outdata3.txt"."

We didn't have enough time to ask about this on Piazza, so we used our best judgement. 

If you would like to use the console for output instead of an output file, change `out_file` to `std::cout` on lines 20-29 of the source code in "main.cpp"

Extra Credit Opportunity:
- Implemented Adjacency List in place of the inferior Adjacency Matrix.
- Implemented Queue-Based Source Removal Algorithm in place of DFS.

Project Reflection:
- Decrease-and-Conquer::Topological-Sorting
- Motivation: Rental properties or Airbnb's need to efficiently select a chain of clients that will yield the maximum total revenue over a given period of time. For example, if a property is available for one year, there will be a chain of clients to rent to that will yield a higher total profit. This implementation gave us a real-world perspective in utilizing the deacrease-and-conquer approach for topologically sorting.

- Input: Text file containing three pieces of client information per line in the form of three integers. You can expect to there to be a move-in (start) date followed by a move-out date (end) date, followed by the amount of money (revenue) they will pay for that duration.

- Output: Can output directly to screen or to files with user provided input. Will provide information on about the number of clients being considered in a particular sample input data, the optimal revenue that can be earned given the dataset, and the particular optimal subset of clients - which clients will be chosen to - contribute to the optimal revenue earned.

In order to employ a decrease-and-conquer approach to achieving this we considered a directed-acyclic-graph-based (DAG) approach such that we could employ a source-removal algorithm, such as Depth First Search (DFS), to ensure all vertices in the graph are visisted - all clients in the sample data set are considered - while constructing an optimal subset of those clients to provide services to in order to achieve the maximum possible revenue.
In representing this DAG, an adjacency list was chosen over an adjacency matrix because of it's potentially limiting space taken by entries in the matrix that don't correspond to useful data. This comes at the cost of time int searching the adjacency list.
In an effort to offset the additional cost of time from our spacially-improved adjacency list representation, a queue-based source-removal algorithm was chosen over a DFS-based source-removal algorithm in order to reduce the time overhead stemming from the recursive calls made in the DFS approach.# cs415-project02-topological-sorting
