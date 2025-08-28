#pragma once
#include "xx_ui_background.h"
#include "xx_ui_imagebutton.h"

namespace xx {

	// todo

	struct DropDownList;
	struct DropDownListItem : MouseEventHandlerNode {
		static constexpr int32_t cTypeId{ 15 };
		Weak<DropDownList> owner;
		int32_t idx{ -1 };
		bool highLight{};

		template<typename S1>
		void Init(Weak<DropDownList> owner_, int32_t idx_, bool highLight_);

		void SetHighLight(bool enable);

		virtual int32_t OnMouseDown(int32_t btnId_) override;

		virtual int32_t OnMouseMove() override;
	};

	struct DropDownList : Button {
		static constexpr int32_t cTypeId{ 15 };
		Ref<Scale9Config> cfgBG;
		List<std::string> data;
		Weak<Background> bg;
		Weak<Scale9> content;
		TinyFrame itemHead;

		DropDownList& InitBegin(int32_t z_, XY position_, XY anchor_, XY fixedSize_
			, Ref<Scale9Config> cfgNormal_, Ref<Scale9Config> cfgHighlight_, Ref<Scale9Config> cfgBG_) {
			typeId = cTypeId;
			isFocus = false;
			z = z_;
			position = position_;
			anchor = anchor_;
			size = fixedSize_;
			cfgNormal = std::move(cfgNormal_);
			cfgHighlight = std::move(cfgHighlight_);
			cfgBG = std::move(cfgBG_);
			FillTrans();
			return *this;
		}

		void InitEnd(int32_t selectedIndex, TinyFrame icon_, TinyFrame itemHead_) {
			itemHead = std::move(itemHead_);
			auto& cfg = GetCfg();

			auto lblLeft = MakeChildren<Label>();
			lblLeft->Init(z + 1, { cfg.txtPadding.x, cfg.txtPaddingRightBottom.y }, {}, cfg.txtScale, cfg.txtColor);
			lblLeft->SetText(data[selectedIndex]);
			
			auto imgSize = size.y * 0.8f;
			auto imgSpacing = (size.y - imgSize) * 0.5f;
			XY imgPos{ size.x - imgSpacing, imgSpacing };
			MakeChildren<Image>()->Init(z + 2, imgPos, { 1, 0 }, imgSize, true, std::move(icon_));

			MakeChildren<Scale9>()->Init(z, 0, {}, cfg.borderScale, size / cfg.borderScale, cfg);

			onClicked = [w = WeakFromThis(this)] {
				assert(w);
				w->PopList();
			};
		}

		void PopList() {
			XY totalSize{ size.x, size.y * data.len };

			auto c = MakeChildren<Scale9>();
			c->Init(z + 1000, 0, {0, 1}, 1, totalSize, *cfgBG);
			content = c.ToWeak();

			// todo: make items

			bg = MakeChildren<Background>();
			bg->Init(z + 999, content).onOutsideClicked = [this] {
				content->SwapRemoveFromParent();
				bg->SwapRemoveFromParent();
			};
		}

		void ApplyCfg() override {
			assert(children.len >= 3);
			auto& cfg = GetCfg();
			assert(children[2]->typeId == Scale9::cTypeId);
			auto bg = (Scale9*)children[2].pointer;
			bg->Init(z, 0, {}, cfg.borderScale, size / cfg.borderScale, cfg);
		}
	};


	template<typename S1>
	inline void DropDownListItem::Init(Weak<DropDownList> owner_, int32_t idx_, bool highLight_) {
		owner = std::move(owner_);
		idx = idx_;
		highLight = highLight_;
		size = owner->size;

		// todo: make childs
		//auto img = MakeChildren<Image>()->Init(z_, )
	}

	inline void DropDownListItem::SetHighLight(bool enable) {
		if (highLight == enable) return;
		highLight = enable;
		assert(children[1]->typeId == Label::cTypeId);
		auto lbl = (Label*)children[1].pointer;
		lbl->color = enable ? RGBA8_Blue : RGBA8_White;
	}

	inline int32_t DropDownListItem::OnMouseDown(int32_t btnId_) {
		assert(owner);
		// todo: callback owner on selected func
		return 1;
	}

	inline int32_t DropDownListItem::OnMouseMove() {
		if (!highLight) {
			SetHighLight(true);
			GameBase::instance->delayFuncs.Emplace([w = WeakFromThis(this)] {
				auto p = w.TryGetPointer();
				if (!p) return 1;
				if (!p->MousePosInArea()) {
					p->SetHighLight(false);
					return 1;
				}
				return 0;
				});
		}
		return 1;
	}

}
