#include "GhostAI.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL.h>
#include <unordered_map>
#include "Ghost.h"
#include "PacMan.h"
#include "Random.h"

GhostAI::GhostAI(class Actor* owner)
:Component(owner, 50)
{
	mGhost = static_cast<Ghost*>(owner);
}

void GhostAI::Update(float deltaTime)
{	
	if (mState == State::Frightened) {
		mFrightTimer += deltaTime;
	}

	if (mState == State::Chase || mState == State::Scatter) {
		mTimer += deltaTime;
	}
	if (mTimer > 25 && mState != State::Frightened && mState != State::Dead) {
		mState = State::Scatter;
		mTimer = 0;
	}
	if (mTimer > 5 && mState != State::Frightened && mState != State::Dead) {
		mState = State::Chase;
	}
		Vector2 direction = mNextNode->GetPosition() - mPrevNode->GetPosition();
		direction = direction.Normalize(direction);
		UpdateAnim(direction);
		mOwner->SetPosition(mOwner->GetPosition() + direction.Normalize(direction) * mSpeed * deltaTime);
		
		if (mOwner->GetComponent<CollisionComponent>()->Intersect(mNextNode->GetComponent<CollisionComponent>())) {
			mOwner->SetPosition(mNextNode->GetPosition());
			switch (GetState()) {
				case State::Scatter:
					if (!mPath.empty()) {
						mPrevNode = mNextNode;
						mNextNode = mPath.back();
						mPath.pop_back();
					}
					else {
						AStar(mNextNode, mGhost->GetScatterNode());
					}
					break;
				case State::Chase:
					UpdateChase(deltaTime);
					break;
				case State::Dead:
					if (!mPath.empty()) {
						mPrevNode = mNextNode;
						mNextNode = mPath.back();
						mPath.pop_back();
					}
					else {
						if (mNextNode == mOwner->GetGame()->mGhostPen) {
							Start(mNextNode);
						}
						else {
							AStar(mNextNode, mOwner->GetGame()->mGhostPen);
						}
						
					}
					break;
				case State::Frightened:
					PathNode* randNode = Random();
					mPrevNode = mNextNode;
					mNextNode = randNode;
					if (mFrightTimer > 7) {
						mFrightTimer = 0;
						mSpeed = 90.0f;
						mState = State::Scatter;
						mPath.clear();
						AStar(mPrevNode, mGhost->GetScatterNode());
					}
					break;
			}

		}

}

void GhostAI::UpdateChase(float deltaTime) {
	PathNode* target;
	Ghost::Type ghostType = mGhost->GetType();
	switch (ghostType) {
	case Ghost::Type::Blinky:
		target = mOwner->GetGame()->mPlayer->GetPrevNode();		
		if (target->GetType() == PathNode::Tunnel) {
			target = GetClosest(mOwner->GetGame()->mPlayer->GetPosition());
		}
		break;
	case Ghost::Type::Pinky:
		target = GetClosest(mOwner->GetGame()->mPlayer->GetPointInFrontOf(80));
		break;
	case Ghost::Type::Inky:
	{
		Vector2 pacPoint = mOwner->GetGame()->mPlayer->GetPointInFrontOf(40);
		Vector2 blinkyPoint = pacPoint - mOwner->GetGame()->mGhosts[0]->GetPosition();
		Vector2 blinkyDouble = blinkyPoint;
		blinkyDouble *= 2;
		target = GetClosest(blinkyDouble);
		break;
	}

	case Ghost::Type::Clyde:
		if ((mOwner->GetGame()->mPlayer->GetPosition() - mOwner->GetPosition()).Length() < 150.0f) {
			target = mOwner->GetGame()->mPlayer->GetPrevNode();
			if (target->GetType() == PathNode::Tunnel) {
				target = GetClosest(mOwner->GetGame()->mPlayer->GetPosition());
			}
		}
		else {
			target = mGhost->GetScatterNode();
		}
		break;
	}

	for (auto node : mOwner->GetGame()->mPathNodes) {
		// reaches intersection
		if (mOwner->GetComponent<CollisionComponent>()->Intersect(node->GetComponent<CollisionComponent>())) {
			AStar(mNextNode, target);
		}
	}
}

PathNode* GhostAI::GetClosest(Vector2 point) {
	PathNode* closest = mOwner->GetGame()->mPathNodes[0];
	for (auto node : mOwner->GetGame()->mPathNodes) {
		float dist = (node->GetPosition() - point).Length();
		float closeDist = (closest->GetPosition() - point).Length();
		if (dist < closeDist && node->GetType() == PathNode::Default) {
			closest = node;
		}
	}
	return closest;
}

void GhostAI::UpdateAnim(Vector2 dir) {
	if (mFrightTimer > 5) {
		mGhost->SetAnim("scared1");
	}
	else if (mState == State::Frightened) {
		mGhost->SetAnim("scared0");
	}
	else if (dir.x == 1) {
		mGhost->SetAnim("right");
		if (mState == State::Dead) {
			mGhost->SetAnim("deadright");
		}
	}
	else if (dir.x == -1) {
		mGhost->SetAnim("left");
		if (mState == State::Dead) {
			mGhost->SetAnim("deadleft");
		}
	}
	else if (dir.y == 1) {
		mGhost->SetAnim("down");
		if (mState == State::Dead) {
			mGhost->SetAnim("deaddown");
		}
	}
	else if (dir.y == -1) {
		mGhost->SetAnim("up");
		if (mState == State::Dead) {
			mGhost->SetAnim("deadup");
		}
	}
}
void GhostAI::Frighten()
{
	mState = State::Frightened;
	mPath.clear();
	PathNode* temp = mPrevNode;
	mPrevNode = mNextNode;
	mNextNode = temp;
	mSpeed = 65.0f;
	mFrightTimer = 0;
	mGhost->SetAnim("scared0");
}

PathNode* GhostAI::Random() {
	std::vector<PathNode*> valid;
	for (auto node : mNextNode->mAdjacent) {
		if (node != mPrevNode && node->GetType() != PathNode::Type::Tunnel && node->GetType() != PathNode::Type::Ghost) {
			valid.push_back(node);
		}
	}
	if (!valid.empty()) {
			int randNode = Random::GetIntRange(0, valid.size() - 1);
			return valid[randNode];
	}
	else {
		std::vector<PathNode*> ghosts;
		for (auto node : mNextNode->mAdjacent) {
			if (node->GetType() == PathNode::Type::Ghost && node != mPrevNode) {
				ghosts.push_back(node);
			}			
		}
		int randNode = Random::GetIntRange(0, ghosts.size() - 1);
		return ghosts[randNode];
	}
}



void GhostAI::Start(PathNode* startNode)
{
	mOwner->SetPosition(startNode->GetPosition());
	mState = State::Scatter;
	mPrevNode = nullptr;
	mNextNode = nullptr;
	mTargetNode = nullptr;
	mPath.clear();
	mSpeed = 90.0f;
	mTimer = 0;
	mFrightTimer = 0;
	AStar(startNode, mGhost->GetScatterNode());
}

void GhostAI::AStar(PathNode* startNode, PathNode* targetNode) {
	struct NodeInfo {
		PathNode* parent = nullptr;
		PathNode* unusable = nullptr;
		float f = 0.0f;
		float g = 0.0f;
		float h = 0.0f;
		bool IsClosed = false;
	};
	mPath.clear();
	std::vector<PathNode*> openSet;
	std::unordered_map<PathNode*, NodeInfo> info;
	struct NodeInfo startInfo;
	for (auto node : mOwner->GetGame()->mPathNodes) {
		info[node] = startInfo;
	}
	PathNode* currentNode = startNode;
	info[startNode].unusable = mPrevNode;
	for (auto adjNode : startNode->mAdjacent) {
		if (adjNode != mPrevNode) {
			info[adjNode].unusable = startNode;
		}
	}
	if (startNode != targetNode) {
		info[currentNode].IsClosed = true;
	}
	
	do {
		for (auto node : currentNode->mAdjacent) {
			if (node->GetType() != PathNode::Type::Tunnel) {
				if (!info[node].IsClosed && node != info[currentNode].unusable) {
					if (VectorContains(openSet, node)) {
						float new_g = info[currentNode].g + Euclidean(currentNode, node);
						if (new_g < info[node].g) {
							info[node].parent = currentNode;
							info[node].g = new_g;
							info[node].f = info[node].g + info[node].h;
						}
					}
					else {
							info[node].parent = currentNode;
							info[node].h = Euclidean(node, targetNode);
							info[node].g = info[currentNode].g + Euclidean(currentNode, node);
							info[node].f = info[node].g + info[node].h;
							openSet.push_back(node);
					}				
				}
			}
		}
		if (openSet.empty()) {
			break;
		}
		currentNode = openSet[0];
		for (auto node : openSet) {
			if (info[node].f < info[currentNode].f) {
				currentNode = node;
			}
		}
		openSet.erase(std::find(openSet.begin(), openSet.end(), currentNode));
		info[currentNode].IsClosed = true;
	} while (currentNode != targetNode);

	// set mpath
	mTargetNode = targetNode;
	mPrevNode = startNode;
	
	PathNode* node = currentNode;
	do {
		mPath.push_back(node);
		node = info[node].parent;
	} while (node != startNode && info[node].parent != nullptr);

	mNextNode = mPath.back();
	mPath.pop_back();
}

float GhostAI::Euclidean(PathNode* node, PathNode* end) {
	float xDistSqrd = (node->GetPosition().x - end->GetPosition().x) *
		(node->GetPosition().x - end->GetPosition().x);
	float yDistSqrd = (node->GetPosition().y - end->GetPosition().y) *
		(node->GetPosition().y - end->GetPosition().y);
	return Math::Sqrt(xDistSqrd + yDistSqrd);

}



bool GhostAI::VectorContains(std::vector<PathNode*> vec, PathNode* target) {
	if (std::count(vec.begin(), vec.end(), target)) {
		return true;
	}
	return false;
}



void GhostAI::Die()
{
	mPath.clear();
	mState = State::Dead;
	mSpeed = 125.0f;
	mFrightTimer = 0;
}

void GhostAI::DebugDrawPath(SDL_Renderer* render)
{
	// Draw a rectangle at the target node
	if (mTargetNode != nullptr)
	{
		const int SIZE = 16;
		SDL_Rect r;
		r.x = static_cast<int>(mTargetNode->GetPosition().x) - SIZE / 2;
		r.y = static_cast<int>(mTargetNode->GetPosition().y) - SIZE / 2;
		r.w = SIZE;
		r.h = SIZE;
		SDL_RenderDrawRect(render, &r);
	}

	// Line from ghost to next node
	if (mNextNode != nullptr)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mOwner->GetPosition().x),
			static_cast<int>(mOwner->GetPosition().y),
			static_cast<int>(mNextNode->GetPosition().x),
			static_cast<int>(mNextNode->GetPosition().y));
	}

	// Exit if no path
	if (mPath.empty())
	{
		return;
	}

	if (mNextNode)
	{
		// Line from next node to subsequent on path
		SDL_RenderDrawLine(render,
			static_cast<int>(mNextNode->GetPosition().x),
			static_cast<int>(mNextNode->GetPosition().y),
			static_cast<int>(mPath.back()->GetPosition().x),
			static_cast<int>(mPath.back()->GetPosition().y));
	}

	// Lines for rest of path
	for (size_t i = 0; i < mPath.size() - 1; i++)
	{
		SDL_RenderDrawLine(render,
			static_cast<int>(mPath[i]->GetPosition().x),
			static_cast<int>(mPath[i]->GetPosition().y),
			static_cast<int>(mPath[i + 1]->GetPosition().x),
			static_cast<int>(mPath[i + 1]->GetPosition().y));
	}
}
