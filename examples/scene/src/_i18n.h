#pragma once
#include "xx_includes.h"
#include "_i18n_.h"
#include "_i18n_cn.h"
#include "_i18n_en.h"
// ...

namespace i18n {

	enum class Languages {
		en,
		cn,
		// ...
		__MAX__
	};

	static constexpr std::u32string_view const* Textss[(int32_t)Languages::__MAX__]{
		TextsEN,
		TextsCN,
		// ...
	};


	struct Lang {
		static_assert((int32_t)Languages::__MAX__ == _countof(Textss));
		static_assert((int32_t)Strs::__MAX__ == _countof(TextsCN));
		static_assert(_countof(TextsCN) == _countof(TextsEN));
		// ...

		Languages language{ Languages::en };	// current
		std::u32string_view const* texts{ Textss[(int32_t)language] };

		XX_INLINE void SetLanguage(Languages language_) {
			language = language_;
			texts = Textss[(int32_t)language_];
		}

		XX_INLINE void SetNextLanguage() {
			language = (Languages)((int32_t)language + 1);
			if (language >= Languages::__MAX__) {
				language = {};
			}
			texts = Textss[(int32_t)language];
		}

		XX_INLINE std::u32string_view const& operator()(Strs k) const {
			assert(!texts[(int32_t)k].empty());
			return texts[(int32_t)k];
		}

	};

}
