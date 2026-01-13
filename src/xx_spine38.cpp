#pragma once
#include "xx_spine38.h"
#include "xx_gamebase.h"

namespace xx {

	// ref: spine-sfml.cpp

	SpinePlayer::SpinePlayer(spine::SkeletonData* skeletonData)
		: animationStateData(skeletonData)
		, skeleton(skeletonData)
		, animationState(&animationStateData)
	{
		worldVertices.ensureCapacity(1000);
		tempUvs.ensureCapacity(16);
		tempColors.ensureCapacity(16);

		quadIndices.add(0);
		quadIndices.add(1);
		quadIndices.add(2);
		quadIndices.add(2);
		quadIndices.add(3);
		quadIndices.add(0);
	}

	SpinePlayer& SpinePlayer::Update(float delta) {
		skeleton.update(delta * timeScale);
		animationState.update(delta * timeScale);
		animationState.apply(skeleton);
		skeleton.updateWorldTransform();
		return *this;
	}

	void SpinePlayer::Draw() {
		auto eg = GameBase::instance;

		// Early out if skeleton is invisible
		if (skeleton.getColor().a == 0) return;

		// if (vertexEffect != NULL)

		RGBA8 color{};
		GLTexture* texture{};
		for (size_t i = 0, e = skeleton.getSlots().size(); i < e; ++i) {
			auto&& slot = *skeleton.getDrawOrder()[i];
			auto&& attachment = slot.getAttachment();
			if (!attachment) {
				clipper.clipEnd(slot);
				continue;
			}
			auto&& attachmentRTTI = attachment->getRTTI();

			// Early out if the slot color is 0 or the bone is not active
			if ((slot.getColor().a == 0 || !slot.getBone().isActive()) && !attachmentRTTI.isExactly(spine::ClippingAttachment::rtti)) {
				clipper.clipEnd(slot);
				continue;
			}

			spine::Vector<float>* vertices = &worldVertices;
			size_t verticesCount{};
			spine::Vector<float>* uvs{};
			spine::Vector<unsigned short>* indices{};
			size_t indicesCount{};
			spine::Color* attachmentColor{};

			if (attachmentRTTI.isExactly(spine::RegionAttachment::rtti)) {
				auto&& regionAttachment = (spine::RegionAttachment*)attachment;
				attachmentColor = &regionAttachment->getColor();

				// Early out if the slot color is 0
				if (attachmentColor->a == 0) {
					clipper.clipEnd(slot);
					continue;
				}

				worldVertices.setSize(8, 0);
				regionAttachment->computeWorldVertices(slot.getBone(), worldVertices, 0, 2);
				verticesCount = 4;
				uvs = &regionAttachment->getUVs();
				indices = &quadIndices;
				indicesCount = 6;
				texture = (GLTexture*)((spine::AtlasRegion*)regionAttachment->getRendererObject())->page->getRendererObject();
			}
			else if (attachmentRTTI.isExactly(spine::MeshAttachment::rtti)) {
				auto&& mesh = (spine::MeshAttachment*)attachment;
				attachmentColor = &mesh->getColor();

				// Early out if the slot color is 0
				if (attachmentColor->a == 0) {
					clipper.clipEnd(slot);
					continue;
				}

				worldVertices.setSize(mesh->getWorldVerticesLength(), 0);
				texture = (GLTexture*)((spine::AtlasRegion*)mesh->getRendererObject())->page->getRendererObject();
				mesh->computeWorldVertices(slot, 0, mesh->getWorldVerticesLength(), worldVertices.buffer(), 0, 2);
				verticesCount = mesh->getWorldVerticesLength() >> 1;
				uvs = &mesh->getUVs();
				indices = &mesh->getTriangles();
				indicesCount = mesh->getTriangles().size();
			}
			else if (attachmentRTTI.isExactly(spine::ClippingAttachment::rtti)) {
				spine::ClippingAttachment* clip = (spine::ClippingAttachment*)slot.getAttachment();
				clipper.clipStart(slot, clip);
				continue;
			}
			else continue;

			auto&& skc = skeleton.getColor();
			auto&& slc = slot.getColor();
			color.r = (uint8_t)(skc.r * slc.r * attachmentColor->r * 255);
			color.g = (uint8_t)(skc.g * slc.g * attachmentColor->g * 255);
			color.b = (uint8_t)(skc.b * slc.b * attachmentColor->b * 255);
			color.a = (uint8_t)(skc.a * slc.a * attachmentColor->a * 255);

			// normal, additive, multiply, screen
			static const constexpr std::array<std::pair<uint32_t, uint32_t>, 4> nonpmaBlendFuncs{ std::pair<uint32_t, uint32_t>
			{ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA }, { GL_SRC_ALPHA, GL_ONE }, { GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA }, { GL_ONE, GL_ONE_MINUS_SRC_COLOR } };

			// pma: normal, additive, multiply, screen
			static const constexpr std::array<std::pair<uint32_t, uint32_t>, 4> pmaBlendFuncs{ std::pair<uint32_t, uint32_t>
			{ GL_ONE, GL_ONE_MINUS_SRC_ALPHA }, { GL_ONE, GL_ONE }, { GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA }, { GL_ONE, GL_ONE_MINUS_SRC_COLOR } };

			std::pair<uint32_t, uint32_t> blend;
			auto bm = slot.getData().getBlendMode();
			if (!usePremultipliedAlpha) {
				blend = nonpmaBlendFuncs[bm];
			}
			else {
				blend = pmaBlendFuncs[bm];
			}
			eg->GLBlendFunc({ blend.first, blend.second, GL_FUNC_ADD });


			if (clipper.isClipping()) {
				clipper.clipTriangles(worldVertices, *indices, *uvs, 2);
				vertices = &clipper.getClippedVertices();
				verticesCount = clipper.getClippedVertices().size() >> 1;
				uvs = &clipper.getClippedUVs();
				indices = &clipper.getClippedTriangles();
				indicesCount = clipper.getClippedTriangles().size();
			}

			// if (vertexEffect != NULL) else {
			auto vs = eg->Spine().Alloc(*texture, (int32_t)indicesCount);
			for (size_t ii = 0; ii < indicesCount; ++ii) {
				auto index = (*indices)[ii] << 1;
				auto&& v = vs[ii];
				v.pos.x = (*vertices)[index];
				v.pos.y = (*vertices)[index + 1];
				v.uv.x = (*uvs)[index] * texture->size.x;
				v.uv.y = (*uvs)[index + 1] * texture->size.y;
				(uint32_t&)v.color = (uint32_t&)color;
			}
			clipper.clipEnd(slot);
		}
		clipper.clipEnd();

		// if (vertexEffect != NULL)
	}

	spine::Vector<spine::Animation*>& SpinePlayer::GetAnimations() {
		return animationStateData.getSkeletonData()->getAnimations();
	}

	spine::Animation* SpinePlayer::FindAnimation(std::string_view name) {
		return animationStateData.getSkeletonData()->findAnimation(name);
	}

	spine::Bone* SpinePlayer::FindBone(std::string_view name) {
		return skeleton.findBone(name);
	}

	spine::Slot* SpinePlayer::FindSlot(std::string_view name) {
		return skeleton.findSlot(name);
	}

	SpinePlayer& SpinePlayer::SetMix(spine::Animation* from, spine::Animation* to, float duration) {
		animationStateData.setMix(from, to, duration);
		return *this;
	}

	SpinePlayer& SpinePlayer::SetMix(std::string_view fromName, std::string_view toName, float duration) {
		animationStateData.setMix(fromName, toName, duration);
		return *this;
	}

	SpinePlayer& SpinePlayer::SetTimeScale(float t) {
		timeScale = t;
		return *this;
	}

	SpinePlayer& SpinePlayer::SetUsePremultipliedAlpha(bool b) {
		usePremultipliedAlpha = b;
		return *this;
	}

	SpinePlayer& SpinePlayer::SetPosition(XY const& xy) {
		return SetPosition(xy.x, xy.y);
	}

	SpinePlayer& SpinePlayer::SetScale(XY const& xy) {
		skeleton.setScaleX(xy.x);
		skeleton.setScaleY(xy.y);
		return *this;
	}

	SpinePlayer& SpinePlayer::SetFirstRotation(float r) {
		skeleton.getBones()[0]->setRotation(r);
		return *this;
	}

	SpinePlayer& SpinePlayer::SetFirstScale(XY const& scale) {
		auto& b = skeleton.getBones()[0];
		b->setScaleX(scale.x);
		b->setScaleY(scale.y);
		return *this;
	}

	SpinePlayer& SpinePlayer::SetPosition(float x, float y) {
		skeleton.setPosition(x, y);
		return *this;
	}

	spine::TrackEntry* SpinePlayer::SetAnimation(size_t trackIndex, std::string_view animationName, bool loop) {
		return animationState.setAnimation(trackIndex, animationName, loop);
	}

	spine::TrackEntry* SpinePlayer::AddAnimation(size_t trackIndex, std::string_view animationName, bool loop, float delay) {
		return animationState.addAnimation(trackIndex, animationName, loop, delay);
	}

	spine::TrackEntry* SpinePlayer::SetAnimation(size_t trackIndex, spine::Animation* anim, bool loop) {
		return animationState.setAnimation(trackIndex, anim, loop);
	}

	spine::TrackEntry* SpinePlayer::AddAnimation(size_t trackIndex, spine::Animation* anim, bool loop, float delay) {
		return animationState.addAnimation(trackIndex, anim, loop, delay);
	}

	// todo: multi anim pack to 1 tex
	GLVertTexture SpinePlayer::AnimToTexture(std::string_view animName, float frameDelay) {
		return AnimToTexture(animationStateData.getSkeletonData()->findAnimation(animName), frameDelay);
	}

	GLVertTexture SpinePlayer::AnimToTexture(spine::Animation* anim, float frameDelay) {
		auto eg = GameBase::instance;
		auto bak = eg->blend;
		SetPosition(0, 0).SetAnimation(0, anim, true);
		eg->ShaderEnd();
		Update(frameDelay);
		Draw();	// draw once for get vert size
		auto&& shader = eg->Spine();
		auto vc = shader.count;

		auto rowByteSize = vc * 32;			// sizeof(float) * 8
		auto texWidth = rowByteSize / 16;	// sizeof(RGBA32F)
		texWidth = (texWidth + 7) & ~7u;	// align 8
		rowByteSize = texWidth * 16;

		auto numFrames = int32_t(anim->getDuration() / frameDelay);
		auto texHeight = numFrames;
		texHeight = (texHeight + 7) & ~7u;	// align 8

		auto d = std::make_unique<char[]>(rowByteSize * texHeight);
		auto vs = shader.data.get();
		for (int i = 0; i < numFrames; ++i) {
			auto bp = (xx::Shader_TexVertElement*)(d.get() + rowByteSize * i);
			for (int j = 0; j < vc; ++j) {
				auto& p = bp[j];
				auto& v = vs[j];
				p.x = v.pos.x;
				p.y = v.pos.y;
				p.u = v.uv.x;
				p.v = v.uv.y;
				static constexpr auto _255_1 = 1.f / 255.f;
				p.r = v.color.r * _255_1;
				p.g = v.color.g * _255_1;
				p.b = v.color.b * _255_1;
				p.a = v.color.a * _255_1;
			}
			shader.count = 0;
			shader.lastTextureId = 0;
			Update(frameDelay);
			Draw();
		}
		shader.count = 0;
		shader.lastTextureId = 0;
		eg->GLBlendFunc(bak);
		return xx::LoadGLVertTexture(d.get(), texWidth, texHeight, vc, numFrames);
	}

	/*****************************************************************************************************************************************************************************************/
	/*****************************************************************************************************************************************************************************************/


	char* SpineExtension::_readFile(const spine::String& pathStr, int* length) {
		std::string_view fn(pathStr.buffer(), pathStr.length());
		auto&& iter = gSpineEnv.fileDatas.find(fn);
		*length = (int)iter->second.len;
		return (char*)iter->second.buf;
	}

	/*****************************************************************************************************************************************************************************************/
	/*****************************************************************************************************************************************************************************************/


	void SpineTextureLoader::load(spine::AtlasPage& page, const spine::String& path) {
		std::string_view fn(path.buffer(), path.length());

		auto&& iter = gSpineEnv.textures.find(fn);
		auto&& tex = iter->second;
		tex->SetParm(
			page.magFilter == spine::TextureFilter_Linear ? GL_LINEAR : GL_NEAREST,
			(page.uWrap == spine::TextureWrap_Repeat && page.vWrap == spine::TextureWrap_Repeat) ? GL_REPEAT : GL_CLAMP_TO_EDGE
		);

		page.width = tex->size.x;
		page.height = tex->size.y;

		// unsafe: ref tex
		++tex.GetHeader()->sharedCount;
		page.setRendererObject(tex.pointer);
	}

	void SpineTextureLoader::unload(void* texture) {
		// unsafe: deref tex
		xx::Shared<xx::GLTexture> tex;
		tex.pointer = (xx::GLTexture*)texture;
	}

	/*****************************************************************************************************************************************************************************************/
	/*****************************************************************************************************************************************************************************************/


	void SpineListener::callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* e) {
		h(state, type, entry, e);
	}

	/*****************************************************************************************************************************************************************************************/
	/*****************************************************************************************************************************************************************************************/


	void SpineEnv::Init() {
		spine::SpineExtension::setInstance(&ext);
	}

	void SpineEnv::Clear() {
		skeletonDatas.clear();
		atlass.clear();
		fileDatas.clear();
		textures.clear();
	}

	void SpineEnv::Load(bool skeletonFileIsJson, std::string const& baseFileNameWithPath, spine::SkeletonData*& sd, xx::Shared<xx::GLTexture>& tex, float scale) {
		assert(spine::SpineExtension::getInstance());	// forget gSpineEnv.Init() ?
		auto fnTex = baseFileNameWithPath + ".png";
		auto fnAtlas = baseFileNameWithPath + ".atlas";
		// todo: error check?
		auto eg = GameBase::instance;
		textures.emplace(fnTex, eg->LoadTexture(fnTex));
		fileDatas.emplace(fnAtlas, eg->LoadFileData(fnAtlas));
		auto a = AddAtlas(fnAtlas);
		if (skeletonFileIsJson) {
			auto fnJson = baseFileNameWithPath + ".json";
			fileDatas.emplace(fnJson, eg->LoadFileData(fnJson));
			sd = AddSkeletonData(true, a, fnJson, scale);
		}
		else {
			auto fnSkel = baseFileNameWithPath + ".skel";
			fileDatas.emplace(fnSkel, eg->LoadFileData(fnSkel));
			sd = AddSkeletonData(false, a, fnSkel, scale);
		}
		tex = textures[fnTex];
		fileDatas.clear();
	}

	spine::Atlas* SpineEnv::AddAtlas(std::string_view atlasFileName) {
		auto r = atlass.emplace(atlasFileName, std::make_unique<spine::Atlas>(atlasFileName, &gSpineEnv.textureLoader));
		if (!r.second) {
			assert(false);	// duplicate load ?
			return nullptr;
		}
		return r.first->second.get();
	}

	spine::SkeletonData* SpineEnv::AddSkeletonData(bool skeletonFileIsJson, spine::Atlas* atlas, std::string_view skeletonFileName, float scale) {
		spine::SkeletonData* sd{};
		if (skeletonFileIsJson) {
			spine::SkeletonJson parser(atlas);
			parser.setScale(scale);
			sd = parser.readSkeletonDataFile(skeletonFileName);
		}
		else {
			spine::SkeletonBinary parser(atlas);
			parser.setScale(scale);
			sd = parser.readSkeletonDataFile(skeletonFileName);
		}
		assert(sd);
		auto r = skeletonDatas.emplace(skeletonFileName, sd);
		if (!r.second) {
			assert(false);	// duplicate load ?
			return nullptr;
		}
		return r.first->second.get();
	}

}
