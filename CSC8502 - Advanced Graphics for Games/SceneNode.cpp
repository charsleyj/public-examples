// File:	SceneNode.cpp
// Author:	James Charsley (150338217)
// Date:	05/11/2019
// Desc:	Definition of a class that structures Meshes in a graph of nodes

#pragma once
#include <algorithm>
#include "SceneNode.h"

SceneNode::SceneNode(Mesh* m, Vector4 colour) :
	mesh(m),
	colour(colour),
	parent(NULL),
	modelScale(Vector3(1,1,1)),
	shader(NULL),
	boundingVolume(NULL),
	distanceFromCamera(0.0f) {
}

SceneNode::~SceneNode() {
	for (unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}

void SceneNode::addChild(SceneNode* s) {
	SceneNode* ansestor = parent;
	while (ansestor != NULL && ansestor != this)
		ansestor = ansestor->parent;
	if (ansestor == this)
		return;

	children.push_back(s);
	s->parent = this;
}

void SceneNode::remove() {
	if (this->parent) {
		vector<SceneNode*> sibs = this->parent->children;
		sibs.erase(std::remove(sibs.begin(), sibs.end(), this), sibs.end());
	}
	delete this;
}

void SceneNode::update(float msec) {
	if (parent)
		worldTransform = parent->worldTransform * transform;
	else
		worldTransform = transform;

	for (auto i = children.begin(); i != children.end(); ++i)
		(*i)->update(msec);
}

void SceneNode::initialiseUniforms(const OGLRenderer& r) {}

void SceneNode::draw(const OGLRenderer& r) {
	if (mesh)
		mesh->Draw();
}

