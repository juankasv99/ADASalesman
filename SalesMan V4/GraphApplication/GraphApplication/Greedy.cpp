#include "pch.h"
#include "Graph.h"

// SalesmanTrackGreedy =========================================================

CTrack SalesmanTrackGreedy(CGraph& graph, CVisits &visits)
{
	CTrack cami(&graph);
	list<CVertex*> candidats;

	/* for (list<CVertex*>::iterator it = visits.m_Vertices.begin(); it != visits.m_Vertices.end(); it++) {

	}*/

	for (list<CVertex*>::iterator it = std::next(visits.m_Vertices.begin(), 1); it != std::prev(visits.m_Vertices.end(), 1); it++) {
		candidats.push_back(*it);
	}

	CVertex* vActual = visits.m_Vertices.front();

	while (!candidats.empty()) {
		DijkstraQueue(graph, vActual);

		double minDist;
		CVertex* vMinDist;
		minDist = candidats.front()->m_DijkstraDistance;
		vMinDist = candidats.front();

		for (CVertex* v : candidats) { //agafem el candidat (visita intermitja) més proxim al punt inicial (vactual)
			if (v->m_DijkstraDistance < minDist) {
				vMinDist = v;
				minDist = v->m_DijkstraDistance;
			}
		}
		string edgeNameStr = vMinDist->m_DijkstraPrev->m_Name;

		const char* edgeNameChar = edgeNameStr.c_str();

		CEdge* prevEdge = graph.FindEdge(edgeNameChar);
		
		CTrack auxTrack(&graph);

		auxTrack.AddLast(prevEdge);

		//cami.AddLast(prevEdge);

		CVertex* aux;

		if (vMinDist->m_DijkstraPrev->m_pDestination != vMinDist) aux = vMinDist->m_DijkstraPrev->m_pDestination;
		else aux = vMinDist->m_DijkstraPrev->m_pOrigin;

		
		

		

		/*if (vMinDist->m_DijkstraPrev == cami.m_Edges.back()) { //REDUNDANTE

		}*/
		//while (aux->m_DijkstraPrev != NULL && aux->m_DijkstraPrev != cami.m_Edges.back()) {
		while (aux->m_DijkstraPrev != NULL) {
			auxTrack.AddFirst(aux->m_DijkstraPrev);
			//aux = aux->m_DijkstraPrev->m_pOrigin;
			if (aux != aux->m_DijkstraPrev->m_pOrigin) aux = aux->m_DijkstraPrev->m_pOrigin;
			else aux = aux->m_DijkstraPrev->m_pDestination;
		}

		
			
		cami.Append(auxTrack);

		vActual = vMinDist;
		candidats.remove(vActual);
	}

	DijkstraQueue(graph, vActual);

	if (cami.m_Edges.empty()) //Cas simple
	{
		CVertex* aux = visits.m_Vertices.back();
		CTrack auxTrack(&graph);

		while (aux->m_DijkstraPrev != NULL) {
			auxTrack.AddFirst(aux->m_DijkstraPrev);
			if (aux != aux->m_DijkstraPrev->m_pOrigin) aux = aux->m_DijkstraPrev->m_pOrigin;
			else aux = aux->m_DijkstraPrev->m_pDestination;
		}

		if (aux->m_DijkstraPrev != NULL) {
			auxTrack.AddFirst(aux->m_DijkstraPrev);
			cout << "ERROR NULL" << endl;
		}

		cami.Append(auxTrack);

		return cami;
	}

		CVertex* aux = visits.m_Vertices.back();
		CTrack auxTrack(&graph);

		while (aux->m_DijkstraPrev != NULL && (!(aux->NeighbordP(cami.m_Edges.back()->m_pOrigin)) || !(aux->NeighbordP(cami.m_Edges.back()->m_pDestination)))) {
			auxTrack.AddFirst(aux->m_DijkstraPrev);
			if (aux != aux->m_DijkstraPrev->m_pOrigin) aux = aux->m_DijkstraPrev->m_pOrigin;
			else aux = aux->m_DijkstraPrev->m_pDestination;
		}
		
		if (aux->m_DijkstraPrev != NULL) {
			auxTrack.AddFirst(aux->m_DijkstraPrev);
			cout << "ERROR NULL" << endl;
		}

		cami.Append(auxTrack);	
	

	
	
	
	return cami;
}
