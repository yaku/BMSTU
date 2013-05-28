#include <stdio.h>
#include <random>
#include <time.h>
#include<iostream>
#define INFINITY 99

using namespace std;

class Dijkstra
{
private:
	int adjMatrix[15][15];
	int predecessor[15],distance[15];
	bool mark[15];
	int source;
	int numOfVertices;

public:

	void read();
	void initialize();
	int getClosestUnmarkedNode();
	void calculateDistance();
	void output();
	void printPath(int);

	void Print();
};

void Dijkstra::read()
{
	cout<<"Enter the number of vertices of the graph\n";
	cin>>numOfVertices;

	while (numOfVertices <= 0) {
		cout<<"Enter the number of vertices of the graph\n";
		cin>>numOfVertices;
	}

	srand(time(NULL));

	for (int i=0;i<numOfVertices;i++) {

		for (int j=0;j<numOfVertices;j++) {

			adjMatrix[i][j] = (i == j) ? 0 : rand() % 15;
		}
	}
	cout<<"Enter the source vertex\n";
	cin>>source;
	while((source<0) && (source>numOfVertices-1)) {
		cout<<"Source vertex should be between 0 and"<<numOfVertices-1<<endl;
		cout<<"Enter the source vertex again\n";
		cin>>source;
	}
}

void Dijkstra::Print()
{

	for (int i=0;i<numOfVertices;i++) {

		for (int j=0;j<numOfVertices;j++)
			cout << adjMatrix[i][j] << "\t";

		cout << endl;
	}
}

void Dijkstra::initialize()
{
	for (int i=0;i<numOfVertices;i++) {
		mark[i] = false;
		predecessor[i] = -1;
		distance[i] = INFINITY;
	}
	distance[source]= 0;
}

int Dijkstra::getClosestUnmarkedNode()
{
	int minDistance = INFINITY;
	int closestUnmarkedNode;
	for (int i=0;i<numOfVertices;i++) {

		if ((!mark[i]) && ( minDistance >= distance[i])) {

			minDistance = distance[i];
			closestUnmarkedNode = i;
		}
	}
	return closestUnmarkedNode;
}

void Dijkstra::calculateDistance()
{
	initialize();
	int minDistance = INFINITY;
	int closestUnmarkedNode;
	int count = 0;

	while(count < numOfVertices) {
		closestUnmarkedNode = getClosestUnmarkedNode();
		mark[closestUnmarkedNode] = true;
		
		for (int i=0;i<numOfVertices;i++) {

			if ((!mark[i]) && (adjMatrix[closestUnmarkedNode][i]>0) ) {

				if (distance[i] > distance[closestUnmarkedNode] + adjMatrix[closestUnmarkedNode][i]) {

					distance[i] = distance[closestUnmarkedNode] + adjMatrix[closestUnmarkedNode][i];
					predecessor[i] = closestUnmarkedNode;
				}
			}
		}
		count++;
	}
}

void Dijkstra::printPath(int node)
{
	if (node == source)
		cout<<(char)(node + 97)<<" -> ";

	else if (predecessor[node] == -1)
		cout << "No path from " << source << " to " << (char)(node + 97) << endl;

	else {
		printPath(predecessor[node]);
		cout << (char) (node + 97) << " -> ";
	}
}

void Dijkstra::output()
{
	for (int i = 0; i < numOfVertices; i++) {
		if (i == source)
			cout<< (char)(source + 97) << " -> " << source;
		else
			printPath(i);

		cout<< " == " <<distance[i]<<endl;
	}
}

int main()
{
	Dijkstra testgraph;
	testgraph.read();
	testgraph.Print();
	testgraph.calculateDistance();
	testgraph.output();

	return 0;
}