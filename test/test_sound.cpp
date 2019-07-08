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
#include <engine/engine.h>
#include <engine/log.h>
#include <engine/globals.h>
#include <utilities/time_utility.h>
#include <sound/sound.h>

class SoundTest : public ::testing::Test, public neko::BasicEngine
{
protected:
	void SetUp() override
	{
		Init();
	}
	void TearDown() override
	{
		Destroy();
	}
};

TEST_F(SoundTest, LoadAnOggFileMusic)
{
	neko::Index index = neko::Sound::LoadMusic("data/test.ogg");
	EXPECT_NE(index, neko::INDEX_INVALID);
	EXPECT_TRUE(neko::Sound::RemoveMusic(index));
}

TEST_F(SoundTest, LoadAndPlayMusic)
{
	neko::Index index = neko::Sound::LoadMusic("data/test.ogg");
	EXPECT_NE(index, neko::INDEX_INVALID);
	EXPECT_TRUE(neko::Sound::PlayMusic(index));
	neko::SleepMs(1000);
	EXPECT_TRUE(neko::Sound::RemoveMusic(index));
}

TEST_F(SoundTest, LoadASoundBuffer)
{
	neko::Index index = neko::Sound::LoadSoundBuffer("data/test.ogg");
	EXPECT_NE(index, neko::INDEX_INVALID);
	EXPECT_TRUE(neko::Sound::RemoveSoundBuffer(index));
}

TEST_F(SoundTest, LoadASoundFromASoundBuffer)
{
	neko::Index index = neko::Sound::LoadSoundBuffer("data/test.ogg");
	EXPECT_NE(index, neko::INDEX_INVALID);
	neko::Index soundIndex = neko::Sound::CreateSoundFromBuffer(index);
	EXPECT_NE(soundIndex, neko::INDEX_INVALID);
	EXPECT_TRUE(neko::Sound::RemoveSoundBuffer(index));
}

TEST_F(SoundTest, LoadASoundAndPlaySound)
{
	neko::Index index = neko::Sound::LoadSoundBuffer("data/test.ogg");
	EXPECT_NE(index, neko::INDEX_INVALID);
	neko::Index soundIndex = neko::Sound::CreateSoundFromBuffer(index);
	EXPECT_NE(soundIndex, neko::INDEX_INVALID);
	EXPECT_TRUE(neko::Sound::PlaySound(soundIndex));
	neko::SleepMs(1000);
	EXPECT_TRUE(neko::Sound::RemoveSoundBuffer(index));
}