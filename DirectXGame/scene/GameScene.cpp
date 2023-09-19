#include "GameScene.h"
#include "ImGuiManager.h"
#include "MathUtilityForText.h"
#include "TextureManager.h"
#include <cassert>
#include <time.h>
#include <math.h>
#include"Vector3.h"
#include"Vector2.h"

GameScene::GameScene(){};

GameScene::~GameScene() {
	delete spriteBG_;    // BG
	delete modelStage_;  // ステージ
	delete modelPlayer_; // プレイヤー
	delete modelBeam_;   // ビーム
	delete modelEnemy_;  // エネミー
	delete spriteTitle_; // タイトル
	delete spriteEnter_; // エンター
}
// 初期化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0.0});

	// ビュープロジェクションの初期化
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelStage_ = Model::Create();
	worldTransformStage_.Initialize();

	// ステージの位置を変更
	// プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	// ステージ
	worldTransformStage_.translation_ = {0, -1.5f, 0};
	worldTransformStage_.scale_ = {4.5f, 1, 40};

	// ビーム
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	/*for (int i = 0; i < 10; i++) {*/

		worldTransformBeam_/*[i]*/.scale_ = {0.5f, 0.5f, 0.5f};
		worldTransformBeam_/*[i]*/.Initialize();
	/*}*/
	// エネミー
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	// 敵の数をループする
	for (int i = 0; i < 10; i++) {
		worldTransformEnemy_[i].scale_ = {0.5f, 0.5f, 0.5f};
		worldTransformEnemy_[i].Initialize();
	}
	
	srand((unsigned int)time(NULL));
	//bom
	textureHandleBom_ = TextureManager::Load("bom.png");
	modelBom_ = Model::Create();
	for (int i = 0; i < 4; i++) {

		worldTransformBom_[i].scale_ = {0.5f, 0.5f, 0.5f};
		worldTransformBom_[i].Initialize();
	}
	// タイトル（スプライト）
	textureHandleTitle_ = TextureManager::Load("title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});
	// エンターキー
	textureHandleEnter_ = TextureManager::Load("enter.png");
	spriteEnter_ = Sprite::Create(textureHandleEnter_, {400, 400});
	// gameOver画面
	textureHandleGameOver_ = TextureManager::Load("gameover.png");
	spriteGameOver_ = Sprite::Create(textureHandleGameOver_, {0, 0});
	// 変更行列を更新
	worldTransformStage_.matWorld_ = MakeAffineMatrix(
	    worldTransformStage_.scale_, worldTransformStage_.rotation_,
	    worldTransformStage_.translation_);
	// 変換行列を定数バッファに転送
	worldTransformStage_.TransferMatrix();
	// デバッグテキスト
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();
	// サウンドデータの読み込み
	soundDataHandleTitleBGM_ = audio_->LoadWave("Audio/Ring05.wav");
	soundDataHandleGamePlayBGM_ = audio_->LoadWave("Audio/Ring08.wav");
	soundDataHandleGameOverBGM_ = audio_->LoadWave("Audio/Ring09.wav");
	soundDataHandleGameEnemyHitSE_ = audio_->LoadWave("Audio/chord.wav");
	soundDataHandleGamePlayerHitSE_ = audio_->LoadWave("Audio/tada.wav");
	// タイトルBGM再生
	voiceHandleBGM_ = audio_->PlayWave(soundDataHandleTitleBGM_, true);
}

void GameScene::Update() {
	// 名シーンの更新処理を呼び出す
	switch (sceneMode_) {
	case 0:
		GamePlayUpdate(); // ゲームプレイ
		break;
	case 1:
		_TitleUpdate(); // タイトル更新
		gameTimer_++;
		break;
	case 2:
		_GameOverUpdate(); // ゲームオーバー画面
		gameTimer_++;
		break;
	}
}

void GameScene::BeamUpdate() {
	BeamMove();
	BeamBorn();
	/*for (int i = 0; i < 10; i++) {*/

		worldTransformBeam_/*[i]*/.matWorld_ = MakeAffineMatrix(
		    worldTransformBeam_/*[i]*/.scale_, worldTransformBeam_/*[i]*/.rotation_,
		    worldTransformBeam_/*[i]*/.translation_);
		// 変換行列を定数バッファに転送

		worldTransformBeam_/*[i]*/.TransferMatrix();
	//}
}

void GameScene::BeamMove() {
	/*for (int a = 0; a < 10; a++) {*/

		if (beamFlag_/*[a]*/ == 1) {
			worldTransformBeam_/*[a]*/.translation_.z += 0.5f;
			worldTransformBeam_/*[a]*/.rotation_.x += 0.1f;
		}
		if (worldTransformBeam_/*[a]*/.translation_.z > 40) {
			beamFlag_/*[a]*/ = 0;
		}
	//}
}

void GameScene::BeamBorn() {
	for (int a = 0; a < 10; a++) {
		if (beamTimer_ == 0) {
			if (input_->TriggerKey(DIK_SPACE) && !beamFlag_/*[a]*/) {

				worldTransformBeam_/*[a]*/.translation_.z = worldTransformPlayer_.translation_.z;
				worldTransformBeam_/*[a]*/.translation_.x = worldTransformPlayer_.translation_.x;
				beamFlag_/*[a]*/ = 1;
				break;
			}

		} else {
			beamTimer_++;
			if (beamTimer_ > 10) {
				beamTimer_ = 0;
			}
		}
	}
}

void GameScene::EnemyUpdate() {
	EnemyMove();
	EnemyBorn();
	// 変換行列を更新
	for (int e = 0; e < 10; e++) {

		worldTransformEnemy_[e].matWorld_ = MakeAffineMatrix(
		    worldTransformEnemy_[e].scale_, worldTransformEnemy_[e].rotation_,
		    worldTransformEnemy_[e].translation_);

		// 変換行列を定数バッファに転送
		worldTransformEnemy_[e].TransferMatrix();
	}
}

void GameScene::EnemyMove() {
	for (int e = 0; e < 10; e++) {
		worldTransformEnemy_[e].translation_.z -= 0.1f;
		worldTransformEnemy_[e].rotation_.x -= 0.1f;
		worldTransformEnemy_[e].translation_.x -= enemySpeed_[e];
		if (worldTransformEnemy_[e].translation_.x > 4.0) {
			enemySpeed_[e] *= -1;
		} else if (worldTransformEnemy_[e].translation_.x < -4.0) {
			enemySpeed_[e] *= -1;
		}
		if (worldTransformEnemy_[e].translation_.z <= -5) {
			EnemyFlag_[e] = 0;
		}
	}
}

void GameScene::EnemyBorn() {
	{
		// 乱数で発生
		if (rand() % 10 == 0) {
			// 敵の数ループする
			for (int e = 0; e < 10; e++) {
				// 存在しなければ
				if (EnemyFlag_[e] == 0) {
					EnemyFlag_[e] = 1;
					worldTransformEnemy_[e].translation_.z = 40.0f;
					int x = rand() % 80;
					float x2 = (float)x / 10 - 4;
					worldTransformEnemy_[e].translation_.x = x2;
					// ループ終了
					// 敵スピード
					if (rand() % 2 == 0) {
						enemySpeed_[e] = 0.1f;
					} else {
						enemySpeed_[e] = -0.1f;
					}
					break;
				};
			}
		}
	}
}
//Vectorの長さ
float VectorSize(float x, float y,float v,float w ) {


	Vector2 VectorSize;
	VectorSize.x = x - v;
	VectorSize.y = y - w;
	float VectorSizeAns = (float)sqrt(VectorSize.x * VectorSize.x + VectorSize.y * VectorSize.y);
	return VectorSizeAns;
}
//正規化　　　VectorSizeをwに入れる
Vector2 GameScene::Nomalize(float x, float y, float w, float answerX,float answerY)
{ 
	
//正規化
	answerX = x / w;
	answerY = y / w;

	return Vector2{answerX, answerY};
}

//pos = Normalize(pos, pos2);



//void GameScene::BomBorn() {
//	for (int a = 0; a < 40; a++) {
//		
//			if (beamFlag_[a]==false) {
//				worldTransformBom_[a].translation_.z;
//				worldTransformBom_[a].translation_.x;
//				beamFlag_[a] = 1;
//				break;
//			}
//
//		
//	}
//}

void GameScene::BomParticle() {

}

void GameScene::Collision() {

	// 衝突判定（プレイヤーと敵）
	CollsionPlayerEnemy();
	// 衝突判定（弾と敵）
	CollisionBeamEnemy();
}

void GameScene::CollsionPlayerEnemy() {
	for (int e = 0; e < 10; e++) {
		// 敵が存在すれば
		if (EnemyFlag_[e] == 1) {
			// 差を求める
			float dx =
			    abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_[e].translation_.x);
			float dz =
			    abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_[e].translation_.z);
			// 衝突したら
			if (dx < 1 && dz < 1) {
				// 存在しない
				EnemyFlag_[e] = 0;
				playerLife_ -= 1;
			}
			if (playerLife_ == 0) {
				// モードをゲームプレイへ変更
				sceneMode_ = 2;
			}
		}
	}
}

void GameScene::CollisionBeamEnemy() {
	// 敵と弾の存在
	for (int e = 0; e < 10; e++) {
		//for (int i = 0; i < 10; i++) {

			if (EnemyFlag_[e] == 1 && beamFlag_/*[i]*/ == 1) {
				float gx = abs(
				    worldTransformBeam_/*[i]*/.translation_.x - worldTransformEnemy_[e].translation_.x);
				float gz = abs(
				    worldTransformBeam_/*[i]*/.translation_.z - worldTransformEnemy_[e].translation_.z);

				if (gx < 1 && gz < 1) {
					EnemyFlag_[e] = 0;
					beamFlag_/*[i]*/ = 0;
					gameScore_ += 100;
					//接触点作成
				    float Length=VectorSize(worldTransformBeam_.translation_.x,);
				    Vector2 dir=Nomalize();
				}
			}
		//}
	}
}

void GameScene::PlayerUpdate() {
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransformPlayer_.translation_.x += 0.1f;
	}
	if (input_->PushKey(DIK_LEFT)) {
		worldTransformPlayer_.translation_.x -= 0.1f;
	}
	if (worldTransformPlayer_.translation_.x < -4) {
		worldTransformPlayer_.translation_.x = -4;
	}
	if (worldTransformPlayer_.translation_.x > 4) {
		worldTransformPlayer_.translation_.x = 4;
	}
	if (input_->PushKey(DIK_UP)) {
		worldTransformPlayer_.translation_.y += 0.1f;
	}
	if (input_->PushKey(DIK_DOWN)) {
		worldTransformPlayer_.translation_.y -= 0.1f;
	}
	if (worldTransformPlayer_.translation_.y < -4) {
		worldTransformPlayer_.translation_.y = -4;
	}
	if (worldTransformPlayer_.translation_.y > 4) {
		worldTransformPlayer_.translation_.y = 4;
	}
	// 変換行列を更新
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);

	// 変換行列を定数バッファに転送
	worldTransformPlayer_.TransferMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	// 各シーンの背景２D表示を呼び出す
	switch (sceneMode_) {
	case 0:

		GamePlayDraw2DBack();
		break;
	case 2:
		GamePlayDraw2DBack();
		break;
	}

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる

	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	// 各シーンの背景3D表示を呼び出す
	switch (sceneMode_) {

	case 0:
		GamePlayDraw3D(); // ゲームプレイ3D背景表示
		break;
	case 2:
		GamePlayDraw3D();
		break;
	}

	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	debugText_->DrawAll();

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	switch (sceneMode_) {
	case 0:
		GamePlayDraw2DNear(); // ゲームプレイ２D背景表示
		break;
	case 1:
		_TitleDraw2DNear(); // タイトル2D表示
		break;
	case 2:
		GamePlayDraw2DNear();
		_GameOverDraw2DNear();
		break;
	}
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GamePlayUpdate() {
	PlayerUpdate();
	BeamUpdate();
	EnemyUpdate();
	Collision();
}

void GameScene::GamePlayDraw3D() {
	// ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);
	// プレイヤー
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
	// ビーム
	//for (int e = 0; e < 10; e++) {
		if (beamFlag_/*[e]*/ == 1) {
			modelBeam_->Draw(worldTransformBeam_/*[e]*/, viewProjection_, textureHandleBeam_);
		}
	//}
	for (int e = 0; e < 10; e++) {

		// 敵
		if (!(!EnemyFlag_[e])) {
			modelEnemy_->Draw(worldTransformEnemy_[e], viewProjection_, textureHandleEnemy_);
		}
	}
}

void GameScene::GamePlayDraw2DBack() {
	// 背景
	spriteBG_->Draw();
}

void GameScene::GamePlayDraw2DNear() {
	// ゲームスコア
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);
	// プレイヤーライフ
	sprintf_s(str, "LIFE %d", playerLife_);
	debugText_->Print(str, 900, 10, 2);
}

void GameScene::_TitleUpdate() {
	// エンターキーを押した瞬間
	if (input_->TriggerKey(DIK_RETURN)) {
		// モードをゲームプレイへ変更
		sceneMode_ = 0;
		_GamePlayerStart();
	}
}

void GameScene::_TitleDraw2DNear() {
	// タイトル表示
	spriteTitle_->Draw();
	// エンター表示
	if (gameTimer_ % 40 >= 20) {
		spriteEnter_->Draw();
	}
}

void GameScene::_GameOverDraw2DNear() {
	// ゲームオーバー
	spriteGameOver_->Draw();

	// ヒットエンターキー
	if (gameTimer_ % 40 >= 20) {
		spriteEnter_->Draw();
	}
}

void GameScene::_GameOverUpdate() {
	if (input_->TriggerKey(DIK_RETURN)) {
		// モードをゲームプレイへ変更
		sceneMode_ = 1;
	}
}

void GameScene::_GamePlayerStart() {
	gameScore_ = 0;
	playerLife_ = 3;
	for (int r = 0; r < 10; r++) {
		EnemyFlag_[r] = 0;
	}
	//for (int e = 0; e < 10; e++) {
		beamFlag_/*[e]*/ = 0;
	//}
	playerLife_ = 3;
	worldTransformPlayer_.translation_.x = 0;
	worldTransformPlayer_.translation_.y = 0;
	worldTransformPlayer_.translation_.z = 0;
}