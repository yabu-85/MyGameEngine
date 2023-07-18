#pragma once
#include "GameObject.h"

class RootJob :
    public GameObject
{
public:
    RootJob(GameObject* parent); //��ԏ��nullptr�ɂ���A����Ȃ��Ƃǂ����I��肩�킩��Ȃ�main�ł���Ă�
    ~RootJob();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

