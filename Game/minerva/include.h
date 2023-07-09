/* Citations
	learnopengl.com for OpenGL instruction
*/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <thread>
#include <cstdlib>
#include <ctype.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <STB/stb_image.h>

namespace Minerva {
	namespace Random {
		int    Range(int min, int max);
		double Range(double min, double max);

		int    Range(int max);
		double Range(double max);

		int    SplitRange(int max);
		double SplitRange(double max);
	}

	namespace Net {
		uint64_t DumpFloat(long double f, unsigned bits, unsigned expbits);
		long double LoadFloat(uint64_t i, unsigned bits, unsigned expbits);

		class Value {
		public:
			virtual void Load(unsigned char* stream, unsigned int* index) {};
			virtual void Dump(unsigned char* stream, unsigned int* index) {};
			virtual unsigned int Size() { return 0; };
			virtual std::string ToString() { return ""; };
		};

		class Int2 : public Value {
		public:
			Int2();
			Int2(int16_t n);
			void Load(unsigned char* data_stream, unsigned int* index);
			void Dump(unsigned char* data_stream, unsigned int* index);
			void DumpRaw(unsigned char* data_stream, unsigned int* index);
			unsigned int Size();
			std::string ToString();
			int16_t internal;
		};

		class IntU2 : public Value {
		public:
			IntU2();
			IntU2(uint16_t n);
			void Load(unsigned char* data_stream, unsigned int* index);
			void Dump(unsigned char* data_stream, unsigned int* index);
			void DumpRaw(unsigned char* data_stream, unsigned int* index);
			unsigned int Size();
			std::string ToString();
			uint16_t internal;
		};

		class Int4 : public Value {
		public:
			Int4();
			Int4(int32_t n);
			void Load(unsigned char* data_stream, unsigned int* index);
			void Dump(unsigned char* data_stream, unsigned int* index);
			void DumpRaw(unsigned char* data_stream, unsigned int* index);
			unsigned int Size();
			std::string ToString();
			int32_t internal;
		};

		class IntU4 : public Value {
		public:
			IntU4();
			IntU4(uint32_t n);
			void Load(unsigned char* data_stream, unsigned int* index);
			void Dump(unsigned char* data_stream, unsigned int* index);
			void DumpRaw(unsigned char* data_stream, unsigned int* index);
			unsigned int Size();
			std::string ToString();
			uint32_t internal;
		};

		class Int8 : public Value {
		public:
			Int8();
			Int8(int64_t n);
			void Load(unsigned char* data_stream, unsigned int* index);
			void Dump(unsigned char* data_stream, unsigned int* index);
			void DumpRaw(unsigned char* data_stream, unsigned int* index);
			unsigned int Size();
			std::string ToString();
			int64_t internal;
		};

		class IntU8 : public Value {
		public:
			IntU8();
			IntU8(uint64_t n);
			void Load(unsigned char* data_stream, unsigned int* index);
			void Dump(unsigned char* data_stream, unsigned int* index);
			void DumpRaw(unsigned char* data_stream, unsigned int* index);
			unsigned int Size();
			std::string ToString();
			uint64_t internal;
		};		
		
		class Float : public Value {
		public:
			Float();
			Float(float n);
			void Load(unsigned char* data_stream, unsigned int* index);
			void Dump(unsigned char* data_stream, unsigned int* index);
			void DumpRaw(unsigned char* data_stream, unsigned int* index);
			unsigned int Size();
			std::string ToString();
			float internal;
		};

		class Double : public Value {
		public:
			Double();
			Double(double n);
			void Load(unsigned char* data_stream, unsigned int* index);
			void Dump(unsigned char* data_stream, unsigned int* index);
			void DumpRaw(unsigned char* data_stream, unsigned int* index);
			unsigned int Size();
			std::string ToString();
			double internal;
		};

		class String : public Value {
		public:
			String();
			String(char* str);
			String(unsigned int size);
			void Load(unsigned char* data_stream, unsigned int* index);
			void Dump(unsigned char* data_stream, unsigned int* index);
			void DumpRaw(unsigned char* data_stream, unsigned int* index);
			unsigned int Size();
			std::string ToString();
			
			IntU2 size;
			char* internal;
		};

		Value* LoadValueType(unsigned char* data_stream, unsigned int* index);

		class Any {
		public:
			Value* internal;
			
			Any();
			Any(Value* internal);

			void Load(unsigned char* data_stream, unsigned int* index);
			void Dump(unsigned char* data_stream, unsigned int* index);
			unsigned int Size();
			unsigned int NetSize();
			std::string ToString();
		};

		class ComponentValue : public Value {
		public:
			IntU4 id;
			String key;
			Any value;

			ComponentValue();
			ComponentValue(IntU4 id, String key, Any value);

			void Load(unsigned char* data_stream, unsigned int* index);
			void Dump(unsigned char* data_stream, unsigned int* index);
			void DumpRaw(unsigned char* data_stream, unsigned int* index);
			unsigned int Size();
			std::string ToString();
		};

		enum PacketType {
			TEST
		};

		class Packet {
		private:
			PacketType type;
			std::vector<Any> data;
		public:
			Packet();
			Packet(PacketType type);
			Packet(PacketType type, std::vector<Any> data);

			void operator+=(Any value);
			unsigned int Size();
			unsigned char* Dump();
			void Load(unsigned char* in);
			std::string ToString();
		};
	};

	namespace Debug {
		namespace Console {
			void Log(const char* text);
			void Warning(const char* text);
			void Error(const char* text);
			void FatalError(const char* text);
		}
	}

	namespace System {
		struct RendererPrefs {
			const char* window_title = "Project";
		};

		class Renderer; // forward declaration
	}

	namespace Component {
		struct Renderer; // forward declaration
	};

	namespace Shading {
		class Shader {
		private:
			std::string FileToBuffer(std::string name);
		public:
			Shader(std::string name, GLint _type);

			int         id;
			GLint       type;
		};

		class Program {
		private:
			GLint LookupUniform(const char* name);

		public:
			unsigned int id;

			Shader* vertex;
			Shader* fragment;

			Program(Minerva::System::Renderer* renderer, const char* _vertex, const char* _fragment);

			void Set(const char* name, bool      val);
			void Set(const char* name, int       val);
			void Set(const char* name, float     val);
			void Set(const char* name, glm::mat4 val);

			void Activate();
		};
	};

	namespace Modeling {
		// parts from learnopengl.com
		enum TextureType {
			TEXTURE_DIFFUSE,
			TEXTURE_SPECULAR
		};

		class Texture {
		public:
			GLuint      id;
			TextureType type;

			Texture(std::string file, TextureType _type);
		};

		/*struct Texture {
			unsigned int id;
			TextureType  type;
		};*/

		struct Vertex {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texture;
		};

		class Mesh {
			public:
				std::vector<Vertex>        vertices;
				std::vector<unsigned int>  indices;
				std::vector<std::string>   textures;
				
				Mesh(std::vector<Vertex>       _vertices, 
					 std::vector<unsigned int> _indices, 
					 std::vector<std::string>  _textures,
					 Minerva::System::Renderer* renderer
				);

				void BindTextures(Minerva::System::Renderer* renderer, Minerva::Component::Renderer* renderer_component);
				void Render(Minerva::System::Renderer* renderer, Minerva::Component::Renderer* renderer_component);
				void RenderInstanced(Minerva::System::Renderer* renderer, Minerva::Component::Renderer* renderer_component, int num_instances);

			//private:
				unsigned int VAO, VBO, EBO;
		};

		class ModelBase {
		public:
			std::vector<Mesh> meshes;
			void Render(Minerva::System::Renderer* renderer, Minerva::Component::Renderer* renderer_component);
			void RenderInstanced(Minerva::System::Renderer* renderer, Minerva::Component::Renderer* renderer_component, int num_instances);
		};

		class CustomModel : public ModelBase {
		public:
			void AddMesh(
				std::vector<Vertex> vertices,
				std::vector<unsigned int> indices,
				Minerva::System::Renderer* renderer
			);
			void AddMesh(
				std::vector<Vertex> vertices,
				std::vector<unsigned int> indices,
				std::vector<std::string> textures,
				Minerva::System::Renderer* renderer
			);
			void AddMeshFromVertexTextureArray(std::vector<float> data, std::vector<std::string> textures, Minerva::System::Renderer* renderer);
		};

		/*class FileModel : public ModelBase {
		public:
			FileModel(std::string _file_name);
			void Load(::Minerva::System::RendererPrefs& prefs);

		private:
			std::string       file_name;
			std::string       path;

			void         ProcessNode(aiNode* node, const aiScene* scene);
			Mesh         ProcessMesh(aiMesh* mesh, const aiScene* scene);
			unsigned int TextureFromFile(const char* file_name);

			std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type, TextureType type_enum);
		};*/
	}

	namespace Component {
		struct Base {
			virtual std::string Name() {
				return "base";
			}

			virtual Base* Clone() {
				return new Base();
			};
		};

		struct Transform : Base {
			glm::vec3 position;
			glm::vec3 scale;

			glm::vec3 rotation;
			float angle = 0;

			Transform() {
				Base();

				position = glm::vec3(0.0f, 0.0f, 0.0f);
				rotation = glm::vec3(0.0f, 0.0f, 0.0f);
				scale    = glm::vec3(1.0f, 1.0f, 1.0f);
			}

			Transform(glm::vec3 _position) {
				Base();

				position = _position;
				rotation = glm::vec3(0.0f, 0.0f, 0.0f);
				scale    = glm::vec3(1.0f, 1.0f, 1.0f);
			}

			Transform(glm::vec3 _position, glm::vec3 _rotation, float _angle) {
				Base();

				position = _position;
				rotation = _rotation;
				angle    = _angle;
				scale    = glm::vec3(1.0f, 1.0f, 1.0f);
			}

			Transform(glm::vec3 _position, glm::vec3 _rotation, float _angle, glm::vec3 _scale) {
				Base();

				position = _position;
				rotation = _rotation;
				angle    = _angle;
				scale    = _scale;
			}

			std::string Name() {
				return "transform";
			}

			static std::string TypeName() {
				return "transform";
			}

			Base* Clone() {
				return new Transform(
					position,
					rotation,
					angle,
					scale
				);
			}
		};

		struct Renderer : Base {
			Modeling::ModelBase* model;
			Shading::Program* program;

			Renderer(std::string _model, std::string _program, Minerva::System::Renderer* renderer);
			Renderer(Modeling::ModelBase* _model, Shading::Program* _program) {
				model   = _model;
				program = _program;
			}

			std::string Name() {
				return "renderer";
			}

			static std::string TypeName() {
				return "renderer";
			}

			Base* Clone() {
				return new Renderer(
					model, program
				);
			}
		};

		struct Camera : Base {
			glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

			Camera() {};
			Camera(glm::vec3 _pos, glm::vec3 _front, glm::vec3 _up) {
				pos   = _pos;
				front = _front;
				up    = _up;
			}
			
			std::string Name() {
				return "camera";
			}

			static std::string TypeName() {
				return "camera";
			}

			Base* Clone() {
				return new Camera(
					pos, front, up
				);
			}
		};

		struct Noclip : Base {
			std::string Name() {
				return "noclip";
			}

			static std::string TypeName() {
				return "noclip";
			}

			Base* Clone() {
				return new Noclip();
			}
		};

		struct Instance : Base {
			Renderer* instance_renderer;

			Instance(Renderer* r) {
				instance_renderer = r;
			}

			std::string Name() {
				return "instance";
			}

			static std::string TypeName() {
				return "instance";
			}
			
			Base* Clone() {
				return new Instance(
					instance_renderer
				);
			}
		};
	}

	class Engine; // forward declaration

	namespace System {
		struct CallbackRequests {
			bool on_initialize;
			bool on_first_cycle;
			bool on_cycle;
			bool on_update;
			bool on_precycle;
			bool on_terminate;
			bool on_input;
			bool on_mouse;
		};

		class Base {
		public:
			virtual CallbackRequests GetCallbackRequests() {
				return { 0 };
			};

			virtual std::string Name() {
				return "base";
			}

			virtual void OnInitialize(Engine* engine) {};
			virtual void OnFirstCycle(Engine* engine) {};
			virtual void OnCycle     (Engine* engine) {};
			virtual void OnUpdate    (Engine* engine) {};
			virtual void OnPrecycle  (Engine* engine) {};
			virtual void OnTerminate (Engine* engine) {};
			virtual void OnInput     (Engine* engine) {};
			virtual void OnMouse     (Engine* engine, double xpos, double ypos) {};
		};

		class Renderer : public Base {
		public:
			CallbackRequests GetCallbackRequests() {
				return {
					true,
					true,
					false,
					true,
					true,
					true,
					false,
					false
				};
			};

			std::string Name() {
				return "renderer";
			}

			static std::string TypeName() {
				return "renderer";
			}

			void OnInitialize(Engine* engine);
			void OnUpdate    (Engine* engine);
			void OnPrecycle  (Engine* engine);
			void OnTerminate (Engine* engine);
			void OnFirstCycle(Engine* engine);

			RendererPrefs prefs;

			glm::mat4 proj;

			unsigned int InstantiateCamera(Engine* engine);

			int  camera = 0;
			std::vector<unsigned int> cameras;

			Shading::Shader*     GetShader (std::string name);
			Shading::Program*    GetProgram(std::string name);
			Modeling::Texture*   GetTexture(std::string name);
			Modeling::ModelBase* GetModel  (std::string name);

			void RegisterModel(std::string name, Modeling::ModelBase* model);
			void RemoveModel  (std::string name);
			void BuildInstanceBuffer(Minerva::Engine* engine);

			GLFWwindow* window;

		private:
			void InitGLFW  ();
			void InitGLAD  ();
			void InitOpenGL();
			void InitSTBI  ();

			void SetFrustum(float fov);

			std::unordered_map<std::string, Shading::Shader>      shaders;
			std::unordered_map<std::string, Shading::Program>     programs;
			std::unordered_map<std::string, Modeling::Texture>    textures;
			std::unordered_map<std::string, Modeling::ModelBase*> models;

			std::unordered_map<Component::Renderer*, std::vector<Component::Transform*>> instances;

			int window_width;
			int window_height;
		};

		namespace Controller {
			class Noclip : public Base {
			public:
				glm::vec3 pos   = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
				glm::vec3 up    = glm::vec3(0.0f, 1.0f, 0.0f);

				std::string Name() {
					return "noclip";
				}

				static std::string TypeName() {
					return "noclip";
				}

				float speed;
				const float sensitivity = 0.1f;
				bool first_mouse = true;
				double lastx, lasty;
				float pitch = 90.0f;
				float yaw = 90.0f;

				CallbackRequests GetCallbackRequests() {
					return {
						false,
						false,
						false,
						false,
						false,
						false,
						true,
						true
					};
				}

				void OnInput(Engine * engine);
				void OnMouse(Engine * engine, double xpos, double ypos);
			};
		}
	}

	class Prototype {
	public:
		std::vector<Component::Base*> components;

		Prototype() {};
		Prototype(std::vector<Component::Base*> _components) {
			components = _components;
		}

		void Add(Component::Base* cmpt) {
			components.push_back(cmpt);
		}
	};

	class Engine {
	public:
		double delta_time = 0;

		Engine();

		void Initialize();
		void Cycle();      // game loop
		void Terminate();

		unsigned int Instantiate();
		unsigned int Instantiate(std::string prototype);
		unsigned int operator()();
		unsigned int operator()(std::string prototype);
		void         Destroy        (unsigned int entity);
		void         AddComponent   (unsigned int entity, Component::Base* cmpt);
		void         RemoveComponent(unsigned int entity, std::string component_name);
		//std::unordered_map<unsigned int, Component::Base*>
		//	              operator[](std::string component_name);
		//Component::Base*  GetComponent(unsigned int entity, std::string component_name);
		//System::Base*     operator()(std::string system_name);

		std::unordered_map<unsigned int, Minerva::Component::Base*> Components(std::string component) {
			return components[component];
		}

		System::Base* AddSystem (System::Base* system);
		void          RemoveSystem   (std::string system_name);
		
		Prototype* AddPrototype   (std::string name, Prototype p);
		void       RemovePrototype(std::string name);
		Prototype* GetPrototype   (std::string name);

		std::unordered_map<std::string, bool> threads_to_complete;
		bool                                  threads_should_terminate = false;

		Component::Base* GetRawComponent(unsigned int id, std::string name);
		System::Base* GetRawSystem(std::string name);

	private:
		int current_id;

		std::unordered_map<std::string, std::unordered_map<unsigned int, Component::Base*>> components;
		std::unordered_map<std::string, System::Base*> systems;
		std::unordered_map<std::string, Minerva::Prototype> prototypes;

		std::unordered_map<std::string, System::Base*> on_initialize;
		std::unordered_map<std::string, System::Base*> on_update;
		std::unordered_map<std::string, System::Base*> on_precycle;
		std::unordered_map<std::string, System::Base*> on_terminate;
		std::unordered_map<std::string, System::Base*> on_input;
		std::unordered_map<std::string, System::Base*> on_mouse;
		std::unordered_map<std::string, System::Base*> on_first_cycle;

		std::unordered_map<std::string, std::thread*> threads;
	};

	void CycleEngineThread(Engine* engine, System::Base* system, std::string system_name);

	template <class T>
	T* Get(Engine* engine, unsigned int id) {
		return (T*)engine->GetRawComponent(id, T::TypeName());
	}

	template <class T>
	T* Get(Engine* engine) {
		return (T*)engine->GetRawSystem(T::TypeName());
	}
}