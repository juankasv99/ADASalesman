#include "pch.h"
#include "Graph.h"
#include <queue>


// =============================================================================
// Dijkstra ====================================================================
// =============================================================================

void Dijkstra(CGraph& graph, CVertex* pStart)
{
	//Inicialitzem tots els vertexs menys el start a distància infinita + tots els vertexs com a no visitats
	for (CVertex& v : graph.m_Vertices) {
		v.m_DijkstraDistance = DBL_MAX;
		v.m_DijkstraVisit = false;
		v.m_DijkstraPrev = NULL;
	}
	pStart->m_DijkstraDistance = 0;
	
	CVertex* vActual = pStart;
	vActual->m_DijkstraDistance = 0;
	int nVisitedV = 0;

	CVertex* neighbour;

	int vCount = graph.GetNVertices();

	while (nVisitedV < vCount) {
		for (CEdge* outEdge : vActual->m_Edges) {
			//outEdge->m_pDestination == vActual ? neighbour = outEdge->m_pOrigin : neighbour = outEdge->m_pDestination;
			//operador ternario ? para mirar el neigh por si es origin o dest
			if (outEdge->m_pDestination == vActual) neighbour = outEdge->m_pOrigin;
			else neighbour = outEdge->m_pDestination;

			if (!neighbour->m_DijkstraVisit) {
				double preDist = vActual->m_DijkstraDistance + vActual->m_Point.Distance(neighbour->m_Point);
				if (neighbour->m_DijkstraDistance == DBL_MAX || preDist < neighbour->m_DijkstraDistance) {
					neighbour->m_DijkstraDistance = preDist;
					neighbour->m_DijkstraPrev = outEdge;
				}
			}
		}

		vActual->m_DijkstraVisit = true;
		nVisitedV++;

		double minDist;
		bool flag = false;
		for (CVertex& v : graph.m_Vertices) {
			if (v.m_DijkstraDistance != DBL_MAX && v.m_DijkstraVisit == false) {
				if (!flag) {
					minDist = v.m_DijkstraDistance;
					flag = true;
					vActual = &v;
				}
				if (v.m_DijkstraDistance < minDist) {
					minDist = v.m_DijkstraDistance;
					vActual = &v;
				}
			}
		}
	}
}

// =============================================================================
// DijkstraQueue ===============================================================
// =============================================================================


struct comparator {
	bool operator() (pair<pair<CVertex*, CVertex*>, double> edge1, pair<pair<CVertex*, CVertex*>, double>edge2) {
		return edge1.second > edge2.second;
	}
};

void DijkstraQueue(CGraph& graph, CVertex *pStart)
{
	for (CVertex& v : graph.m_Vertices) {
		v.m_DijkstraDistance = DBL_MAX;
		v.m_DijkstraVisit = false;
		v.m_DijkstraPrev = NULL;
	}
	pStart->m_DijkstraDistance = 0;
	priority_queue<pair<pair<CVertex*, CVertex*>, double>, std::vector<pair<pair<CVertex*, CVertex*>, double>>, comparator> priorityQueue;
	
	CVertex* vActual = pStart;
	vActual->m_DijkstraDistance = 0;
	int nVisitedV = 0;
	int vCount = graph.GetNVertices();

	CVertex* neighbour;

	while (nVisitedV < vCount) {
		for (CEdge* outEdge : vActual->m_Edges) {
			if (outEdge->m_pDestination == vActual) neighbour = outEdge->m_pOrigin;
			else neighbour = outEdge->m_pDestination;
			

			if (!neighbour->m_DijkstraVisit) {
				double preDist = vActual->m_DijkstraDistance + vActual->m_Point.Distance(neighbour->m_Point);
				if (neighbour->m_DijkstraDistance == DBL_MAX || preDist < neighbour->m_DijkstraDistance) {
					neighbour->m_DijkstraDistance = preDist;
					neighbour->m_DijkstraPrev = outEdge;
					priorityQueue.push(make_pair(make_pair(vActual, neighbour), preDist));
				}
			}
		}
		vActual->m_DijkstraVisit = true;

		while (!priorityQueue.empty() && priorityQueue.top().first.second->m_DijkstraVisit == true)
			priorityQueue.pop();

		if (!priorityQueue.empty())
			vActual = priorityQueue.top().first.second;

		nVisitedV++;
	}

}
