/******************************************************************************/
/*!
\file   graphic_system.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the methods of GraphicSystem class
*/
/******************************************************************************/

#include <glew.h>
#include <graphic_system.hpp>
#include <scene_manager.hpp>
#include <asset_manager.hpp>
#include <scene.hpp>
#include <shader.hpp>
#include <math_util.hpp>
#include <mat4.hpp>
#include <colors.hpp>

#include <transform.hpp>
#include <camera.hpp>
#include <renderer.hpp>
#include <light.hpp>
#include <vertex.hpp>

jeBegin


const std::vector<float> quadVertices =
{
	-.5f, .5f, 0.0f, 0.f, 0.f, 1.f, 0.0f, 0.0f ,
	-.5f, -.5f, 0.0f, 0.f, 0.f, 1.f, 0.0f, 1.0f,
	.5f, -.5f, 0.0f, 0.f, 0.f, 1.f, 1.0f, 1.0f,
	.5f,  .5f, 0.0f, 0.f, 0.f, 1.f, 1.0f, 0.0f
};
const std::vector<unsigned> quadIndices = { 2, 0, 1, 2, 3, 0 };

std::vector<float> cubeVertices =
{
	// positions          
	/*-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f*/

	 -.5f, -.5f, -.5f,
	 -.5f, -.5f, .5f,
	 -.5f, .5f, -.5f,
	 -.5f, .5f, .5f,
	 .5f, -.5f, -.5f,
	 .5f, -.5f, .5f,
	 .5f, .5f, -.5f,
	 .5f, .5f, .5f
};
const unsigned cubeVerticesSize = 108;
const std::vector<unsigned> cubeIndices =
{
	/*0, 6, 4,
	0, 2, 6,
	0, 3, 2,
	0, 1, 3,
	2, 7, 6,
	2, 3, 7,
	4, 6, 7,
	4, 7, 5,
	0, 4, 5,
	0, 5, 1,
	1, 5, 7,
	1, 7, 3*/

	4, 6, 0,
	6, 2, 0,
	2, 3, 0,
	3, 1, 0,
	6, 7, 2,
	7, 3, 2,
	7, 6, 4,
	5, 7, 4,
	5, 4, 0,
	1, 5, 0,
	7, 5, 1,
	3, 7, 1
};
const unsigned cubeIndicesSize = 36;

const std::string type("].mode"), position("].position"),
innerAngle("].innerAngle"), outerAngle("].outerAngle"), fallOff("].fallOff"),
aColor("].aColor"), sColor("].sColor"), dColor("].dColor"), 
aIntense("].aIntensity"), dIntense("].dIntensity"), sIntense("].sIntensity"),
activate("].activate"), radius("].radius"),
constant("].constant"), linear("].linear"), quadratic("].quadratic");

vec3 GraphicSystem::resScaler_;
GraphicSystem::Shaders GraphicSystem::shader_;
int GraphicSystem::widthStart_ = 0, GraphicSystem::heightStart_ = 0;
float GraphicSystem::width_ = 0.f, GraphicSystem::height_ = 0.f;
unsigned GraphicSystem::quadVao_ = 0, GraphicSystem::quadVbo_ = 0, GraphicSystem::quadEbo_ = 0,
GraphicSystem::drVao_ = 0, GraphicSystem::drVbo_ = 0, GraphicSystem::fbo_[] = { 0 },
GraphicSystem::skyboxVao_ = 0, GraphicSystem::skyboxVbo_ = 0, GraphicSystem::skyboxEbo_ = 0,
GraphicSystem::quadIndicesSize_ = 6,
GraphicSystem::environmentTextures_[] = {0},
GraphicSystem::depthrenderbuffer_[] = { 0 };

std::stack<GraphicSystem::Graphic> GraphicSystem::graphicStack_;
Camera* GraphicSystem::mainCamera_ = nullptr;
GraphicSystem::Renderers GraphicSystem::renderers_;
GraphicSystem::Cameras GraphicSystem::cameras_;
GraphicSystem::Lights GraphicSystem::lights_;
vec4 GraphicSystem::backgroundColor = vec4::zero, GraphicSystem::screenColor = vec4::zero;
GraphicSystem::Grid GraphicSystem::grid;
GraphicSystem::Skybox GraphicSystem::skybox;
int GraphicSystem::copyIndex_ = -1;

void GraphicSystem::set_camera(Camera* camera)
{
	mainCamera_ = camera;
}

Camera* GraphicSystem::get_camera()
{
	return mainCamera_;
}

void GraphicSystem::initialize() {

	// set main camera
	if (!mainCamera_ && !(cameras_.empty()))
		mainCamera_ = *cameras_.begin();

	// set skybox
	if (!skybox.textures[0])
	{
		skybox.textures[0] = AssetManager::get_texture("skybox_front");
		skybox.textures[1] = AssetManager::get_texture("skybox_back");
		skybox.textures[2] = AssetManager::get_texture("skybox_right");
		skybox.textures[3] = AssetManager::get_texture("skybox_left");
		skybox.textures[4] = AssetManager::get_texture("skybox_top");
		skybox.textures[5] = AssetManager::get_texture("skybox_bottom");
	}

	//for (auto& model : models_)
	//	model->initialize();
}

void GraphicSystem::update(float dt) {

	// get current scene color
	backgroundColor = SceneManager::get_current_scene()->background;

	// scissor out the letterbox area
	glEnable(GL_SCISSOR_TEST);
	glScissor(static_cast<GLsizei>(widthStart_),
		static_cast<GLsizei>(heightStart_),
		static_cast<GLsizei>(width_),
		static_cast<GLsizei>(height_));

	// cull out invisible face
	glEnable(GL_CULL_FACE);

	// copy all the renderers
	render_copy(dt);

	glClearColor(backgroundColor.x,
		backgroundColor.y,
		backgroundColor.z,
		backgroundColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(static_cast<GLsizei>(widthStart_),
		static_cast<GLsizei>(heightStart_),
		static_cast<GLsizei>(width_),
		static_cast<GLsizei>(height_));

	// update main camera
	mainCamera_->update(dt);

	// render skybox
	if (skybox.render)
		render_skybox();

	// update lights
	update_lights(dt);

	// update renderers
	for (auto& r : renderers_)
		r->draw(dt);

	// render grid
	if (grid.render)
		render_grid();

	glDisable(GL_SCISSOR_TEST);
}

void GraphicSystem::close() {

	lights_.clear();
	lights_.shrink_to_fit();

	cameras_.clear();
	cameras_.shrink_to_fit();

	renderers_.clear();
	renderers_.shrink_to_fit();

	mainCamera_ = nullptr;
	glDeleteTextures(6, environmentTextures_);
	for (int i = 0; i < 6 ; ++i)
	{
		environmentTextures_[i] = 0;
		skybox.textures[i] = 0;
	}
}

void GraphicSystem::render_grid()
{
	Shader* shader = shader_[GRID];
	shader->use();

	shader->set_matrix("m4_translate", mat4::translate(vec3::zero));
	shader->set_matrix("m4_scale", mat4::scale(vec3::one ));
	shader->set_matrix("m4_rotate", mat4::identity);
	shader->set_vec3("v3_color", grid.color);
	shader->set_float("thickness", grid.thickness);
	shader->set_int("divisions", grid.divisions);

	switch (grid.prjType)
	{
	case Renderer::ProjectType::PERSPECTIVE:
	{
		mat4 perspective = mat4::perspective(
			Math::deg_to_rad(mainCamera_->fovy_ + mainCamera_->zoom), mainCamera_->aspect_,
			mainCamera_->near_, mainCamera_->far_);

 		shader->set_matrix("m4_projection", perspective);
		break;
	}

	case Renderer::ProjectType::ORTHOGONAL:
	default:
	{
		float right_ = GraphicSystem::width_ * GraphicSystem::resScaler_.x;
		float left_ = -right_;
		float top_ = GraphicSystem::height_ * GraphicSystem::resScaler_.y;
		float bottom_ = -top_;

		mat4 orthogonal = mat4::orthogonal(left_, right_, bottom_, top_, mainCamera_->near_, mainCamera_->far_);
		shader->set_matrix("m4_projection", orthogonal);
		break;
	}
	}

	// Send camera info to shader
	mat4 viewport = mat4::look_at(mainCamera_->position, mainCamera_->position + mainCamera_->front_, mainCamera_->up_);
	shader->set_matrix("m4_viewport", viewport);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

	glBindVertexArray(quadVao_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEbo_);
	glDrawElements(GL_TRIANGLES, quadIndicesSize_, GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void GraphicSystem::render_skybox()
{
	Shader* shader = shader_[SKYBOX];
	shader->use();

	shader->set_matrix("m4_translate", mat4::translate(mainCamera_->position));
	shader->set_matrix("m4_scale", mat4::identity);
	shader->set_matrix("m4_rotate", mat4::identity);
	shader->set_vec3("v3_color", skybox.color);
	shader->set_vec3("v3_cameraPosition", mainCamera_->position);
	shader->set_float("f_scale", skybox.scale);

	mat4 perspective = mat4::perspective(
		Math::deg_to_rad(mainCamera_->fovy_ + mainCamera_->zoom), 
		mainCamera_->aspect_, mainCamera_->near_, mainCamera_->far_);

	shader->set_matrix("m4_projection", perspective);

	// Send camera info to shader
	mat4 viewport = mat4::look_at(mainCamera_->position, mainCamera_->position + mainCamera_->front_, mainCamera_->up_);
	shader->set_matrix("m4_viewport", viewport);

	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(skyboxVao_);
	
	for (int i = 0; i < 6; ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, skybox.textures[i]);
		shader->set_int(std::string("sampler[" + std::to_string(i) + "]").c_str(), i);
	}

	// glDrawArrays(GL_TRIANGLES, 0, cubeVerticesSize);
	glDrawElements(GL_TRIANGLES, cubeIndicesSize, GL_UNSIGNED_INT, nullptr);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}

void GraphicSystem::render_copy(float dt)
{
	// Start copy
	glClearColor(backgroundColor.x,
		backgroundColor.y,
		backgroundColor.z,
		backgroundColor.w);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_[0]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 512, 512);

	vec3 camPos = mainCamera_->position;
	float camYaw = mainCamera_->yaw_, 
		camPitch = mainCamera_->pitch_,
		aspect = mainCamera_->aspect_,
		fovy = mainCamera_->fovy_;

	mainCamera_->aspect_ = 1.f;
	mainCamera_->fovy_ = 90.f;
	mainCamera_->position.set_zero();

	for (copyIndex_ = 0; copyIndex_ < 6; ++copyIndex_) {

		switch (copyIndex_) {

		default:
		case 0:// front
			mainCamera_->set_pitch(0.f);
			mainCamera_->set_yaw(270.f);
			break;
		case 1: // back
			mainCamera_->set_pitch(0.f);
			mainCamera_->set_yaw(90.f);
			break;
		case 2: // right
			mainCamera_->set_pitch(0.f);
			mainCamera_->set_yaw(0.f);
			break;
		case 3:  // left
			mainCamera_->set_pitch(0.f);
			mainCamera_->set_yaw(180.f);
			break;
		case 4: // top
			mainCamera_->set_pitch(90.f);
			mainCamera_->set_yaw(0.f);
			break;
		case 5:	// bottom
			mainCamera_->set_pitch(270.f);
			mainCamera_->set_yaw(0.f);
			break;
		}
		
		mainCamera_->update(dt);

		// render skybox
		if (skybox.render)
			render_skybox();

		// update lights
		update_lights(dt);

		// update renderers
		for (auto& r : renderers_)
			r->draw(dt);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 
			environmentTextures_[copyIndex_], 0);
	}

	copyIndex_ = -1;
	mainCamera_->aspect_ = aspect;
	mainCamera_->fovy_ = fovy;
	mainCamera_->position = camPos;
	mainCamera_->pitch_ = camPitch;
	mainCamera_->yaw_ = camYaw;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GraphicSystem::add_renderer(Renderer* model) 
{
	renderers_.emplace_back(model);
}

void GraphicSystem::add_camera(Camera* camera) 
{ 
	cameras_.emplace_back(camera);
}

void GraphicSystem::add_light(Light* light)
{
	lights_.emplace_back(light);
}

void GraphicSystem::remove_renderer(Renderer* model)
{
	renderers_.erase(std::remove(renderers_.begin(), renderers_.end(), model), renderers_.end());
}

void GraphicSystem::remove_camera(Camera* camera) 
{
	cameras_.erase(std::remove(cameras_.begin(), cameras_.end(), camera), cameras_.end());
}

void GraphicSystem::remove_light(Light* light)
{
	lights_.erase(std::remove(lights_.begin(), lights_.end(), light), lights_.end());
}

void GraphicSystem::pause()
{
	Graphic grp;
	grp.backgroundColor = backgroundColor;
	grp.screenColor = screenColor;
	grp.mainCamera = mainCamera_;
	grp.grid = grid;
	grp.renderers = renderers_;
	grp.cameras = cameras_;
	grp.lights = lights_;

	graphicStack_.emplace(grp);

	// 
	renderers_.clear();
	cameras_.clear();
	lights_.clear();
}

void GraphicSystem::resume()
{
	if (!graphicStack_.empty())
	{
		auto grp = graphicStack_.top();
		graphicStack_.pop();

		backgroundColor = grp.backgroundColor;
		screenColor = grp.screenColor;
		mainCamera_ = grp.mainCamera;
		grid = grp.grid;
		renderers_ = grp.renderers;
		cameras_ = grp.cameras;
		lights_ = grp.lights;
	}
}

void GraphicSystem::update_lights(float dt)
{
	for (unsigned i = 0; i < lights_.size(); ++i)
	{
		Shader* shader = shader_[SPRITE];
		shader->use();

		// Update shader uniform info
		std::string str("light[" + std::to_string(i));
		shader->set_bool((str + activate).c_str(), lights_[i]->activate);

		// Set strings as a static
		if (lights_[i]->activate) {

			/*Calculate the light max and set the radius for light volume optimization*/
			// Calculate the light max
			//float ambientMax = std::fmaxf(std::fmaxf(lights_[i]->ambient.x, lights_[i]->ambient.y),lights_[i]->ambient.z);
			//float diffuseMax = std::fmaxf(std::fmaxf(lights_[i]->diffuse.x, lights_[i]->diffuse.y),lights_[i]->diffuse.z);
			//float speculaMax = std::fmaxf(std::fmaxf(lights_[i]->specular.x, lights_[i]->specular.y), lights_[i]->specular.z);
			//float lightMax = std::fmaxf(std::fmaxf(ambientMax, diffuseMax), speculaMax);

			// Get radius
			float lightConstant = lights_[i]->constant;
			float lightLinear = lights_[i]->linear;
			float lightQuadratic = lights_[i]->quadratic;
			//float lightRadius = (-lightLinear + std::sqrtf(lightLinear * lightLinear
			//	- 4 * lightQuadratic * (lightConstant - (256.f / 5.f) * lightMax))) * 0.5f * lightQuadratic;

			// Update light direction
			shader->set_int((str + type).c_str(), static_cast<int>(lights_[i]->type));
			shader->set_vec3((str + position).c_str(), lights_[i]->transform_->position);
			shader->set_float((str + constant).c_str(), lightConstant);
			shader->set_float((str + linear).c_str(), lightLinear);
			shader->set_float((str + quadratic).c_str(), lightQuadratic);
			shader->set_vec3((str + aColor).c_str(), lights_[i]->ambient);
			shader->set_vec3((str + sColor).c_str(), lights_[i]->specular);
			shader->set_vec3((str + dColor).c_str(), lights_[i]->diffuse);
			shader->set_float((str + aIntense).c_str(), lights_[i]->ambientIntensity);
			shader->set_float((str + dIntense).c_str(), lights_[i]->diffuseIntensity);
			shader->set_float((str + sIntense).c_str(), lights_[i]->specularIntensity);
			shader->set_float((str + fallOff).c_str(), lights_[i]->fallOff);
			//shader->set_float((str + radius).c_str(), lightRadius);
			shader->set_float((str + innerAngle).c_str(), Math::deg_to_rad(lights_[i]->innerAngle));
			shader->set_float((str + outerAngle).c_str(), Math::deg_to_rad(lights_[i]->outerAngle));
		}

		shader = shader_[MODEL];
		shader->use();

		// Update shader uniform info
		shader->set_bool((str + activate).c_str(), lights_[i]->activate);

		// Set strings as a static
		if (lights_[i]->activate) {

			/*Calculate the light max and set the radius for light volume optimization*/
			// Calculate the light max
			//float ambientMax = std::fmaxf(std::fmaxf(lights_[i]->ambient.x, lights_[i]->ambient.y),lights_[i]->ambient.z);
			//float diffuseMax = std::fmaxf(std::fmaxf(lights_[i]->diffuse.x, lights_[i]->diffuse.y),lights_[i]->diffuse.z);
			//float speculaMax = std::fmaxf(std::fmaxf(lights_[i]->specular.x, lights_[i]->specular.y), lights_[i]->specular.z);
			//float lightMax = std::fmaxf(std::fmaxf(ambientMax, diffuseMax), speculaMax);

			// Get radius
			float lightConstant = lights_[i]->constant;
			float lightLinear = lights_[i]->linear;
			float lightQuadratic = lights_[i]->quadratic;
			//float lightRadius = (-lightLinear + std::sqrtf(lightLinear * lightLinear
			//	- 4 * lightQuadratic * (lightConstant - (256.f / 5.f) * lightMax))) * 0.5f * lightQuadratic;

			// Update light direction
			shader->set_int((str + type).c_str(), static_cast<int>(lights_[i]->type));
			shader->set_vec3((str + position).c_str(), lights_[i]->transform_->position);
			shader->set_float((str + constant).c_str(), lightConstant);
			shader->set_float((str + linear).c_str(), lightLinear);
			shader->set_float((str + quadratic).c_str(), lightQuadratic);
			shader->set_vec3((str + aColor).c_str(), lights_[i]->ambient);
			shader->set_vec3((str + sColor).c_str(), lights_[i]->specular);
			shader->set_vec3((str + dColor).c_str(), lights_[i]->diffuse);
			shader->set_float((str + aIntense).c_str(), lights_[i]->ambientIntensity);
			shader->set_float((str + dIntense).c_str(), lights_[i]->diffuseIntensity);
			shader->set_float((str + sIntense).c_str(), lights_[i]->specularIntensity);
			shader->set_float((str + fallOff).c_str(), lights_[i]->fallOff);
			//shader->set_float((str + radius).c_str(), lightRadius);
			shader->set_float((str + innerAngle).c_str(), Math::deg_to_rad(lights_[i]->innerAngle));
			shader->set_float((str + outerAngle).c_str(), Math::deg_to_rad(lights_[i]->outerAngle));
		}

		lights_[i]->draw(dt);
	}
}

void GraphicSystem::initialize_shaders()
{
	// init shaders
	// do shader stuff
	for (unsigned int i = 0; i < Pipeline::END; ++i) {

		Shader* newShader = new Shader;
		newShader->create_shader(Shader::vsDirectory_[i].c_str(), Shader::Type::VERTEX);

		// TODO
		// Work on geometry shader
		//shader_[i]->CreateShader(Shader::m_geometryShader[i], Shader::JE_GEOMETRY);

		newShader->create_shader(Shader::fsDirectory_[i].c_str(), Shader::Type::PIXEL);
		newShader->combine_shaders(i);

		shader_.push_back(newShader);
	}

	// remove shader directories
	//Shader::vsDirectory_.clear();
	//Shader::fsDirectory_.clear();

	jeDebugPrint("*Compiled and linked shaders.\n");
}

void GraphicSystem::close_shaders()
{
	for (unsigned i = 0; i < Pipeline::END; ++i)
		delete shader_[i];

	shader_.clear();
}

void GraphicSystem::recompile_shaders()
{
	close_shaders();
	initialize_shaders();
}

void GraphicSystem::initialize_graphics()
{
	initialize_shaders();

	// init buffers
	/**************************** QUAD BUFFER ******************************/
	// generate vertex array
	glGenVertexArrays(1, &quadVao_);
	glBindVertexArray(quadVao_);

	// generate vertex buffer
	glGenBuffers(1, &quadVbo_);
	glBindBuffer(GL_ARRAY_BUFFER, quadVbo_);
	glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(float), &quadVertices[0], GL_STATIC_DRAW);

	// vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normals of vertices
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coordinate position
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// generate index buffer
	glGenBuffers(1, &quadEbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEbo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadIndices.size() * sizeof(unsigned), &quadIndices[0], GL_STATIC_DRAW);

	// unbind buffer
	glBindVertexArray(0);

	/**************************** SKYBOX BUFFER ******************************/

	glGenVertexArrays(1, &skyboxVao_);
	glBindVertexArray(skyboxVao_);

	glGenBuffers(1, &skyboxVbo_);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo_);
	glBufferData(GL_ARRAY_BUFFER, cubeVerticesSize * sizeof(float), &cubeVertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// generate index buffer
	glGenBuffers(1, &skyboxEbo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEbo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndicesSize * sizeof(unsigned), &cubeIndices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	/**************************** DEBUG RENDERER BUFFER ******************************/

	glGenVertexArrays(1, &drVao_);
	glBindVertexArray(drVao_);

	glGenBuffers(1, &drVbo_);
	glBindBuffer(GL_ARRAY_BUFFER, drVbo_);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::normal)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::texCoords)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, Vertex::color)));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);

	/**************************** FRAME BUFFER ******************************/

	glGenFramebuffers(1, &fbo_[0]);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_[0]);

	glGenTextures(6, environmentTextures_);

	for (int i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, environmentTextures_[i]);

		// Give an empty image to OpenGL ( the last "0" means "empty" )
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

		// The depth buffer
	glGenRenderbuffers(1, &depthrenderbuffer_[0]);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer_[0]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer_[0]);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	/*for (int i = 0; i < 6; i++) {

		glGenTextures(1, &environmentTextures_[i]);
		glBindTexture(GL_TEXTURE_2D, environmentTextures_[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenFramebuffers(1, &fbo_[i]);
		glGenRenderbuffers(1, &depthrenderbuffer_[i]);

		glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer_[i]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo_[i]);
		glViewport(0, 0, 512, 512);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, environmentTextures_[i], 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer_[i]);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}*/
}

void GraphicSystem::close_graphics()
{
	glDeleteVertexArrays(1, &quadVao_);
	glDeleteBuffers(1, &quadVbo_);
	glDeleteBuffers(1, &quadEbo_);

	glDeleteVertexArrays(1, &skyboxVao_);
	glDeleteBuffers(1, &skyboxVbo_);

	glDeleteVertexArrays(1, &drVao_);
	glDeleteBuffers(1, &drVbo_);

	glDeleteFramebuffers(6, fbo_);

	close_shaders();
}

float GraphicSystem::get_width() { return width_; }

float GraphicSystem::get_height() { return height_; }

unsigned GraphicSystem::get_num_of_lights()
{
	return static_cast<unsigned>(lights_.size());
}

//void GraphicSystem::RenderToFramebuffer() const
//{
//	// Render to framebuffer
//	glBindFramebuffer(GL_FRAMEBUFFER, GLM::fbo_);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glClearColor(0.f, 0.f, 0.f, 0.f);
//	//glViewport(0, 0, GLint(width_), GLint(height_));
//
//	// Backface culling
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	glFrontFace(GL_CCW);
//}
//
//void GraphicSystem::RenderToScreen() const
//{
//	const static GLsizei sizeOfPlaneIndices
//		= static_cast<GLsizei>(GLM::targetMesh_[GLM::JE_TARGET_SCREEN]->GetIndiceCount());
//
//	// Bind default framebuffer and render to screen
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glClearColor(backgroundColor_.x, backgroundColor_.y, backgroundColor_.z, backgroundColor_.w);
//
//	glDisable(GL_CULL_FACE);	//Disable face culling
//	glDisable(GL_DEPTH_TEST);	//Disable depth test
//
//	// Render to plane 2d
//	glBindVertexArray(GLM::targetMesh_[GLM::JE_TARGET_SCREEN]->vao_);
//	Shader::Use(GLM::JE_SHADER_SCREEN);
//	Shader::pCurrentShader_->SetVector4("v4_screenColor", screenColor_);
//
//	// Impose screen effect 
//	Shader::pCurrentShader_->SetEnum("enum_effectType", screenEffect_);
//
//	if (screenEffect_ == JE_EFFECT_BLUR) {
//		Shader::pCurrentShader_->SetFloat("float_blurSize", blurSize_);
//		Shader::pCurrentShader_->SetFloat("float_blurAmount", blurAmount_);
//	}
//	else if (screenEffect_ == JE_EFFECT_SOBEL)
//		Shader::pCurrentShader_->SetFloat("float_sobelAmount", sobelAmount_);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, GLM::texColorBuf_);
//	glDrawElements(GL_TRIANGLES, sizeOfPlaneIndices, GL_UNSIGNED_INT, nullptr);
//	glEnable(GL_DEPTH_TEST);
//
//	glBindVertexArray(0);
//}

jeEnd
