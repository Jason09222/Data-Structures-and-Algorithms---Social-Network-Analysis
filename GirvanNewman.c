// Girvan-Newman Algorithm for community discovery
// COMP2521 Assignment 2

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "CentralityMeasures.h"
#include "GirvanNewman.h"
#include "Graph.h"
#include "FloydWarshall.h" // Will use sps.dist[][]
/**
 * Generates  a Dendrogram for the given graph g using the Girvan-Newman
 * algorithm.
 * 
 * The function returns a 'Dendrogram' structure.
 */

static Dendrogram new(); // Helper function to construct the tree node
static void dorecursion(Graph g, Dendrogram den, int comp[]);
static bool whethercomp(Graph g, int i, int v); // To identify after removal, whether forms components or not

Dendrogram GirvanNewman(Graph g) {
	// TODO: Implement this function
	Dendrogram den = new(); // Create the root
	int numNodes = GraphNumVertices(g); // Get vertex number of graph
	int comp[numNodes]; // Create the array to record the formed component
	for (int i = 0; i < numNodes; i++) { // Initialize all to 1
		comp[i] = 1;
	}
	dorecursion(g, den, comp);

	return den;
}

/**
 * Frees all memory associated with the given Dendrogram  structure.  We
 * will call this function during testing, so you must implement it.
 */
void freeDendrogram(Dendrogram d) {
	// TODO: Implement this function
	if (d == NULL) {
		return;
	}
	
	freeDendrogram(d->left);
	freeDendrogram(d->right);
	free(d);
}

static Dendrogram new() { // Help to create the tree node
	Dendrogram den = malloc(sizeof(*den));
	den->left = NULL;
	den->right = NULL;
	return den;
}

static void dorecursion(Graph g, Dendrogram den, int comp[]) {
	EdgeValues ed = edgeBetweennessCentrality(g); // Get the EdgeValues ed

	double Max = -1; //To get the max value of ed.values
	for (int i = 0; i < ed.numNodes; i++) {
		for (int j = 0; j < ed.numNodes; j++) {
			if ((comp[i] == 1) && (comp[j] == 1)) {
				if (ed.values[i][j] > Max) {
					Max = ed.values[i][j];
				}
			}
		}
	}

	Dendrogram left = new(); // Create left and right node of current node
	den->left = left;
	
	Dendrogram right = new();
	den->right = right;

	bool leafrightvertex = false; // To determine whether the leaf node appear and which side
	bool leafleftvertex = false;
	
	for (int i = 0; i < ed.numNodes; i++) {
		for (int j = 0; j < ed.numNodes; j++) {
			if ((comp[i] == 1) && (comp[j] == 1)) {
				if (ed.values[i][j] == Max) { // Find the Max between
					GraphRemoveEdge(g, i, j); // Free the connection
					ShortestPaths sps = FloydWarshall(g); // Get the sps from Floyid
					if ((sps.dist[i][j] == INFINITY) && (sps.dist[j][i] == INFINITY) && (!whethercomp(g, i, j))) {
						if ((GraphOutIncident(g, i) == NULL) && (GraphInIncident(g, i) == NULL)) {
							if (leafleftvertex == false) { // When meets leaf condition
								left->vertex = i;
								leafleftvertex = true;
							} else {
								right->vertex = i;
								leafrightvertex = true;
							}
						}
						if ((GraphOutIncident(g, j) == NULL) && (GraphInIncident(g, j) == NULL)) {
							if (leafleftvertex == false) { // When meets leaf condition
								left->vertex = j;
								leafleftvertex = true;
							} else {
								right->vertex = j;
								leafrightvertex = true;
							}
						}
						if (leafrightvertex == true) { // If right and left sides have all been inserted, over the recursion
							return;
						}

						if (leafleftvertex == false) { // If meets component condition
							int leftcomp[ed.numNodes];
							for (int a = 0; a < ed.numNodes; a++) {
								leftcomp[a] = -1;
							}

							leftcomp[i] = 1; 
							for (int a = 0; a < ed.numNodes; a++) {
								if (comp[a] == 1) {
									if ((sps.dist[a][i] != INFINITY) || (sps.dist[i][a] != INFINITY)) {
										leftcomp[a] = 1;
									}
								}
							}
							dorecursion(g, left, leftcomp); // If left meets component condition, do left recursion
						}

						int rightcomp[ed.numNodes]; // Same ideal with left
						for (int a = 0; a < ed.numNodes; a++) {
							rightcomp[a] = -1;
						}
						rightcomp[j] = 1;
						for (int b = 0; b < ed.numNodes; b++) {
							if (comp[b] == 1) {
								if ((sps.dist[b][j] != INFINITY) || (sps.dist[j][b] != INFINITY)) {
									rightcomp[b] = 1;
								}
							}
						}

						dorecursion(g, right, rightcomp); // Firstly insert left, after all the excution, do right recursion
					}
				}
			}
		}
	}
}

static bool whethercomp(Graph g, int i, int v) { // To identify after removal, whether forms components or not
	int num = GraphNumVertices(g);
	ShortestPaths sps = FloydWarshall(g);
	for (int a = 0; a < num; a++) {
		if ((sps.dist[i][a] != INFINITY) && (sps.dist[v][a] != INFINITY)) {
			return true;
		} else if ((sps.dist[a][i] != INFINITY) && (sps.dist[a][v] != INFINITY)) {
			return true;
		}
	}
	return false;
}

						
						
				
				