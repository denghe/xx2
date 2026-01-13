#pragma once
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

		spine::SkeletonData* AddSkeletonData(bool skeletonFileIsJson, spine::Atlas* atlas, std::string_view skeletonFileName, float scale = 1.f);

		void Init();

		void Load(bool skeletonFileIsJson, std::string const& baseFileNameWithPath, spine::SkeletonData*& sd, xx::Shared<xx::GLTexture>& tex, float scale = 1.f);

		// unsafe. careful SkeletonData* refs
		void Clear();
	};

	inline SpineEnv gSpineEnv;	// need init at GameBase

}
