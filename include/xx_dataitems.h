#pragma once
#include "xx_ptr.h"
#include "xx_list.h"

/*
* data memory merged container( speed up 5+ ratio when class size big )
* example:

struct FooData {
	int32_t n{}, m{};
};

struct Foo : xx::DataItemBase<FooData, Foo> {
	char dummy[2000];	// big data sim
};

using Foos = xx::DataItems<FooData, Foo>;

Foos foos;
foos.Update([](FooData& o, int32_t i)->int32_t {
	++o.n;
	return o.n >= o.m;
});
*/

namespace xx {

	template<typename D, typename T>
	struct DataItems;

	template<typename D, typename T>
	struct DataItemBase {
		DataItems<D, T>* container{};
		int32_t indexAtContainer{ -1 };	// DataItems.items / datas[ index ]
		D& Data();
		void Release();
	};

	template<typename D, typename T>
	struct DataItems {
		xx::List<xx::Shared<T>> items;
		xx::List<D> datas;				// memory merged for fast visit

		// F: [](D& d, int32_t i)->int32_t { return 0; }
		template <typename F>
		void Update(F&& func) {
			for (auto i = datas.len - 1; i >= 0; --i) {
				if (func(datas[i], i)) items[i]->Release();
			}
		}

		template<typename U> requires std::derived_from<U, DataItemBase<D, T>>
		xx::Shared<U> Make() {
			auto o = xx::MakeShared<U>();
			o->container = this;
			o->indexAtContainer = items.len;
			items.Emplace(o);
			datas.Emplace();
			return o;
		}
	};

	template<typename D, typename T>
	inline D& DataItemBase<D, T>::Data() {
		assert(indexAtContainer != -1);
		assert(container->items[indexAtContainer].pointer == this);
		return container->datas[indexAtContainer];
	}

	template<typename D, typename T>
	inline void DataItemBase<D, T>::Release() {
		auto m = std::exchange(container, nullptr);
		auto i = std::exchange(indexAtContainer, -1);
		m->datas.SwapRemoveAt(i);
		m->items.Back()->indexAtContainer = i;
		m->items.SwapRemoveAt(i);
	}

}
