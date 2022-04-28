#include "AnimatedSprite.h"
#include "Actor.h"
#include "Game.h"

AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
{
}

void AnimatedSprite::Update(float deltaTime)
{
	if (!mAnimName.empty() && !mIsPaused) {
		mAnimTimer += mAnimFPS * deltaTime;
		if (mAnimTimer >= mAnims[GetAnimName()].size()) {
			while (mAnimTimer >= mAnims[GetAnimName()].size()) {
				mAnimTimer -= mAnims[GetAnimName()].size();
			}
		}
		SDL_Texture* tex = mAnims[GetAnimName()][(int)mAnimTimer];
		SetTexture(tex);
	}

}

void AnimatedSprite::SetAnimation(const std::string& name, bool resetTimer)
{
	if (mAnimName != name)
	{
		mAnimName = name;
	}
	
	if (resetTimer)
	{
		mAnimTimer = 0.0f;
	}
}

void AnimatedSprite::AddAnimation(const std::string& name,
								  const std::vector<SDL_Texture*>& images)
{
	mAnims.emplace(name, images);
}
