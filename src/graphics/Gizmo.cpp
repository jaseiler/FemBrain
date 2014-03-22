/*
 * Gizmo.cpp
 *
 *  Created on: Dec 28, 2013
 *      Author: pourya
 */
#include "Gizmo.h"
#include "selectgl.h"
#include "ShaderManager.h"
#include "base/Logger.h"

#define DEFAULT_AXIS_LENGTH 2.0f
#define DEFAULT_AXIS_THICKNESS 0.05f

using namespace PS::GL;

namespace PS {
    namespace SG {
        
        
        
        //The Effect for Gizmos
        class GizmoEffect : public SGEffect {
        public:
            GizmoEffect():m_idColor(-1) {
            }
            
            GizmoEffect(GLShader* s):SGEffect(s) {
                m_idColor = m_lpShader->getUniformLocation("color");
            }
            
            void setColor(const vec4f& color) {
                m_color = color;
                if(m_lpShader->isRunning())
                    m_lpShader->setUniform(m_idColor, m_color);
            }
            
            void bind() {
                SGEffect::bind();
                m_lpShader->setUniform(m_idColor, m_color);
            }
            
        private:
            vec4f m_color;
            int m_idColor;
            
        };
        
        //////////////////////////////////////////////////////////
        int GizmoInterface::setAxis(const PS::MATH::Ray &r) {
            //1 = x, 2 = y, 3 = z
        	int res = this->intersect(r);
            if(res > 0)
                this->setAxis((GizmoAxis)(res-1));

            //Set selected
            this->setSelected(res > 0);
            return res;
        }
        
        vec4f GizmoInterface::axisColor(GizmoAxis a) {
            if(a == m_axis)
                return vec4(0.7, 0.7, 0.7, 1.0);
            if(a == axisX)
                return vec4(1,0,0,1);
            else if(a == axisY)
                return vec4(0,1,0,1);
            else if(a == axisZ)
                return vec4(0,0,1,1);
            else
                return vec4(0.7, 0.7, 0.7, 1.0);
        }
      
        ///////////////////////////////////////////////////////////
        //The Translation Gizmo
        GizmoTranslate::GizmoTranslate(): GizmoInterface() {
            this->setName("GizmoTranslate");
            setup();
        }
        
        void GizmoTranslate::setup() {
            
            Geometry x, y, z;
            x.init(3, 4, 2, ftTriangles);
            x.addCircle3D(16, DEFAULT_AXIS_THICKNESS);
            x.extrude(vec3f(DEFAULT_AXIS_LENGTH, 0, 0));
            x.addCone(16, 0.2f, 0.4f, vec3f(DEFAULT_AXIS_LENGTH, 0, 0));
            x.computeNormalsFromFaces();
            y = x;
            quat q1;
            q1.fromAxisAngle(vec3f(0, 0, 1), 90.0f);
            y.transform(mat44f::quatToMatrix(q1));
            
            z = x;
            quat q2;
            q2.fromAxisAngle(vec3f(0, 1, 0), 90.0f);
            z.transform(mat44f::quatToMatrix(q2));
            
            x.addPerVertexColor(vec4f(1, 0, 0, 1), x.countVertices());
            y.addPerVertexColor(vec4f(0, 1, 0, 1), x.countVertices());
            z.addPerVertexColor(vec4f(0, 0, 1, 1), x.countVertices());
            
            
            //Setup
            m_x.setup(x);
            m_y.setup(y);
            m_z.setup(z);
            
            Geometry total = x + y + z;
            GLMeshBuffer::setup(total);
            
            if(TheShaderManager::Instance().has("gizmo")) {
                m_spEffect = SmartPtrSGEffect(new GizmoEffect(TheShaderManager::Instance().get("gizmo")));
            }
            
            this->setAxis(axisX);
        }
        
        void GizmoTranslate::draw() {
            //glClear(GL_DEPTH_BUFFER_BIT);

            //m_spTransform->bind();
            GizmoEffect* peff = dynamic_cast<GizmoEffect*>(m_spEffect.get());
            peff->bind();
            
            //X
            peff->setColor(axisColor(axisX));
            m_x.drawNoEffect();
            
            //Y
            peff->setColor(axisColor(axisY));
            m_y.drawNoEffect();
            
            //Z
            peff->setColor(axisColor(axisZ));
            m_z.drawNoEffect();
            
            m_spEffect->unbind();
           // m_spTransform->unbind();
        }
        
        
        int GizmoTranslate::intersect(const PS::MATH::Ray &r) {
            if(m_x.intersect(r))
                return 1;
            else if(m_y.intersect(r))
                return 2;
            else if(m_z.intersect(r))
                return 3;
            return -1;
        }
        //////////////////////////////////////////////////////////
        GizmoScale::GizmoScale(): GizmoInterface() {
            this->setName("GizmoScale");
            setup();
        }
        
        void GizmoScale::setup() {
            Geometry x, y, z;
            x.init(3, 4, 2, ftTriangles);
            x.addCircle3D(16, DEFAULT_AXIS_THICKNESS);
            
            vec3f c = vec3f(DEFAULT_AXIS_LENGTH, 0, 0);
            x.extrude(c);
            x.computeNormalsFromFaces();
            x.addCube(c - vec3f(0.15f), c + vec3f(0.15f));
            
            
            y = x;
            quat q1;
            q1.fromAxisAngle(vec3f(0, 0, 1), 90.0f);
            y.transform(mat44f::quatToMatrix(q1));
            
            z = x;
            quat q2;
            q2.fromAxisAngle(vec3f(0, 1, 0), 90.0f);
            z.transform(mat44f::quatToMatrix(q2));
            
            x.addPerVertexColor(vec4f(1, 0, 0, 1), x.countVertices());
            y.addPerVertexColor(vec4f(0, 1, 0, 1), x.countVertices());
            z.addPerVertexColor(vec4f(0, 0, 1, 1), x.countVertices());
            
            
            //Setup
            m_x.setup(x);
            m_y.setup(y);
            m_z.setup(z);
            
            Geometry total = x + y + z;
            GLMeshBuffer::setup(total);
            
            if(TheShaderManager::Instance().has("gizmo")) {
                m_spEffect = SmartPtrSGEffect(new GizmoEffect(TheShaderManager::Instance().get("gizmo")));
            }
            
            this->setAxis(axisX);
        }
        
        void GizmoScale::draw() {
            glClear(GL_DEPTH_BUFFER_BIT);
            m_spTransform->bind();
            GizmoEffect* peff = dynamic_cast<GizmoEffect*>(m_spEffect.get());
            peff->bind();
            
            //X
            peff->setColor(axisColor(axisX));
            m_x.drawNoEffect();
            
            //Y
            peff->setColor(axisColor(axisY));
            m_y.drawNoEffect();
            
            //Z
            peff->setColor(axisColor(axisZ));
            m_z.drawNoEffect();
            
            m_spEffect->unbind();
            m_spTransform->unbind();
        }
        
        int GizmoScale::intersect(const PS::MATH::Ray &r) {
            if(m_x.intersect(r))
                return 1;
            else if(m_y.intersect(r))
                return 2;
            else if(m_z.intersect(r))
                return 3;
            return -1;
        }

        //////////////////////////////////////////////////////////
        GizmoRotate::GizmoRotate(): GizmoInterface() {
            this->setName("GizmoRotate");
            setup();
        }
        
        void GizmoRotate::setup() {
            Geometry x, y, z;
            x.init(3, 4, 2, ftLineStrip);
            x.addCircle2D(64, DEFAULT_AXIS_LENGTH);
            
            y = x;
            quat q1;
            q1.fromAxisAngle(vec3f(0, 0, 1), 90.0f);
            y.transform(mat44f::quatToMatrix(q1));
            
            z = x;
            quat q2;
            q2.fromAxisAngle(vec3f(0, 1, 0), 90.0f);
            z.transform(mat44f::quatToMatrix(q2));
            
            x.addPerVertexColor(vec4f(1, 0, 0, 1), x.countVertices());
            y.addPerVertexColor(vec4f(0, 1, 0, 1), x.countVertices());
            z.addPerVertexColor(vec4f(0, 0, 1, 1), x.countVertices());
            
            
            //Setup
            m_x.setup(x);
            m_y.setup(y);
            m_z.setup(z);
            
            Geometry total = x + y + z;
            GLMeshBuffer::setup(total);
            
            if(TheShaderManager::Instance().has("gizmo")) {
                m_spEffect = SmartPtrSGEffect(new GizmoEffect(TheShaderManager::Instance().get("gizmo")));
            }
            
            this->setAxis(axisX);
        }
        
        void GizmoRotate::draw() {
            glClear(GL_DEPTH_BUFFER_BIT);
            
            m_spTransform->bind();
            GizmoEffect* peff = dynamic_cast<GizmoEffect*>(m_spEffect.get());
            peff->bind();
            
            //X
            peff->setColor(axisColor(axisX));
            m_x.drawNoEffect();
            
            //Y
            peff->setColor(axisColor(axisY));
            m_y.drawNoEffect();
            
            //Z
            peff->setColor(axisColor(axisZ));
            m_z.drawNoEffect();
            
            m_spEffect->unbind();
            m_spTransform->unbind();
        }
        
        int GizmoRotate::intersect(const PS::MATH::Ray &r) {
            if(m_x.intersect(r))
                return 1;
            else if(m_y.intersect(r))
                return 2;
            else if(m_z.intersect(r))
                return 3;
            return -1;
        }

        
        //////////////////////////////////////////////////////////
        GizmoAvatar::GizmoAvatar(): GizmoInterface() {
            this->setName("GizmoAvatar");
            setup();
        }
        
        void GizmoAvatar::setup() {
            
        }
        ////////////////////////////////////////////////////////////
        GizmoManager::GizmoManager():SGNode() {
            m_buttonState = ArcBallCamera::bsUp;
            m_pressedPos = vec2i(0, 0);

            //Create All needed Gizmo Widgets
            m_lpGizmoTranslate = new GizmoTranslate();
            m_lpGizmoScale = new GizmoScale();
            m_lpGizmoRotate = new GizmoRotate();
            m_lpGizmoAvatar = new GizmoAvatar();
            
            m_gizmoType = gtTranslate;
            m_gizmoAxis = axisFree;
            m_lpGizmoCurrent = m_lpGizmoTranslate;

            this->resetTransform();
        }
        
        GizmoManager::~GizmoManager() {
            SAFE_DELETE(m_lpGizmoTranslate);
            SAFE_DELETE(m_lpGizmoScale);
            SAFE_DELETE(m_lpGizmoRotate);
            SAFE_DELETE(m_lpGizmoAvatar);
        }
        
        void GizmoManager::draw() {
            if(m_lpGizmoCurrent && isVisible()) {
            	glPushMatrix();
            	glTranslatef(m_pos.x, m_pos.y, m_pos.z);

                m_lpGizmoCurrent->draw();

                glPopMatrix();
            }
        }
        
        int GizmoManager::intersect(const Ray& r) {
            if(m_lpGizmoCurrent)
                return m_lpGizmoCurrent->intersect(r);
            else            
                return -1;
        }
        
        void GizmoManager::setAxis(const Ray& r) {
            if(m_lpGizmoCurrent) {
                if(m_lpGizmoCurrent->setAxis(r)) {
                	m_gizmoAxis = m_lpGizmoCurrent->axis();
                	LogInfoArg1("Set selected axis to: %d", m_gizmoAxis);
                }

                //Selection
                this->setSelected(m_lpGizmoCurrent->isSelected());
            }
        }
        
        void GizmoManager::setAxis(GizmoAxis axis) {
        	m_gizmoAxis = axis;
            if(m_lpGizmoCurrent) {
            	m_lpGizmoCurrent->setTransform(this->transform());
            	m_lpGizmoCurrent->setAxis(axis);
            }
        }
        
        void GizmoManager::setType(GizmoType gtype) {
            m_gizmoType = gtype;
            switch (gtype) {
                case gtTranslate:
                    m_lpGizmoCurrent = m_lpGizmoTranslate;
                    break;
                case gtScale:
                    m_lpGizmoCurrent = m_lpGizmoScale;
                    break;
                case gtRotate:
                    m_lpGizmoCurrent = m_lpGizmoRotate;
                    break;
                case gtAvatar:
                    m_lpGizmoCurrent = m_lpGizmoAvatar;
                    break;
                    
                default:
                    break;
            }
            
            //Apply the selected axis to this Gizmo
            m_lpGizmoCurrent->setAxis(m_gizmoAxis);
            m_lpGizmoCurrent->setTransform(this->transform());
            glutPostRedisplay();
        }
        
        void GizmoManager::mousePress(int button, int state, int x, int y) {
            ArcBallCamera::MouseButton b = (ArcBallCamera::MouseButton)button;
            m_buttonState = (ArcBallCamera::ButtonState)state;
            if(b == ArcBallCamera::mbLeft && m_buttonState == ArcBallCamera::bsDown) {
                Ray r = TheSceneGraph::Instance().screenToWorldRay(x, y);
                r.setStart(r.getStart() - m_pos);
                setAxis(r);
            }

            m_pressedPos = vec2i(x, y);
        }

        void GizmoManager::mouseMove(int x, int y) {
        	printf("Button %d\n", m_buttonState);
        	if(m_buttonState != ArcBallCamera::bsDown)
        		return;


        	float dx = m_pressedPos.x - x;
        	float dy = m_pressedPos.y - y;

        	dx *= 0.005f;
        	dy *= 0.005f;
        	m_pressedPos = vec2i(x, y);

        	string strAxis;
        	vec3f delta(0.0f);

        	switch (m_gizmoAxis) {
        	case axisX:
        		delta.x = dx;
        		strAxis = "X";
        		break;
        	case axisY:
        		delta.y = dy;
        		strAxis = "Y";
        		break;
        	case axisZ:
        		delta.z = dx;
        		strAxis = "Z";
        		break;

        	case axisFree:
        		delta = delta + vec3f(dx, dy, 0.0);
        		strAxis = "FREE";
        		break;

        	case axisCount: {

        	}
        	break;
        	}


        	//apply
        	m_pos = m_pos + delta;
        	//transform()->translate(delta);

//        	char buffer[1024];
//        	sprintf(buffer,
//        			"HAPTIC DELTA=(%.4f, %.4f), AVATAR=(%.4f, %0.4f, %.4f), AXIS=%s PRESS F4 To Change.",
//        			dx, dy, wpos.x, wpos.y, wpos.z, strAxis.c_str());
        	//g_infoLines[INDEX_HAPTIC_INFO] = string(buffer);


        }


    }
}

