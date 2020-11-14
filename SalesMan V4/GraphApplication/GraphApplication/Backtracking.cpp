#include "pch.h"
#include "Graph.h"


//Classe Node per transformar els vertexs a nodes
class CNode {
private:
	CVertex* vPtr;
	int pathID;
	CEdge* edgePath;
	//pair<CNode*, CEdge*> connection;
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

void getNeighs(CVertex* v) {
	for (CEdge* e : v->m_Edges) {
		v->m_neighbords.push_back(e->m_pDestination);
	}
}

/*void getNeighs(CNode* n) {
	for (CEdge* e : n->getVertexPtr()->m_Edges) {
	}
}*/


// =============================================================================
// SalesmanTrackBacktracking ===================================================
// =============================================================================

void BacktrackingPure(CVisits* visitsCopy, list<CNode*>* stackTrack, CNode* actNode, CVertex* destinationVertex, CTrack* optTrack, CTrack* actTrack) {
	
	if (actNode->getEdgePath() != NULL) stackTrack->push_front(actNode);
	getNeighs(actNode->getVertexPtr());
	CNode* nextNode;
	bool belongsVisits;
	actTrack->Clear();
	for (CNode* n : *stackTrack) actTrack->AddFirst(n->getEdgePath());
	//actTrack->AppendBefore(stackTrack);
	
	/*if (stackTrack->size() > 1) {
		cout << "stackTrack size=" << stackTrack->size() << endl;
		list<CNode*>::iterator it1=stackTrack->begin();
		list<CNode*>::iterator it2=next(stackTrack->begin(),1);

		for (it1; it1 != prev(stackTrack->end()); it1++) {
			for (CEdge* e : (*it1)->getVertexPtr()->m_Edges) {
				if (e->m_pDestination->m_Point == (*it2)->getVertexPoint())
				{
					actTrack->AddFirst(e->m_pReverseEdge);
					break;
				}
			}
			it2++;
		}
		
	}*/

	/*if (optTrack->Length() == 0 || actTrack->Length() < optTrack->Length()) {
		if (actNode->getVertexPoint() == destinationVertex->m_Point && visitsCopy->m_Vertices.empty()) *optTrack = *actTrack;
		for (CVertex* v : actNode->getVertexPtr()->m_neighbords) {
			if (visitsCopy->m_Vertices.empty() && v->m_Point == destinationVertex->m_Point) {
				nextNode = new CNode(destinationVertex, actNode->getPathID());
				BacktrackingPure(visitsCopy, stackTrack, nextNode, destinationVertex, optTrack, actTrack);
			}
			else if (!visitsCopy->m_Vertices.empty() || v->m_Point != destinationVertex->m_Point) {
				if (getBelongingPath(v, stackTrack) != actNode->getPathID()) {
					nextNode = new CNode(v, actNode->getPathID());
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
	}*/

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
	else cout << "ARA" << endl;
		

	
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

	//stackTrack size = 0
	//hay que encontrar una manera de pasar los edges en vez de los vertices

	//ver los edges del nodo actual y añadirlos a los posibles candidatos

	getNeighs(actNode->getVertexPtr());

	BacktrackingPure(&visitsCopy, stackTrack, actNode, destinationVertex, optTrack, actTrack);

	return *optTrack;

	//return CTrack(&graph);
}

// =============================================================================
// SalesmanTrackBacktrackingGreedy =============================================
// =============================================================================


CTrack SalesmanTrackBacktrackingGreedy(CGraph& graph, CVisits& visits)
{
	return CTrack(&graph);
}
