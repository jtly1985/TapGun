
#include "cocos2d.h"

#include"Base/GameMaster.h"
 
//#define CN//カメラノードを使用するテスト
USING_NS_CC;
using namespace TapGun;
  
//実態をNULLで初期化
Camera* GameMaster::camera3D = NULL;
Camera* GameMaster::camera2D = NULL;
Node* GameMaster::CamNode = NULL;

/**
*	ゲームパラメータクラスの生成
*
*	@author	sasebon
*	@param	なし
*	@return	パラメータクラスの値
*	@date	1/16 Ver 1.0
*/
GameMaster* GameMaster::GetInstance(void)
{
	static GameMaster* P;
	if(!P)
	{
		P = new GameMaster;

	}
	return P;
}


/**
*	スクリーンサイズの取得
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::InitScreenSize(void)
{
	//引数なしの場合は自動取得
	//引数を指定することで、スクリーンサイズを任意の値に設定できるようにする？
	screenSize = Director::getInstance()->getWinSize();//スクリーンサイズを取得
}


/**
*	2Dカメラのインスタンス
*
*	@author	sasebon
*	@param	なし
*	@return	2D用カメラ
*	@date	1/16 Ver 1.0
*/
Camera* GameMaster::Get2DCamInstance(void)
{
	if(!camera2D)
	{
		GameMaster::camera2D = Camera::create();
	}
	return camera2D;
}



/**
*	3Dカメラのインスタンス
*
*	@author	sasebon
*	@param	なし
*	@return	3D用カメラ
*	@date	1/16 Ver 1.0
*/
Camera* GameMaster::Get3DCamInstance(void)
{
	if(!camera3D)
	{
		GameMaster::camera3D = Camera::create();
		GameMaster::CamNode = Node::create();
	}
	return camera3D;
}




Node* GameMaster::GetCamNodeInstance(void)
{
	if(!CamNode)
	{
		GameMaster::CamNode = Node::create();
	}
	return CamNode;
}



/**
*	ゲームパラメータの初期化
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::InitParam()
{
	playerState = PSTATE_IDLE;
	gameState = GSTATE_INIT;
	wave = 0;
	playerHP = 0;
	mutekiTime = 0;

	touchState = TSTATE_OFF;
	touchFlag = TFLAG_OFF;

	gameActionTime = TIME_MAXTIME;

	sPoint = 0;

	//移動ポイント定義は今後外部ファイルからの読み込みにし、ロード時に1度のみ読み込むようにします
	//プレイヤーの移動ポイントを設定する
	for (int i = 0; i < 100; i++)
	{
		//初期化
		stagePoint[i].pPos = Vec3(0.0f, 0.0f, 0.0f);
		stagePoint[i].pRot = Vec3(0.0f, 0.0f, 0.0f);
		stagePoint[i].cPos = Vec3(0.0f, 0.0f, 0.0f);
		stagePoint[i].cRot = Vec3(0.0f, 0.0f, 0.0f);
		stagePoint[i].pointType = PO_NONE;
		stagePoint[i].playerSide = PSIDE_RIGHT;//初期化は全てRIGHT
	}
	//stagePointの定義


	//ゲーム開始地点
	stagePoint[POINT_START].pPos = Vec3(-3.47f, 0.0f, 0.0f);//
	stagePoint[POINT_START].pRot = Vec3(0.0f, 140.0f, 0.0f);
	stagePoint[POINT_START].pointType = PO_START;

	//W
	stagePoint[POINT_W1].pPos = Vec3(6.856f, 0.0f, -3.849f);//
	stagePoint[POINT_W1].pRot = Vec3(0.0f, 165.0f, 0.0f);
	stagePoint[POINT_W1].cPos = Vec3(6.363f - 6.856f, 1.539f, -1.101f - (-3.849f)) + Vec3(0.8f, 0.0f, 0.25f);//
	stagePoint[POINT_W1].cRot = Vec3(87.167f - 90.0f, -23.778f + 180.0f - 165.0f, 0.0f) + Vec3(0.0f, 0.0f, 0.0f);
	stagePoint[POINT_W1].pointType = PO_BATTLE;
	stagePoint[POINT_W1].playerSide = PSIDE_LEFT;
	stagePoint[POINT_W1].hidePoint = setHidePoint(stagePoint[POINT_W1]);//カメラの回避座標をセット


	stagePoint[POINT_L2a].pPos = Vec3(8.881f, 0.0f, -8.851f);//
	stagePoint[POINT_L2a].pointType = PO_CHANGE;
	stagePoint[POINT_L2a].pointType = PO_FINISH;//現在はここでゲーム終了


	stagePoint[POINT_L2b].pPos = Vec3(11.452f, 0.0f, -11.488f);//
	stagePoint[POINT_L2b].pointType = PO_CHANGE;

	stagePoint[POINT_L2c].pPos = Vec3(13.985f, 0.0f, -12.64f);//
	stagePoint[POINT_L2c].pointType = PO_CHANGE;

	stagePoint[POINT_L2d].pPos = Vec3(17.354f, 0.0f, -13.744f);//
	stagePoint[POINT_L2d].pointType = PO_CHANGE;

	//クリア
	stagePoint[POINT_END].pPos = Vec3(18.0f, 0.0f, -13.744f);//
	stagePoint[POINT_END].pRot = Vec3(0.0f, 125.0f, 0.0f);
	stagePoint[POINT_END].cPos = Vec3(6.363f, 1.0f, -1.101f);//
	stagePoint[POINT_END].cRot = Vec3(MACRO_CROT_X(87.167f), MACRO_CROT_Y(-23.778f), 0.0f);
	stagePoint[POINT_END].pointType = PO_FINISH;

}



/**
*	ゲームパラメータクラスの初期化
*
*	@author	sasebon
*	@param	指定したウェーブ番号
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::InitParam(int wave)
{

}


/**
*	2D用カメラの座標セット
*
*	@author	sasebon
*	@param	指定の3次元ベクトル
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::SetCamera2DPos(cocos2d::Vec3 pos)
{
	camera2D->setPosition3D(pos);
}


/**
*	3D用カメラの座標セット
*
*	@author	sasebon
*	@param	カメラ座標
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::SetCamera3DPos(cocos2d::Vec3 pos)
{
	camera3D->setPosition3D(pos);
}



/**
*	3D用カメラの角度セット
*
*	@author	sasebon
*	@param	カメラ角度
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::SetCamera3DRot(cocos2d::Vec3 rot)
{
	//この関数は基本的に使わず、親ノードの回転で対応する
	camera3D->setRotation3D(rot);

}


/**
*	3D用カメラの座標加算
*
*	@author	sasebon
*	@param	加算するカメラ座標
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::AddCamera3DPos(cocos2d::Vec3 pos)
{
	camera3D->setPosition3D(camera3D->getPosition3D() + pos);
}


/**
*	3D用カメラの角度加算
*
*	@author	sasebon
*	@param	加算するカメラ角度
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::AddCamera3DRot(cocos2d::Vec3 rot)
{
	//この関数は基本的に使わず、親ノードの回転で対応する
	camera3D->setRotation3D(camera3D->getRotation3D() + rot);
}




/**
*	3D用カメラのノード座標セット
*
*	@author	sasebon
*	@param	カメラ座標
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::SetCameraNodePos(cocos2d::Vec3 pos)
{
	CamNode->setPosition3D(pos);
}



/**
*	3D用カメラのノード角度セット
*
*	@author	sasebon
*	@param	カメラ角度
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::SetCameraNodeRot(cocos2d::Vec3 rot)
{
	//カメラの回転は基本的にこちらを使用する
	CamNode->setRotation3D(rot);
}


/**
*	3D用カメラのノード座標加算
*
*	@author	sasebon
*	@param	加算するカメラ座標
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::AddCameraNodePos(cocos2d::Vec3 pos)
{
	CamNode->setPosition3D(camera3D->getPosition3D() + pos);
}


/**
*	3D用カメラのノード角度加算
*
*	@author	sasebon
*	@param	加算するカメラ角度
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::AddCameraNodeRot(cocos2d::Vec3 rot)
{
	CamNode->setRotation3D(camera3D->getRotation3D() + rot);
}


/**
*	3D用カメラの座標取得
*
*	@author	sasebon
*	@param	なし
*	@return	3D用カメラの座標
*	@date	2/23 Ver 1.0
*/
cocos2d::Vec3 GameMaster::GetCamera3DPos(void)
{
	return camera3D->getPosition3D();
}


/**
*	3D用カメラの角度取得
*
*	@author	sasebon
*	@param	なし
*	@return	3D用カメラの角度
*	@date	2/23 Ver 1.0
*/
cocos2d::Vec3 GameMaster::GetCamera3DRot(void)
{
	return camera3D->getRotation3D();
}


/**
*	3D用カメラの注視点セット
*
*	@author	sasebon
*	@param	注視点座標
*	@return	なし
*	@date	2/23 Ver 1.0
*/
void GameMaster::SetCameraTarget(cocos2d::Vec3 pos)
{
	camTarget = pos;
	camera3D->lookAt(pos, Vec3::UNIT_Y);//lookAtは原点に置き、setPositionで視点を動かします。
}

/**
*	3D用カメラの注視点取得
*
*	@author	sasebon
*	@param	なし
*	@return	注視点座標
*	@date	2/24 Ver 1.0
*/
Vec3 GameMaster::GetCameraTarget(void)
{
	return camTarget;
}

/**
*	3D用カメラの初期化
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::InitCamera3D()
{
	//今後、ゲームやプレイヤーの状態などで様々なカメラセットが出来るようにする？
	//camera3D->createPerspective(20, (GLfloat)s.width / s.height, 1, 1000);
	CamNode = Node::create();
	camera3D = Camera::createPerspective(PERSE, (GLfloat)screenSize.width / screenSize.height, 1, 500);

	camTarget = Vec3(0.0f, 0.0f, 0.0f);
	camera3D->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3::UNIT_Y);//lookAtは原点に置き、setPositionで視点を動かします。
	camera3D->setCameraFlag(CameraFlag::USER1);//USER1を3D用にする
	CamNode->addChild(camera3D);
}


/**
*	2D用カメラの初期化
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::InitCamera2D()
{
	//今後、ゲームやプレイヤーの状態などで様々なカメラセットが出来るようにする？
	//camera3D->createPerspective(20, (GLfloat)s.width / s.height, 1, 1000);

	camera2D = Camera::createOrthographic(screenSize.width, screenSize.height, 0, 100);//
	camera2D->setCameraFlag(CameraFlag::DEFAULT);//
}


/**
*	プレイヤー状態のセット
*
*	@author	sasebon
*	@param	playerState
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::SetPlayerState(int state)
{
	playerState = state;
}



/**
*	タッチ座標のセット
*
*	@author	sasebon
*	@param	なし
*	@return	タッチ座標の二次元ベクトル
*	@date	1/16 Ver 1.0
*/
void GameMaster::SetTouchPos(Touch* tch)
{
	touch = tch;
}




/**
*	タッチマネージャーの更新
*
*	@author	sasebon
*	@param	タッチ状態
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::UpdateTouchManager( void)
{
	switch(touchState)
	{
	case TSTATE_OFF://タッチしていない状態で

		if(TFLAG_ON == touchFlag)//タッチフラグが立った
		{
			touchState = TSTATE_ON;//タッチした状態にする
		}
		break;
	case TSTATE_ON://タッチした状態で

		if(TFLAG_MOVE == touchFlag)//移動した
		{
			touchState = TSTATE_MOVE;//移動した状態にする
		}
		else if(TFLAG_RELEASE == touchFlag)//離した
		{
			touchState = TSTATE_RELEASE;//離した状態にする
			touchFlag = TFLAG_OFF;//タッチフラグもOFFにする
		}
		else//それ以外の場合
		{
			touchState = TSTATE_ON;//タッチした状態のまま
		}

		break;
	case TSTATE_MOVE://移動中

		if(TFLAG_MOVE == touchFlag)//移動した
		{
			touchState = TSTATE_MOVE;//移動した状態のまま
		}
		else if(TFLAG_RELEASE == touchFlag)//離した
		{
			touchState = TSTATE_RELEASE;//離した状態にする
			touchFlag = TFLAG_OFF;//タッチフラグもOFFにする
		}
		else//それ以外の場合
		{
			touchState = TSTATE_ON;//タッチした状態にする
		}
		break;
	case TSTATE_RELEASE:
		touchState = TSTATE_OFF;
		touchFlag = TFLAG_OFF;//タッチフラグもOFFにする
		break;
	}
}


/**
*	タッチフラグのセット
*
*	@author	sasebon
*	@param	指定のタッチフラグ
*	@return	1:引数が正常 -1:無効な引数
*	@date	1/16 Ver 1.0
*/
int GameMaster::SetTouchFlag(int flag)
{
	if(TFLAG_NUM <= flag || 0 > flag)
	{
		touchFlag = 0;//
		return FALSE;
	}
	touchFlag = flag;
	return TRUE;
}


/**
*	2D用カメラの取得
*
*	@author	sasebon
*	@param	なし
*	@return	2D用Cemera
*	@date	1/16 Ver 1.0
*/
Camera* GameMaster::GetCamera2D(void)
{
	return camera2D;
}


/**
*	3D用カメラの取得
*
*	@author	sasebon
*	@param	なし
*	@return	3D用Cemera
*	@date	1/16 Ver 1.0
*/
Camera* GameMaster::GetCamera3D(void)
{
	return camera3D;
}



/**
*	3D用カメラの親ノードの取得
*
*	@author	sasebon
*	@param	なし
*	@return	3D用Cemera
*	@date	1/16 Ver 1.0
*/
const Node* GameMaster::GetCameraNode(void)
{
	return CamNode;
}


/**
*	プレイヤー状態の取得
*
*	@author	sasebon
*	@param	なし
*	@return	現在のplayerState
*	@date	1/16 Ver 1.0
*/
int GameMaster::GetPlayerState(void)
{
	return playerState;
}





/**
*	ゲーム状態のセット
*
*	@author	sasebon
*	@param	gameState
*	@return	なし
*	@date	1/16 Ver 1.0
*/
void GameMaster::SetGameState(int gState)
{
	gameState = gState;
	if(GSTATE_WAIT == gState)
	{
		waitFlag = 0;//ウェイトフラグも一緒に初期化する
	}
}


/**
*	ゲーム状態の取得
*
*	@author	sasebon
*	@param	なし
*	@return	現在のGameState
*	@date	1/16 Ver 1.0
*/
int GameMaster::GetGameState(void)
{
	return gameState;
}




/**
*	タッチ座標の取得
*
*	@author	sasebon
*	@param	なし
*	@return	タッチ座標の二次元ベクトル
*	@date	1/16 Ver 1.0
*/
Vec2 GameMaster::GetTouchPosInView()
{
	return touch->getLocationInView();
}


/**
*	タッチ座標の取得
*
*	@author	sasebon
*	@param	なし
*	@return	タッチ座標の二次元ベクトル
*	@date	1/16 Ver 1.0
*/
Vec2 GameMaster::GetTouchPos()
{
	return touch->getLocation();
}



/**
*	タッチ状態の取得
*
*	@author	sasebon
*	@param	なし
*	@return	タッチ座標の二次元ベクトル
*	@date	1/16 Ver 1.0
*/
int GameMaster::GetTouchFlag(void)
{
	return touchFlag;
}



/**
*	タッチ状態の取得
*
*	@author	sasebon
*	@param	なし
*	@return	タッチ座標の二次元ベクトル
*	@date	1/16 Ver 1.0
*/
int GameMaster::GetTouchState(void)
{
	return touchState;
}



/**
*	回避動作の軸座標の計算
*
*	@author	sasebon
*	@param	stagePoint
*	@return	軸座標
*	@date	2/1 Ver 1.0
*/
Vec2 GameMaster::setHidePoint(StagePoint stagePoint)
{
	Vec2 hidePoint = Vec2(-HIDEPOINT_X, -HIDEPOINT_Y);

	if(PSIDE_LEFT == stagePoint.playerSide)
	{
		hidePoint.x = hidePoint.x * cosf(stagePoint.pRot.y) - hidePoint.y * sinf(stagePoint.pRot.y);
		hidePoint.y = hidePoint.x * sinf(stagePoint.pRot.y) + hidePoint.y * cosf(stagePoint.pRot.y);
	}
	else
	{
		hidePoint = Vec2(HIDEPOINT_X, -HIDEPOINT_Y);

		hidePoint.x = hidePoint.x * cosf(stagePoint.pRot.y) - hidePoint.y * sinf(stagePoint.pRot.y);
		hidePoint.y = hidePoint.x * sinf(stagePoint.pRot.y) + hidePoint.y * cosf(stagePoint.pRot.y);
	}
	return hidePoint;
}


/**
*	プレイヤーHPのゲッター
*
*	@author	minaka
*	@param	なし
*	@return	HP
*	@date	2/6 Ver 1.0
*/
int GameMaster::GetPlayerHP(void)
{
	return playerHP;
}

/**
*	プレイヤーHPのセッター
*
*	@author	minaka
*	@param	パラメーター
*	@return	適切な引数:1,不正な引数:-1
*	@date	2/22 Ver 1.0
*/
int GameMaster::SetPlayerHP(int value)
{
	if (value > STS_MAXPLAYERHP)
	{
		playerHP = STS_MAXPLAYERHP;
		return FALSE;
	}
	else if (value < 0)
	{
		playerHP = 0;
		return FALSE;
	}
	else
	{
		playerHP = value;
		return TRUE;
	}
}


/**
*	プレイヤーHPのセッター
*
*	@author	sasebon
*	@param	パラメーター
*	@return	適切な引数:1,不正な引数:-1
*	@date	2/6 Ver 1.0
*/
int GameMaster::AddPlayerHP(int value)
{
	playerHP += value;
	if (playerHP > STS_MAXPLAYERHP)
	{
		//
		playerHP = STS_MAXPLAYERHP;
		return FALSE;
	}
	else if (playerHP < 0)
	{
		//
		playerHP = 0;
		return FALSE;
	}
	return TRUE;
}


/**
*	プレイヤー弾数のゲッター
*
*	@author	sasebon
*	@param	なし
*	@return	現在弾数
*	@date	2/9 Ver 1.0
*/
int GameMaster::GetPlayerBullets(void)
{
	return nowBullets;
}

/**
*	プレイヤー弾数のセッター
*
*	@author	sasebon
*	@param	パラメーター
*	@return	適切な引数:1,不正な引数:-1
*	@date	2/9 Ver 1.0
*/
int GameMaster::SetPlayerBullets(int value)
{
	nowBullets = value;
	if(0 > value)
	{
		nowBullets = 0;
		return FALSE;
	}
	else if(STS_MAXBULLETS < value)
	{
		nowBullets = STS_MAXBULLETS;
		return FALSE;
	}
	return TRUE;
}


/**
*	プレイヤー弾数の加算
*
*	@author	sasebon
*	@param	パラメーター
*	@return	適切な引数:1,不正な引数:-1
*	@date	2/9 Ver 1.0
*/
int GameMaster::AddPlayerBullets(int value)
{
	nowBullets += value;
	if(nowBullets < 0)
	{
		nowBullets = 0;
		return FALSE;
	}
	else if(STS_MAXBULLETS < nowBullets)
	{
		nowBullets = STS_MAXBULLETS;
		return FALSE;
	}
	return TRUE;
}




/**
*	現在ゲーム時間の取得
*
*	@author	sasebon
*	@param	ミリ秒
*	@return	現在ゲーム時間
*	@date	2/24 Ver 1.0
*/
int GameMaster::GetGameTime(void)
{
	return gameActionTime;
}


/**
*	現在ゲーム時間のセット
*
*	@author	sasebon
*	@param	ミリ秒
*	@return	適切な引数:1,不正な引数:-1
*	@date	2/24 Ver 1.0
*/
int GameMaster::setGameTime(int time)
{
	gameActionTime += time;
	if(gameActionTime < 0.0f)
	{
		gameActionTime = 0.0f;
		return FALSE;
	}
	else if(TIME_MAXTIME < gameActionTime)
	{
		gameActionTime = TIME_MAXTIME;
		return FALSE;
	}
	return TRUE;
}


/**
*	現在ゲーム時間の加算
*
*	@author	sasebon
*	@param	パラメーター
*	@return	適切な引数:1,不正な引数:-1
*	@date	2/24 Ver 1.0
*/
int GameMaster::AddGameTime(int time)
{
	gameActionTime += time;
	if(gameActionTime < 0)
	{
		gameActionTime = 0.0f;
		return FALSE;
	}
	else if(TIME_MAXTIME < gameActionTime)
	{
		gameActionTime = TIME_MAXTIME;
		return FALSE;
	}
	return TRUE;
}


/**
*	注視点に対するカメラの向きの計算
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	2/24 Ver 1.0
*/
void GameMaster::CalcCameraRot()
{
	//カメラ視点座標と注視点座標の位置からカメラ角度を設定
	//カメラの向きベクトルを取得
	Vec3 cVec = GetCameraTarget() - GetCamera3DPos();

	//ベクトルの正規化を行う
	cVec.normalize();

	//カメラの向きを調整
	double ry = atan2f(cVec.z, cVec.x);
	ry = CC_RADIANS_TO_DEGREES(ry);

	double rx = atan2f(cVec.z, cVec.y);
	rx = CC_RADIANS_TO_DEGREES(rx);

	SetCamera3DRot(Vec3(90.0f - rx, 90.0f - ry + 180.0f, 0.0f));//
}