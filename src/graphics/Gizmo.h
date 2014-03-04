/*
 * Gizmo.h
 *
 *  Created on: Dec 28, 2013
 *      Author: pourya
 */

#ifndef GIZMO_H_
#define GIZMO_H_

#include "SGMesh.h"
#include <loki/Singleton.h>

using namespace PS::GL;

/*!
 * Create Affine widgets for controlling transformations or Avatar for interaction with model.
 */

namespace PS {
namespace SG {

enum GizmoAxis {axisX, axisY, axisZ, axisFree, axisCount};
enum GizmoType {gtTranslate, gtRotate, gtScale, gtAvatar, gtCount};

//Interface for all affine Gizmo Widgets
class GizmoInterface : public SGMesh {
public:
	GizmoInterface() {m_axis = axisFree;}
	virtual ~GizmoInterface() {}

    //Axis
    GizmoAxis axis() const {return m_axis;}
    void setAxis(GizmoAxis axis) {m_axis = axis;}
    void setAxis(const Ray& r);
    
    vec4f axisColor(GizmoAxis a);
protected:
	GizmoAxis m_axis;
};

//Translate
class GizmoTranslate : public GizmoInterface {
public:
	GizmoTranslate();

	void draw();
    int intersect(const Ray& r);
protected:
	void setup();

private:
	SGMesh m_x;
	SGMesh m_y;
	SGMesh m_z;
};

//Scale
class GizmoScale : public GizmoInterface {
public:
	GizmoScale();
protected:
	void setup();

private:
	SGMesh m_x;
	SGMesh m_y;
	SGMesh m_z;
};

//Rotate
class GizmoRotate : public GizmoInterface {
public:
	GizmoRotate();

protected:
	void setup();

private:
	SGMesh m_x;
	SGMesh m_y;
	SGMesh m_z;
};

//Avatar
class GizmoAvatar : public GizmoInterface {
public:
	GizmoAvatar();

protected:
	void setup();
};

/*!
 * Gizmo Manager controls the UI Widgets for sketching, transforming, cutting and poking of
 * deformable models.
 */
class GizmoManager : public SGNode {
public:
	GizmoManager();
	virtual ~GizmoManager();

	void draw();
    int intersect(const Ray& r);

    //Get Transform
    vec3f translate() const {return m_translate;}
    void setTranslate(const vec3f& v) {m_translate = v;}


    //Get
    GizmoType gizmoType() const {return m_gizmoType;}
    GizmoAxis axis() const {return m_gizmoAxis;}


    //Set Gizmo Type and Axis
    void setAxis(const Ray& r);
    void setAxis(GizmoAxis axis);
	void setType(GizmoType gtype);

	//Return current Gizmo Type
	GizmoInterface* current() const {return m_lpGizmoCurrent;}
private:
	GizmoTranslate* m_lpGizmoTranslate;
	GizmoScale* m_lpGizmoScale;
	GizmoRotate* m_lpGizmoRotate;
	GizmoAvatar* m_lpGizmoAvatar;

	GizmoInterface* m_lpGizmoCurrent;
	GizmoType m_gizmoType;
	GizmoAxis m_gizmoAxis;

	//Transform
	vec3f m_translate;
	vec3f m_scale;
	quat m_rotate;
};

//Singleton for UI Gizmo Manager
typedef SingletonHolder<GizmoManager, CreateUsingNew, PhoenixSingleton> TheGizmoManager;



}
}


#endif /* GIZMO_H_ */
