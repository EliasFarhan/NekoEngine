#include "95_hello_2dgame/game2d_program.h"

namespace neko
{


Hello2dGameProgram::Hello2dGameProgram() :
  transform2dManager_(entityManager_),
  spriteManager_(entityManager_, textureManager_, transform2dManager_)
{
}

void Hello2dGameProgram::Init() {
}

void Hello2dGameProgram::Update(seconds dt) {
}

void Hello2dGameProgram::Destroy() {
}

void Hello2dGameProgram::DrawImGui() {
}

void Hello2dGameProgram::Render() {
}

void Hello2dGameProgram::OnEvent(const SDL_Event &event) {
}
}
