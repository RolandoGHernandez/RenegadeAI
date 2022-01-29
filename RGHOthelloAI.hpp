// RGHOthelloAI.hpp


#include "OthelloAI.hpp"

namespace rolandoh
{
	class RGHOthelloAI : public OthelloAI
	{

	public:

		// Main driver function that returns best move possible.
		std::pair<int, int> chooseMove(const OthelloGameState& state) override;

		// The recursion function that searches "itself" until the best game state is found.
		int search(std::unique_ptr<OthelloGameState>& s, int depth, bool color);

		// Evaluation function to determine best state, NOT the state itself.
		int evaluation(std::unique_ptr<OthelloGameState>& state);

	private:
		// Nodes for a linked list.
		struct Node
		{
			Node* next;
			//std::unique_ptr<std::unique_ptr<OthelloGameState>> uniqueGameState;
			//std::unique_ptr<OthelloGameState> gameState;
			std::pair<int, int> cell;
			//int priority;
		};
	};
}


