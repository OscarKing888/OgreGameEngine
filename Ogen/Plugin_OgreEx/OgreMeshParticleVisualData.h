/********************************************************************
filename:   OgreMeshParticleVisualData.h

purpose:    keep the data that used to render the mesh particle.
*********************************************************************/

#ifndef __MeshParticleVisualData_H__
#define __MeshParticleVisualData_H__

#include <OgreParticle.h>
#include <OgreMaterial.h>

namespace Ogre {

	class SceneNode;
	class Entity;

	class MeshParticleVisualData : public ParticleVisualData
	{
	public:
		MeshParticleVisualData();
		~MeshParticleVisualData();

		void setPosition( const Vector3 &pos );
		
        /** ���ݴ���ķ������÷�λ��Ϣ
            @param dir Ҫ���õķ���
            @param parentOrientation ����ϵͳ�ڵ�ķ�λ
        */
		void setDirection( const Vector3 &dir, const Quaternion& parentOrientation);	

		void setOrientation( const Radian &yaw, const Radian &pitch, const Radian &roll );
		void setOrientation( const Quaternion& q );

        /** ���÷�λ
            @param rotate �Ƿ�Ҫ������ת��
            @param parentOrientation ����ϵͳ�ڵ�ķ�λ
        */
		void setOrientation(bool rotate, const Quaternion& parentOrientation);

		void setScale( Real xzSize, Real ySize );

		void setVisible(bool visible);
		bool getVisible(void)
		{
			return mVisible;
		}

		void setColour( const ColourValue &colour );

		void modifyMesh(const String &meshName);
		void modifyMaterial(const String &matName);

        void setAnimationSpeedFactor(Real factor)
        {
            mAnimationSpeedFactor = factor;
        }

        void updateAnimation(void);

        void resetAnimation(void);

        void setRenderQueueGroup(uint8 queueID);

//	protected:

		void createVisualData(const String &meshName, const String &matName, Node* parentNode);
		void destroyVisualData(void);

		void cloneMaterial(const String& matName);

		bool isInit(void)
		{
			return mInit;
		}

        void setPoolIndex(size_t index)
        {
            mPoolIndex = index;
        }
        size_t getPoolIndex(void) const
        {
            return mPoolIndex;
        }

	public:

		/// Current rotation value
		Radian mYawRotation;

		/// Current rotation value
		Radian mPitchRotation;

		/// Current rotation value
		Radian mRollRotation;

		/// Speed of rotation in radians/sec
		Radian mYawRotationSpeed;

		/// Speed of rotation in radians/sec
		Radian mPitchRotationSpeed;

		/// Speed of rotation in radians/sec
		Radian mRollRotationSpeed;

		Quaternion mOrientation;

        /// ��֮֡���ʱ������animation����Ӱ��������
        Real mDeltaTime;

        bool mAnimationLoop;

        String mAnimationName;

        Real mAnimationSpeedFactor;

        bool mAnimationUpdated;

	protected:

		SceneNode *mSceneNode;
		Entity *mEntity;

		MaterialPtr mMaterial;

		bool mInit;

		bool mVisible;

        AnimationState* mAnimState;

        size_t mPoolIndex;
	};
}

#endif // __MeshParticleVisualData_H__ 