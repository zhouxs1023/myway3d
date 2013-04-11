#pragma once

namespace game {

	class GEntity
	{
		DECLARE_ALLOC();

	public:
		GEntity();
		virtual ~GEntity();

		void SetPosition(float x, float y, float z);
		void SetPosition(const Vec3 & v);
		Vec3 GetPosition() const;

		void SetOrientation(float x, float y, float z, float w);
		void SetOrientation(const Quat & q);
		Quat GetOrientation() const;

		void SetScale(float x);
		float GetScale();

	protected:
		SceneNode * mNode;
	};

}