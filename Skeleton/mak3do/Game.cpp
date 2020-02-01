#include "Game.h"
#include "types.h"
#include "Director.h"
#include "Scene.h"
#include "Node.h"
#include "actions/ActionInterval.h"

namespace mak3do {
void Game::startGame()
{
    auto director = Director::get();
    auto scene = std::make_shared<Scene>();
    auto node = std::make_shared<Node>();
    scene->addNode(node);
    director->setScene(scene);
    
    node->runAction(RepeatForever::make(RotateBy::make(2, 360)));
}
}
