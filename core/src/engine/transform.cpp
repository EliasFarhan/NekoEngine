/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

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
#include <engine/transform.h>
#include "engine/globals.h"


namespace neko
{

Index Scale2dManager::AddComponent(Entity entity)
{
	ResizeIfNecessary(components_, entity, Vec2f::one);
	return ComponentManager::AddComponent(entity);
}

void Transform3dManager::UpdateTransform(Entity entity)
{
    Mat4f transform = Mat4f::Identity;
    transform = Mat4f::Translate(transform, position3DManager_.GetComponent(entity));
    transform = Mat4f::Scale(transform, scale3DManager_.GetComponent(entity));
    transform = Mat4f::Rotate(transform, rotation3DManager_.GetComponent(entity));

}
}
