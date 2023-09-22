#pragma once

#include "Audio.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"Vector3.h"

// ctl+K+Oでcpp展開
/// <summary>
/// ゲームシーン
/// </summary>


class GameScene 
{
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	
	GameScene();
	// 音声
	void PlayerUpdate();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	void BeamUpdate();
	// ビームの移動
	void BeamMove();
	// ビーム発生
	void BeamBorn();
	// enemy
	void EnemyUpdate();
	// エネミーの移動
	void EnemyMove();
	// enemyboon
	void EnemyBorn();
	float VectorSize(float x, float y, float v, float w);
	Vector2 Nomalize(float x, float y,float w);
	/// <summary>
	//
	
	/// </summary>
	/// <returns></returns>
	/// /// bom
	void BomMove();
	void BomBorn();
	void ParticleInitilize(float x, float y);
	void ParticleUpdate(float  x,float y,float z);
	// 衝突判定
	void Collision();
	// 衝突判定(プレイヤーと敵）
	void CollisionPlayerEnemy();
	// 衝突判定（ビームと敵）
	void CollisionBeamEnemy();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	///
	// BG(スプライト)
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;
	// ビュープロジェクション（共通）
	ViewProjection viewProjection_;
	// ステージ
	uint32_t textureHandleStage_ = 0;
	Model* modelStage_ = nullptr;
	WorldTransform worldTransformStage_;
	// プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;
	// ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_/*[10]*/;
	//弾の半径
	const float BeamR_ = 0.25;
	int beamFlag_/*[10]*/;
	int beamTimer_ = 0; // ビーム発射タイマー
	// エネミー
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_[10];
	int EnemyFlag_[10] = {};
	float enemySpeed_[10] = {}; // 敵のスピード
	//ボンバー
	uint32_t textureHandleBom_ = 0;
	uint32_t textureHandlekakunin_ = 0;
	Model* modelBom_ = nullptr;
	Model* modelkakunin = nullptr;
	WorldTransform worldTransformBom_[4];
	bool bomFlag_[4] = {false};
	float BomR_ = 0.125;
	int bomTimer_ = 0;
	Vector2 touch;
	float Power[4];
	float tmpY=0.0f;

	
	// タイトル(スプライト)
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = 0;
	// エンターキー
	uint32_t textureHandleEnter_ = 0;
	Sprite* spriteEnter_ = 0;
	// ゲームオーバー
	uint32_t textureHandleGameOver_ = 0;
	Sprite* spriteGameOver_ = 0;
	DebugText* debugText_ = nullptr;
	// サウンド
	uint32_t soundDataHandleTitleBGM_ = 0;        // タイトルBGM
	uint32_t soundDataHandleGamePlayBGM_ = 0;     // ゲームプレイBGM
	uint32_t soundDataHandleGameOverBGM_ = 0;     // ゲームオーバーBGM
	uint32_t soundDataHandleGameEnemyHitSE_ = 0;  // テキヒットSE
	uint32_t soundDataHandleGamePlayerHitSE_ = 0; // プレイヤーひっとSE
	uint32_t voiceHandleBGM_ = 0;                 // 音声再生ハンドル
	int gameScore_ = 0;
	int playerLife_ = 3;
	int gameTimer_ = 0;
	int sceneMode_ = 1; // シーンモード(0:ゲームプレイ　１：タイトル)
	Vector2 start;
	Vector2 end ;
	Vector2 components;

	void GamePlayUpdate();
	void GamePlayDraw3D();
	void GamePlayDraw2DBack();
	void GamePlayDraw2DNear();
	void _TitleUpdate();     // タイトル更新
	void _TitleDraw2DNear(); // タイトル２D
	void _GameOverDraw2DNear();
	void _GameOverUpdate();
	void _GamePlayerStart();
};