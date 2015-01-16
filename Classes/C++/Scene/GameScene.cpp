#include "GameScene.h"

//各レイヤーを管理するソースコードをインクルードしておく

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "GameModelsLayer.h"
#include "GameUILayer.h"
//#include"GameStatus.h"

#else

#include "C++/Scene/GameModelsLayer.h"
#include "C++/Scene/GameUILayer.h"
//#include "C++/Base/GameStatus.h"

#endif

USING_NS_CC;
using namespace TapGun;


/*
GameScene
（将来的にGameSceneなどに名前を変更する）
ゲーム本編の更新処理を行う
ここで処理された内容を元に、モデルデータを扱うレイヤーとＵＩを扱うレイヤーで描画を行う
*/


Camera *camera3;//3D用カメラ
Camera *camera2;//2D用カメラ


GameModelsLayer* gGameLayer;
GameUILayer* gUILayer;
 
static GameScene *multiSceneLayerInstance;
 
/*
インクリメント
*/
//GameScene::GameScene()
//{
//	for(int i = 0; i < MAX_MODEL; i++)
//	{
//		Unit[MAX_MODEL].valid = 0;
//
//	}
//}



/**
*	ゲーム管理シーンクリエイト
*
*	@author	sasebon
*	@param	なし
*	@return	シーンのポインタ
*	@date	1/8 Ver 1.0
*/
Scene* GameScene::CreateScene()
{
	Scene *scene = Scene::create();//GameSceneのシーンを作成
	GameScene *layer = GameScene::create();//上記シーンに

	scene->addChild(layer);

	return scene;
};


//ゲームレイヤーの初期化関数
//GameLayer* GameScene::gameLayer()
//{
//	cocos2d::Node* layer = this->getChildByTag(GameLayerTag);
//	return (GameLayer *)layer;
//}


//UILayer* GameScene::uiLayer()
//{
//	cocos2d::Node* layer = GameScene::sharedLayer()->getChildByTag(UILayerTag);
//	return (UILayer *)layer;
//}




/**
*	ゲームシーン初期化
*
*	@author	sasebon
*	@param	なし
*	@return	正常:1 初期化失敗:-1
*	@date	1/8 Ver 1.0
*/
bool GameScene::init()
{
	//レイヤー初期化
	if(!Layer::init())
	{
		return false;
	}
	//multiSceneLayerInstance = this;

	//ゲームレイヤーを作成
	gGameLayer = GameModelsLayer::create();
	this->addChild(gGameLayer);


	//UIレイヤーを作成（現在はコメントアウト）
	//gUILayer = GameUILayer::create();
	//this->addChild(gUILayer);

//	GameStatus* gameStat = GameStatus::getObject();

	//タッチ座標を初期化
	touch_pos.x = 0.0f;
	touch_pos.y = 0.0f;

	game_state = GSTATE_INIT;
	player_state = 0;//

	//現在はタッチイベントのリスナーをここに用意しています
	auto dispatcher = Director::getInstance()->getEventDispatcher();

	listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);

	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//setTouchMode(Touch::DispatchMode::ONE_BY_ONE);


	this->scheduleUpdate();
	this->schedule(schedule_selector(GameScene::moveTime), 0.016f);//1秒60Fでゲーム更新

	return true;
}



/**
*	ゲームシーンのカメラ初期化
*
*	@author	sasebon
*	@param	なし
*	@return	正常:1
*	@date	1/8 Ver 1.0
*/
int GameScene::InitCamera()
{

	auto s = Director::getInstance()->getWinSize();


	//2D用カメラの実装
	//camera2 = Camera::create();
	camera2 = Camera::createPerspective(20, (GLfloat)s.width / s.height, 1, 1000);
	//camera2 = Camera::createOrthographic(20, (GLfloat)s.width / s.height, 1, 1000);//本来はこちらです
	// set parameters for camera
	//camera3->setPosition3D(Vec3(0.0f, 0.0f, 0.0f));

	//プレイヤーの座標取得はとりあえずこのような形で記述しています

	if(NULL != gUILayer)
	{
		Vec3 cameraPos2 = gGameLayer->unit[playerNum].sprite3d->getPosition3D();

		cameraPos2.x += 0.5f;
		cameraPos2.y += 1.5f;
		cameraPos2.z += 3.1f;

		camera2->setPosition3D(cameraPos2);
		camera2->lookAt(Vec3(0, 0, cameraPos2.z - 120.0f), Vec3(0, 1, 0));

		addChild(camera2);//add camera to the scene


		// set parameters for camera
		//camera3->setPosition3D(Vec3(0.0f, 0.0f, 0.0f));
	}
	camera3 = Camera::create();
	camera3 = Camera::createPerspective(20, (GLfloat)s.width / s.height, 1, 1000);

	//3D用カメラの実装
	if(NULL != gGameLayer)
	{
		//プレイヤーの座標取得はとりあえずこのような形で記述しています
		Vec3 cameraPos = gGameLayer->unit[playerNum].sprite3d->getPosition3D();

		cameraPos.x += 0.5f;
		cameraPos.y += 1.5f;
		cameraPos.z += 3.1f;

		camera3->setPosition3D(cameraPos);
		camera3->lookAt(Vec3(0, 0, cameraPos.z - 120.0f), Vec3(0, 1, 0));
		camera3->setRotation3D(Vec3(0.0f, -5.0f, 0.0f));
		addChild(camera3);//add camera to the scene
	}

	return TRUE;
}



//更新系


/**
*	指定フレームごとのゲームシーン更新
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void GameScene::moveTime(float delta)
{

	switch (game_state)
	{

	case GSTATE_INIT://子レイヤー内の変数初期化を行う


		if (NULL != gGameLayer)//現在は子レイヤーをクリエイトしたかを確認する
		{
			playerNum = gGameLayer->InitLayer();//
		}
		if (NULL != gUILayer)//現在は子レイヤーをクリエイトしたかを確認する
		{
			gUILayer->InitLayer();//
		}

		InitCamera();

		game_state = GSTATE_PLAY;

		break;

	case GSTATE_PLAY:
		if (NULL != gGameLayer)//現在は初期化チェック確認する
		{
			gGameLayer->UpdateLayer(&player_state,touch_pos, camera3);//レイヤーの更新(現在はタッチ座標とカメラ構造体を引数として渡しています)
		}
		if (NULL != gUILayer)//現在は初期化チェック確認する
		{

		}
		UpdateCamera();//モデルの移動をもとにカメラ移動

		break;

	}
}


/**
*	ゲームシーン更新（現在使用していません）
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
void GameScene::update(float delta)
{
	
}



/**
*	ゲームシーンのカメラ更新
*
*	@author	sasebon
*	@param	なし
*	@return	なし
*	@date	1/8 Ver 1.0
*/
int GameScene::UpdateCamera()
{

	//現在は3Dモデルに追従させて2Dカメラを更新しています
	auto s = Director::getInstance()->getWinSize();
	if(NULL != gUILayer)
	{
		Vec3 CamRot = camera2->getRotation3D();
		Vec3 CamPos = camera2->getPosition3D();

		gUILayer->UpdateUI(CamPos.x, CamPos.y, CamPos.z, CamRot.x, CamRot.y, CamRot.z);
		gUILayer->setPosition3D(CamPos);
		gUILayer->setRotation3D(CamRot);
	}

	if(NULL != gGameLayer)
	{
		Vec3 CamRot = camera3->getRotation3D();
		Vec3 CamPos = camera3->getPosition3D();
	}

	return TRUE;
}






bool GameScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	touch_pos = pTouch->getLocation();
	player_state = 1;
	return true;
}


void GameScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{

	Director *pDirector;
	Point touchPoint;
	Node *pNode;
	Rect spRect;

	static float rx;
	static float ry;
	static float rz;

	auto s = Director::getInstance()->getWinSize();//画面サイズ取得
	//タッチ座標を取得する
	pDirector = Director::getInstance();
	touchPoint = pDirector->convertToGL(pTouch->getLocationInView());

	if(touchPoint.x >= s.width * 2 / 3 && (touchPoint.y >= s.height / 3 && touchPoint.y < s.height * 2 / 3))
	{
		//Vec3 tmpVec = camera3->getPosition3D();
		//tmpVec.y += 0.2f;
		//camera3->setPosition3D(tmpVec);
		//ry -= 1.0f;
		//if(NULL != gGameLayer)
		//{
		//	camera3->setRotation3D(Vec3(0.0f, ry, 0.0f));
		//}
		//if(NULL != gUILayer)
		//{
		//	camera2->setRotation3D(Vec3(0.0f, ry, 0.0f));
		//}
	}
	else if(touchPoint.x < s.width / 3 && (touchPoint.y >= s.height / 3 && touchPoint.y < s.height * 2 / 3))
	{
		//Vec3 tmpVec = camera3->getPosition3D();
		//tmpVec.y -= 0.2f;
		//camera3->setPosition3D(tmpVec);
		//ry += 1.0f;
		//if(NULL != gGameLayer)
		//{
		//	camera3->setRotation3D(Vec3(0.0f, ry, 0.0f));
		//}
		//if(NULL != gUILayer)
		//{
		//	camera2->setRotation3D(Vec3(0.0f, ry, 0.0f));
		//}
	}
	else if(touchPoint.y < s.height / 3 && (touchPoint.x >= s.width / 3 && touchPoint.x < s.width * 2 / 3))
	{
		//Vec3 tmpVec = camera3->getPosition3D();
		//tmpVec.z += 0.2f;
		//camera3->setPosition3D(tmpVec);
	}
	else if(touchPoint.y >= s.height * 2 / 3 && (touchPoint.x >= s.width / 3 && touchPoint.x < s.width * 2 / 3))
	{
		//Vec3 tmpVec = camera3->getPosition3D();
		//tmpVec.z -= 0.2f;
		//camera3->setPosition3D(tmpVec);
	}
}



void GameScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
{
	
}

