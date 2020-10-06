#pragma once
/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <engine/entity.h>
#include <engine/globals.h>
#include <utilities/vector_utility.h>
#include <utilities/json_utility.h>


namespace neko
{

enum class ComponentType : EntityMask
{
    EMPTY = 1u << 0u,
    POSITION2D = 1u << 1u,
    SCALE2D = 1u << 2u,
    ROTATION2D = 1u << 3u,
    TRANSFORM2D = ROTATION2D|SCALE2D|POSITION2D,
    POSITION3D = 1u << 4u,
    SCALE3D = 1u << 5u,
    ROTATION3D = 1u << 6u,
    TRANSFORM3D = POSITION3D|SCALE3D|ROTATION3D,
    SPRITE2D = 1u << 7u,
    BODY2D = 1u << 8u,
    BOX_COLLIDER2D = 1u << 9u,
    CIRCLE_COLLIDER2D = 1u << 10u,
    POLYGON_COLLIDER2D = 1u << 11u,
    CONVEX_SHAPE2D = 1u << 12u,
    PREFAB = 1u << 13u,
    OTHER_TYPE = 1u << 14u
};

struct Component
{
    Entity entity = INVALID_ENTITY;
    ComponentType componentType = ComponentType::EMPTY;
};

/**
 * \brief The Component Manager connects data to entity
 */

template<typename T, EntityMask componentType>
class ComponentManager
{
public:
    explicit ComponentManager(EntityManager& entityManager) :
        entityManager_(entityManager)
    {
		entityManager_.get().RegisterComponentManager(*this);
        ResizeIfNecessary(components_, INIT_ENTITY_NMB - 1, T{});
    }

    virtual ~ComponentManager()
    {};

    virtual Index AddComponent(Entity entity);

    virtual void DestroyComponent(Entity entity);

	virtual void SetComponent(Entity entity, const T& component);

    [[nodiscard]] const T& GetComponent(Entity entity) const
    { return components_[entity]; }


    [[nodiscard]] const T* GetComponentPtr(Entity entity) const
    { return &components_[entity]; }


    [[nodiscard]] const std::vector<T>& GetComponentsVector() const
    { return components_; }

    virtual void UpdateDirtyComponent([[maybe_unused]]Entity entity){};
protected:
    std::vector<T> components_;
    std::reference_wrapper<EntityManager> entityManager_;
};

template<typename T, EntityMask componentType>
Index ComponentManager<T, componentType>::AddComponent(Entity entity)
{
    ResizeIfNecessary(components_, entity, T{});
    entityManager_.get().AddComponentType(entity, componentType);
    return entity;
}

template<typename T, EntityMask componentType>
void ComponentManager<T, componentType>::DestroyComponent(Entity entity)
{
    entityManager_.get().RemoveComponentType(entity, static_cast<EntityMask>(componentType));
}

template <typename T, EntityMask componentType>
void ComponentManager<T, componentType>::SetComponent(Entity entity, const T& component)
{
	components_[entity] = component;
}


/**
 * \brief Sync Buffers is typically called on the render thread before the Main thread update and rendering
 */
class SyncBuffersInterface
{
public:
    virtual ~SyncBuffersInterface() = default;
    virtual void SyncBuffers() = 0;
};
/**
 * \brief Double Buffer Component Manager allows for component to be share between two threads by
 * using two buffers instead of one. It can be used for example for component that are modified by the
 * main thread and rendered by the render thread.
 */
template<typename  T, EntityMask  componentType>
class DoubleBufferComponentManager :
	public ComponentManager<T, componentType>,
    public SyncBuffersInterface
{
public:
	explicit DoubleBufferComponentManager(EntityManager& entityManager):
		ComponentManager<T, componentType>(entityManager)
	{
	}

	[[nodiscard]] const T& GetCurrentComponent(Entity entity) const
    {
        return currentComponents_[entity];
    }


    [[nodiscard]] const T* GetCurrentComponentPtr(Entity entity) const
    {
        return &currentComponents_[entity];
    }


    [[nodiscard]] const std::vector<T>& GetCurrentComponentsVector() const
    {
        return currentComponents_;
    }

	void SyncBuffers() override
	{
        const auto newSize = this->components_.size();
        ResizeIfNecessary(currentComponents_, newSize - 1, {});
		for(size_t i = 0; i < newSize; i++)
		{
			if(this->entityManager_.get().HasComponent(i, EntityMask(componentType)))
			{
                currentComponents_[i] = this->components_[i];
			}
		}
        std::swap(this->components_, currentComponents_);
	}
	
protected:
    std::vector<T> currentComponents_;
};
}
