
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "Sprite3D.h"

#else

#include "C++/Base/Sprite3D.h"

#endif

namespace TapGun
{
	class Enemy
	{
	public:
		cocos2d::Vec3 standbyPos;//
		cocos2d::Vec3 targetPos;
		
		int number;
		int kind;//敵の種類

		int nextEnemyNum;//この敵が消えたときに、次に出てくる敵

	private:

	};
}

#endif // __ENEMY_H__
