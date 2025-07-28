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
}
