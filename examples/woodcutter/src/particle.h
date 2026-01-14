#pragma once
#include "xx_listlink.h"
#include "xx_gamebase.h"

namespace xx {

	// code ref from HGE

	struct ParticleItem {
		XY pos, velocity;
		float gravity, radialAccel, tangentialAccel, spin, spinDelta, size, sizeDelta, age, terminalAge;
		RGBA color, colorDelta; // + alpha
	};

	struct ParticleConfig {
		Frame frame;
        RGBA8 frameColor{ RGBA8_White };

		int emission;	// 0 ~ 10000
		float lifetime;	// 0 ~ 10s, -1.0f

		std::pair<float, float> particleLife;	// 0 ~ 5

		float direction;	// 0 ~ 2PI
		float spread;	// 0 ~ 2PI
		int relative;	// 0, 1

		std::pair<float, float> speed;	// -300 ~ 300
		std::pair<float, float> gravity;	// -900 ~ 900
		std::pair<float, float> radialAccel;	// -900 ~ 900
		std::pair<float, float> tangentialAccel;	// -900 ~ 900
		std::pair<float, float> size;	// 1 ~ 100
		float sizeVar;	// 0 ~ 1

		std::pair<float, float> spin;	// -50, 50
		float spinVar;	// 0 ~ 1

		std::pair<RGBA, RGBA> color; // + alpha
		float colorVar, alphaVar;	// 0 ~ 1
	};

	struct Particle {

        xx::Shared<ParticleConfig> cfg;
        xx::ListLink<ParticleItem, int> particles;
        XY prevPos, pos, rootPos;
        float age, emissionResidue;
		bool enableBlendPremultipliedAlpha{};

		void Init(xx::Shared<ParticleConfig> cfg_, size_t const& cap = 1000, bool enableBlendPremultipliedAlpha_ = true) {
			particles.Clear<true>();
			particles.Reserve(cap);
            enableBlendPremultipliedAlpha = enableBlendPremultipliedAlpha_;
			cfg = std::move(cfg_);

			pos = prevPos = rootPos = {};

			emissionResidue = {};
			age = -2.0;
		}

		void Update(Rnd& rnd_, float delta_) {
            int i, e;
            float ang;

            if (age >= 0) {
                age += delta_;
                if (age >= cfg->lifetime) {
                    age = -2.0f;
                }
            }

            // update all alive particles

            int prev = -1, next{};
            for (auto idx = particles.head; idx != -1;) {
                auto& p = particles[idx];

                p.age += delta_;
                if (p.age >= p.terminalAge) {
                    next = particles.Remove(idx, prev);
                } else {
                    next = particles.Next(idx);
                    prev = idx;
                    {
                        auto vecAccel = (p.pos - pos).Normalize();
                        XY vecAccel2 = vecAccel;
                        vecAccel *= p.radialAccel;

                        // vecAccel2.Rotate(M_PI_2);
                        // the following is faster
                        ang = vecAccel2.x;
                        vecAccel2.x = -vecAccel2.y;
                        vecAccel2.y = ang;

                        vecAccel2 *= p.tangentialAccel;
                        p.velocity += (vecAccel + vecAccel2) * delta_;
                        p.velocity.y += p.gravity * delta_;

                        p.pos += p.velocity * delta_;

                        p.spin += p.spinDelta * delta_;
                        p.size += p.sizeDelta * delta_;
                        p.color += p.colorDelta * delta_;
                    }
                }
                idx = next;
            }

            // generate new particles

            if (age != -2.0f) {
                const auto particlesNeeded = cfg->emission * delta_ + emissionResidue;
                int n = (uint32_t)particlesNeeded;
                emissionResidue = particlesNeeded - n;

                auto left = particles.Left();
                if (n > left) {
                    n = left;
                }
                for (i = 0; i < n; i++) {
                    auto&& p = particles.Emplace();

                    p.age = 0.0f;
                    p.terminalAge = rnd_.Next(cfg->particleLife);

                    p.pos = prevPos + (pos - prevPos) * rnd_.Next(0.f, 1.f) + XY{ rnd_.Next(-2.f, 2.f), rnd_.Next(-2.f, 2.f) };

                    ang = cfg->direction - float(M_PI / 2) + rnd_.Next(0.f, cfg->spread) - cfg->spread / 2.0f;
                    if (cfg->relative) {
                        auto d = prevPos - pos;
                        ang += std::atan2f(d.y, d.x) + float(M_PI / 2);
                    }
                    p.velocity = XY{ std::cosf(ang), std::sinf(ang) } *rnd_.Next(cfg->speed);

                    p.gravity = rnd_.Next(cfg->gravity);
                    p.radialAccel = rnd_.Next(cfg->radialAccel);
                    p.tangentialAccel = rnd_.Next(cfg->tangentialAccel);

                    p.size = rnd_.Next(cfg->size.first, cfg->size.first + (cfg->size.second - cfg->size.first) * cfg->sizeVar);
                    p.sizeDelta = (cfg->size.second - p.size) / p.terminalAge;

                    p.spin = rnd_.Next(cfg->spin.first, cfg->spin.first + (cfg->spin.second - cfg->spin.first) * cfg->spinVar);
                    p.spinDelta = (cfg->spin.second - p.spin) / p.terminalAge;

                    p.color.r = rnd_.Next2(cfg->color.first.r, cfg->color.first.r + (cfg->color.second.r - cfg->color.first.r) * cfg->colorVar);
                    p.color.g = rnd_.Next2(cfg->color.first.g, cfg->color.first.g + (cfg->color.second.g - cfg->color.first.g) * cfg->colorVar);
                    p.color.b = rnd_.Next2(cfg->color.first.b, cfg->color.first.b + (cfg->color.second.b - cfg->color.first.b) * cfg->colorVar);
                    p.color.a = rnd_.Next2(cfg->color.first.a, cfg->color.first.a + (cfg->color.second.a - cfg->color.first.a) * cfg->alphaVar);

                    p.colorDelta = (cfg->color.second - p.color) / p.terminalAge;
                }
            }

            prevPos = pos;
		}

        void FireAt(XY const& xy) {
            Stop();
            MoveTo(xy);
            Fire();
        }

        void Fire() {
            if (cfg->lifetime == -1.0f) {
                age = -1.0f;
            } else {
                age = 0.0f;
            }
        }

        void MoveTo(XY const& xy, bool moveParticles = false) {
            if (moveParticles) {
                const auto d = xy - pos;

                for (auto idx = particles.head; idx != -1; idx = particles.Next(idx)) {
                    particles[idx].pos += d;
                }

                prevPos += d;
            } else {
                if (age == -2.0) {
                    prevPos = xy;
                } else {
                    prevPos = pos;
                }
            }

            pos = xy;
        }

        void Stop(bool killParticles = false) {
            age = -2.0f;
            if (killParticles) {
                particles.Clear();
            }
        }
        bool Empty() const {
            return particles.Count() == 0;
        }

        // warning: maybe change blend
        void Draw(Camera& cam_) const {
            auto e = GameBase::instance;
			//auto bakBlend = e->blend;
            e->SetBlendPremultipliedAlpha(enableBlendPremultipliedAlpha);

			auto scale_ = cam_.scale;

            auto fc = cfg->frameColor;
			auto& shader = e->Quad();
            for (auto idx = particles.head; idx != -1; idx = particles.Next(idx)) {
                auto&& par = particles[idx];

                if (cfg->color.first.r < 0) {
                    fc.a = par.color.a * 255;
                } else {
                    fc = par.color;
                }

				shader.DrawFrame(cfg->frame, cam_.ToGLPos(par.pos + rootPos), par.size * scale_, par.spin * par.age, 1.f, fc);
            }
			//e->GLBlendFunc(bakBlend);
        }

	};
}
