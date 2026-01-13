#pragma once
#include "xx_prims.h"
#include <box2d/box2d.h>
#include <box2d/math_functions.h>
#include <TaskScheduler.h>	// for multi-thread

namespace xx {

	template<typename T>
	struct alignas(4) B2Id {
		T id{};

		B2Id() = default;
		B2Id(B2Id const&) = delete;
		B2Id& operator=(B2Id const&) = delete;
		B2Id(B2Id&& o) noexcept { std::swap(id, o.id); }
		B2Id& operator=(B2Id&& o) noexcept { std::swap(id, o.id); return *this; }

		B2Id(T id_) : id(id_) {} // unsafe: for easy use
		operator T const& () const { return id; }

		bool IsNull() const {
			auto p = (int32_t*)&id;
			if constexpr (sizeof(id) == 4) if (p[0]) return false;
			if constexpr (sizeof(id) == 8) if (p[0] || p[1]) return false;
			return true;
		}

		// unsafe
		void ZeroMem() {
			assert(!IsNull());
			auto p = (int32_t*)&id;
			if constexpr (sizeof(id) == 4) p[0] = 0;
			if constexpr (sizeof(id) == 8) p[0] = p[1] = 0;
		}

		// unsafe
		template<bool B = false>
		void Destroy() {
			static_assert(sizeof(b2WorldId) <= 8 && sizeof(b2BodyId) <= 8 && sizeof(b2ShapeId) <= 8
				&& sizeof(b2JointId) <= 8 && sizeof(b2ChainId) <= 8);
			if constexpr (std::is_same_v<T, b2WorldId>) b2DestroyWorld(id);
			if constexpr (std::is_same_v<T, b2BodyId>) b2DestroyBody(id);
			if constexpr (std::is_same_v<T, b2ShapeId>) b2DestroyShape(id, B);
			if constexpr (std::is_same_v<T, b2JointId>) b2DestroyJoint(id, B);
			if constexpr (std::is_same_v<T, b2ChainId>) b2DestroyChain(id);
		}

		template<bool B = false>
		void Reset() {
			if (!IsNull()) {
				Destroy<B>();
				ZeroMem();
			}
		}

		~B2Id() {
			Reset();
		}
	};

	struct B2Task : public enki::ITaskSet {
		b2TaskCallback* task{};
		void* taskContext{};

		B2Task() = default;
		void ExecuteRange(enki::TaskSetPartition range_, uint32_t threadIndex_) override {
			task(range_.start, range_.end, threadIndex_, taskContext);
		}
	};

	struct B2World : B2Id<b2WorldId> {
		using B2Id<b2WorldId>::B2Id;

		enki::TaskScheduler taskScheduler;
		B2Task tasks[64];
		int taskCount{};

		void InitDef(b2WorldDef const& b2worlddef_, int workerCount_ = 8) {
			assert(IsNull());
			if (workerCount_ > 1) {
				taskScheduler.Initialize(workerCount_);
				auto def = b2worlddef_;
				def.workerCount = workerCount_;
				def.enqueueTask = EnqueueTask;
				def.finishTask = FinishTask;
				def.userTaskContext = this;
				id = b2CreateWorld(&def);
			}
			else {
				id = b2CreateWorld(&b2worlddef_);
			}
		}

		void Step(float timeStep_ = 1.f / 120.f, int subStepCount_ = 4) {
			b2World_Step(id, timeStep_, subStepCount_);
			taskCount = 0;
		}

		inline static void* EnqueueTask(b2TaskCallback* task_, int32_t itemCount_, int32_t minRange_, void* taskContext, void* userContext_) {
			auto o = (B2World*)userContext_;
			if (o->taskCount < _countof(o->tasks)) {
				B2Task& t = o->tasks[o->taskCount];
				t.m_SetSize = itemCount_;
				t.m_MinRange = minRange_;
				t.task = task_;
				t.taskContext = taskContext;
				o->taskScheduler.AddTaskSetToPipe(&t);
				++o->taskCount;
				return &t;
			}
			else {
				// This is not fatal but the maxTasks should be increased
				assert(false);
				task_(0, itemCount_, 0, taskContext);
				return nullptr;
			}
		}

		inline static void FinishTask(void* taskPtr_, void* userContext_) {
			if (taskPtr_ != nullptr) {
				auto t = (B2Task*)taskPtr_;
				auto o = (B2World*)userContext_;
				o->taskScheduler.WaitforTask(t);
			}
		}
	};


	struct B2Body : B2Id<b2BodyId> {
		using B2Id<b2BodyId>::B2Id; 

		void InitDef(B2World const& b2World_, b2BodyDef const& b2bodydef_) {
			assert(IsNull());
			assert(!b2World_.IsNull());
			id = b2CreateBody(b2World_, &b2bodydef_);
		}

		B2Body& InitTypePos(B2World const& b2World_, XY pos_, b2BodyType type_ = b2_staticBody) {
			auto def = b2DefaultBodyDef();
			def.type = type_;
			def.position = (b2Vec2&)pos_;
			InitDef(b2World_, def);
			return *this;
		}
		// ...

		b2Transform GetTransform() const {
			return b2Body_GetTransform(id);
		}

		XY GetPos() const {
			return b2Body_GetTransform(id).p;
		}

		std::pair<XY, float> GetPosRadians() const {
			auto tran = GetTransform();
			return { tran.p, b2Rot_GetAngle(tran.q) };
		}

		void SetTransform(XY pos_, float radians_) {
			b2Body_SetTransform(id, (b2Vec2&)pos_, b2MakeRot(radians_));
		}

		void SetTransform(XY pos_, b2Rot rot_) {
			b2Body_SetTransform(id, (b2Vec2&)pos_, rot_);
		}

		void SetTransform(b2Transform const& tran_) {
			b2Body_SetTransform(id, tran_.p, tran_.q);
		}

		//void SetPos(XY pos_) {
		//	auto trans = b2Body_GetTransform(id);
		//	b2Body_SetTransform(id, (b2Vec2&)pos_, trans.q);
		//}

		// ...
	};

	struct B2Shape : B2Id<b2ShapeId> {
		using B2Id<b2ShapeId>::B2Id;

		void InitDefPolygon(B2Body const& b2body_, b2Polygon const& b2polygon_
			, b2ShapeDef const& b2shapedef_ = b2DefaultShapeDef()) {
			assert(!b2body_.IsNull());
			Reset();
			id = b2CreatePolygonShape(b2body_, &b2shapedef_, &b2polygon_);
		}

		void InitBox(B2Body const& b2body_, XY halfSize_
			, b2ShapeDef const& b2shapedef_ = b2DefaultShapeDef()) {
			auto b2polygon = b2MakeBox(halfSize_.x, halfSize_.y);
			InitDefPolygon(b2body_, b2polygon, b2shapedef_);
		}

		void InitCircle(B2Body const& b2body_, XY center_, float radius_
			, b2ShapeDef const& b2shapedef_ = b2DefaultShapeDef()) {
			Reset();
			auto circle = b2Circle{ .center = (b2Vec2&)center_, .radius = radius_ };
			id = b2CreateCircleShape(b2body_, &b2shapedef_, &circle);
		}
		// ...
	};

	// ...

}
