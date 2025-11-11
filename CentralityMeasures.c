// Centrality Measures ADT interface
// COMP2521 Assignment 2

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "FloydWarshall.h"
#include "Graph.h"

static void Path(int v, int w, Graph g, EdgeValues e, ShortestPaths sps);

/**
 * Finds  the  edge  betweenness  centrality  for each edge in the given
 * graph and returns the results in a  EdgeValues  structure.  The  edge
 * betweenness centrality of a non-existant edge should be set to -1.0.
 */
EdgeValues edgeBetweennessCentrality(Graph g) {
	// TODO: Implement this function
	EdgeValues e = {0};
	ShortestPaths sps = FloydWarshall(g); // Get the sps
	e.numNodes = GraphNumVertices(g); // Get the nodes numebr of graph g

	e.values = calloc(e.numNodes, sizeof(double *)); // Create space
	for (int i = 0; i < e.numNodes; i++) {
		e.values[i] = calloc(e.numNodes, sizeof(double));
	}

	for (int i = 0; i < e.numNodes; i++) {
		for (int j = 0; j < e.numNodes; j++) {
			e.values[i][j] = -1.0; // Initialize all to -1.0, note that e.values[][] is type double
		}
	}

	for (int i = 0; i < e.numNodes; i++) {
		AdjList l = GraphOutIncident(g, i); // Get g->outLinks[i]
		while (l != NULL) {
			int vertex = l->v;
			e.values[i][vertex] = 0.0; // if two vertex has edge between, changing the exsisted edages to 0.0
			l = l->next;
		}
	}

	for (int i = 0; i < e.numNodes; i++) {
		for (int j = 0; j < e.numNodes; j++) {
			Path(i, j, g, e, sps); // Static function to change values of e.values[][]
		}
	}
	freeShortestPaths(sps); // Avoid memory leaks

	return e;
}

/**
 * Prints  the  values in the given EdgeValues structure to stdout. This
 * function is purely for debugging purposes and will NOT be marked.
 */
void showEdgeValues(EdgeValues evs) {

}

/**
 * Frees all memory associated with the given EdgeValues  structure.  We
 * will call this function during testing, so you must implement it.
 */
void freeEdgeValues(EdgeValues evs) {
	// TODO: Implement this function
	for (int v = 0; v < evs.numNodes; v++) {
		free(evs.values[v]);
	}
	free(evs.values);
}


static void Path(int u, int v, Graph g, EdgeValues e, ShortestPaths sps) {
	if (sps.dist[u][v] != INFINITY) { // If has path between
		while (u != v) {		
			int src = sps.next[u][v];
			e.values[u][src]++; // If any shortets path include this edge, add 1 to its e.value
			u = src;
		}
	}
}