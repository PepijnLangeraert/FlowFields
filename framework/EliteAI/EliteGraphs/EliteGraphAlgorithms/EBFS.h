#pragma once
#include "projects/App_FlowFields/FlowField.h"
namespace Elite 
{
	template <class T_NodeType, class T_ConnectionType>
	class BFS
	{
	public:
		BFS(IGraph<T_NodeType, T_ConnectionType>* pGraph);

		void FindPath(T_NodeType* pDestinationNode,FlowField* flowField);
	private:
		IGraph<T_NodeType, T_ConnectionType>* m_pGraph;
	};

	template <class T_NodeType, class T_ConnectionType>
	BFS<T_NodeType, T_ConnectionType>::BFS(IGraph<T_NodeType, T_ConnectionType>* pGraph)
		: m_pGraph(pGraph)
	{
	}

	template <class T_NodeType, class T_ConnectionType>
	void BFS<T_NodeType, T_ConnectionType>::FindPath(T_NodeType* pDestinationNode, FlowField* flowField)
	{
		// here we will calculate our path using bfs
		std::list<T_NodeType*> openList;
		std::list<T_NodeType*> closedList;
		//std::map<T_NodeType*,T_NodeType*> closedListPath;

		flowField->ResetValues();

		//openList.push(pStartNode);
		openList.push_back(pDestinationNode);
		flowField->SetValueAt(pDestinationNode->GetIndex(), 0);
	
		while (!openList.empty())
		{
			T_NodeType* currentNode = openList.front();
			openList.pop_front();

			int currentIndex = currentNode->GetIndex();


			for (auto con : m_pGraph->GetNodeConnections(currentNode->GetIndex()))
			{

				if (flowField->GetCostAt(con->GetTo()) < int(TerrainType::Water))
				{

					float endCost = flowField->GetValueAt(currentIndex) + flowField->GetCostAt(con->GetTo());
					T_NodeType* nextNode = m_pGraph->GetNode(con->GetTo());
					if (std::find(closedList.begin(), closedList.end(), nextNode) == closedList.end())
					{
						if (std::find(openList.begin(), openList.end(), nextNode) == openList.end())
						{
							openList.push_back(nextNode);

						}
						if (flowField->GetValueAt(con->GetTo()) > endCost)
							flowField->SetValueAt(con->GetTo(), endCost);
					}
				}
			}
			closedList.push_back(currentNode);
		}

		//track back
	
		for (int i{}; i < m_pGraph->GetNrOfNodes(); i++)
		{
			float value{ flowField->GetValueAt(i) };
			int index{};
			for (auto con : m_pGraph->GetNodeConnections(i))
			{
				float temp = flowField->GetValueAt(con->GetTo());
				//check for diagonal
				if (con->GetTo() != i + 1 && con->GetTo() != i - 1 && con->GetTo() != i + flowField->GetWidth() && con->GetTo() != i - flowField->GetWidth())
					temp += 0.5f;
				if (temp < value)
				{
					value = temp;
					index = con->GetTo();
				}
			}
			
			if(flowField->GetCostAt(i)<10&& flowField->GetValueAt(i) != 0)
				flowField->SetDirtAt(i ,m_pGraph->GetNodePos(index)- m_pGraph->GetNodePos(i));
			else 
				flowField->SetDirtAt(i, Vector2{});


		}
		if (flowField->GetValueAt(0) != 0)
		{
			if (flowField->GetDirectionAt(1) == Elite::Vector2{ -1,0 } && flowField->GetDirectionAt(20) == Elite::Vector2{ 0,-1 } && flowField->GetDirectionAt(21) == Elite::Vector2{ -1,-1 })
			{
				for (int i = 0; i < flowField->GetWidth()*flowField->GetHeight(); i++)
				{
					flowField->SetDirtAt(i, Vector2{101,101});

				}
	
			}
		}
	}
}

