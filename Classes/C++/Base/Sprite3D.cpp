
#include <fstream>
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "Sprite3D.h"

#else

#include "C++/Base/Sprite3D.h"

#endif

USING_NS_CC;
using namespace std;

namespace TapGun
{
	/**
	*	3Dスプライトの作成
	*
	*	@author	minaka
	*	@param	firstPath リソースファイル名
	*	@return	作成したスプライトへのポインタ
	*	@date	1/3	Ver 1.0
	*/
	_Sprite3D* _Sprite3D::create( const string& firstPath)
	{
		return createObject( firstPath.c_str(), nullptr, nullptr);
	}

	/**
	*	3Dスプライトの作成
	*
	*	@author	minaka
	*	@param	firstPath リソースファイル名
	*	@param	secondPath リソースファイル名
	*	@return	作成したスプライトへのポインタ
	*	@date	1/3	Ver 1.0
	*/
	_Sprite3D* _Sprite3D::create( const string& firstPath, const string& secondPath)
	{
		return createObject( firstPath.c_str(), secondPath.c_str(), nullptr);
	}

	/**
	*	3Dスプライトの作成
	*
	*	@author	minaka
	*	@param	firstPath リソースファイル名
	*	@param	secondPath リソースファイル名
	*	@param	thirdPath リソースファイル名
	*	@return	作成したスプライトへのポインタ
	*	@date	1/5	Ver 1.0
	*/
	_Sprite3D* _Sprite3D::create( const string& firstPath, const string& secondPath, const string& thirdPath)
	{
		return createObject( firstPath.c_str(), secondPath.c_str(), thirdPath.c_str());
	}

	_Sprite3D* _Sprite3D::createObject( const char* firstPath, const char* secondPath, const char* thirdPath)
	{
		string filePath;
		bool Flag[ResouceType::Num] = { false };
		map< int, string> str;
		auto sprite = new (nothrow) _Sprite3D();

		if( &firstPath == nullptr) return nullptr;
		else str[0] = firstPath;
		if( secondPath != nullptr) str[1] = secondPath;
		if( thirdPath != nullptr) str[2] = thirdPath;

		for( int i = 0; i < str.size(); i++)
		{
			if( &str.at(i) == nullptr) break;

			switch( checkResourcePath(str.at(i)))
			{
			case ResouceType::NoExt:
				if( Flag[ResouceType::NoExt] == false)
				{
					filePath = getResourcePath( ResouceType::NoExt);
				#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
				  #ifdef DEBUG
					filePath = filePath + str.at(i) + ".c3t";
				  #else
					filePath = filePath + str.at(i) + ".c3b";
				  #endif
				#else
				#ifdef _DEBUG
					filePath = filePath + str.at(i) + ".c3t";
				  #else
					filePath = filePath + str.at(i) + ".c3b";
				  #endif
				#endif
					if (sprite && sprite->initWithFile(filePath))
					{
						sprite->_contentSize = sprite->getBoundingBox().size;
						sprite->autorelease();
					}
					Flag[ResouceType::NoExt] = true;
				}
				else
				{
					return nullptr;
				}
				break;

			case ResouceType::Model:
				if( Flag[ResouceType::Model] == false)
				{
					filePath = getResourcePath( ResouceType::Model);
					filePath = filePath + str.at(i);
					if (sprite && sprite->initWithFile(filePath))
					{
						sprite->_contentSize = sprite->getBoundingBox().size;
						sprite->autorelease();
					}
					Flag[ResouceType::Model] = true;
				}
				else
				{
					return nullptr;
				}
				break;

			case ResouceType::Anime:
				if( Flag[ResouceType::Anime] == false)
				{
					filePath = getResourcePath( ResouceType::Anime);
					filePath = filePath + str.at(i);
					sprite -> load3DModelAnimeData( filePath);
					Flag[ResouceType::Anime] = true;
				}
				else
				{
					return nullptr;
				}
				break;

			case ResouceType::Texture:
				if( Flag[ResouceType::Texture] == false)
				{
					filePath = getResourcePath( ResouceType::Texture);
					filePath = filePath + str.at(i);
					sprite -> load3DModelTextureData( filePath);
					sprite -> setTextureList();
					Flag[ResouceType::Texture] = true;
				}
				else
				{
					return nullptr;
				}
				break;

			case ResouceType::Picture:
				if( Flag[ResouceType::Picture] == false)
				{
					filePath = getResourcePath( ResouceType::Picture);
					filePath = filePath + str.at(i);
					sprite->setTexture(filePath);
					Flag[ResouceType::Picture] = true;
				}
				else
				{
					return nullptr;
				}
				break;

			default:
				CC_SAFE_DELETE(sprite);
				return nullptr;
			}
		}
		if( Flag[ResouceType::NoExt] == false && Flag[ResouceType::Model] == false)
		{
			CC_SAFE_DELETE(sprite);
			return nullptr;
		}
		return sprite;
	}

	ResouceType _Sprite3D::checkResourcePath( const string& filePath)
	{
		string str = filePath;
		int point = str.rfind( '.', str.size());
		if( point == string::npos) return ResouceType::NoExt;
		str.erase( 0, str.size() - ( str.size() - point));
		if( str == ".obj" || str == ".c3t" || str == ".c3b") return ResouceType::Model;
		else if( str == ".anime") return ResouceType::Anime;
		else if( str == ".texture") return ResouceType::Texture;
		else return ResouceType::Picture;
	}

	string _Sprite3D::getResourcePath( ResouceType type)
	{
		switch( type)
		{
		case ResouceType::NoExt:
		case ResouceType::Model:
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			return "";
		#else
			return "Graph/Models/";
		#endif
			break;

		case ResouceType::Anime:
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			return "";
		#else
			return "Parameter/Animation/";
		#endif
			break;

		case ResouceType::Texture:
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			return "";
		#else
			return "Parameter/Texture/";
		#endif
			break;

		case ResouceType::Picture:
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			return "";
		#else
			return "Graph/Textures/";
		#endif
			break;

		default:
			break;
		}
	}

	/**
	*	3Dモデルデータ用アニメーション設定ファイルの読み込み
	*
	*	@author	minaka
	*	@param	fileName モデルデータ名
	*	@return	正常終了:0 エラー発生:-1
	*	@date	1/3	Ver 1.0
	*/
	int _Sprite3D::load3DModelAnimeData( const string& fileName)
	{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		std::string dir = FileUtils::getInstance() -> fullPathForFilename(fileName);
	#endif
		ifstream file( dir, ios::in);
		if( file.fail())
		{
			return -1;
		}
		string str;
		while( getline( file, str))
		{
			string name;
			string path;
			string tmp;
			istringstream stream(str);
			getline( stream, tmp, ',');
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			/*ファイル名のみを抽出！*/ /*@*/
			path = "mot_enemy_jump.c3b";
	#else
			path = "Graph/Models/" + tmp;
	#endif
			getline( stream, tmp);
			name = tmp;
			modelAnimeList[name] = path;
		}
		return 0;
	}

	/**
	*	3Dモデルのアニメーション再生
	*
	*	@author	minaka
	*	@param	animeName アニメーション名
	*	@return	正常終了:0 エラー発生:-1
	*	@date	1/3	Ver 1.0
	*/
	int _Sprite3D::startAnimation( const string& animeName)
	{
		animation = cocos2d::Animation3D::create( modelAnimeList[animeName]);
		if( animation == nullptr) return -1;
		animate = cocos2d::Animate3D::create( animation);
		if( animate == nullptr) return -1;
		runAction( animate);
		return 0;
	}

	/**
	*	3Dモデルのアニメーション再生（ループ）
	*
	*	@author	minaka
	*	@param	animeName アニメーション名
	*	@return	正常終了:0 エラー発生:-1
	*	@date	1/3	Ver 1.0
	*/
	int _Sprite3D::startAnimationLoop( const string& animeName)
	{
		string str = modelAnimeList[animeName];
		animation = cocos2d::Animation3D::create( str);
		if( animation == nullptr) return -1;
		animate = cocos2d::Animate3D::create( animation);
		if( animate == nullptr) return -1;
		runAction( cocos2d::RepeatForever::create( animate));
		return 0;
	}

	/**
	*	3Dモデルのアニメーション停止
	*
	*	@author	minaka
	*	@param	animeName アニメーション名
	*	@return	正常終了:0
	*	@date	1/3	Ver 1.0
	*/
	int _Sprite3D::stopAnimation( const string& animeName)
	{
		stopAction( this -> animate);
		return 0;
	}

	/**
	*	全てのアニメーションを停止
	*
	*	@author	minaka
	*	@return	正常終了:0
	*	@date	1/3	Ver 1.0
	*/
	int _Sprite3D::stopALLAnimation( void)
	{
		stopAllActions();
		return 0;
	}

	/**
	*	3Dモデルのアニメーション再生速度を設定
	*
	*	@author	minaka
	*	@param	speed アニメーション速度
	*	@return	正常終了:0
	*	@date	1/3	Ver 1.0
	*/
	int _Sprite3D::setAnimationSpeed( float speed)
	{
		animate -> setSpeed( speed);
		return 0;
	}

	/**
	*	3Dモデルのアニメーション状態チェック
	*
	*	@author	minaka
	*	@return	アニメーション中ではない:0　アニメーション中:1
	*	@date	1/3	Ver 1.0
	*/
	int _Sprite3D::checkAnimationState( void)
	{
		if( numberOfRunningActions() == 0 )
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}

	/**
	*	3Dモデルのアニメーション情報解放
	*
	*	@author	minaka
	*	@date	1/3	Ver 1.0
	*/
	void _Sprite3D::releaseAnimation( void)
	{
		map< const string, string>().swap( modelAnimeList);
	}

	/**
	*	3Dモデルデータ用テクスチャ設定ファイルの読み込み
	*
	*	@author	minaka
	*	@param	fileName モデルデータ名
	*	@return	正常終了:0 エラー発生:-1
	*	@date	1/5	Ver 1.0
	*/
	int _Sprite3D::load3DModelTextureData( const string& fileName)
	{
		ifstream file( fileName, ios::in);
		if( file.fail())
		{
			return -1;
		}
		string str;
		textureData *data = new textureData;
		while( getline( file, str))
		{
			string tmp;
			istringstream stream(str);
			getline( stream, tmp, ',');
			data -> path = tmp;
			getline( stream, tmp);
			data -> name = tmp;
			modelTextureList.push_back(*data);
		}
		return 0;
	}

	/** /@/
	*	3Dモデルデータにテクスチャを設定
	*
	*	@author	minaka
	*	@date	1/5	Ver 1.0
	*/
	void _Sprite3D::setTextureList( void)
	{
		for( auto &data : modelTextureList)
		{
			string filePath = "Graph/Textures/" + data.path;
			auto mesh = getMeshByName( data.name);
			mesh -> setTexture( filePath);
		}
	}

	/**
	*	3Dモデルのテクスチャ情報解放
	*
	*	@author	minaka
	*	@date	1/3	Ver 1.0
	*/
	void _Sprite3D::releaseTexture( void)
	{
		vector<textureData>().swap( modelTextureList);
	}

	int _Sprite3D::setShaderFile( const string& fileName)
	{
		auto shader = new cocos2d::GLProgram();
		string filePath[2] = { "Shader/" + fileName + ".vsh", "Shader/" + fileName + ".fsh"};
		shader -> initWithFilenames( filePath[0], filePath[1]);
		shader -> bindAttribLocation( cocos2d::GLProgram::ATTRIBUTE_NAME_POSITION, cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
		shader -> bindAttribLocation( cocos2d::GLProgram::ATTRIBUTE_NAME_TEX_COORD, cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORDS);
		shader -> bindAttribLocation( cocos2d::GLProgram::ATTRIBUTE_NAME_COLOR, cocos2d::GLProgram::VERTEX_ATTRIB_COLOR);
		shader -> link();
		shader -> updateUniforms();
		this -> setShaderProgram( shader);
		return 0;
	}

	int _Sprite3D::setShaderFile( const string& vshFile, const string& fshFile)
	{
		auto shader = new cocos2d::GLProgram();
		string filePath[2] = { "Shader/" + vshFile + ".vsh", "Shader/" + fshFile + ".fsh"};
		shader -> initWithFilenames( filePath[0], filePath[1]);
		shader -> bindAttribLocation( cocos2d::GLProgram::ATTRIBUTE_NAME_POSITION, cocos2d::GLProgram::VERTEX_ATTRIB_POSITION);
		shader -> bindAttribLocation( cocos2d::GLProgram::ATTRIBUTE_NAME_TEX_COORD, cocos2d::GLProgram::VERTEX_ATTRIB_TEX_COORDS);
		shader -> bindAttribLocation( cocos2d::GLProgram::ATTRIBUTE_NAME_COLOR, cocos2d::GLProgram::VERTEX_ATTRIB_COLOR);
		shader -> link();
		shader -> updateUniforms();
		this -> setShaderProgram( shader);
		return 0;
	}
}