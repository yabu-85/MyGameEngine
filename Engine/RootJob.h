#pragma once
#include "GameObject.h"

class RootJob :
    public GameObject
{
public:
    RootJob(GameObject* parent); //ˆê”Ôã‚Ínullptr‚É‚·‚éA‚¶‚á‚È‚¢‚Æ‚Ç‚±‚ªI‚í‚è‚©‚í‚©‚ç‚È‚¢main‚Å‚â‚Á‚Ä‚é
    ~RootJob();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

