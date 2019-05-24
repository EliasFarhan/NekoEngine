#pragma once
namespace neko
{

class System
{
public:
    System() = default;

    virtual ~System() = default;

    System &operator=(const System &) = delete;

    System(System &&) = default; //move constructor
    System(const System &) = delete; //delete copy constructor

    virtual void Init() = 0;

    virtual void Update() = 0;

	virtual void Destroy() = 0;


};

}