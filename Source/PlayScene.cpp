#include "PlayScene.h"
#include <DxLib.h>
#include "Stage.h"
#include "MapTip.h"
#include "MapEdit.h"
#include "Input.h"

PlayScene::PlayScene()
{
	new Stage
	{
		new MapChip{},
		new MapEdit{},
	};
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update()
{
	Input::Instance()->Update();

	if (CheckHitKey(KEY_INPUT_T)) {
		SceneManager::ChangeScene("TITLE");
	}
}

void PlayScene::Draw()
{
	DrawString(0, 0, "PLAY SCENE", GetColor(255, 255, 255));
	DrawString(100, 400, "Push [T]Key To Title", GetColor(255, 255, 255));
}
