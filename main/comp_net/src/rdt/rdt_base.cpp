//
// Created by efarhan on 7/24/20.
//

#include <engine/engine.h>
#include "rdt/rdt_base.h"


namespace neko::rdt
{


void RdtManager::Send(std::string_view msg)
{
    channel_.receiver_.ClearMsg();
    channel_.sender_.Send(msg);
}



void RdtManager::Init()
{
    const auto& config = BasicEngine::GetInstance()->config;
    shapeShader_.LoadFromFile(config.dataRootPath+"shaders/comp_net/shape.vert",
                              config.dataRootPath+"shaders/comp_net/shape.frag");
    camera_.SetExtends(Vec2f(config.windowSize));
    camera_.position = Vec3f(0, 0, 1);
    camera_.WorldLookAt(Vec3f::zero);
	
	cube_.Init();
}

void RdtManager::Update(seconds dt)
{
    channel_.Update(dt);

}
void RdtManager::Render()
{
    const auto& config = BasicEngine::GetInstance()->config;
    const auto windowSize = Vec2f(config.windowSize);
    camera_.SetExtends(windowSize);
	//Render both
    shapeShader_.Bind();
    shapeShader_.SetMat4("view", camera_.GenerateViewMatrix());
    shapeShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());

    auto model = Mat4f::Identity;
    model = Transform3d::Scale(model, Vec3f::one * 200.0f);
    model = Transform3d::Translate(model, Vec3f(-windowSize.x * 3.0f/ 4.0f, 0, 0));
    shapeShader_.SetMat4("model", model);
    shapeShader_.SetVec3("ourColor", channel_.GetSender().IsComplete()?Vec3f(1,1,1): Vec3f(0.5f, 0.5f, 0.5f));
    cube_.Draw();

    model = Mat4f::Identity;

    model = Transform3d::Scale(model, Vec3f::one * 200.0f);
    model = Transform3d::Translate(model, Vec3f(windowSize.x * 3.0f / 4.0f, 0, 0));
    shapeShader_.SetMat4("model", model);
    shapeShader_.SetVec3("ourColor", channel_.GetReceiver().IsComplete() ? Vec3f(1, 1, 1) : Vec3f(0.5f, 0.5f, 0.5f));
    cube_.Draw();

	for(auto& packet : channel_.packets_)
	{
	    if(packet.currentDelay < 0.0f)
            continue;
        const auto length = 2.0f * Vec3f(windowSize.x * 3.0f / 4.0f, 0, 0);
        model = Mat4f::Identity;
        model = Transform3d::Scale(model, Vec3f::one * 100.0f);
	    const auto sign = packet.flags & Packet::ACK || packet.flags & Packet::NAK ? 1.0f : -1.0f;
        model = Transform3d::Translate(model, Vec3f(
            sign*windowSize.x * 3.0f / 4.0f, 0, 0)-sign*length*packet.currentDelay/packet.totalDelay);
        shapeShader_.SetMat4("model", model);
        if (packet.currentDelay > packet.totalDelay / 2.0f && packet.flags & Packet::CORRUPTED)
        {
            shapeShader_.SetVec3("ourColor", Vec3f(238.0f / 255.0f, 130.0f / 255.0f, 238.0f / 255.0f));
        }
		else if(packet.flags & Packet::NAK)
		{
            shapeShader_.SetVec3("ourColor", Vec3f(0.7f, 0.0f, 0.0f));
		}
        else if(packet.flags & Packet::ACK)
        {
            shapeShader_.SetVec3("ourColor", Vec3f(0.0f, 0.7f, 0.7f));
        }
        else
        {
        	
            shapeShader_.SetVec3("ourColor", Vec3f(0.0f, 0.7f, 0.0f));
        }
        cube_.Draw();
	}
	
}


void RdtManager::Destroy()
{
    channel_.receiver_.ClearMsg();
    shapeShader_.Destroy();
    cube_.Destroy();
}

void RdtManager::DrawImGui()
{

}

std::string_view GetDefaultString()
{
    static const auto txt = "C'est un trou de verdure ou chante une riviere\n"\
        "Accrochant follement aux herbes des haillons\n"\
        "D'argent ; ou le soleil, de la montagne fiere,\n"\
        "Luit : c'est un petit val qui mousse de rayons.\n"\
        "Un soldat jeune, bouche ouverte, tete nue,\n"\
        "Et la nuque baignant dans le frais cresson bleu,\n"\
        "Dort; il est etendu dans l'herbe sous la nue,\n"\
        "Pale dans son lit vert ou la lumiere pleut.\n"\
        "Les pieds dans les glaieuls, il dort.Souriant comme\n"\
        "Sourirait un enfant malade, il fait un somme : \n"\
        "Nature, berce - le chaudement : il a froid.\n"\
        "Les parfums ne font pas frissonner sa narine;\n"\
        "Il dort dans le soleil, la main sur sa poitrine\n"\
        "Tranquille.Il a deux trous rouges au cote droit.";
    return txt;
}
}
