/////////////////////////////////////////////
//main.cpp
//Copyright(c) 2019 Luta Vlad
//https://github.com/meemknight/OpenGLEngine
/////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <Windows.h>
#include <cmath>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
//#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletDynamics/Character/btCharacterControllerInterface.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include <SFML/Graphics.hpp>

#include "Camera.h"
#include "GameObject.h"
#include "Light.h"
#include "customBulletdebuggClass.h"
#include "shapesGenerator.h"
#include "GameObjectPool.h"
#include "CharacterMouseController.h"
#include "ParticleSystem.h"

#include "tools.h"

extern "C"
{
	//Enable dedicated graphics
	//__declspec(dllexport) DWORD NvOptimusEnablement = true;
	//__declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = true;
}

float width = 1280;
float height = 720;

using glm::mat4;

btDynamicsWorld *world;
btDispatcher *dispatcher;
btBroadphaseInterface *broadPhase;
btConstraintSolver *solver;
btCollisionConfiguration *collisionConfiguration;

btRigidBody addSphere(float rad, float x, float y, float z, float mass)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin({ x, y, z });

	btSphereShape *sphere = new btSphereShape(rad);
	btVector3 inertia = { 0,0,0 };
	if (mass != 0.0)
	{
		sphere->calculateLocalInertia(mass, inertia);
	}
	btMotionState *motion = new btDefaultMotionState(t);
	
	//btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);
	btRigidBody body(mass, motion, sphere, inertia);
	
	return body;
}


int main()
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	broadPhase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();

	world = new btDiscreteDynamicsWorld(dispatcher, broadPhase, solver, collisionConfiguration);
	world->setGravity({ 0, -9.81f, 0 });
	

	btTransform t;
	t.setIdentity();
	t.setOrigin({0, -1, 0});
	
	btStaticPlaneShape *plane = new btStaticPlaneShape({0,1,0}, 0);
	btMotionState *motion =new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0f, motion, plane);
	btRigidBody body(info);

	world->addRigidBody(&body);

	//auto sphere = addSphere(1, -6, 2, -6, 10);
	//world->addRigidBody(&sphere);
	//sphere.applyImpulse({ 10,0,0 }, { 0,0,0 });


	sf::ContextSettings contextSettings;
	contextSettings.depthBits = 24;
	contextSettings.stencilBits = 8;
	contextSettings.antialiasingLevel = 2;
	//contextSettings.majorVersion = 3;
	//contextSettings.minorVersion = 3;

	sf::RenderWindow window(sf::VideoMode(width, height), "glEngine", sf::Style::Default, contextSettings);

	auto windoHandle = window.getSystemHandle();

	window.setVerticalSyncEnabled(false);

	glewInit();
	glewExperimental = GL_TRUE;
	glEnable(GL_DEPTH_TEST);
	//glBlendFunc(GL_DST_COLOR, GL_ZERO);

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
#endif
	
	glEnable(GL_CULL_FACE);


	float cube2[] = {
	-1.0f, +1.0f, +1.0f, // 0
	+1.0f, +0.0f, +0.0f, // Color
	+1.0f, +1.0f, +1.0f, // 1
	+1.0f,  0.0f, +0.0f, // Color
	+1.0f, +1.0f, -1.0f, // 2
	+1.0f, +0.0f, +0.0f, // Color
	-1.0f, +1.0f, -1.0f, // 3
	+1.0f, +0.0f, +0.0f, // Color
	-1.0f, +1.0f, -1.0f, // 4
	+0.0f, +1.0f, +0.0f, // Color
	+1.0f, +1.0f, -1.0f, // 5
	+0.0f, +1.0f, +0.0f, // Color
	+1.0f, -1.0f, -1.0f, // 6
	+0.0f, +1.0f, +0.0f, // Color
	-1.0f, -1.0f, -1.0f, // 7
	+0.0f, +1.0f, +0.0f, // Color
	+1.0f, +1.0f, -1.0f, // 8
	+0.0f, +0.0f, +1.0f, // Color
	+1.0f, +1.0f, +1.0f, // 9
	+0.0f, +0.0f, +1.0f, // Color
	+1.0f, -1.0f, +1.0f, // 10
	+0.0f, +0.0f, +1.0f, // Color
	+1.0f, -1.0f, -1.0f, // 11
	+0.0f, +0.0f, +1.0f, // Color
	-1.0f, +1.0f, +1.0f, // 12
	+1.0f, +1.0f, +0.0f, // Color
	-1.0f, +1.0f, -1.0f, // 13
	+1.0f, +1.0f, +0.0f, // Color
	-1.0f, -1.0f, -1.0f, // 14
	+1.0f, +1.0f, +0.0f, // Color
	-1.0f, -1.0f, +1.0f, // 15
	+1.0f, +1.0f, +0.0f, // Color
	+1.0f, +1.0f, +1.0f, // 16
	+0.0f, +1.0f, +1.0f, // Color
	-1.0f, +1.0f, +1.0f, // 17
	+0.0f, +1.0f, +1.0f, // Color
	-1.0f, -1.0f, +1.0f, // 18
	+0.0f, +1.0f, +1.0f, // Color
	+1.0f, -1.0f, +1.0f, // 19
	+0.0f, +1.0f, +1.0f, // Color
	+1.0f, -1.0f, -1.0f, // 20
	+1.0f, +0.0f, +1.0f, // Color
	-1.0f, -1.0f, -1.0f, // 21
	+1.0f, +0.0f, +1.0f, // Color
	-1.0f, -1.0f, +1.0f, // 22
	+1.0f, +0.0f, +1.0f, // Color
	+1.0f, -1.0f, +1.0f, // 23
	+1.0f, +0.0f, +1.0f, // Color
	};

	unsigned int cubeIndices[] = {
	0,   1,  2,  0,  2,  3, // Top
	4,   5,  6,  4,  6,  7, // Front
	8,   9, 10,  8, 10, 11, // Right
	12, 13, 14, 12, 14, 15, // Left
	16, 17, 18, 16, 18, 19, // Back
	20, 22, 21, 20, 23, 22, // Bottom
	};

	AssetManager<Texture> textureManager;
	AssetManager<LoadedIndexModel> modelManager;
	LightContext light;
	light.pushElement(Light::roomLight());

	Camera camera(85.f, &width, &height, 0.01f, 1500.f);
	camera.mSpeed = 16.0f;

	camera.position = { 0, 3, -4 };
	//camera.viewDirection = { 0, 0, 1 };
	///
	camera.firstPersonCamera = true;
	camera.distanceFromPlayer = 8;
	camera.cameraAngle = glm::radians(25.f);
	camera.topDownAngle = 3.141f;

	float playerAngle = 0;
	float playerRotationSpeed = glm::radians(120.f);
	float mouseScroll = 0;

	CharacterMouseController characterController;
	characterController.topDownRotation = &playerAngle;
	characterController.cameraAngle = &camera.cameraAngle;
	characterController.downMax = glm::radians(-45.f);
	characterController.mouseScroll = &camera.distanceFromPlayer;

	ShaderProgram program(VertexShader("vertex.vert"), FragmentShader("fragment.frag"));
	ShaderProgram normalProgram(VertexShader("vertn.vert"), FragmentShader("fragn.frag"));
	ShaderProgram textureProgram(VertexShader("vertt.vert"), FragmentShader("fragt.frag"));
	ShaderProgram debugShader(VertexShader("debugShader.vert"), FragmentShader("debugShader.frag"));
	ShaderProgram particleShader({ "particleVert.vert" }, { "particleFrag.frag" });

	customBulletdebuggClass debugDrawer(&debugShader ,&camera);

	world->setDebugDrawer(&debugDrawer);
	world->getDebugDrawer()->setDebugMode(btIDebugDraw::DebugDrawModes::DBG_DrawWireframe);

	GameObjectPool gameObjectPool;
	gameObjectPool.initialize(&textureProgram, &camera, &light, world, &textureManager, &modelManager, &particleShader);
	gameObjectPool.load("maps//map1.txt");
	auto dataFromFile = gameObjectPool.emptyObjectVector.getElementById(422);
	ilog(dataFromFile.x, dataFromFile.y, dataFromFile.z);

	//gameObjectPool.phisicalObjectVector.getElementById(250).objectData[0].material = Material::ruby();

	//GameObject tempObject(&textureProgram, &camera, &light);
	//tempObject.loadPtn323()

	float *planVertexes = 0;
	float *planVertexes2 = 0;

	unsigned int *planIndices = 0;
	unsigned int *planIndices2 = 0;

	int plansize = 0;
	int plansize2 = 0;
	int planIndicessize = 0;
	int planIndicessize2 = 0;


	//generateMountains(&planVertexes2, &planIndices2, 2, plansize2, planIndicessize2);
	shapeGenerator::generatePlane(&planVertexes, &planIndices, 512, plansize, planIndicessize);
	ilog(glGetString(GL_VERSION));

	indexBuffer ib(cubeIndices, sizeof(cubeIndices));

	//light.pushElement(Light::roomLight(0.3f));
	//light.pushElement(Light::SunLight());
	//light.getAmbience(0)  = glm::vec3(0.5, 0.1, 0.1);
	//light.getDiffuseness(0)  = glm::vec3(1.f, 0.4, 0.4);
	//light.getSpecularity(0) = glm::vec3(1.0, 0.2, 0.2);
	//light.getStrength(0) = 0.0003;
	//light.getPosition(1).y = 20;
	//light.getStrength(1) = 0.0003f;


	//ComplexObject o2;
	//o2.camera = &camera;
	//o2.loadPtn323("objects//fireMonkey.obj", textureProgram, &manager);
	//o2.pushElement();
	//o2.getInstance(0).setPosition(0, 10, 0);

	GameObject lightObject;
	lightObject.setData(vertexBuffer(cube2, sizeof(cube2)), indexBuffer(cubeIndices, sizeof(cubeIndices)), vertexAttribute({ 3,3 }), &program, &camera);
	lightObject.pushElement(glm::mat4(0));
	glm::vec3 lightPosition = { 1, 1, -5 };

	GameObject plan(vertexBuffer(planVertexes, plansize * 4), indexBuffer(planIndices, planIndicessize * 4), vertexAttribute({ 3,3,3 }), &normalProgram, &camera);
	plan.pushElement(glm::mat4(0));
	plan.getInstance(0).setPosition(0, -1, 0);
	plan.getInstance(0).setRotation(0, 0, 0);
	plan.getInstance(0).setScale(1);
	plan.setMaterial(Material::greyMaterial(1, 0.5f, 0.01f, 1));


	sf::Clock c;
	sf::Clock fpsClock;
	bool updatemouse = 0;
	int frames = 0;
	//window.setMouseCursorVisible(0);

	PhisicalObject playerObject(&camera, &textureProgram, &light, world, nullptr/*new btSphereShape(1)*/, 10);
	//playerObject.loadCollisionBox("objects//fireMonkey.obj", nullptr);
	//playerObject.loadPtn323("objects//fireMonkey.obj", manager);
	playerObject.loadCollisionBox(modelManager.getData("objects//sphere.obj"), nullptr);
	playerObject.loadPtn323(modelManager.getData("objects//sphere.obj"), textureManager);
	//playerObject.appendObject(lmodel, manager, {0, 0, 3});

	
	//playerObject.objectData[0].material = Material::emerald();
	//playerObject.objectData[0].texture = manager.getTexture("textures//cobble.jpg");

	playerObject.pushElement({ 0, 3 ,0 });

	//playerObject.getIndtance(0)->setFriction(0.5f);
	

	//playerObject.getIndtance(0)->setCollisionFlags(playerObject.getIndtance(0)->getCollisionFlags() |
	//btCollisionObject::CF_KINEMATIC_OBJECT);
	//playerObject.getIndtance(0)->setActivationState(DISABLE_DEACTIVATION);

	ParticleSystem particles(400, 8, &particleShader, { 1,0,0 }, { 1,1,0 }, { -1,-1,-1 }, {1,1,1}, 0.5, 0.5);
	particles.camera = &camera;
	particles.light = &light;
	particles.affectedByLight = true;
	particles.fadeColor = { 0,0,1 };
	particles.fadeWeight = -4;
	particles.scale = 0.2f;
	particles.gravity = { 0,0,0 };
	particles.kd = 0.2f;

	ParticleSystem particles2("particles/test1.part");
	particles2.setup(&particleShader, &light, &camera);
	particles2.position = { 0,10,0 };

	window.setTitle((char*)glGetString(GL_RENDERER));
	while (window.isOpen())
	{
		float deltatime = c.restart().asSeconds();
		if(deltatime > (1.f/4.f))
		{
			deltatime = 1.f / 4.f;
		}

		frames++;
		if (fpsClock.getElapsedTime().asSeconds() >= 1)
		{
			float time = fpsClock.restart().asSeconds();
			sf::String t;
			time = frames / time;
			char c[12];
			sprintf(c, "%f", time);
			window.setTitle(c);
			frames = 0;
		}

		auto pos = playerObject.getInstance(0)->getCenterOfMassPosition();
		//particles.position = glm::vec3{ pos.x(),pos.y(),pos.z() } + glm::vec3{0, 1, 0};

		//glClearColor(0.1, 0.5, 1.0, 1.0);
		glViewport(0, 0, width, height);

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		mouseScroll *= 0.4f;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				//std::cin.get();
				exit(0);
			}
			else
			if (event.type == sf::Event::Resized)
			{
				width = window.getSize().x;
				height = window.getSize().y;

			}
			else
			if (event.type == sf::Event::MouseLeft)
			{
				updatemouse = 0;
			}
			else
			if (event.type == sf::Event::MouseEntered)
			{
				//updatemouse = 1;
				//camera.oldMousePosition = { sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y };
			}
			else
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				mouseScroll = event.mouseWheelScroll.delta;
			}else
			if(event.type == sf::Event::GainedFocus)
			{
				updatemouse = 1;
				camera.oldMousePosition = { sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y };
			}

		}

#pragma region keys
		if (sf::Keyboard::isKeyPressed((sf::Keyboard::Escape)))
		{
			exit(0);
			//ShowWindow((HWND)(windoHandle), SW_SHOWNOACTIVATE);
			//SendMessage((HWND)windoHandle, WM_KILLFOCUS, 0, 0);
			//updatemouse = 0;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			camera.moveFront(deltatime);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			camera.moveBack(deltatime);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			camera.moveLeft(deltatime);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			camera.moveRight(deltatime);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			camera.moveUp(deltatime);
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		{
			camera.moveDown(deltatime);
		}

		//
		float lightSpeed = 4.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
		{
			lightPosition.y += lightSpeed * deltatime;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
		{
			lightPosition.y -= lightSpeed * deltatime;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			lightPosition.z += lightSpeed * deltatime;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			lightPosition.z -= lightSpeed * deltatime;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			lightPosition.x += lightSpeed * deltatime;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			lightPosition.x -= lightSpeed * deltatime;
		}


		

		if (updatemouse)
		{
			if (!camera.firstPersonCamera)
			{
				camera.mouseUpdate({ (float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y }, window);
			}
			else
			{
				characterController.update({ (float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y }, true, mouseScroll, window);
			}

		}

		if (window.hasFocus())
		{
			window.setMouseCursorVisible(0);
			updatemouse = 1;
		}
		else
		{
			window.setMouseCursorVisible(1);
			updatemouse = 0;
		}

		 float maxSpeed = 35000 * deltatime;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
		{
			playerObject.getInstance(0)->applyCentralForce({ 0,0,-maxSpeed * cos(playerAngle) });
			playerObject.getInstance(0)->applyCentralForce({ -maxSpeed * sin(playerAngle), 0, 0 });
			playerObject.getInstance(0)->activate(1);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
		{
			playerObject.getInstance(0)->applyCentralForce({ 0,0,maxSpeed * cos(playerAngle) });
			playerObject.getInstance(0)->applyCentralForce({ maxSpeed * sin(playerAngle), 0, 0 });
			playerObject.getInstance(0)->activate(1);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		{
			playerObject.getInstance(0)->applyCentralForce({ -maxSpeed * cos(playerAngle),0,0 });
			playerObject.getInstance(0)->applyCentralForce({ 0,0, maxSpeed * sin(playerAngle) });
			playerObject.getInstance(0)->activate(1);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
		{
			playerObject.getInstance(0)->applyCentralForce({ maxSpeed * cos(playerAngle),0,0 });
			playerObject.getInstance(0)->applyCentralForce({ 0,0,maxSpeed * -sin(playerAngle) });
			playerObject.getInstance(0)->activate(1);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		{
			playerAngle += playerRotationSpeed * deltatime;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
		{
			playerAngle -= playerRotationSpeed * deltatime;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			playerObject.getInstance(0)->applyCentralForce({ 0,maxSpeed*2,0 });
			playerObject.getInstance(0)->activate(1);
		}

		auto v = playerObject.getInstance(0)->getLinearVelocity();
		const int maxVelocity = 6;
		if (v.getZ() > maxVelocity) { v.setZ(maxVelocity); }
		if (v.getZ() < -maxVelocity) { v.setZ(-maxVelocity); }
		if (v.getX() > maxVelocity) { v.setX(maxVelocity); }
		if (v.getX() < -maxVelocity) { v.setX(-maxVelocity); }
		playerObject.getInstance(0)->setLinearVelocity(v);
#pragma endregion

		world->stepSimulation(deltatime);

		if (gameObjectPool.phisicalObjectVector.getElementById(250).colidesWith(0, playerObject, 0))
		{
			gameObjectPool.phisicalObjectVector.getElementById(250).objectData[0].material = Material::cyanRubber();
		}else
		{
			gameObjectPool.phisicalObjectVector.getElementById(250).objectData[0].material = Material::gold();
		}


		light.getPosition(0) = glm::vec4(lightPosition, 1);
		//light.getPosition(1) = glm::vec4(lightPosition.x, lightPosition.y - 4, lightPosition.z, 1);		


		glm::vec3 playerPos;
		btTransform playerTransform;
		playerObject.getInstance(0)->getMotionState()->getWorldTransform(playerTransform);
		playerObject.getInstance(0)->setWorldTransform(playerTransform);
		playerPos = { playerTransform.getOrigin().x(), playerTransform.getOrigin().y(), playerTransform.getOrigin().z() };
		

		camera.playerPosition = playerPos;
		camera.topDownAngle = playerAngle;
		playerObject.draw();
		
		gameObjectPool.drawAll(deltatime);

		//gameObjectPool.clearAll();
		//gameObjectPool.load("maps//map1.txt");

		lightObject.sp->uniform("u_ambience", 1, 1, 1);

		lightObject.getInstance(0).setPosition(lightPosition.x, lightPosition.y, lightPosition.z);
		lightObject.getInstance(0).setScale(0.1f);

		lightObject.draw();

		//particles.draw(deltatime);
		//particles2.draw(deltatime);
		

		//world->debugDrawWorld();

		//plan.draw();
		/*
		window.pushGLStates();
		glDisable(GL_DEPTH_TEST);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glUseProgram(0);
		//window.resetGLStates();

		for(int i=0; i<14; i++)
		{
			glDisableVertexAttribArray(i);
		}

		//glViewport(0, 0, width, height);
		sf::RectangleShape s({ 100,100 });
		s.setPosition({ 100, 100 });

		s.setFillColor(sf::Color::Green);
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			window.draw(s);
		}
		
		window.setView(sf::View({ 0, 0, width, height }));
		window.display();

		window.popGLStates();
		glEnable(GL_DEPTH_TEST);
		*/

		//window.setView(sf::View({ 0, 0, width, height }));
		window.display();
	}


	/*
	playerObject.cleanup();

	world->removeCollisionObject(&body);
	delete body.getMotionState();
	delete body.getCollisionShape();

	world->removeCollisionObject(&sphere);
	delete sphere.getMotionState();
	delete body.getCollisionShape();

	delete dispatcher;
	delete collisionConfiguration;
	delete solver;
	delete broadPhase;
	delete world;

	textureManager.cleanUp();
	modelManager.cleanUp();
	*/
	return 0;
}


