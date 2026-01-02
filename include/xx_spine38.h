#pragma once
#include "xx_shader_spine.h"
#include <spine/spine.h>

namespace xx {

	struct SpineListener : spine::AnimationStateListenerObject {
		using HandlerType = std::function<void(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* e)>;
		HandlerType h;
		SpineListener() = default;
		template<typename CB>
		void SetCallBack(CB&& cb) { h = std::forward<CB>(cb); }
		void callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* e) override;
	};

	struct SpinePlayer {
		spine::AnimationStateData animationStateData;
		spine::Skeleton skeleton;
		spine::AnimationState animationState;
		SpineListener spineListener;							// can SetCallBack( func )
		float timeScale{ 1 };

		SpinePlayer(spine::SkeletonData* skeletonData);
		SpinePlayer() = delete;
		SpinePlayer(SpinePlayer const&) = delete;
		SpinePlayer& operator=(SpinePlayer const&) = delete;

		spine::Vector<spine::Animation*>& GetAnimations();
		spine::Animation* FindAnimation(std::string_view name);
		spine::Bone* FindBone(std::string_view name);
		spine::Slot* FindSlot(std::string_view name);
		SpinePlayer& Update(float delta);
		GLVertTexture AnimToTexture(spine::Animation* anim, float frameDelay);
		GLVertTexture AnimToTexture(std::string_view animName, float frameDelay);
		void Draw();	// careful: blend maybe changed
		SpinePlayer& SetTimeScale(float t);
		SpinePlayer& SetUsePremultipliedAlpha(bool b);
		SpinePlayer& SetPosition(float x, float y);
		SpinePlayer& SetPosition(XY const& pos);		// whole
		SpinePlayer& SetScale(XY const& scale);			// whole
		SpinePlayer& SetFirstRotation(float r);			// first bone
		SpinePlayer& SetFirstScale(XY const& scale);	// first bone
		SpinePlayer& SetMix(spine::Animation* from, spine::Animation* to, float duration);
		SpinePlayer& SetMix(std::string_view fromName, std::string_view toName, float duration);
		spine::TrackEntry* SetAnimation(size_t trackIndex, std::string_view animationName, bool loop);
		spine::TrackEntry* AddAnimation(size_t trackIndex, std::string_view animationName, bool loop, float delay);
		spine::TrackEntry* SetAnimation(size_t trackIndex, spine::Animation* anim, bool loop);
		spine::TrackEntry* AddAnimation(size_t trackIndex, spine::Animation* anim, bool loop, float delay);
		// ...

	protected:
		mutable spine::Vector<float> worldVertices;
		mutable spine::Vector<float> tempUvs;
		mutable spine::Vector<spine::Color> tempColors;
		mutable spine::Vector<unsigned short> quadIndices;
		mutable spine::SkeletonClipping clipper;
		mutable bool usePremultipliedAlpha{ false };
	};

	struct SpineTextureLoader : public spine::TextureLoader {
		void load(spine::AtlasPage& page, const spine::String& path) override;
		void unload(void* texture) override;
	};

	struct SpineExtension : spine::DefaultSpineExtension {
	protected:
		char* _readFile(const spine::String& pathStr, int* length) override;
	};

	// spine's global env
	struct SpineEnv {
		SpineExtension ext;
		SpineTextureLoader textureLoader;

		// key: file path
		std::unordered_map<std::string, Shared<GLTexture>, StdStringHash, std::equal_to<>> textures;							// need preload
		std::unordered_map<std::string, Data, StdStringHash, std::equal_to<>> fileDatas;										// need preload Atlas & SkeletonData files
		std::unordered_map<std::string, std::unique_ptr<spine::Atlas>, StdStringHash, std::equal_to<>> atlass;					// fill by AddAtlas
		std::unordered_map<std::string, std::unique_ptr<spine::SkeletonData>, StdStringHash, std::equal_to<>> skeletonDatas;	// fill by AddSkeletonData

		spine::Atlas* AddAtlas(std::string_view atlasFileName);

		template<bool skeletonFileIsJson>
		spine::SkeletonData* AddSkeletonData(spine::Atlas* atlas, std::string_view skeletonFileName, float scale = 1.f);

		void Init();

		template<bool skeletonFileIsJson = false>
		void Load(std::string const& baseFileNameWithPath, spine::SkeletonData*& sd, xx::Shared<xx::GLTexture>& tex, float scale = 1.f);

		// unsafe. careful SkeletonData* refs
		void Clear();
	};
	inline SpineEnv gSpineEnv;	// need init at GameBase


	/*****************************************************************************************************************************************************************************************/
	/*****************************************************************************************************************************************************************************************/
	// ref: spine-sfml.cpp

	inline SpinePlayer::SpinePlayer(spine::SkeletonData* skeletonData)
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

	inline SpinePlayer& SpinePlayer::Update(float delta) {
		skeleton.updateWorldTransform();
		skeleton.update(delta * timeScale);
		animationState.update(delta * timeScale);
		animationState.apply(skeleton);
		return *this;
	}

	inline void SpinePlayer::Draw() {
		auto& eg = *GameBase_shader::GetInstance();

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
			eg.GLBlendFunc({ blend.first, blend.second, GL_FUNC_ADD });


			if (clipper.isClipping()) {
				clipper.clipTriangles(worldVertices, *indices, *uvs, 2);
				vertices = &clipper.getClippedVertices();
				verticesCount = clipper.getClippedVertices().size() >> 1;
				uvs = &clipper.getClippedUVs();
				indices = &clipper.getClippedTriangles();
				indicesCount = clipper.getClippedTriangles().size();
			}

			// if (vertexEffect != NULL) else {
			auto vs = eg.Spine().Alloc(*texture, (int32_t)indicesCount);
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

	XX_INLINE spine::Vector<spine::Animation*>& SpinePlayer::GetAnimations() {
		return animationStateData.getSkeletonData()->getAnimations();
	}

	XX_INLINE spine::Animation* SpinePlayer::FindAnimation(std::string_view name) {
		return animationStateData.getSkeletonData()->findAnimation(name);
	}

	XX_INLINE spine::Bone* SpinePlayer::FindBone(std::string_view name) {
		return skeleton.findBone(name);
	}

	XX_INLINE spine::Slot* SpinePlayer::FindSlot(std::string_view name) {
		return skeleton.findSlot(name);
	}

	XX_INLINE SpinePlayer& SpinePlayer::SetMix(spine::Animation* from, spine::Animation* to, float duration) {
		animationStateData.setMix(from, to, duration);
		return *this;
	}

	XX_INLINE SpinePlayer& SpinePlayer::SetMix(std::string_view fromName, std::string_view toName, float duration) {
		animationStateData.setMix(fromName, toName, duration);
		return *this;
	}

	XX_INLINE SpinePlayer& SpinePlayer::SetTimeScale(float t) {
		timeScale = t;
		return *this;
	}

	XX_INLINE SpinePlayer& SpinePlayer::SetUsePremultipliedAlpha(bool b) {
		usePremultipliedAlpha = b;
		return *this;
	}

	XX_INLINE SpinePlayer& SpinePlayer::SetPosition(XY const& xy) {
		return SetPosition(xy.x, xy.y);
	}

	XX_INLINE SpinePlayer& SpinePlayer::SetScale(XY const& xy) {
		skeleton.setScaleX(xy.x);
		skeleton.setScaleY(xy.y);
		return *this;
	}

	XX_INLINE SpinePlayer& SpinePlayer::SetFirstRotation(float r) {
		skeleton.getBones()[0]->setRotation(r);
		return *this;
	}

	XX_INLINE SpinePlayer& SpinePlayer::SetFirstScale(XY const& scale) {
		auto& b = skeleton.getBones()[0];
		b->setScaleX(scale.x);
		b->setScaleY(scale.y);
		return *this;
	}

	XX_INLINE SpinePlayer& SpinePlayer::SetPosition(float x, float y) {
		skeleton.setPosition(x, y);
		return *this;
	}

	XX_INLINE spine::TrackEntry* SpinePlayer::SetAnimation(size_t trackIndex, std::string_view animationName, bool loop) {
		return animationState.setAnimation(trackIndex, animationName, loop);
	}

	XX_INLINE spine::TrackEntry* SpinePlayer::AddAnimation(size_t trackIndex, std::string_view animationName, bool loop, float delay) {
		return animationState.addAnimation(trackIndex, animationName, loop, delay);
	}

	XX_INLINE spine::TrackEntry* SpinePlayer::SetAnimation(size_t trackIndex, spine::Animation* anim, bool loop) {
		return animationState.setAnimation(trackIndex, anim, loop);
	}

	XX_INLINE spine::TrackEntry* SpinePlayer::AddAnimation(size_t trackIndex, spine::Animation* anim, bool loop, float delay) {
		return animationState.addAnimation(trackIndex, anim, loop, delay);
	}

	// todo: multi anim pack to 1 tex
	inline GLVertTexture SpinePlayer::AnimToTexture(std::string_view animName, float frameDelay) {
		return AnimToTexture(animationStateData.getSkeletonData()->findAnimation(animName), frameDelay);
	}

	inline GLVertTexture SpinePlayer::AnimToTexture(spine::Animation* anim, float frameDelay) {
		auto& eg = *GameBase_shader::GetInstance();
		auto bak = eg.blend;
		SetPosition(0, 0).SetAnimation(0, anim, true);
		eg.ShaderEnd();
		Update(frameDelay);
		Draw();	// draw once for get vert size
		auto&& shader = eg.Spine();
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
		eg.GLBlendFunc(bak);
		return xx::LoadGLVertTexture(d.get(), texWidth, texHeight, vc, numFrames);
	}

	/*****************************************************************************************************************************************************************************************/
	/*****************************************************************************************************************************************************************************************/


	inline char* SpineExtension::_readFile(const spine::String& pathStr, int* length) {
		std::string_view fn(pathStr.buffer(), pathStr.length());
		auto&& iter = gSpineEnv.fileDatas.find(fn);
		*length = (int)iter->second.len;
		return (char*)iter->second.buf;
	}

	/*****************************************************************************************************************************************************************************************/
	/*****************************************************************************************************************************************************************************************/


	inline void SpineTextureLoader::load(spine::AtlasPage& page, const spine::String& path) {
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

	inline void SpineTextureLoader::unload(void* texture) {
		// unsafe: deref tex
		xx::Shared<xx::GLTexture> tex;
		tex.pointer = (xx::GLTexture*)texture;
	}

	/*****************************************************************************************************************************************************************************************/
	/*****************************************************************************************************************************************************************************************/


	inline void SpineListener::callback(spine::AnimationState* state, spine::EventType type, spine::TrackEntry* entry, spine::Event* e) {
		h(state, type, entry, e);
	}

	/*****************************************************************************************************************************************************************************************/
	/*****************************************************************************************************************************************************************************************/


	inline void SpineEnv::Init() {
		spine::SpineExtension::setInstance(&ext);
	}

	inline void SpineEnv::Clear() {
		skeletonDatas.clear();
		atlass.clear();
		fileDatas.clear();
		textures.clear();
	}

	template<bool skeletonFileIsJson>
	inline void SpineEnv::Load(std::string const& baseFileNameWithPath, spine::SkeletonData*& sd, xx::Shared<xx::GLTexture>& tex, float scale) {
		assert(spine::SpineExtension::getInstance());	// forget gSpineEnv.Init() ?
		auto fnTex = baseFileNameWithPath + ".png";
		auto fnAtlas = baseFileNameWithPath + ".atlas";
		// todo: error check?
		auto& eg = *GameBase_shader::GetInstance();
		textures.emplace(fnTex, eg.LoadTexture(fnTex));
		fileDatas.emplace(fnAtlas, eg.LoadFileData(fnAtlas).first);
		auto a = AddAtlas(fnAtlas);
		if constexpr (skeletonFileIsJson) {
			auto fnJson = baseFileNameWithPath + ".json";
			fileDatas.emplace(fnJson, eg.LoadFileData(fnJson).first);
			sd = AddSkeletonData<true>(a, fnJson, scale);
		}
		else {
			auto fnSkel = baseFileNameWithPath + ".skel";
			fileDatas.emplace(fnSkel, eg.LoadFileData(fnSkel).first);
			sd = AddSkeletonData<false>(a, fnSkel, scale);
		}
		tex = textures[fnTex];
		fileDatas.clear();
	}

	inline spine::Atlas* SpineEnv::AddAtlas(std::string_view atlasFileName) {
		auto r = atlass.emplace(atlasFileName, std::make_unique<spine::Atlas>(atlasFileName, &gSpineEnv.textureLoader));
		if (!r.second) {
			assert(false);	// duplicate load ?
			return nullptr;
		}
		return r.first->second.get();
	}

	template<bool skeletonFileIsJson>
	inline spine::SkeletonData* SpineEnv::AddSkeletonData(spine::Atlas* atlas, std::string_view skeletonFileName, float scale) {
		std::conditional_t<skeletonFileIsJson, spine::SkeletonJson, spine::SkeletonBinary> parser(atlas);
		parser.setScale(scale);
		auto sd = parser.readSkeletonDataFile(skeletonFileName);
		assert(sd);
		auto r = skeletonDatas.emplace(skeletonFileName, sd);
		if (!r.second) {
			assert(false);	// duplicate load ?
			return nullptr;
		}
		return r.first->second.get();
	}

}
