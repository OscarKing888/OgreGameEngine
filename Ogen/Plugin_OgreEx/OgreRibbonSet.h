#ifndef __RibbonSet_H__
#define __RibbonSet_H__

#include "OgreExPrerequisites.h"
#include <OgreBillboardChain.h>
#include <OgreParticle.h>

namespace Ogre
{
    class _OgreExExport RibbonSet : public BillboardChain
    {
    public:

        RibbonSet(const String& name, size_t maxElements = 20, size_t numberOfChains = 1, 
            bool useTextureCoords = true, bool useColours = true);

        void _notifyResized(void);
        void setDefaultSize(Real width, Real length);

        /** @copydoc BillboardChain::setNumberOfChains */
        void setNumberOfChains(size_t numChains);
        /** @copydoc BillboardChain::setMaxChainElements */
        void setMaxChainElements(size_t maxElements);

        /** ����ָ����ribbon
        @param index ribbon��index��������RibbonParticleData�У�
        @param pos ribbonͷ����ǰ��λ�ã��п������������꣬Ҳ�п����Ǿֲ����꣩
        @param worldSpace ��ʶpos�����Ƿ�����������
        @param ribbon��ǰ����ɫֵ
        @param elemWidth ribbon��ǰ�Ŀ��
        */
        void updateRibbon(size_t index, const Vector3& pos, bool worldSpace, const ColourValue& colour, Real elemWidth);

        /** ����ָ����ribbon
        @param index ribbon��index��������RibbonParticleData�У�       
        */
        void resetRibbon(size_t index);

        void setHeadAlpha(Real alpha);
        Real getHeadAlpha(void) const
        {
            return mRibbonHeadAlpha;
        }

        void setTailAlpha(Real alpha);
        Real getTailAlpha(void) const
        {
            return mRibbonTailAlpha;
        }

        void setHeadWidthScale(Real scale);
        Real getHeadWidthScale(void) const
        {
            return mRibbonHeadWidthScale;
        }

        void setTailWidthScale(Real scale);
        Real getTailWidthScale(void) const
        {
            return mRibbonTailWidthScale;
        }

    protected:

        /// Ĭ�ϵĿ��ֵ����ͬ��particle system��particle_width��particle_height��
        Real mDefaultElemWidth;
        Real mDefaultElemLength;

        /// ��ǰribbon�Ŀ���Ƿ񾭹���affectorӰ��
        bool mAllDefaultSize;

        /// ÿ��element��ƽ���߶�
        Real mSquaredElemLength;

        Real mRibbonHeadAlpha;
        Real mRibbonTailAlpha;
        bool mNeedChangeColour;

        Real mRibbonHeadWidthScale;
        Real mRibbonTailWidthScale;
        bool mNeedChangeWidth;
    };

    /// visual data�����ڱ������particle����Ӧ��ribbon��index
    class _OgreExExport RibbonParticleData : public ParticleVisualData
    {
    public:
        RibbonParticleData(size_t chainIndex)
            : mRibbonIndex(chainIndex)
        {
        }

        size_t getRibbonIndex(void)
        {
            return mRibbonIndex;
        }

    protected:

        size_t mRibbonIndex;
    };
}

#endif // __RibbonSet_H__