#include "pch.h"
#include "Graph.h"


//Classe Node per transformar els vertexs a nodes
class CNode {
private:
	CVertex* vPtr;
	int pathID;
	CEdge* edgePath;
public:
	CNode(CVertex* vertex, int pathNum) { vPtr = vertex; pathID = pathNum; }
	CVertex* getVertexPtr() { return vPtr; }
	CGPoint getVertexPoint() { return vPtr->m_Point; }
	int getPathID() { return pathID; }
	void setPathID(int newID) { pathID = newID; }
	void setEdgePath(CEdge* e) { edgePath = e; }
	CEdge* getEdgePath() { return edgePath; }
};

int getBelongingPath(CVertex* v, list<CNode*>* stackTrack) {
	list <CNode*>::iterator it;
	for (it = stackTrack->begin(); it != stackTrack->end(); it++) {
		if ((*it)->getVertexPoint() == v->m_Point) {
			return (*it)->getPathID();
		}
	}

	
	return -1;
}




// =============================================================================
// SalesmanTrackBacktracking ===================================================
// =============================================================================

void BacktrackingPure(CVisits* visitsCopy, list<CNode*>* stackTrack, CNode* actNode, CVertex* destinationVertex, CTrack* optTrack, CTrack* actTrack) {
	
	if (actNode->getEdgePath() != NULL) stackTrack->push_front(actNode);
	CNode* nextNode;
	bool belongsVisits;
	actTrack->Clear();
	for (CNode* n : *stackTrack) actTrack->AddFirst(n->getEdgePath());

	if (optTrack->Length() == 0 || actTrack->Length() < optTrack->Length()) {
		if (actNode->getVertexPoint() == destinationVertex->m_Point && visitsCopy->m_Vertices.empty()) *optTrack = *actTrack;
		for (CEdge* e : actNode->getVertexPtr()->m_Edges) {
			if (visitsCopy->m_Vertices.empty() && e->m_pDestination->m_Point == destinationVertex->m_Point) {
				nextNode = new CNode(destinationVertex, actNode->getPathID());
				nextNode->setEdgePath(e);
				BacktrackingPure(visitsCopy, stackTrack, nextNode, destinationVertex, optTrack, actTrack);
			}
			else if (!visitsCopy->m_Vertices.empty() || e->m_pDestination->m_Point != destinationVertex->m_Point) {
				if (getBelongingPath(e->m_pDestination, stackTrack) != actNode->getPathID()) {
					nextNode = new CNode(e->m_pDestination, actNode->getPathID());
					nextNode->setEdgePath(e);
					belongsVisits = visitsCopy->MemberP(nextNode->getVertexPtr());

					if (belongsVisits) {
						nextNode->setPathID(nextNode->getPathID() + 1);
						visitsCopy->m_Vertices.remove(nextNode->getVertexPtr());
					}
					BacktrackingPure(visitsCopy, stackTrack, nextNode, destinationVertex, optTrack, actTrack);
					if (belongsVisits) {
						nextNode->setPathID(actNode->getPathID());
						visitsCopy->Add(nextNode->getVertexPtr());
					}
				}
			}
		}
	}

	if (stackTrack->size() != 0) stackTrack->pop_front();
		

	
}



CTrack SalesmanTrackBacktracking(CGraph &graph, CVisits &visits)
{
	CVertex* destinationVertex;
	CTrack* optTrack = new CTrack(&graph);
	CTrack* actTrack = new CTrack(&graph);
	CVisits visitsCopy = visits;
	CNode* actNode = new CNode(visitsCopy.m_Vertices.front(), 0);
	visitsCopy.m_Vertices.pop_front();
	list<CNode*>* stackTrack = new list<CNode*>;
	destinationVertex = visitsCopy.m_Vertices.back();
	visitsCopy.m_Vertices.pop_back();

	BacktrackingPure(&visitsCopy, stackTrack, actNode, destinationVertex, optTrack, actTrack);

	return *optTrack;

	//return CTrack(&graph);
}

// =============================================================================
// SalesmanTrackBacktrackingGreedy =============================================
// =============================================================================


void BacktrackingGreedy(vector<vector<double>>& visitsTracksLenghtMat, vector<bool>* inStackVec, int actVertex, vector<int>* stackTrack, vector<int>* optTrack, int nVertexToVisit, double* optDist, double* actDist) {
	stackTrack->push_back(actVertex);
	(*inStackVec)[actVertex] = true;
	if (stackTrack->size() == nVertexToVisit && stackTrack->back() == nVertexToVisit - 1) {
		if (*actDist < *optDist) {
			*optTrack = *stackTrack;
			*optDist = *actDist;
		}
	}
	else {
		for (int nextVertex = 0; nextVertex < nVertexToVisit; nextVertex++) {
			if (!(*inStackVec)[nextVertex]) {
				*actDist += visitsTracksLenghtMat[actVertex][nextVertex];
				BacktrackingGreedy(visitsTracksLenghtMat, inStackVec, nextVertex, stackTrack, optTrack, nVertexToVisit, optDist, actDist);
			}
		}
	}
	int popVertex = stackTrack->back();
	stackTrack->pop_back();
	(*inStackVec)[popVertex] = false;
	if (stackTrack->size() > 0) *actDist -= visitsTracksLenghtMat[popVertex][stackTrack->back()]; else *actDist = 0;
	return;

}


CTrack SalesmanTrackBacktrackingGreedy(CGraph& graph, CVisits& visits)
{
	vector<vector<CTrack*>> visitsTracksMat(visits.GetNVertices(), vector<CTrack*>(visits.GetNVertices(), 0));
	vector<vector<double>> visitsTracksLengthMat(visits.GetNVertices(), vector<double>(visits.GetNVertices(), 0));
	int nVertexToVisit = visits.m_Vertices.size();

	int i = 0;
	int j = 0;
	for (CVertex* iVertex : visits.m_Vertices) {
		DijkstraQueue(graph, iVertex);
		j = 0;
		for (CVertex* jVertex : visits.m_Vertices) {
			CTrack* aux = new CTrack(&graph);
			CVertex* actV = jVertex;
			while (actV != iVertex) {
				aux->AddFirst(actV->m_DijkstraPrev);
				actV = actV->m_DijkstraPrev->m_pOrigin;
			}
			if (iVertex != jVertex) {
				//aux->AddFirst(iVertex->m_DijkstraPrev);
				visitsTracksMat[i][j] = aux;
				visitsTracksLengthMat[i][j] = aux->Length();
			}
			
			j++;
		}
		i++;
	}

	vector<int> stackTrack;
	vector<bool> inStackMap(nVertexToVisit, false);
	vector<int> optimousTrack;
	int actVertex = 0;
	double optimousDist = std::numeric_limits<double>::max();
	double actDist = 0;
	BacktrackingGreedy(visitsTracksLengthMat, &inStackMap, actVertex, &stackTrack, &optimousTrack, nVertexToVisit, &optimousDist, &actDist);

	CTrack resultTrack(&graph);
	int dest = 0;

	optimousTrack.erase(optimousTrack.begin());
	//resultTrack.AddFirst(visits.m_Vertices.front());
	for (int orig : optimousTrack) {
		CTrack* t = visitsTracksMat[dest][orig];
		//t->m_Edges.pop_front();
		resultTrack.Append(*t);
		dest = orig;
	}

	return resultTrack;

	//return CTrack(&graph);
}
