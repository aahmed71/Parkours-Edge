#include "Block.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Math.h"
#include "Game.h"

Block::Block(Game* game, char blockType)
	:Actor(game)
{
	mSprite = new SpriteComponent(this);
	std::string texture = "Assets/Block";
	texture.push_back(blockType);
	texture += ".png";
	// choose texture for type of block
	mSprite->SetTexture(mGame->GetTexture(texture));
	mCollComp = new CollisionComponent(this);
	mCollComp->SetSize(32, 32);
	mGame->AddBlock(this);
}


CollisionComponent* Block::GetCollComp() {
	return mCollComp;
}
