// *************************************************************************************
// 
// 	Copyright (c) 2007 XGames
// 
// 	All Rights Reserved 
// 
// 	Created by		:		Oscar.Chen
// 	Create Date		:		2007-10-25
// *************************************************************************************

#pragma once
#include "EnginePrerequisites.h"
#include "DynamicRenderable.h"



	class _EngineExport DynamicLines : public DynamicRenderable
	{
		typedef Vector3 Vector3;
		typedef Quaternion Quaternion;
		typedef Camera Camera;
		typedef Real Real;
		typedef RenderOperation::OperationType OperationType;

	public:
		/// Constructor - see setOperationType() for description of argument.
		DynamicLines(const String& name = "", OperationType opType=RenderOperation::OT_LINE_STRIP);
		DynamicLines(const Vector3List& pointList, const String& name = "",  OperationType opType=RenderOperation::OT_LINE_STRIP);
		virtual ~DynamicLines();

		/// Add a point to the point list
		void addPoint(const Vector3 &p);
		/// Add a point to the point list
		void addPoint(Real x, Real y, Real z);

		void setPoint(const Vector3List& pointList);

		/// Change the location of an existing point in the point list
		void setPoint(unsigned short index, const Vector3 &value);

		/// Return the location of an existing point in the point list
		const Vector3& getPoint(unsigned short index) const;

		/// Return the total number of points in the point list
		unsigned short getNumPoints(void) const;

		/// Remove all points from the point list
		void clear();

		/// Call this to update the hardware buffer after making changes.  
		void update();

		/** Set the type of operation to draw with.
		* @param opType Can be one of 
		*    - RenderOperation::OT_LINE_STRIP
		*    - RenderOperation::OT_LINE_LIST
		*    - RenderOperation::OT_POINT_LIST
		*    - RenderOperation::OT_TRIANGLE_LIST
		*    - RenderOperation::OT_TRIANGLE_STRIP
		*    - RenderOperation::OT_TRIANGLE_FAN
		*    The default is OT_LINE_STRIP.
		*/
		void setOperationType(OperationType opType);
		OperationType getOperationType() const;

		void setLineColour(const ColourValue& clr);
		const ColourValue& getLineColour() const;

	protected:
		/// Implementation DynamicRenderable, creates a simple vertex-only decl
		virtual void createVertexDeclaration();
		/// Implementation DynamicRenderable, pushes point list out to hardware memory
		virtual void fillHardwareBuffers();

		void _init(OperationType opType);

	private:
		Vector3List mPoints;
		bool mDirty;
		ColourValue mColour;
	};
