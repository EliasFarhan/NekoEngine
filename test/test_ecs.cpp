/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

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

#include <gtest/gtest.h>
#include <engine/entity.h>

TEST(Entity, AddEntity)
{
    neko::EntityManager entityManager;
    const Index entityNmb = 1024u;

    for (Index i = 0; i < entityNmb; i++)
    {
        auto entity = entityManager.CreateEntity();
        EXPECT_EQ(entity, i + 1);
        entityManager.AddComponentType(entity, 1);
        EXPECT_TRUE(entityManager.HasComponent(entity, 1));
        entityManager.AddComponentType(entity, 1u << 1u);
        EXPECT_TRUE(entityManager.HasComponent(entity, 1));
        EXPECT_TRUE(entityManager.HasComponent(entity, 1u << 1u));
        entityManager.RemoveComponentType(entity, 1);
        EXPECT_FALSE(entityManager.HasComponent(entity, 1));
        EXPECT_TRUE(entityManager.HasComponent(entity, 1u << 1u));
    }
    EXPECT_EQ(entityManager.GetEntitiesNmb(), entityNmb);

    for (Index i = 0; i < entityNmb / 2; i++)
    {
        entityManager.DestroyEntity(i * 2 + 1u);
    }
    EXPECT_EQ(entityManager.GetEntitiesNmb(), entityNmb / 2);
}

