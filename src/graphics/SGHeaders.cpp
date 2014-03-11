/*
 * SGHeaders.cpp
 *
 *  Created on: Mar 8, 2014
 *      Author: pourya
 */

#include "SGHeaders.h"
#include "selectgl.h"
#include "GLFuncs.h"

namespace PS {
namespace SG {

SGHeaders::SGHeaders():SGNode() {
	setName("headers");
}

SGHeaders::~SGHeaders() {
	cleanup();
}

void SGHeaders::cleanup() {
	m_vHeaders.resize(0);
	m_hashHeaders.cleanup();
}

void SGHeaders::draw() {
	for(int i=0; i<m_vHeaders.size(); i++)
		DrawText(m_vHeaders[i].cptr(), 10, 20 + i * 15);
}


int SGHeaders::addHeaderLine(const AnsiStr& title, const AnsiStr& strInfo) {
	m_vHeaders.push_back(strInfo);
	int id = m_vHeaders.size() - 1;
	m_hashHeaders.add(id, title.cptr());
	return id;
}

int SGHeaders::getHeaderId(const char* title) {
	return m_hashHeaders.get(title);
}

bool SGHeaders::removeHeaderLine(const char* title) {
	int id = m_hashHeaders.get(title);
	if(id >= 0) {
		m_vHeaders.erase(m_vHeaders.begin() + id);
		m_hashHeaders.remove(title);
	}
	return (id >= 0);
}

void SGHeaders::removeAllHeaders() {
	cleanup();
}

bool SGHeaders::updateHeaderLine(int id, const AnsiStr& strInfo) {
	if(id < 0 || id >= m_vHeaders.size())
		return false;

	m_vHeaders[id] = strInfo;
	return true;
}

bool SGHeaders::updateHeaderLine(const AnsiStr& title, const AnsiStr& strInfo) {
	return updateHeaderLine(getHeaderId(title.cptr()), strInfo);
}

void SGHeaders::DrawText(const char* chrText, int x, int y) {
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, vp[2], vp[3], 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	float clFont[] = { 0, 0, 1, 1 };
	DrawString(chrText, x, y, clFont, GLUT_BITMAP_8_BY_13);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

}
}
