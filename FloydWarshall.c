// Floyd Warshall ADT interface
// COMP2521 Assignment 2

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "FloydWarshall.h"
#include "Graph.h"

/**
 * Finds all shortest paths between all pairs of nodes.
 * 
 * The  function  returns  a 'ShortestPaths' structure with the required
 * information:
 * - the number of vertices in the graph
 * - distance matrix
 * - matrix of intermediates (see description above)
 */
ShortestPaths FloydWarshall(Graph g) {
	// TODO: Implement this function
	ShortestPaths sps = {0};
	sps.numNodes = GraphNumVertices(g); // Get the nodes number of graph g

	sps.dist = calloc(sps.numNodes, sizeof(int *)); // Create space
	for (int i = 0; i < sps.numNodes; i++) {
		sps.dist[i] = calloc(sps.numNodes, sizeof(int));
	}
	
	sps.next = calloc(sps.numNodes, sizeof(int *));
	for (int i = 0; i < sps.numNodes; i++) {
		sps.next[i] = calloc(sps.numNodes, sizeof(int));
	}
	
	for (int i = 0; i < sps.numNodes; i++) { // Initialize sps.dist and sps.next
		for (int j = 0; j < sps.numNodes; j++) {
			sps.dist[i][j] = INT_MAX; // Defined INFINITY as INT_MAX
			sps.next[i][j] = -1;
		}
	}

	for (int i = 0; i < sps.numNodes; i++) {
		AdjList l = GraphOutIncident(g, i); // Take out the g->outLinks[i]
		while (l != NULL) {
			int vertex = l->v;
			int weight = l->weight;
			sps.next[i][vertex] = vertex;
			sps.dist[i][vertex] = weight;
			l = l->next;
		}
	}
	
	for (int i = 0; i < sps.numNodes; i++) {
		sps.dist[i][i] = 0; // For the same vertex, gives sps.dist and sps.next a specified value
		sps.next[i][i] = -1;
	}

	for (int k = 0; k < sps.numNodes; k++) {
		for (int i = 0; i < sps.numNodes; i++) {
			for (int j = 0; j < sps.numNodes; j++) {
				if (sps.dist[i][k] != INT_MAX && sps.dist[k][j] != INT_MAX) { // If has path between
					if (sps.dist[i][j] > (sps.dist[i][k] + sps.dist[k][j])) {
						sps.dist[i][j] = sps.dist[i][k] + sps.dist[k][j];
						sps.next[i][j] = sps.next[i][k];
					}
				}
			}
		}
	}
	return sps;
}

/**
 * This  function  is  for  you to print out the ShortestPaths structure
 * while you are debugging/testing your implementation. 
 * 
 * We will not call this function during testing, so you may  print  out
 * the  given  ShortestPaths  structure in whatever format you want. You
 * may choose not to implement this function.
 */
void showShortestPaths(ShortestPaths sps) {

}

/**
 * Frees  all  memory associated with the given ShortestPaths structure.
 * We will call this function during testing, so you must implement it.
 */
void freeShortestPaths(ShortestPaths sps) {
	// TODO: Implement this function
	for (int v = 0; v < sps.numNodes; v++) {
		free(sps.dist[v]);
		free(sps.next[v]);
	}
	free(sps.dist);
	free(sps.next);
}

