#ifndef _GLTF_H_
#define _GLTF_H_

#define TINYGLTF_LOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "tiny_gltf_loader.h"

#include <osgViewer/Viewer>

#include <osgDB/ReadFile>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgDB/fstream>
#include <osgDB/Registry>
#include <osgDB/Input>
#include <osgDB/Output>

#include <osg/Material>
#include <osgUtil/Tessellator>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/Group>
#include <osg/Notify>
#include <osg/Geode>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>

#include <osgAnimation/Bone>
#include <osgAnimation/Skeleton>
#include <osgAnimation/RigGeometry>
#include <osgAnimation/UpdateBone>
#include <osgAnimation/BasicAnimationManager>
#include <osgAnimation/UpdateMatrixTransform>
#include <osgAnimation/StackedTranslateElement>
#include <osgAnimation/StackedQuaternionElement>
#include <osgAnimation/StackedTransform>
#include <osgAnimation/StackedRotateAxisElement>
#include <osgAnimation/StackedScaleElement>

#include <fstream>
#include <cstdio>
#include <iostream>

#ifdef _DEBUG
#pragma comment(lib, "osgDBd.lib")
#pragma comment(lib, "osgd.lib")
#pragma comment(lib, "osgViewerd.lib")
#pragma comment(lib, "osgGAd.lib")
#pragma comment(lib, "osgUtild.lib")
#pragma comment(lib, "osgAnimationd.lib")
#else
#pragma comment(lib, "osgDB.lib")
#pragma comment(lib, "osg.lib")
#pragma comment(lib, "osgViewer.lib")
#pragma comment(lib, "osgGA.lib")
#pragma comment(lib, "osgUtil.lib")
#pragma comment(lib, "osgAnimation.lib")
#endif

using namespace std;
using namespace tinygltf;
using namespace osg;
using namespace osgDB;

namespace gltf {
	typedef std::map<std::string, tinygltf::Node>::const_iterator itNode_t;
	typedef std::vector<std::string>::const_iterator itString_t;

	class ReaderWriterGLTF : public osgDB::ReaderWriter{
	public:
		typedef std::map<std::string, osg::ref_ptr<osg::StateSet>> MaterialToStateSetMap;
		typedef union datatype{
			int dataInt;
			unsigned int dataUnsignedInt;
			float dataFloat;
			double dataDouble;
			short dataShort;
			unsigned short dataUnsignedShort;
		}dataType_t;
	public:
		ReaderWriterGLTF();
		virtual const char *className() const;
		virtual ReadResult readNode(const string&, const Options*) const;

	public:
		//convert model to scene
		osg::Node* convertModelToSceneGraph(Scene&, string, string&) const;

		//get all nodes in model
		bool getNode(const string, const Scene&, const string, Group*, string&) const;

		//add geode to transform
		bool addGeodeToTransform(const itNode_t, 
			const Scene&, const string, osg::ref_ptr<osg::MatrixTransform>, string&) const;

		//add riggeode to transform
		bool addRigGeodeToTransform(const itNode_t, 
			const Scene&, const string, osg::ref_ptr<osg::MatrixTransform>, string&) const;

		//get geode
		bool getGeode(const std::string, 
			const tinygltf::Scene&, const std::string, osg::Geode*, string&) const;

		//build material map
		void buildMaterialToStateSetMap(const tinygltf::Scene&, 
			const std::string, MaterialToStateSetMap&) const;

		//convert 'mesh' to geometry
		osg::Geometry* convertElementListToGeometry(const tinygltf::Scene&, const std::string, 
			std::vector<tinygltf::Primitive>::const_iterator, string&) const;

		bool getBufferData(const tinygltf::Accessor&, 
			dataType_t&, int&, std::vector<unsigned char>::const_iterator &) const;

		bool getContextBuffer(const tinygltf::Scene &, 
			const tinygltf::Accessor &, const tinygltf::BufferView &, osg::Vec3Array *) const;

		bool getContextBuffer(const tinygltf::Scene&, const tinygltf::Accessor &, 
					  const tinygltf::BufferView &, osg::Vec2Array *) const;

		bool getContextBuffer(const tinygltf::Scene &scene, const tinygltf::Accessor &accessor, 
		const tinygltf::BufferView &bufferView, osg::Vec4Array *vertices) const;

		bool getContextBuffer(const tinygltf::Scene &scene, const tinygltf::Accessor &accessor, 
		const tinygltf::BufferView &bufferView, osg::ref_ptr<osg::DrawElementsUShort> &indices) const;

		bool getContextBuffer(const tinygltf::Scene &scene, const tinygltf::Accessor &accessor, 
					  const tinygltf::BufferView &bufferView, std::vector<float> &scalar) const;

		bool getContextBuffer(const tinygltf::Scene &scene, const tinygltf::Accessor &accessor, 
					  const tinygltf::BufferView &bufferView, osg::MatrixdArray *matrix) const;
		//get context of bufferView
		template<typename T>
		bool getContextBufferView(const tinygltf::Scene &scene,
			const tinygltf::Accessor &accessor, T &vertices, string &err) const;

		//get context of accessor
		template<typename T>
		bool getContextAccessor(const tinygltf::Scene &scene, 
			const std::string &accessorName, T &vertices, string &err) const;

		//get context of accessor and return accessor
		bool getContextAccessor(const tinygltf::Scene &scene,
			const std::string &accessorName, tinygltf::Accessor &accessor, string &err) const;

		bool addTexture(osg::Geometry*, 
			const tinygltf::Scene&, std::string, std::string) const;

	private:
		bool TypeofPrimitives(const Scene &scene, const tinygltf::Accessor &accessor, 
			Geometry *geometry, vector<tinygltf::Primitive>::const_iterator &itPrimitive, string &err) const;

		void pushVec2Data(int flagType, 
			dataType_t *bufferData, osg::Vec2Array *vertices) const;
		void pushVec3Data(int flagType, 
			dataType_t *bufferData, osg::Vec3Array *vertices) const;
		void pushVec4Data(int flagType, 
			dataType_t *bufferData, osg::Vec4Array *vertices) const;

		bool getContextTexture(const tinygltf::Scene &, const string &,
			ref_ptr<Texture2D> &, const string) const;

		bool getContextImage(const tinygltf::Scene &, const string,  
			const string, ref_ptr<osg::Image> &) const;

		//create animation
		bool createKeyframeAnimation(osg::Group *group, const tinygltf::Scene &scene, string &) const;

		//get accessor name in parameter that used in channel's sampler
		bool getAccessorNamefromChannel(
			std::vector<tinygltf::AnimationChannel>::const_iterator itAnimChannel, 
			std::map<std::string, tinygltf::Animations>::const_iterator itAnimation, 
			std::map<std::string, std::string> &accessorName) const;

		bool LoadShader(const tinygltf::Scene scene, std::string basePath, osg::StateSet *ss, 
			std::string vertexShaderName, std::string fragmentShaderName) const;

		bool getShaderNamebyPrimitives(const tinygltf::Scene &scene, vector<Primitive>::const_iterator itPrimitive, 
			string &vertexShaderName, string &fragmentShaderName) const;
	};
	REGISTER_OSGPLUGIN(gltf, ReaderWriterGLTF)
}

#endif