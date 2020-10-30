#pragma once

#include <array>
#include <vector>
#include <string_view>
#include <functional>
#include <graphics/graphics.h>
#include <gl/shape.h>
#include <gl/shader.h>
#include "engine/system.h"
#include "graphics/camera.h"

namespace neko::rdt
{
struct Packet
{
    constexpr static size_t packetSize = 32;
    std::array<char, packetSize> data{};
    float currentDelay = 0.0f;
    float totalDelay = 0.0f;
    char checksum = 0;
    unsigned char sequenceNumber = 1;
    std::uint8_t flags = 0;
    enum Status : std::uint8_t
    {
        ACK = 1u,
        NAK = 1u << 1u,
    	CORRUPTED = 1u << 2u,
    	LOST = 1u << 3u,
    };
};
class Channel;
enum class ClientType
{
    SENDER,
	RECEIVER
};
template<ClientType type>
class Client
{
public:
	virtual ~Client() = default;
	explicit Client(Channel& channel) : channel_(channel){}
    virtual void Send(std::string_view msg) = 0;
    virtual void ReceiveRaw(const Packet& packet) = 0;
    [[nodiscard]] std::string_view GetReceivedMsg() const { return receivedMsg_; }
    virtual void ClearMsg() { receivedMsg_=""; }
    virtual void Update(seconds dt){}
    [[nodiscard]] virtual bool IsComplete() const = 0;
protected:
    virtual void SendRaw(const Packet& packet);
    Channel& channel_;
    std::string receivedMsg_;
    const ClientType clientType_ = type;
};

class Channel
{
public:
    explicit Channel(Client<ClientType::SENDER>& sender, Client<ClientType::RECEIVER>& receiver):sender_(sender), receiver_(receiver){}
    virtual void SendRaw(const Packet& packet) = 0;
    virtual void Update(seconds dt) = 0;
    const float baseDelay = 1.0f;
    const float pktDelay = 0.1f;
    [[nodiscard]] const std::vector<Packet>& GetPackets() const { return packets_; }
    [[nodiscard]] const Client<ClientType::SENDER>& GetSender() const { return sender_; }
    [[nodiscard]] const Client<ClientType::RECEIVER>& GetReceiver() const { return receiver_; }
protected:
    friend class RdtManager;
    Client<ClientType::SENDER>& sender_;
    Client<ClientType::RECEIVER>& receiver_;
    std::vector<Packet> packets_;
};

template<ClientType type>
void Client<type>::SendRaw(const Packet& packet)
{
    channel_.SendRaw(packet);
}

class RdtManager : public RenderProgram, public DrawImGuiInterface
{
public:
    explicit RdtManager(Channel& channel) : channel_(channel){}

    void Send(std::string_view msg);
    void Render() override;
    [[nodiscard]] std::string_view GetReceivedMsg() { return channel_.receiver_.GetReceivedMsg(); }

    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

private:
    Channel& channel_;
    gl::RenderQuad cube_{Vec3f::zero, Vec2f::one};
    gl::Shader shapeShader_;

    Camera2D camera_;

};

std::string_view GetDefaultString();
}
