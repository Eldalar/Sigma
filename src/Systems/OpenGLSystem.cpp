#include "OpenGLSystem.h"
#include "GLSLShader.h"
#include "GLSixDOFView.h"
#include "../Components/GLSprite.h"
#include "../Components/GLIcoSphere.h"
#include "../Components/GLCubeSphere.h"
#include "../Components/GLMesh.h"

OpenGLSystem::OpenGLSystem() : windowWidth(800), windowHeight(600), deltaAccumulator(0.0) {
	this->view = new GLSixDOFView(); 
}

OpenGLSystem::~OpenGLSystem() {
	delete this->view;
	for (auto eitr = this->components.begin(); eitr != this->components.end(); ++eitr) {
		for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
			delete citr->second;
		}
	}
}

IGLComponent* OpenGLSystem::Factory(const std::string type, const unsigned int entityID, std::vector<Property> &properties) {
	if (type == "GLSprite") {
		GLSprite* spr = new GLSprite(entityID);
		spr->Initialize();
		if (entityID == 2) {
			spr->Transform()->Translate(2,2,0);
		}
		this->components[entityID][0] = spr;
		spr->Transform()->Translate(0,0,0);
		return spr;
	} else if (type == "GLIcoSphere") {
		GLIcoSphere* sphere = new GLIcoSphere(entityID);
		sphere->Initialize();
		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		int componentID = 0;
		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			Property*  p = &(*propitr);
			if (p->GetName() == "scale") {
				scale = p->Get<float>();
				continue;
			} else if (p->GetName() == "x") {
				x = p->Get<float>();
				continue;
			} else if (p->GetName() == "y") {
				y = p->Get<float>();
				continue;
			} else if (p->GetName() == "z") {
				z = p->Get<float>();
				continue;
			} else if (p->GetName() == "id") {
				componentID = p->Get<int>();
			}
		}
		sphere->Transform()->Scale(scale,scale,scale);
		sphere->Transform()->Translate(x,y,z);
		this->components[entityID][componentID] = sphere;
		return sphere;
	} else if (type == "GLCubeSphere") {
		GLCubeSphere* sphere = new GLCubeSphere(entityID);
		sphere->Initialize();
		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		int componentID = 0;
		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			Property*  p = &(*propitr);
			if (p->GetName() == "scale") {
				scale = p->Get<float>();
				continue;
			} else if (p->GetName() == "x") {
				x = p->Get<float>();
				continue;
			} else if (p->GetName() == "y") {
				y = p->Get<float>();
				continue;
			} else if (p->GetName() == "z") {
				z = p->Get<float>();
				continue;
			} else if (p->GetName() == "id") {
				componentID = p->Get<int>();
			}
		}
		sphere->Transform()->Scale(scale,scale,scale);
		sphere->Transform()->Translate(x,y,z);
		this->components[entityID][componentID] = sphere;
		return sphere;
	} else if (type=="GLMesh") {
		GLMesh* mesh = new GLMesh(entityID);
		float scale = 1.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		int componentID = 0;
		for (auto propitr = properties.begin(); propitr != properties.end(); ++propitr) {
			Property*  p = &*propitr;
			if (p->GetName() == "scale") {
				scale = p->Get<float>();
				continue;
			} else if (p->GetName() == "x") {
				x = p->Get<float>();
				continue;
			} else if (p->GetName() == "y") {
				y = p->Get<float>();
				continue;
			} else if (p->GetName() == "z") {
				z = p->Get<float>();
				continue;
			} else if (p->GetName() == "meshFile") {
				mesh->LoadMesh(p->Get<std::string>());
			} else if (p->GetName() == "id") {
				componentID = p->Get<int>();
			}
		}
		mesh->Initialize();
		mesh->Transform()->Scale(scale,scale,scale);
		mesh->Transform()->Translate(x,y,z);
		this->components[entityID][componentID] = mesh;
	}
	return nullptr;
}

bool OpenGLSystem::Update(const double delta) {
	this->deltaAccumulator += delta;
	// Check if the deltaAccumulator is greater than 1/60 of a second.
	if (deltaAccumulator > 16.7) {
		this->view->UpdateViewMatrix();
		// Set up the scene to a "clean" state.
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glViewport(0, 0, windowWidth, windowHeight); // Set the viewport size to fill the window  
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear required buffers

		// Set the ViewProjection matrix to be used in the shader.
		// Loop through and draw each component.
		for (auto eitr = this->components.begin(); eitr != this->components.end(); ++eitr) {
			for (auto citr = eitr->second.begin(); citr != eitr->second.end(); ++citr) {
				citr->second->Update(&this->view->ViewMatrix[0][0], &this->ProjectionMatrix[0][0]);
			}
		}

		this->deltaAccumulator = 0.0;
		return true;
	}
	return false;
}

IGLComponent* OpenGLSystem::GetComponent(const unsigned int entityID, const unsigned int componentID) {
	if (this->components.find(entityID) != this->components.end()) {
		if (this->components.at(entityID).find(componentID) != this->components.at(entityID).end()) {
			return this->components[entityID][componentID];
		}
	}
	return nullptr;
}

const int* OpenGLSystem::Start() {
	//Checking GL version
	const GLubyte *GLVersionString = glGetString(GL_VERSION);

	//Or better yet, use the GL3 way to get the version number
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

	// Now that GL is up and running load the shaders
	GLSprite::LoadShader();
	GLIcoSphere::LoadShader();
	GLCubeSphere::LoadShader();

	// Generates a really hard-to-read matrix, but a normal, standard 4x4 matrix nonetheless
	this->ProjectionMatrix = glm::perspective(
		45.0f,
		(float)this->windowWidth / (float)this->windowHeight,
		0.1f,
		10000.0f
		);

	this->view->Move(4.0f,3.0f,-10.f);

	// App specific global gl settings
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_MULTISAMPLE_ARB);

	return OpenGLVersion;
}

void OpenGLSystem::Move(float x, float y, float z) {
	this->view->Move(x,y,z);
}

void OpenGLSystem::Rotate(float x, float y, float z) {
	this->view->Rotate(x,y,z);
}

void OpenGLSystem::SetViewportSize(const unsigned int width, const unsigned int height) {
	this->windowHeight = height;
	this->windowWidth = width;
	this->ProjectionMatrix = glm::perspective(
		45.0f,
		(float)this->windowWidth / (float)this->windowHeight,
		0.1f,
		10000.0f
		);
}
