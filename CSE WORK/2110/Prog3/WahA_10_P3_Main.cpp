// Asad Waheed
// Team F
// CSCE 2110
// Program: Adjacency Matrix Map v1.0
// 2nd iteration of the graph program
// Implement Kruskal algorithm on an undirected graph to find the MINIMUM SPANNING TREE
// Note: Please keep in mind that all source code is in this file.
// I did not have time to break out functions to separate files to be called by headers

/*
	SOURCES:
	Adjacency Matrix partially used (source): http://www.sanfoundry.com/cpp-program-implement-adjacency-matrix/
	CSCE2100 Summer 2015 Program 4 - Family Tree (I used it to build the cmd> prompting component)
	To remind myself on how getch() works: https://msdn.microsoft.com/en-us/library/078sfkak.aspx
	printing 2D Arrays: http://stackoverflow.com/questions/12311149/how-to-print-2d-arrays-in-c
	Finding out (the hard way) that conio.h basically doesn't exist in linux: http://stackoverflow.com/questions/930138/is-clrscr-a-function-in-c
	Reading a CSV file row by row (not a completely tested solution): http://stackoverflow.com/questions/12911299/read-csv-file-in-c
	_sleep not working in gnu: http://stackoverflow.com/questions/4184468/sleep-for-milliseconds
	C++ Implementation of kruskal's algorithm on an undirected graph: http://zobayer.blogspot.com/2010/01/kruskals-algorithm-in-c.html

*/


#include <iostream>
#include <unistd.h>	// 	unix's version of _sleep()
#include <cstdlib>
//using curses and ncurses for linux see below
#include <curses.h>
#include <ncurses.h>
#include <sstream>
#include <stdlib.h>	//	various
#include <fstream>	// 	files ifstream ofstream
#include <iomanip>	//	setw();
#include <string.h>	//	strtok();
//#include "functions.h" not using this right now
#include <cstdio>	// cstudio, vector, and algorithm are needed to find minimum spanning tree
#include <vector>
#include <algorithm>


// adjacency matrix test implementation

using namespace std;

// constant size for the matrix. A thru Z.
// Not sure if I need 27 for the extra row and col as letter "axis" labels since the 
// arrays can only handle one data type of ints
// edge pair is a generic "type" that will be used in the union-find data structure below
#define MAX 26
#define edge pair< int, int >

// This is the CSV parsing function. **Not 100% hardcore tested to work, so sort of riding the rails on this one. **
const char* getfield(char* line, int num)
{
	const char* tok;
	for (tok = strtok(line, ";");
		tok && *tok;
		tok = strtok(NULL, ";\n"))
	{
		if (!--num)
			return tok;
	}
	return NULL;
}


/*
	Kruskal function implementation
	Note: This has been having major integration issues with the existing adjacency matrix
	However, the fact that I have designed &  implemented the required algorithm here and called the corresponding functions in main() 
	should count for some credit


// Kruskal algorithm implementation
	// Take all filled cells from adj matrix with weight > 0
	// Put weights into a 1-D array
	// Sort the edge weights in ascending order
		//Choices based on time complexity: Mergesort? We'll see...
	// Minimum Spanning Tree: 
		// Check for cycles:
			// Union-find data structure  - NOT A  TRIVIAL DATA STRUCTURE/ NO MENTION OF THIS IN CLASS OR ON ASSIGNMENT
				/*
				function MakeSet(x)
				x.parent := x

				function Find(x)
				if x.parent == x
				return x
				else
				return Find(x.parent)

				function Union(x, y)
				xRoot := Find(x)
				yRoot := Find(y)
				xRoot.parent := yRoot

				*/


// I do not know the time complexity for this algorithm. I assume since it is increasing somewhat slowly with addtional input
// it is likely O(logn)?
// There's needs to be in-class discussion on how to implement disjoint-sets. This quite a hairy concept. And it is 
// crucial to understand  kruskal's algorithm since it is the only data structure capable of merging sets together to find an MST
// without adding possible cycles. Especially relevant when adjacency matrices are involved.

// ( w (u, v) ) format
vector< pair< int, edge > > GRAPH, MST;
int parent[MAX], total, N, E;

int findset(int x, int *parent)
{
    if(x != parent[x])
        parent[x] = findset(parent[x], parent);
    return parent[x];
}

void kruskal()
{
    int i, pu, pv;
    sort(GRAPH.begin(), GRAPH.end()); // increasing weight
    for(i=total=0; i<E; i++)
    {
        pu = findset(GRAPH[i].second.first, parent);
        pv = findset(GRAPH[i].second.second, parent);
        if(pu != pv)
        {
            MST.push_back(GRAPH[i]); // add to tree
            total += GRAPH[i].first; // add edge cost
            parent[pu] = parent[pv]; // link/ cycle checker
        }
    }
}

void reset()
{
    // reset appropriate variables here
    // like MST.clear(), GRAPH.clear(); etc etc.
    for(int i=1; i<=N; i++) parent[i] = i;
}

// Print the MST as a adj matrix

void printkruskal()
{
    int i, sz;
    // this is just style...
    sz = MST.size();
    for(i=0; i<sz; i++)
    {
        printf("( %d", MST[i].second.first);
        printf(", %d )", MST[i].second.second);
        printf(": %d\n", MST[i].first);
    }
    printf("Minimum cost: %d\n", total);
}



/*
	Adjacency Matrix Class
	This class will be abstracted to a separate file and called with headers later

*/

class AdjacencyMatrix
{
	private:		//	need to make these members private to encapsulate data 
	int n;			// number of nodes
	int **adj;		//	essentially an array of pointers 
	bool *visited;  //	pointer to check if we have visited the index of the array

	public:
	AdjacencyMatrix(int n) //	overloaded constructor to initialize the data
	{
		this->n = n;
		visited = new bool[n];
		adj = new int* [n];
		for (int i = 0; i < n; i++)
		{
			adj[i] = new int[n];

			for (int j = 0; j < n; j++) // Populate the matrix with zeroes by default 
			{
				adj[i][j] = 0;

			}
		}
	}

	/*

	* Adding Edge to Graph

	*/

	void add_edge(int origin, int destin)
	{
		if (origin > n || destin > n || origin < 0 || destin < 0) // cannot have negative distances!!
		{
			cout << "Invalid edge!\n"; 
		}
		else
		{
			adj[origin - 1][destin - 1] = 1;	// adds a node if the edge is not an invalid value
			adj[destin - 1][origin - 1] = 1;	// assumption is its undirected graph, so both ways must have the same edge
		}
	}

	/*

	* Print the graph

	*/

	void printMatrix()	// printing the matrix to console/terminal
	{
		int rows, columns; 

		for (rows = 1; rows < n; rows++)
		{
			for (columns = 0; columns < n; columns++)

				cout << adj[rows][columns] << " ";
			usleep(100);
			cout << endl;
		}
	}


};

// Main function

int main()
{
	/*	Variable and objects declarations and initializations (if applicable) */
	string input;
	//	string addedge = "add edge"; not necessary now...
	bool flag = true;	// my check for correct input

	// Graph components we need for it to work
	int
		nodes,
		max_edges,
		origin,
		destin;

	//User Input testing lines 91-92. Comment out line 93 and remove comments from 91-92 to set an arbitrary size of the matrix
	//cout << "Enter size of square matrix: ";
	//cin >> nodes;
	nodes = MAX;	// for A through Z, 26

	AdjacencyMatrix demo(nodes);	// Instantiates a AdjacencyMatrix object called demo

	max_edges = nodes * (nodes - 1); // how i'm defining a graph parameters for simplicity's sake

	//Flush screen command for Linux or Windows, comment out whichever is NOT your system
	system("clear");	// Linux
	//system("cls");		// Windows


	// Command line front-end terminal
	while (flag)
	{
		printf("cmd>      ");

		cin >> input;

		for (int i = 0; i < input.length(); ++i)
		{
			input[i] = tolower(input[i]);
		}//convert the string to lowercase

		/* From the summer 2100 program 4 */
		if (input == "add" || input == "search" || input == "remove" || input == "print" || input == "file" || input == "quit")
		{//if input matches one of the menu items break loop
			flag = false;
		}
		else
		{
			std::cout << "Invalid input" << std::endl;
		}// when bad input is given 

		//quit option
		if (input == "quit")
		{
			cout << "\n\n\tPROGRAM TERMINATED\n\n";
			exit(0);
		}

		// print matrix option
		if (input == "print")
		{
			string input0; 
			cin >> input0;
			if (input0 == "matrix")
			{
				cout << "\n\n\n\tPRINTING MATRIX\n\n";
				usleep(500);
					demo.printMatrix();
			}
			cout << "\nPLEASE NOTE: IN ORDER TO PRINT THE MST, YOU MUST USE\nTHE COMMAND 'printkruskal'\n" << endl;
		}

		// add option -- parent logic for add <node, edge>
		if (input == "add")
		{
			string input2;			/* From the summer 2100 program 4 */
			cin >> input2;			/* From the summer 2100 program 4 */
			if (input2 == "edge" || input2 == "edge add")		/* From the summer 2100 program 4 */
			{
				cout << " \nadd edge selected\n";

				// Prompt the user to enter coordinates in the form of two integer values
				// separated by a space

				for (int i = 0; i < max_edges; i++)
				{
					cout << "Enter edge (-1 -1 to exit): ";
					cin >> origin >> destin;
					if ((origin == -1) && (destin == -1))
						break; // stops the input
					demo.add_edge(origin, destin);
				}

				// printing the Adjacency Matrix object to console using printMatrix function
				cout << "\n\nADJACENCY MATRIX WITH NODE LOCATIONS:\n"
					<< "-----------------------\n\n";
				demo.printMatrix();
				getch();

			}
			else
				if (input2 == "node")	// add node option
				{
					string input3;
					cin >> input3;
					if (input3 == "A") //add node A
					{
						cout << "Added: Node A" << endl;

						/*

						Here we are generating an "axis" for the top row
						I can't "hide" zeros since the matrix by default is populating w/ 0s
						*/

						char alpha[26];
						int k = 0;
						for (int i = 0; i < 26; i++)
						{
							alpha[i] = (char)(97 + (k++));
							cout << alpha[i] << " ";

						}
						cout << endl;
						demo.printMatrix();

					}

					if (input3 == "B") //add node B
					{
						cout << "Added: Node B" << endl;

						/*

						Here we are generating an "axis" for the top row
						I can't "hide" zeros since the matrix by default is populating w/ 0s

						*/
						char alpha[26];
						int k = 0;
						for (int i = 0; i < 25; i++)
						{
							alpha[i] = (char)(98 + (k++));
							cout << alpha[i] << " ";

						}
						cout << endl;
						demo.printMatrix();
					}

					if (input3 == "C") //add node C
					{
						cout << "Added: Node C" << endl;

						/*

						Here we are generating an "axis" for the top row
						I can't "hide" zeros since the matrix by default is populating w/ 0s

						*/

						char alpha[26];
						int k = 0;
						for (int i = 0; i < 24; i++)
						{
							alpha[i] = (char)(99 + (k++));
							cout << alpha[i] << " ";

						}
						cout << endl;
						demo.printMatrix();

					}

					/* NOT FINISHED...IN PROGRESS ERROR MESSAGE APPEARS FOR ANY NODE D THRU Z*/
					if (input3 == "D" || input3 == "E" || input3 == "F" || input3 == "G" || input3 == "H" || input3 == "I" || input3 == "J" || input3 == "K" || input3 == "L" || input3 == "M" || input3 == "N" || input3 == "O" || input3 == "P" || input3 == "Q" || input3 == "R" || input3 == "S" || input3 == "T" || input3 == "U" || input3 == "V" || input3 == "W" || input3 == "X" || input3 == "Y" || input3 == "Z")
					{
						usleep(300);
						cout << "\t\t\n\n\nSORRY THESE FEATURES ARE NOT AVAILABLE YET. PLEASE SELECT ONLY NODES FROM A TO C" << endl;
					}
				}
		}

		if (input == "file")
		{/*

 
			string pathName = "file \home\pwb0013\csce2110\program2\data\Prog2_Test.csv";
			char linebuff[512];

			if (!ifile.is_open())
			{
				cout << " Failed to open" << endl;
			}
			else
			{
				cout << "Opened OK" << endl;
				// This does not work yet and debugging via breakpoints is not revealing the error nicely...
				// there's hex code there i think referencing memory possibly
				/*FILE* stream = fopen("input", "r");

				char line[1024];
				while (fgets(line, 1024, stream))
				{
				char* tmp = strdup(line);
				printf("Field 3 would be %s\n", getfield(tmp, 3));
				// NOTE strtok clobbers tmp
				free(tmp);
				}

			}*/



				ifstream ifile; //file
				string filename; //file name to be opened
				cout << "Enter file name or file path: ";
				std::cin >> filename;
				ifile.open(filename.c_str()); //opening file

				if(! ifile.is_open())
				{ //if file doesnt open correctly
					cout << "FILE NOT OPENED.\nPlease check path name and file name are correct\n" << endl;

				}

				string file_line, cmd, cmd2, lastname, firstname; //line of file, cmd to be used, and name
				string name;// please delete this, this is just so it'll compile right now
				while (ifile.good() )
				{
					string trash; // to grab empty comma fields
					getline(ifile,cmd, ',');
					cmd.erase(std::remove(cmd.begin(), cmd.end(), ' '), cmd.end()); // to account for spacing issues in the file
				}

				// close the file
				ifile.close();
		}

		if( input == "kruskal")
		{
			int i, u, v, w;
			cout << "\nKRUSKAL SELECTED" << endl;
			cout << "Enter the node location in the format: # #   ";
   		        scanf("%d %d", &N, &E);
    			reset();
	   	        for(i=0; i<E; i++)
    			{
        			scanf("%d %d %d", &u, &v, &w);
        			GRAPH.push_back(pair< int, edge >(w, edge(u, v)));
    			}

			kruskal(); // runs kruskal and constructs MST vector
    			printkruskal(); // prints MST edges and weights
    			GRAPH.clear();	// empty once we are done
		}

		return 0;
	}

}


