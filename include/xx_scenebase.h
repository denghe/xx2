#pragma once

namespace xx {

	// for game scene logic
	struct SceneBase {
		virtual ~SceneBase() = default;
		virtual void Update() {};
		virtual void Draw() {};
		virtual void OnResize(bool modeChanged_) {};
		virtual void OnFocus(bool focused_) {};
	};

}
