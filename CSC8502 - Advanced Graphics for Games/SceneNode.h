// File:	SceneNode.h
// Author:	James Charsley (150338217)
// Date:	05/11/2019
// Desc:	Declaration of a class that structures Meshes in a graph of nodes

#pragma once
#include <vector>
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mesh.h"
#include "BoundingVolume.h"

class SceneNode {
	
public:
	SceneNode(Mesh* m = NULL, Vector4 colour = Vector4(1, 1, 1, 1));
	~SceneNode();

	void setTransform(const Matrix4& matrix) { transform = matrix; }
	const Matrix4& getTransform() const { return transform; }
	Matrix4 getWorldTransform() const { return worldTransform; }

	Vector4 getColour() const { return colour; }
	void setColour(Vector4 c) { colour = c; }

	Vector3 getModelScale() const { return modelScale; }
	void setModelScale(Vector3 s) { modelScale = s; }

	Mesh* getMesh() const { return mesh; }
	void setMesh(Mesh* m) { mesh = m; }

	void addChild(SceneNode* s);
	void remove();

	virtual void update(float msec);
	virtual void initialiseUniforms(const OGLRenderer& r);
	virtual void draw(const OGLRenderer& r);

	bool hasShader() const { return shader == NULL; }
	Shader* getShader() const { return shader; }
	void setShader(Shader* s) { shader = s; }

	SceneNode* getParent() const { return parent; }

	std::vector<SceneNode*>::const_iterator getStartChild() {
		return children.begin();
	}

	std::vector<SceneNode*>::const_iterator getEndChild() {
		return children.end();
	}

	BoundingVolume* getBoundingVol() const { return boundingVolume; }
	void setBoundingVol(BoundingVolume* vol) {
		boundingVolume = vol;
		boundingVolume->setParent(this);
	}
	
	float getCameraDistance() const { return distanceFromCamera; }
	void setCameraDistance(float f) { distanceFromCamera = f; }

	static bool compareByCameraDistance(SceneNode* a, SceneNode* b) {
		return (a->distanceFromCamera < b->distanceFromCamera);
	}

protected:
	SceneNode* parent;
	Mesh* mesh;
	Matrix4 worldTransform;
	Matrix4 transform;
	Vector3 modelScale;
	Vector4 colour;
	std::vector<SceneNode*> children;
	Shader* shader;

	BoundingVolume* boundingVolume;
	float distanceFromCamera;

};
