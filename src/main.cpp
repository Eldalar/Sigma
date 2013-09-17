#include <iostream>

#include "systems/OpenGLSystem.h"
#include "systems/SimplePhysics.h"
#include "systems/FactorySystem.h"
#include "controllers/GLSixDOFViewController.h"
#include "components/ViewMover.h"
#include "SCParser.h"

#ifdef OS_Win32
#include "os/win32/win32.h"
#elif OS_SDL
#include "os/sdl/SDLSys.h"
#endif

int main(int argCount, char **argValues) {
	OpenGLSystem glsys;
	SimplePhysics physys;
	FactorySystem::getInstance().register_Factory(&glsys);
	FactorySystem::getInstance().register_Factory(&physys);
	IOpSys* os = nullptr;
#ifdef OS_Win32
	os = new win32();
#elif OS_SDL
	os = new SDLSys();
#endif

	os->CreateGraphicsWindow(1024,768);

	const int* version = glsys.Start();
	glsys.SetViewportSize(os->GetWindowWidth(), os->GetWindowHeight());

	if (version[0] == -1) {
		std::cout<< "Error starting OpenGL!"<<std::endl;
	} else {
		std::cout<<"OpenGL version: " << version[0] << "." << version[1] << std::endl;
	}

	Sigma::parser::SCParser parser;

	parser.Parse("test.sc");

	for (unsigned int i = 0; i < parser.EntityCount(); ++i) {
		const Sigma::parser::Entity* e = parser.GetEntity(i);
		for (auto itr = e->components.begin(); itr != e->components.end(); ++itr) {
            FactorySystem::getInstance().create(
                        itr->type,e->id,
                        const_cast<std::vector<Property>&>(itr->properties));
		}
	}

	std::vector<Property> props;
	physys.Factory("ViewMover", 9, props);
	ViewMover* mover = reinterpret_cast<ViewMover*>(physys.GetComponent(9));

	Sigma::event::handler::GLSixDOFViewController cameraController(glsys.View(), mover);
	IOpSys::KeyboardEventSystem.Register(&cameraController);


	os->SetupTimer();

	double delta;
	bool isWireframe=false;

	while (os->MessageLoop()) {
		delta = os->GetDeltaTime();
		float deltaSec = (float)delta/1000.0f;

		// Translation keys
		/*if (os->KeyDown('W', true)) { // Move forward
			if (os->KeyDown('B', true)) {
				glsys.Move(0.0f, 0.0f, 100.0f*deltaSec);
			} else {
				glsys.Move(0.0f, 0.0f, 10.0f*deltaSec);
			}
		} else if (os->KeyDown('S', true)) { // Move backward
			glsys.Move(0.0f, 0.0f, -10.0f*deltaSec);
		}

		if (os->KeyDown('A', true)) {
			glsys.Rotate(0.0f, -90.0f*deltaSec, 0.0f); // Yaw left.
		} else if (os->KeyDown('D', true)) {
			glsys.Rotate(0.0f, 90.0f*deltaSec, 0.0f); // Yaw right.
		}

		if (os->KeyDown('F', true)) {
			glsys.Move(-10.0f*deltaSec, 0.0f, 0.0f); // Strafe Left
		} else if (os->KeyDown('G', true)) {
			glsys.Move(10.0f*deltaSec, 0.0f, 0.0f); // Strafe Right
		}

		if (os->KeyDown('E', true)) { // Move up
			glsys.Move(0.0f, 10.0f*deltaSec, 0.0f);
		} else if (os->KeyDown('C', true)) { // Move down
			glsys.Move(0.0f, -10.0f*deltaSec, 0.0f);
		}

		if (os->KeyDown('Q', true)) { // Pitch Up
			glsys.Rotate(-90.0f * deltaSec, 0.0f, 0.0f);
		} else if (os->KeyDown('Z', true)) { // Pitch Down
			glsys.Rotate(90.0f*deltaSec, 0.0f, 0.0f);
		}

		if (os->KeyDown('R', true)) { // Roll left
			glsys.Rotate(0.0f, 0.0f, -90.0f * deltaSec);
		} else if (os->KeyDown('T', true)) { // Roll right
			glsys.Rotate(0.0f, 0.0f, 90.0f*deltaSec);
		}*/

		if (os->KeyReleased('P', true)) { // Wireframe mode
			if (!isWireframe) {
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				isWireframe = true;
			} else {
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				isWireframe = false;
			}
		}
		if (os->KeyReleased('M', true)) {
			os->ToggleFullscreen();
			glsys.SetViewportSize(os->GetWindowWidth(), os->GetWindowHeight());
		}

		physys.Update(delta);

		if (glsys.Update(delta)) {
			os->Present();
		}
	}

	delete os;

	return 0;
}
