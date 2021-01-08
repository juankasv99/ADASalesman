#include "pch.h"
#include "Graph.h"
#include <queue>
#include <iostream>
#include <iomanip> 

class BBNode {
public:
	BBNode() { ; }
	BBNode(int nVisits, vector<vector<double>>* distancesMatrix) {
		m_indexes = new vector<int>;
		for (int i = 0; i < nVisits; i++) m_indexes->push_back(i);

		m_yetToModify = new vector<bool>;
		for (int i = 1; i < nVisits - 1; i++) m_yetToModify->push_back(i);

		m_nVisits = nVisits;
		m_distancesMat = distancesMatrix;
		m_canModifiyPointer = 1;
		m_actualLength = (*distancesMatrix)[0][1];
		m_cotaInferior = 0;
	}

	BBNode* expandNode(int idx) {
		BBNode* child = new BBNode();
		child->m_nVisits = this->m_nVisits;
		child->m_distancesMat = this->m_distancesMat;
		child->m_yetToModify = new vector<bool>(*this->m_yetToModify);
		child->m_indexes = new vector<int>(*this->m_indexes);
		child->m_canModifiyPointer = this->m_canModifiyPointer;
		child->m_actualLength = 0;

		int valueAtBorder = (*child->m_indexes)[child->m_canModifiyPointer];
		if (idx != valueAtBorder) {
			vector<int>::iterator it = child->m_indexes->begin();
			while (*it != idx) it++;
			*it = valueAtBorder;

			(*child->m_indexes)[child->m_canModifiyPointer] = idx;
		}

		for (int i = 1; i < child->m_canModifiyPointer + 2; i++) child->m_actualLength += (*child->m_distancesMat)[(*child->m_indexes)[i - 1]][(*child->m_indexes)[i]];

		child->m_canModifiyPointer++;
		(*child->m_yetToModify)[idx - 1] = false;
		return child;
	}

	BBNode* expandNode2(int idx, vector<double>* minimumDistances) {
		BBNode* childNode = this->expandNode(idx);

		childNode->m_cotaInferior = 0;
		for (int i = 0; i < (*childNode->getYetToModifiVector()).size(); i++) if (i != 0 && (*childNode->getYetToModifiVector())[i]) childNode->m_cotaInferior += (*minimumDistances)[i + 1];

		return childNode;
	}

	BBNode* expandNode3(int idx) {
		BBNode* childNode = this->expandNode(idx);

		childNode->m_cotaInferior = 0;
		int lastVisited = (*childNode->getIndexesVector())[childNode->getCanModifyPointer() - 1];
		for (int i = 0; i < (*childNode->getYetToModifiVector()).size(); i++) {
			if ((*childNode->getYetToModifiVector())[i]) {
				int idxNotVisited = i + 1;
				childNode->m_cotaInferior += (*m_distancesMat)[lastVisited][idxNotVisited];
			}
		}

		return childNode;
	}

	vector<bool>* getYetToModifiVector() { return m_yetToModify; }
	vector<int>* getIndexesVector() { return m_indexes; }
	int getCanModifyPointer() { return m_canModifiyPointer; }
	double getActualLength() { return m_actualLength; }
	double getActualCotaInferior() { return m_cotaInferior; }

private:
	int m_nVisits;
	vector<vector<double>>* m_distancesMat;
	vector<bool>* m_yetToModify;
	vector<int>* m_indexes;
	int m_canModifiyPointer;
	double m_actualLength;
	double m_cotaInferior;
};

struct comparator {
	bool operator()(BBNode* node1, BBNode* node2) {
		return node1->getActualLength() > node2->getActualLength();
	}
};

struct comparator2 {
	bool operator()(BBNode* node1, BBNode* node2) {
		return node1->getActualLength() + node1->getActualCotaInferior() > node2->getActualLength() + node2->getActualCotaInferior();
	}
};

void printQueue(priority_queue<BBNode*, vector<BBNode*>, comparator> pq) {
	cout << "==========>>PRINTING PRIORITY QUEUE<<==========" << endl;
	BBNode* aux = pq.top();
	for (int v : *(aux->getIndexesVector())) cout << v << " " << endl;
	cout << "   --->Len: " << aux->getActualLength() << endl;
	cout << "   --->Lim: " << aux->getCanModifyPointer() << endl;
}


// SalesmanTrackBranchAndBound1 ===================================================

CTrack SalesmanTrackBranchAndBound1(CGraph& graph, CVisits& visits)
{
	vector<vector<CTrack*>> visitsTracksMat(visits.GetNVertices(), vector<CTrack*>(visits.GetNVertices(), 0));
	vector<vector<double>> visitsTracksLenghtMat(visits.GetNVertices(), vector<double>(visits.GetNVertices(), 0));
	int i = 0;
	int j = 0;
	/*CVisits visitsCopy = visits;
	visitsCopy.m_Vertices.pop_front();*/
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
			visitsTracksMat[i][j] = aux;
			visitsTracksLenghtMat[i][j] = aux->Length();
			j++;

		}
		i++;
	}

	BBNode answerNode;
	double answerLength = numeric_limits<double>::max();

	double baseDistance = 0;
	for (int i = 1; i < visits.GetNVertices(); i++) baseDistance += visitsTracksLenghtMat[i - 1][i];

	int nVisits = visits.GetNVertices();
	priority_queue<BBNode*, vector<BBNode*>, comparator> openAnswers;
	BBNode* initialNode = new BBNode(nVisits, &visitsTracksLenghtMat);
	openAnswers.push(initialNode);

	while (!openAnswers.empty()) {
		BBNode* actualNode = openAnswers.top();
		openAnswers.pop();

		for (int i = 0; i < actualNode->getYetToModifiVector()->size(); i++) {
			if ((*actualNode->getYetToModifiVector())[i]) {
				BBNode* aux = new BBNode();
				*aux = *actualNode->expandNode(i + 1);
				if (aux->getActualLength() < answerLength && aux->getActualLength() <= baseDistance) openAnswers.push(aux);
				else delete aux;
			}
		}

		if (actualNode->getCanModifyPointer() == nVisits - 1 && actualNode->getActualLength() < answerLength) {
			answerLength = actualNode->getActualLength();
			answerNode = *actualNode;
		}
		delete actualNode;
	}

	CTrack resultTrack(&graph);
	int dest = 0;
	//resultTrack.AddFirst(visits.m_Vertices.front()->m_DijkstraPrev); //dudoso
	for (int orig : (*answerNode.getIndexesVector())) {
		CTrack* t = visitsTracksMat[dest][orig];
		//dest = 0;  //quitar luego
		resultTrack.Append(*t);
		dest = orig;

	}

	return resultTrack;
}

// SalesmanTrackBranchAndBound2 ===================================================

CTrack SalesmanTrackBranchAndBound2(CGraph& graph, CVisits &visits)
{
	vector<vector<CTrack*>> visitsTracksMat(visits.GetNVertices(), vector<CTrack*>(visits.GetNVertices(), 0));
	vector<vector<double>> visitsTracksLengthMat(visits.GetNVertices(), vector<double>(visits.GetNVertices(), 0));
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
			visitsTracksMat[i][j] = aux;
			visitsTracksLengthMat[i][j] = aux->Length();
			j++;
		}
		i++;
	}

	vector<double> minimumTracksLengths;
	minimumTracksLengths.resize(visits.GetNVertices(), -1);
	for (int x = 1; x < visits.GetNVertices(); x++) {
		double min = numeric_limits<double>::max();
		for (int y = 1; y < visits.GetNVertices() - 1; y++) {
			if (x != y && visitsTracksLengthMat[y][x] < min) min = visitsTracksLengthMat[x][y];
		}
		minimumTracksLengths[x] = min;
	}

	BBNode answerNode;
	double answerLength = numeric_limits<double>::max();

	double baseDistance = 0;
	for (int i = 1; i < visits.GetNVertices(); i++) baseDistance += visitsTracksLengthMat[i - 1][i];

	int nVisits = visits.GetNVertices();
	priority_queue<BBNode*, vector<BBNode*>, comparator2> openAnswers;
	BBNode* initialNode = new BBNode(nVisits, &visitsTracksLengthMat);
	openAnswers.push(initialNode);

	while (!openAnswers.empty()) {
		BBNode* actualNode = openAnswers.top();
		openAnswers.pop();

		for (int i = 0; i < actualNode->getYetToModifiVector()->size(); i++) {
			if ((*actualNode->getYetToModifiVector())[i]) {
				BBNode* aux = new BBNode();
				*aux = *actualNode->expandNode2(i + 1, &minimumTracksLengths);
				if (aux->getActualLength() < answerLength && aux->getActualLength() <= baseDistance) openAnswers.push(aux);
				else delete aux;
			}
		}

		if (actualNode->getCanModifyPointer() == nVisits - 1 && actualNode->getActualLength() < answerLength) {
			answerLength = actualNode->getActualLength();
			answerNode = *actualNode;
		}
		
		delete actualNode;
	}

	CTrack resultTrack(&graph);
	int dest = 0;
	for (int orig : (*answerNode.getIndexesVector())) {
		CTrack* t = visitsTracksMat[dest][orig];
		resultTrack.Append(*t);
		dest = orig;

	}

	return resultTrack;

	
}

// SalesmanTrackBranchAndBound3 ===================================================

CTrack SalesmanTrackBranchAndBound3(CGraph& graph, CVisits &visits)
{
	vector<vector<CTrack*>> visitsTracksMat(visits.GetNVertices(), vector<CTrack*>(visits.GetNVertices(), 0));
	vector<vector<double>> visitsTracksLengthMat(visits.GetNVertices(), vector<double>(visits.GetNVertices(), 0));
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
			visitsTracksMat[i][j] = aux;
			visitsTracksLengthMat[i][j] = aux->Length();
			j++;
		}
		i++;
	}

	vector<double> minimumTracksLengths;
	minimumTracksLengths.resize(visits.GetNVertices(), -1);
	for (int x = 1; x < visits.GetNVertices(); x++) {
		double min = numeric_limits<double>::max();
		for (int y = 1; y < visits.GetNVertices() - 1; y++) {
			if (x != y && visitsTracksLengthMat[y][x] < min) min = visitsTracksLengthMat[x][y];
		}
		minimumTracksLengths[x] = min;
	}

	BBNode answerNode;
	double answerLength = numeric_limits<double>::max();

	double baseDistance = 0;
	for (int i = 1; i < visits.GetNVertices(); i++)baseDistance += visitsTracksLengthMat[i - 1][i];

	int nVisits = visits.GetNVertices();
	priority_queue<BBNode*, vector<BBNode*>, comparator2> openAnswers;
	BBNode* initialNode = new BBNode(nVisits, &visitsTracksLengthMat);
	openAnswers.push(initialNode);

	while (!openAnswers.empty()) {
		BBNode* actualNode = openAnswers.top();
		openAnswers.pop();

		for (int i = 0; i < actualNode->getYetToModifiVector()->size(); i++) {
			if ((*actualNode->getYetToModifiVector())[i]) {
				BBNode* aux = new BBNode();
				*aux = *actualNode->expandNode3(i + 1);
				if (aux->getActualLength() < answerLength && aux->getActualLength() <= baseDistance) openAnswers.push(aux);
				else delete aux;
			}
		}

		if (actualNode->getCanModifyPointer() == nVisits - 1 && actualNode->getActualLength() < answerLength) {
			answerLength = actualNode->getActualLength();
			answerNode = *actualNode;
		}
		
		delete actualNode;
	}

	CTrack resultTrack(&graph);
	int dest = 0;
	for (int orig : (*answerNode.getIndexesVector())) {
		CTrack* t = visitsTracksMat[dest][orig];
		resultTrack.Append(*t);
		dest = orig;
	}

	return resultTrack;
}
