#include "gltf.h"

#include <osgViewer/Viewer>

using namespace gltf;


bool ReaderWriterGLTF::getBufferData(const tinygltf::Accessor &accessor, dataType_t &bufferData, int &flagType,
		std::vector<unsigned char>::const_iterator &bufferDataOrigin) const{
	switch (accessor.componentType)
	{
	case TINYGLTF_COMPONENT_TYPE_FLOAT:{
		memcpy(&(bufferData.dataFloat), &bufferDataOrigin[0], sizeof(float));//将二进制数据解析成float类型
		bufferDataOrigin += sizeof(float);//根据需要解析成的类型，对迭代器进行操作，使其执行下一个需要操作的二进制数据地址
		flagType = TINYGLTF_COMPONENT_TYPE_FLOAT;
		break;
	}
	case TINYGLTF_COMPONENT_TYPE_SHORT:{
		memcpy(&(bufferData.dataShort), &bufferDataOrigin[0], sizeof(short)); 
		bufferDataOrigin += sizeof(short);
		flagType = TINYGLTF_COMPONENT_TYPE_SHORT;
		break;
	}
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:{
		memcpy(&(bufferData.dataUnsignedShort), &bufferDataOrigin[0], sizeof(unsigned short)); 
		bufferDataOrigin += sizeof(unsigned short);
		flagType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
		break;
	}
	case TINYGLTF_COMPONENT_TYPE_INT:{
		memcpy(&(bufferData.dataInt), &bufferDataOrigin[0], sizeof(int)); 
		bufferDataOrigin += sizeof(int);
		flagType = TINYGLTF_COMPONENT_TYPE_INT;
		break;
	}
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:{
		memcpy(&(bufferData.dataUnsignedInt), &bufferDataOrigin[0], sizeof(unsigned int)); 
		bufferDataOrigin += sizeof(unsigned);
		flagType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT;
		break;
	}
	case TINYGLTF_COMPONENT_TYPE_DOUBLE:{
		memcpy(&(bufferData.dataDouble), &bufferDataOrigin[0], sizeof(double)); 
		bufferDataOrigin += sizeof(double);
		flagType = TINYGLTF_COMPONENT_TYPE_DOUBLE;
		break;
	}

	default:
		break;
	}

	return true;
}


void ReaderWriterGLTF::pushVec3Data(int flagType, 
		dataType_t *bufferData, osg::Vec3Array *vertices) const{
	//根据flagType的值确定compontType类型，double、float、int等
	switch(flagType){
	case TINYGLTF_COMPONENT_TYPE_FLOAT:{
		osg::Vec3 temp(bufferData[0].dataFloat, bufferData[1].dataFloat, 
			bufferData[2].dataFloat);
		//			std::cout << bufferData[0].dataFloat << " " << bufferData[1].dataFloat << " "
		//				<< bufferData[2].dataFloat << std::endl;
		vertices->push_back(temp);
		break;}
	case TINYGLTF_COMPONENT_TYPE_SHORT:{
		osg::Vec3 temp(bufferData[0].dataShort, bufferData[1].dataShort, 
			bufferData[2].dataShort);
		vertices->push_back(temp);
		break;}
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:{
		osg::Vec3 temp(bufferData[0].dataUnsignedShort, bufferData[1].dataUnsignedShort, 
			bufferData[2].dataUnsignedShort);
		vertices->push_back(temp);
		break;}
	case TINYGLTF_COMPONENT_TYPE_INT:{
		osg::Vec3 temp(bufferData[0].dataInt, bufferData[1].dataInt, 
			bufferData[2].dataInt);
		vertices->push_back(temp);
		break;}
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:{
		osg::Vec3 temp(bufferData[0].dataUnsignedInt, bufferData[1].dataUnsignedInt, 
			bufferData[2].dataUnsignedInt);
		vertices->push_back(temp);
		break;}
	case TINYGLTF_COMPONENT_TYPE_DOUBLE:{
		osg::Vec3d temp(bufferData[0].dataDouble, bufferData[1].dataDouble, 
			bufferData[2].dataDouble);
		vertices->push_back(temp);
		break;}
	default:
		break;
	}
}

//获取Buffer中的内容，根据accessor、bufferview的信息将二进制文件内容解析成顶点信息
//解析Vec3Array
bool ReaderWriterGLTF::getContextBuffer(const tinygltf::Scene &scene, const tinygltf::Accessor &accessor, 
					  const tinygltf::BufferView &bufferView, osg::Vec3Array *vertices) const{
	std::map<std::string, tinygltf::Buffer>::const_iterator it;
	it = scene.buffers.find(bufferView.buffer);
	if(it == scene.buffers.end()){
		return false;
	}else{
		tinygltf::Buffer buffer;

		buffer.name = it->second.name;
		buffer.data = it->second.data;


		//根据accessor.type的类型确定数据的类型Vec3Array、Vec2Array等
		int byteOffsetVertice = bufferView.byteOffset + accessor.byteOffset;
		std::vector<unsigned char>::const_iterator itData = buffer.data.begin() + byteOffsetVertice;
		vertices->reserve(accessor.count);

		for(size_t i=0; i<accessor.count; i++){
			int flagType = 0;
			//迭代器指向当前需要操作的二进制数据的地址

			std::vector<unsigned char>::const_iterator itDataOrigin = itData;

			dataType_t bufferData[3];
			for(int j=0; j<3; j++){
				//从itData迭代器指向的地址获取数据存储在bufferData中，
				//执行完成后将itData指向下一个需要操作的数据
				getBufferData(accessor, bufferData[j], flagType, itData);
			}

			if(accessor.byteStride != 0){ 
				itData = itDataOrigin + accessor.byteStride;
			}

			pushVec3Data(flagType, bufferData, vertices);
		}
	}

	return true;
}



void ReaderWriterGLTF::pushVec2Data(int flagType, 
		dataType_t *bufferData, osg::Vec2Array *vertices) const{
			//根据flagType的值确定compontType类型，double、float、int等
	switch(flagType){
	case TINYGLTF_COMPONENT_TYPE_FLOAT:{
		osg::Vec2 temp(bufferData[0].dataFloat, bufferData[1].dataFloat);
		//			std::cout << bufferData[0].dataFloat << " " << bufferData[1].dataFloat 
		//				<< std::endl;
		vertices->push_back(temp);
		break;}
	case TINYGLTF_COMPONENT_TYPE_SHORT:{
		osg::Vec2 temp(bufferData[0].dataShort, bufferData[1].dataShort);
		vertices->push_back(temp);
		break;}
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:{
		osg::Vec2 temp(bufferData[0].dataUnsignedShort, bufferData[1].dataUnsignedShort);
		vertices->push_back(temp);
		break;}
	case TINYGLTF_COMPONENT_TYPE_INT:{
		osg::Vec2 temp(bufferData[0].dataInt, bufferData[1].dataInt);
		vertices->push_back(temp);
		break;}
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:{
		osg::Vec2 temp(bufferData[0].dataUnsignedInt, bufferData[1].dataUnsignedInt);
		vertices->push_back(temp);
		break;}
	case TINYGLTF_COMPONENT_TYPE_DOUBLE:{
		osg::Vec2d temp(bufferData[0].dataDouble, bufferData[1].dataDouble);
		vertices->push_back(temp);
		break;}
	default:
		break;
	}
}
//获取Buffer中的内容，根据accessor、bufferview的信息将二进制文件内容解析成顶点信息
//解析Vec2Array
bool ReaderWriterGLTF::getContextBuffer(const tinygltf::Scene &scene, const tinygltf::Accessor &accessor, 
					  const tinygltf::BufferView &bufferView, osg::Vec2Array *vertices) const{
	std::map<std::string, tinygltf::Buffer>::const_iterator it;
	it = scene.buffers.find(bufferView.buffer);
	if(it == scene.buffers.end()){
		return false;
	}else{
		tinygltf::Buffer buffer;

		buffer.name = it->second.name;
		buffer.data = it->second.data;

		int byteOffsetVertice = bufferView.byteOffset + accessor.byteOffset;
		std::vector<unsigned char>::const_iterator itData = buffer.data.begin() + byteOffsetVertice;
		vertices->reserve(accessor.count);

		for(size_t i=0; i<accessor.count; i++){
			int flagType = 0;
			//迭代器指向当前需要操作的二进制数据的地址
			std::vector<unsigned char>::const_iterator itDataOrigin = itData;

			dataType_t bufferData[2];
			for(int j=0; j<2; j++){
				//从itData迭代器指向的地址获取数据存储在bufferData中，
				//执行完成后将itData指向下一个需要操作的数据
				getBufferData(accessor, bufferData[j], flagType, itData);
			}

			if(accessor.byteStride != 0){
				itData = itDataOrigin + accessor.byteStride;
			}

			pushVec2Data(flagType, bufferData, vertices);
		}
	}
	return true;
}


void ReaderWriterGLTF::pushVec4Data(int flagType, 
		dataType_t *bufferData, osg::Vec4Array *vertices) const{
	//根据flagType的值确定compontType类型，double、float、int等
	switch(flagType){
	case TINYGLTF_COMPONENT_TYPE_FLOAT:{
		osg::Vec4 temp(bufferData[0].dataFloat, bufferData[1].dataFloat, 
			bufferData[2].dataFloat, bufferData[3].dataFloat);
		//	std::cout << bufferData[0].dataFloat << " " << bufferData[1].dataFloat << " " << bufferData[2].dataFloat 
		//		<< " " << bufferData[3].dataFloat << std::endl;
		vertices->push_back(temp);
		break;}
	case TINYGLTF_COMPONENT_TYPE_SHORT:{
		osg::Vec4 temp(bufferData[0].dataShort, bufferData[1].dataShort, 
			bufferData[3].dataShort, bufferData[4].dataShort);
		vertices->push_back(temp);
		break;}
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:{
		osg::Vec4 temp(bufferData[0].dataUnsignedShort, bufferData[1].dataUnsignedShort, 
			bufferData[3].dataUnsignedShort, bufferData[4].dataUnsignedShort);
		vertices->push_back(temp);
		break;}
	case TINYGLTF_COMPONENT_TYPE_INT:{
		osg::Vec4 temp(bufferData[0].dataInt, bufferData[1].dataInt, 
			bufferData[3].dataInt, bufferData[4].dataInt);
		vertices->push_back(temp);
		break;}
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:{
		osg::Vec4 temp(bufferData[0].dataUnsignedInt, bufferData[1].dataUnsignedInt, 
			bufferData[3].dataUnsignedInt, bufferData[4].dataUnsignedInt);
		vertices->push_back(temp);
		break;}
	case TINYGLTF_COMPONENT_TYPE_DOUBLE:{
		osg::Vec4d temp(bufferData[0].dataDouble, bufferData[1].dataDouble, 
			bufferData[3].dataDouble, bufferData[4].dataDouble);
		vertices->push_back(temp);
		break;}
	default:
		break;
	}
}


//获取Buffer中的内容，根据accessor、bufferview的信息将二进制文件内容解析成顶点信息
//解析Vec4Array
bool ReaderWriterGLTF::getContextBuffer(const tinygltf::Scene &scene, const tinygltf::Accessor &accessor, 
		const tinygltf::BufferView &bufferView, osg::Vec4Array *vertices) const{
	std::map<std::string, tinygltf::Buffer>::const_iterator it;
	it = scene.buffers.find(bufferView.buffer);
	if(it == scene.buffers.end()){
		return false;
	}else{
		tinygltf::Buffer buffer;

		buffer.name = it->second.name;
		buffer.data = it->second.data;

		int byteOffsetVertice = bufferView.byteOffset + accessor.byteOffset;
		std::vector<unsigned char>::const_iterator itData = buffer.data.begin() + byteOffsetVertice;
		vertices->reserve(accessor.count);

		for(size_t i=0; i<accessor.count; i++){
			int flagType = 0;
			//迭代器指向当前需要操作的二进制数据的地址
			std::vector<unsigned char>::const_iterator itDataOrigin = itData;

			dataType_t bufferData[4];
			for(int j=0; j<4; j++){
				//从itData迭代器指向的地址获取数据存储在bufferData中，
				//执行完成后将itData指向下一个需要操作的数据
				getBufferData(accessor, bufferData[j], flagType, itData);
			}

			if(accessor.byteStride != 0){
				itData = itDataOrigin + accessor.byteStride;
			}

			//		compareMaxvalueAndMinvalue(accessor, bufferData); //比较最大值，最小值
			pushVec4Data(flagType, bufferData, vertices);
			
		}
	}
	return true;
}

//获取Buffer中的内容，根据accessor、bufferview的信息将二进制文件内容解析成顶点信息
//解析SCALAR
bool ReaderWriterGLTF::getContextBuffer(const tinygltf::Scene &scene, const tinygltf::Accessor &accessor, 
		const tinygltf::BufferView &bufferView, osg::ref_ptr<osg::DrawElementsUShort> &indices) const{
	std::map<std::string, tinygltf::Buffer>::const_iterator it;
	it = scene.buffers.find(bufferView.buffer);
	if(it == scene.buffers.end()){
		return false;
	}else{
		tinygltf::Buffer buffer;

		buffer.name = it->second.name;
		buffer.data = it->second.data;

		int byteOffsetVertice = bufferView.byteOffset + accessor.byteOffset;
		std::vector<unsigned char>::const_iterator itData = buffer.data.begin() + byteOffsetVertice;

		for(size_t i=0; i<accessor.count; i++){
			int flagType = 0;
			std::vector<unsigned char>::const_iterator itDataOrigin = itData;
			dataType_t bufferData;
			getBufferData(accessor, bufferData, flagType, itData);
			//		std::cout << bufferData.dataUnsignedShort << std::endl;

			if(accessor.byteStride != 0){
				itData = itDataOrigin + accessor.byteStride;
			}

			indices->push_back(bufferData.dataUnsignedShort);
		}
	}

	return true;
}


bool ReaderWriterGLTF::getContextBuffer(const tinygltf::Scene &scene, const tinygltf::Accessor &accessor, 
					  const tinygltf::BufferView &bufferView, std::vector<float> &scalar) const{
	std::map<std::string, tinygltf::Buffer>::const_iterator it;
	it = scene.buffers.find(bufferView.buffer);
	if(it == scene.buffers.end()){
		return false;
	}else{
		tinygltf::Buffer buffer;

		buffer.name = it->second.name;
		buffer.data = it->second.data;

		int byteOffsetVertice = bufferView.byteOffset + accessor.byteOffset;
		std::vector<unsigned char>::const_iterator itData = buffer.data.begin() + byteOffsetVertice;

		for(size_t i=0; i<accessor.count; i++){
			int flagType = 0;
			std::vector<unsigned char>::const_iterator itDataOrigin = itData;

			dataType_t bufferData;
			getBufferData(accessor, bufferData, flagType, itData);
			//		std::cout << bufferData.dataUnsignedShort << std::endl;
			if(accessor.byteStride != 0){
				itData = itDataOrigin + accessor.byteStride;
			}
			scalar.push_back(bufferData.dataFloat);
		}
	}
						  
	return true;
}

bool ReaderWriterGLTF::getContextBuffer(const tinygltf::Scene &scene, const tinygltf::Accessor &accessor, 
					  const tinygltf::BufferView &bufferView, osg::MatrixdArray *matrix) const{
	std::map<std::string, tinygltf::Buffer>::const_iterator it;
	it = scene.buffers.find(bufferView.buffer);
	if(it == scene.buffers.end()){
		return false;
	}else{
		tinygltf::Buffer buffer;
		buffer.name = it->second.name;
		buffer.data = it->second.data;
		int byteOffsetVertice = bufferView.byteOffset + accessor.byteOffset;

		for(size_t i=0; i<accessor.count; i++){
			std::vector<unsigned char>::const_iterator itData = 
				buffer.data.begin() + byteOffsetVertice + i*sizeof(float)*16;
			float mat[16];
			for (int k=0; k<16; k++) {
				std::vector<unsigned char>::const_iterator itData1 = itData + k*sizeof(float);
				int flagType = 0;
				dataType_t bufferData;
				getBufferData(accessor, bufferData, flagType, itData1);
				mat[k] = bufferData.dataFloat;
			}
			matrix->push_back(osg::Matrix(mat[0], mat[1], mat[2], mat[3], 
				mat[4], mat[5], mat[6], mat[7], 
				mat[8], mat[9], mat[10], mat[11], 
				mat[12], mat[13], mat[14], mat[15]));
		}
	}
						  
	return true;
}

//获取BufferView中的内容，根据其中的信息解析buffer
template<typename T>
bool ReaderWriterGLTF::getContextBufferView(const tinygltf::Scene &scene,
		const tinygltf::Accessor &accessor, T &vertices, string &err) const{
	map<string, tinygltf::BufferView>::const_iterator it;
	it = scene.bufferViews.find(accessor.bufferView);
	if(it==scene.bufferViews.end()){
		err += "Failed to find bufferView: " + accessor.bufferView + "\n";
		return false;
	}else{
		tinygltf::BufferView bufferView;
		bufferView.buffer = it->second.buffer;
		bufferView.byteLength = it->second.byteLength;
		bufferView.byteOffset = it->second.byteOffset;
		bufferView.name = it->second.name;
		bufferView.target = it->second.target;

		getContextBuffer(scene, accessor, bufferView, vertices);
		return true;
	}
}

//获得Accessor中的内容，根据其中的信息去解析指向的bufferview
template<typename T>
bool ReaderWriterGLTF::getContextAccessor(const tinygltf::Scene &scene, 
		const std::string &accessorName, T &vertices, string &err) const{
	tinygltf::Accessor accessor;

	map<string, tinygltf::Accessor>::const_iterator it;
	it = scene.accessors.find(accessorName);
	if(it==scene.accessors.end()){
		err += "Failed to find accessor:" + accessorName + "\n"; 
		return false;
	}else{
		accessor.bufferView = it->second.bufferView;
		accessor.byteOffset = it->second.byteOffset;
		accessor.componentType = it->second.componentType;
		accessor.count = it->second.count;
		accessor.type = it->second.type;
		accessor.byteStride = it->second.byteStride;
		accessor.maxValues = it->second.maxValues;
		accessor.minValues = it->second.minValues;
		accessor.name = it->second.name;

		getContextBufferView(scene, accessor, vertices, err);

		return true;
	}
}


bool ReaderWriterGLTF::getContextAccessor(const tinygltf::Scene &scene,
			const std::string &accessorName, tinygltf::Accessor &accessor, string &err) const{
	map<string, tinygltf::Accessor>::const_iterator it;
	it = scene.accessors.find(accessorName);
	if(it==scene.accessors.end()){
		err += "Failed to find accessor: " + accessorName + "\n";
		return false;
	}else{
		accessor.bufferView = it->second.bufferView;
		accessor.byteOffset = it->second.byteOffset;
		accessor.componentType = it->second.componentType;
		accessor.count = it->second.count;
		accessor.type = it->second.type;
		accessor.byteStride = it->second.byteStride;
		accessor.maxValues = it->second.maxValues;
		accessor.minValues = it->second.minValues;
		accessor.name = it->second.name;
		return true;
	}
}

void ReaderWriterGLTF::buildMaterialToStateSetMap(const tinygltf::Scene &scene, const std::string materialName, 
								MaterialToStateSetMap &materialToStateSetMap) const{
	map<string, tinygltf::Material>::const_iterator it;
	it = scene.materials.find(materialName);

	osg::ref_ptr<osg::StateSet> stateset = new osg::StateSet;
	osg::ref_ptr<osg::Material> osgMaterial = new osg::Material;

	osgMaterial->setName(it->second.name);

	std::map<std::string, tinygltf::Parameter>::const_iterator 
		itValue(it->second.values.begin());
	std::map<std::string, tinygltf::Parameter>::const_iterator
		itValueEnd(it->second.values.end());
	for(; itValue != itValueEnd; itValue++){
		osg::Vec4d *valueVec4 = NULL;
		double valueDouble;
		if(itValue->second.numberArray.size() == 4){
			double Value[4];
			for(int i=0; i<4; i++){
				Value[i] = *(itValue->second.numberArray.begin() + i);
			}
			valueVec4 = new osg::Vec4d(Value[0], Value[1], Value[2], Value[3]);
		}
		else if(itValue->second.numberArray.size() == 1){
			valueDouble = *(itValue->second.numberArray.begin());
		}

		if(itValue->first == "ambient"){
			osgMaterial->setAmbient(osg::Material::FRONT_AND_BACK, *valueVec4);
		}
		else if(itValue->first == "emission"){
			osgMaterial->setEmission(osg::Material::FRONT_AND_BACK, *valueVec4);
		}
		else if(itValue->first == "specular"){
			osgMaterial->setSpecular(osg::Material::FRONT_AND_BACK, *valueVec4);
		}
		else if(itValue->first == "diffuse"){
			if(itValue->second.stringValue.size() != 0){
				//finished in addTexture function
			}
			else if(itValue->second.numberArray.size() != 0)
				osgMaterial->setDiffuse(osg::Material::FRONT_AND_BACK, *valueVec4);
		}
		else if(itValue->first == "shininess")
			osgMaterial->setShininess(osg::Material::FRONT_AND_BACK, valueDouble);

		if(itValue->second.numberArray.size() == 4){
			delete valueVec4;
			valueVec4 = NULL;
		}
	}
	stateset->setAttribute(osgMaterial);
	materialToStateSetMap[it->first] = stateset.get();
}

bool ReaderWriterGLTF::TypeofPrimitives(const Scene &scene, const tinygltf::Accessor &accessor, 
		osg::Geometry *geometry, vector<tinygltf::Primitive>::const_iterator &itPrimitive, string &err) const{
	switch (itPrimitive->mode)
	{
	case GL_POINTS:{
			osg::ref_ptr<osg::DrawElementsUShort> index = 
				new osg::DrawElementsUShort(GL_POINTS, accessor.count);
			getContextAccessor(scene, itPrimitive->indices, index, err);

			//利用顶点索引信息填充geometry对象
			geometry->addPrimitiveSet(index.get());
			break;
		}
	case GL_LINES:{
			osg::ref_ptr<osg::DrawElementsUShort> index = 
				new osg::DrawElementsUShort(GL_LINES, accessor.count);
			getContextAccessor(scene, itPrimitive->indices, index, err);

			//利用顶点索引信息填充geometry对象
			geometry->addPrimitiveSet(index.get());
			break;
		}
	case GL_LINE_LOOP:{
			osg::ref_ptr<osg::DrawElementsUShort> index = 
				new osg::DrawElementsUShort(GL_LINE_LOOP, accessor.count);
			getContextAccessor(scene, itPrimitive->indices, index, err);

			//利用顶点索引信息填充geometry对象
			geometry->addPrimitiveSet(index.get());
			break;
		}
	case GL_LINE_STRIP:{
			osg::ref_ptr<osg::DrawElementsUShort> index = 
				new osg::DrawElementsUShort(GL_LINE_STRIP, accessor.count);
			getContextAccessor(scene, itPrimitive->indices, index, err);

			//利用顶点索引信息填充geometry对象
			geometry->addPrimitiveSet(index.get());
			break;
		}   
	case GL_TRIANGLES:{
			osg::ref_ptr<osg::DrawElementsUShort> index = 
				new osg::DrawElementsUShort(GL_TRIANGLES, accessor.count);
			getContextAccessor(scene, itPrimitive->indices, index, err);

			//利用顶点索引信息填充geometry对象
			geometry->addPrimitiveSet(index.get());
			break;
		}
	case GL_TRIANGLE_STRIP:{
			osg::ref_ptr<osg::DrawElementsUShort> index = 
				new osg::DrawElementsUShort(GL_TRIANGLE_STRIP, accessor.count);
			getContextAccessor(scene, itPrimitive->indices, index, err);

			//利用顶点索引信息填充geometry对象
			geometry->addPrimitiveSet(index.get());
			break;
		}
	case GL_TRIANGLE_FAN:{
			osg::ref_ptr<osg::DrawElementsUShort> index = 
				new osg::DrawElementsUShort(GL_TRIANGLE_FAN, accessor.count);
			getContextAccessor(scene, itPrimitive->indices, index, err);

			//利用顶点索引信息填充geometry对象
			geometry->addPrimitiveSet(index.get());
			break;
		}
	default:
		err += "No right mode to render primitives\n";
		return false;
	}
	return true;
}

bool ReaderWriterGLTF::getShaderNamebyPrimitives(const tinygltf::Scene &scene, std::vector<tinygltf::Primitive>::const_iterator itPrimitive, 
							   std::string &vertexShaderName, std::string &fragmentShaderName) const{
	map<string, tinygltf::Material>::const_iterator it;
	it = scene.materials.find(itPrimitive->material);
	if(it == scene.materials.end()){
		return false;
	}

	map<string, tinygltf::Techniques>::const_iterator itTechniques;
	itTechniques = scene.technique.find(it->second.technique);
	if(itTechniques == scene.technique.end()){
		return false;
	}

	map<string, tinygltf::Program>::const_iterator itProgram;
	itProgram = scene.programs.find(itTechniques->second.program);
	if(itProgram == scene.programs.end()){
		return false;
	}

	fragmentShaderName = itProgram->second.fragmentShader;
	vertexShaderName = itProgram->second.vertexShader;

	return true;
}

void ReaderWriterGLTF::addShaderUniform(osg::StateSet *ss, const tinygltf::Scene scene) const{
	std::map<string, Techniques>::const_iterator itScene = scene.technique.begin();
	for(; itScene != scene.technique.end(); itScene++){
		std::map<string, string>::const_iterator itUniform = itScene->second.uniforms.begin();
		for(; itUniform!=itScene->second.uniforms.end(); itUniform++){
			ss->addUniform(new osg::Uniform(itUniform->first.c_str(), 0));
		}

	}

}


bool ReaderWriterGLTF::LoadShader(const tinygltf::Scene scene, std::string basePath, osg::StateSet *ss, 
					std::string vertexShaderName, std::string fragmentShaderName) const{
	std::string vsPath = basePath + "\\";
	std::string fsPath = basePath + "\\";

	osg::ref_ptr<osg::Program> programObject = new osg::Program;
	osg::ref_ptr<osg::Shader> vertexObject = new osg::Shader(osg::Shader::VERTEX);
	osg::ref_ptr<osg::Shader> fragmentObject = new osg::Shader(osg::Shader::FRAGMENT);

	map<string, tinygltf::Shaders>::const_iterator it;
	it = scene.shaders.find(vertexShaderName);
	if(it == scene.shaders.end()){
		std::cout << "Could not found: " << vertexShaderName << std::endl;
		return false;
	}
	vsPath += it->second.uri;
	bool success = vertexObject->loadShaderSourceFromFile(vsPath);
	if(!success){
		std::cout << "Could not load file: " << vsPath << std::endl;
		return false;
	}
	programObject->addShader(vertexObject.get());

	it = scene.shaders.find(fragmentShaderName);
	if(it == scene.shaders.end()){
		std::cout << "Could not found: " << fragmentShaderName << std::endl;
		return false;
	}
	fsPath += it->second.uri;
	success = fragmentObject->loadShaderSourceFromFile(fsPath);
	if(!success){
		std::cout << "Could not load file: " << fsPath << std::endl;
		return false;
	}
	programObject->addShader(fragmentObject.get());

	if(ss){
		ss->setAttributeAndModes(programObject.get(), osg::StateAttribute::ON);
	}

	addShaderUniform(ss, scene);

	return true;
}

osg::Geometry* ReaderWriterGLTF::convertElementListToGeometry(const tinygltf::Scene &scene, const std::string basePath, 
		std::vector<tinygltf::Primitive>::const_iterator itPrimitive, string &err) const{
	osg::Geometry *geometry = new osg::Geometry;
	//load shader
	//std::string vertexShaderName, fragmentShaderName;
	//getShaderNamebyPrimitives(scene, itPrimitive, vertexShaderName, fragmentShaderName);
	//osg::StateSet *ss = geometry->getOrCreateStateSet();
	//LoadShader(scene, basePath, ss, vertexShaderName, fragmentShaderName);

	std::map<std::string, std::string>::const_iterator itAttr = itPrimitive->attributes.begin();
	std::map<std::string, std::string>::const_iterator itAttrEnd = itPrimitive->attributes.end();

	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;
	osg::ref_ptr<osg::Vec4Array> joint = new osg::Vec4Array;
	osg::ref_ptr<osg::Vec4Array> weight = new osg::Vec4Array;
	
	map<string, string>::const_iterator it;
	it = itPrimitive->attributes.find("POSITION");
	if(it != itPrimitive->attributes.end()){
		if(!getContextAccessor(scene, it->second, vertices, err)){
			err += "Failed to get primitives attributes POSITION\n";
		}
		geometry->setVertexArray(vertices.get());
	}

	it = itPrimitive->attributes.find("NORMAL");
	if(it != itPrimitive->attributes.end()){
		if(!getContextAccessor(scene, it->second, normals, err)){
			err += "Failed to get primitives attributes NORMAL\n";
		}

		geometry->setNormalArray(normals.get());
		geometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	}

	it = itPrimitive->attributes.find("TEXCOORD_0");
	if(it != itPrimitive->attributes.end()){
		if(!getContextAccessor(scene, it->second, texcoords, err)){
			err += "Failed to get primitives attributes " + it->first + "\n";
		}

		osg::ref_ptr<osg::Vec2Array> texcoordsTmp = new osg::Vec2Array;
		osg::Vec2 temp;
		int count = texcoords->getNumElements();
		for(int i = 0; i < count; i++){
			temp = (*texcoords)[i];
			osg::Vec2 texcoordsValue(temp.x(), 1-temp.y());
			texcoordsTmp->push_back(texcoordsValue);
		}

		geometry->setTexCoordArray(0, texcoordsTmp.get());//设置纹理坐标信息
	}

	it = itPrimitive->attributes.find("JOINT");
	if(it != itPrimitive->attributes.end()){
		if(!getContextAccessor(scene, it->second, joint, err)){
			err += "Failed to get primitives attributes JOINT\n";
		}

	}

	it = itPrimitive->attributes.find("WEIGHT");
	if(it != itPrimitive->attributes.end()){
		if(!getContextAccessor(scene, it->second, weight, err)){
			err += "Failed to get primitives attributes WEIGHT\n";
		}
	}

	tinygltf::Accessor accessor;
	if(!getContextAccessor(scene, itPrimitive->indices, accessor, err)){
		err += "Failed to get primitives indeices\n";
	}

	//The type of primitives to render
	if(!TypeofPrimitives(scene, accessor, geometry, itPrimitive, err)){
		err += "Not correct primitives mode\n";
	}

	return geometry;
}

bool ReaderWriterGLTF::getContextImage(const tinygltf::Scene &scene, const std::string imageName,  
		const std::string basePath, osg::ref_ptr<osg::Image> &image) const{
	map<string, tinygltf::Image>::const_iterator it;
	it = scene.images.find(imageName);
	if(it == scene.images.end()){
		return false;
	}else{
		string imagePath;
		imagePath = basePath + "\\" + it->second.uri;
		image = osgDB::readImageFile(imagePath);
	}

	return true;
}

bool ReaderWriterGLTF::getContextTexture(const tinygltf::Scene &scene, const std::string &textureName,
		osg::ref_ptr<osg::Texture2D> &texture2D, const std::string basePath) const{
	map<string, tinygltf::Texture>::const_iterator it;
	it = scene.textures.find(textureName);
	if(it == scene.textures.end()){
		return false;
	}else{
		osg::ref_ptr<osg::Image> image;
		getContextImage(scene, it->second.source, basePath, image);
		texture2D->setImage(image.get());
	}

	return true;
}

bool ReaderWriterGLTF::addTexture(osg::Geometry *geometry, const tinygltf::Scene &scene, 
				std::string basePath, std::string materialName) const{
	map<string, tinygltf::Material>::const_iterator itMaterial;
	itMaterial = scene.materials.find(materialName);
	if(itMaterial == scene.materials.end()){
		return false;
	}else{
		map<string, tinygltf::Parameter>::const_iterator it;
		it = itMaterial->second.values.find("diffuse");
		if(it == itMaterial->second.values.end()){
			return false;
		}else{
			osg::ref_ptr<osg::Texture2D> texture2D = new osg::Texture2D;
			/*texture2D->setDataVariance(osg::Object::DYNAMIC);
			texture2D->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
			texture2D->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
			texture2D->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP);
			texture2D->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP);*/
			if(it->second.stringValue.size() != 0){
				getContextTexture(scene, it->second.stringValue, texture2D, basePath);
			}
			geometry->getOrCreateStateSet()->ref();
			geometry->getOrCreateStateSet()->setTextureAttributeAndModes(0, 
				texture2D.get(), osg::StateAttribute::ON);
		}
	}
	
	return true;
}

//get geode object
bool ReaderWriterGLTF::getGeode(const std::string meshName, const tinygltf::Scene &scene, 
						   const std::string basePath, osg::Geode *geode, string &err) const{
	map<string, tinygltf::Mesh>::const_iterator it;
	it = scene.meshes.find(meshName);
	std::vector<tinygltf::Primitive>::const_iterator itPrimitive = 
		it->second.primitives.begin();
	std::vector<tinygltf::Primitive>::const_iterator itPrimitiveEnd = 
		it->second.primitives.end();
	//add all primitives in meshs
	for(; itPrimitive != itPrimitiveEnd; itPrimitive++){
		//set material
		MaterialToStateSetMap materialToStateSetMap;
		buildMaterialToStateSetMap(scene, itPrimitive->material, materialToStateSetMap);

		//convert every primitives to geometry object 
		osg::ref_ptr<osg::Geometry> geometry = convertElementListToGeometry(scene, basePath, itPrimitive, err);
		osg::ref_ptr<osg::StateSet> stateSet = materialToStateSetMap[itPrimitive->material].get();
		geometry->setStateSet(stateSet);

		//add texture
		addTexture(geometry, scene, basePath, itPrimitive->material);

		geode->addDrawable(geometry);
	}

	return true;
}

bool ReaderWriterGLTF::addGeodeToTransform(const itNode_t itNode, const Scene &scene, 
		const string basePath, osg::ref_ptr<osg::MatrixTransform> transform, string &err) const{
	osg::Matrix matrix;

	osg::ref_ptr<osg::Geode> geode= new osg::Geode;
	std::vector<std::string>::const_iterator itmesh = itNode->second.meshes.begin();
	std::vector<std::string>::const_iterator itmeshEnd = itNode->second.meshes.end();

	for(; itmesh != itmeshEnd; itmesh++){
		if(!getGeode(*itmesh, scene, basePath, geode, err)){
			err += "Failed to get geode\n";
		}
	}
	//this scene do not have animation
	if(scene.animation.size() == 0){
		//if node has matrix
		if(itNode->second.matrix.size() != 0){
			double matrixData[16];
			for(size_t i = 0; i < 16; i++){
				matrixData[i] = *(itNode->second.matrix.begin()+i);
			}
			matrix =  osg::Matrix(matrixData[0], matrixData[1], matrixData[2], matrixData[3],
				matrixData[4], matrixData[5], matrixData[6], matrixData[7],
				matrixData[8], matrixData[9], matrixData[10], matrixData[11],
				matrixData[12], matrixData[13], matrixData[14], matrixData[15]);
		}

		//if node has rotation
		if(itNode->second.rotation.size() != 0){
			std::vector<double>::const_iterator it = itNode->second.rotation.begin();
			matrix = matrix * osg::Matrix::rotate(*it, *(it+1), *(it+2), *(it+3));
		}
		//if node has translation
		if(itNode->second.translation.size() != 0){
			std::vector<double>::const_iterator it = itNode->second.translation.begin();
			matrix = matrix * osg::Matrix::translate(*it, *(it+1), *(it+2));
		}
		//if node has scale
		if(itNode->second.scale.size() != 0){
			std::vector<double>::const_iterator it = itNode->second.scale.begin();
			matrix = matrix * osg::Matrix::scale(*it, *(it+1), *(it+2));
		}

		//transform->setName(itNode->first);
		transform->setMatrix(matrix);
	}else{
		osg::ref_ptr<osgAnimation::UpdateMatrixTransform> update = new osgAnimation::UpdateMatrixTransform(itNode->first);					
		//if node has translation
		if(itNode->second.translation.size() != 0){
			std::vector<double>::const_iterator it = itNode->second.translation.begin();
			update->getStackedTransforms().push_back(new osgAnimation::StackedTranslateElement(
				"translation", osg::Vec3(*it, *(it+1), *(it+2))));
		}
		//if node has rotation
		if(itNode->second.rotation.size() != 0){
			std::vector<double>::const_iterator it = itNode->second.rotation.begin();
			update->getStackedTransforms().push_back(new osgAnimation::StackedQuaternionElement(
				"rotation", osg::Vec4(*it, *(it+1), *(it+2), *(it+3))));
		}
		//if node has scale
		if(itNode->second.scale.size() != 0){
			std::vector<double>::const_iterator it = itNode->second.scale.begin();
			update->getStackedTransforms().push_back(new osgAnimation::StackedScaleElement(
				"scale", osg::Vec3(*it, *(it+1), *(it+2))));
		}

		transform->setName(itNode->first);
		transform->setUpdateCallback(update);
	}

	transform->addChild(geode);

	return true;
}

bool ReaderWriterGLTF::addRigGeodeToTransform(const itNode_t itNode, const Scene &scene, 
		const string basePath, osg::ref_ptr<osg::MatrixTransform> transform, string &err)const{
//	osg::Group *group= new osg::Group;
//	std::vector<std::string>::const_iterator itmesh = itNode->second.meshes.begin();
//	std::vector<std::string>::const_iterator itmeshEnd = itNode->second.meshes.end();
//	std::vector<std::string>::const_iterator itskeleton = itNode->second.skeletons.begin();
//	std::vector<std::string>::const_iterator itskeletonEnd = itNode->second.skeletons.end();
//
//	for(; itmesh != itmeshEnd && itskeleton != itskeletonEnd; itmesh++, itskeleton++){
//		osg::Geode *geode= new osg::Geode;
//		getRigGeode(*itmesh, scene, basePath, geode);
//		tinygltf::Node node = findNodeByName(scene, *itskeleton);
//
//		osg::ref_ptr<osgAnimation::Skeleton> skelroot = new osgAnimation::Skeleton;
//		skelroot->setDefaultUpdateCallback();
//
//		addChildrenNode(scene, node, skelroot);
//
//		skelroot->addChild(geode);
//		group->addChild(skelroot);
//	}
//	transform->addChild(group);
//
	return true;
}

bool ReaderWriterGLTF::getNode(const std::string nodeName, const tinygltf::Scene &scene, 
				   const std::string basePath, osg::Group *group, std::string &err) const{
	map<string, tinygltf::Node>::const_iterator itNode;
	itNode = scene.nodes.find(nodeName);

	group->setName(nodeName);

	//make some transform
	osg::ref_ptr<osg::MatrixTransform> transform = new osg::MatrixTransform;
	if(!addGeodeToTransform(itNode, scene, basePath, transform, err)){
		err += "Failed to add geode object to transform\n";
		return false;
	}


	////if node has no 'mesh'
	//if(itNode->second.meshes.size() == 0){
	//	if(!addGeodeToTransform(itNode, scene, basePath, transform, err)){
	//		err += "Failed to add geode object to transform\n";
	//		return false;
	//	}
	//}
	////if node has 'mesh' and do not have 'skeleton'
	//else if(itNode->second.meshes.size() != 0 && itNode->second.skeletons.size() == 0){
	//	if(!addGeodeToTransform(itNode, scene, basePath, transform, err)){
	//		err += "Failed to add geode object to transform\n";
	//		return false;
	//	}
	//}
	////if node has 'mesh' and 'skeleton'
	//else if(itNode->second.meshes.size() != 0 && itNode->second.skeletons.size() != 0){
	//	if(!addRigGeodeToTransform(itNode, scene, basePath, transform, err)){
	//		err += "Failed to add riggeode object to transform\n";
	//		return false;
	//	}
	//}

	//if node has child, add every child to group
	if(itNode->second.children.size() != 0){
		vector<string>::const_iterator it = itNode->second.children.begin();
		for(; it!=itNode->second.children.end(); it++){
			osg::Group *childGroup = new osg::Group;
			if(!getNode(*it, scene, basePath, childGroup, err)){
				err += "Failed to add child node\n";
				return false;
			}

			transform->addChild(childGroup);
		}
	}

	group->addChild(transform);

	return true;
}

bool ReaderWriterGLTF::getAccessorNamefromChannel(
		std::vector<tinygltf::AnimationChannel>::const_iterator itAnimChannel, 
		std::map<std::string, tinygltf::Animations>::const_iterator itAnimation, 
		std::map<std::string, std::string> &accessorName) const{

	map<string, tinygltf::AnimationSampler>::const_iterator it;
	it = itAnimation->second.samplers.find(itAnimChannel->sampler);
	if(it == itAnimation->second.samplers.end()){
		return false;
	}else{
		if(it->second.input == "TIME"){
			accessorName["input"] = itAnimation->second.parameters.TIME;
		}

		if(it->second.output == "rotation"){
			accessorName["output"] = itAnimation->second.parameters.rotation;
		}
		else if(it->second.output == "scale"){
			accessorName["output"] = itAnimation->second.parameters.scale;
		}
		else if(it->second.output == "translation"){
			accessorName["output"] = itAnimation->second.parameters.translation;
		}

		return true;
	}
}

bool ReaderWriterGLTF::findAnimationEndTime(const tinygltf::Scene &scene, 
											float &animationEndTime, string &err) const{
	std::map<std::string, tinygltf::Animations>::const_iterator it = scene.animation.begin();
	std::map<std::string, tinygltf::Animations>::const_iterator itEnd = scene.animation.end();
	for(; it!=itEnd; it++){
		std::vector<float> time;
		if(!getContextAccessor(scene, it->second.parameters.TIME, time, err)){
			err += "Failed to get animation time\n";
			return false;
		}

		if(animationEndTime < *(time.end()-1)){
			animationEndTime = *(time.end()-1);
		}
	}

	return true;
}


bool ReaderWriterGLTF::createKeyframeAnimation(osg::Group *group, 
		const tinygltf::Scene &scene, string &err) const{
	std::map<std::string, tinygltf::Animations>::const_iterator itAnimation =
		scene.animation.begin();
	std::map<std::string, tinygltf::Animations>::const_iterator itAnimationEnd =
		scene.animation.end();
	
	// find the end time of animation
	float animationEndTime = 0;
	if(!findAnimationEndTime(scene, animationEndTime, err)){
		err += "Failed to find animation end time\n";
		return false;
	}


	osgAnimation::BasicAnimationManager* manager = new osgAnimation::BasicAnimationManager;
	group->setUpdateCallback(manager);
	//traverse all animation
	for(; itAnimation != itAnimationEnd; itAnimation++){
		std::vector<tinygltf::AnimationChannel>::const_iterator itAnimChannel = 
			itAnimation->second.channels.begin();
		std::vector<tinygltf::AnimationChannel>::const_iterator itAnimChannelEnd = 
			itAnimation->second.channels.end();

		osg::ref_ptr<osgAnimation::Animation> animation = new osgAnimation::Animation;
		animation->setPlayMode(osgAnimation::Animation::LOOP);

		//one animation has several channels, but the channels' target ID is same. 
		//when we create updater, the updater's name is same. we only need to push 
		//defferent transform.
		//std::string targetNodeName = itAnimChannel->target.id;

		//traverse all chinnal in animation
		for(; itAnimChannel != itAnimChannelEnd; itAnimChannel++){
			//get accessor name in parameters using in channel
			//channel -> sampler -> output/input -> accessor name
			std::map<std::string, std::string> accessorName;
			getAccessorNamefromChannel(itAnimChannel, itAnimation, accessorName);
			std::map<std::string, std::string>::const_iterator itSamplersToAccessorName = 
				accessorName.begin();
			std::map<std::string, std::string>::const_iterator itSamplersToAccessorNameEnd = 
				accessorName.end();

			//get accessor using accessor name
			tinygltf::Accessor accessor;
			getContextAccessor(scene, itSamplersToAccessorName->second, accessor, err);

			std::map<std::string, tinygltf::AnimationSampler>::const_iterator itAnimSample;
			itAnimSample = itAnimation->second.samplers.find(itAnimChannel->sampler);
			if(itAnimSample == itAnimation->second.samplers.end()){
				return false;
			}else{
				if(itAnimSample->second.interpolation == "LINEAR"){
					//the output is rotation
					if(itAnimSample->second.output == "rotation"){
						osg::ref_ptr<osgAnimation::QuatKeyframeContainer> rotKey = new osgAnimation::QuatKeyframeContainer;
						osg::ref_ptr<osgAnimation::QuatSphericalLinearSampler> rotSampler = new osgAnimation::QuatSphericalLinearSampler;
						osg::ref_ptr<osgAnimation::QuatSphericalLinearChannel> rotChannel = new osgAnimation::QuatSphericalLinearChannel(rotSampler.get());
						
						rotSampler->setKeyframeContainer(rotKey.get());
						rotChannel->setName(itAnimChannel->target.path);
						rotChannel->setTargetName(itAnimChannel->target.id);
						animation->addChannel(rotChannel.get());
						//get buffer
						std::vector<float> input;
						osg::ref_ptr<osg::Vec4Array> output = new osg::Vec4Array;
						for(; itSamplersToAccessorName != itSamplersToAccessorNameEnd; itSamplersToAccessorName++){
							if(itSamplersToAccessorName->first == "input"){
								getContextAccessor(scene, itSamplersToAccessorName->second, input, err);
							}
						
							if(itSamplersToAccessorName->first == "output"){
								getContextAccessor(scene, itSamplersToAccessorName->second, output, err);
							}
						}
						
						//push keyframe to container
						float time;
						osg::Vec4 outputVec4;
						//animation must start at 0
						rotKey->push_back(osgAnimation::QuatKeyframe(0, (*output)[0]));
						for(size_t i = 0; i < accessor.count; i++){
							time = input[i];
							outputVec4 = (*output)[i];
							rotKey->push_back(osgAnimation::QuatKeyframe(time, outputVec4));
						}
						//add the end time animation
						rotKey->push_back(osgAnimation::QuatKeyframe(animationEndTime, outputVec4));

						////create channel, the channel connect updater through name,
						////channel's targetName = updater's name
						////channel's name = updater's transform
						//osg::ref_ptr<osgAnimation::QuatSphericalLinearChannel> channel = 
						//	new osgAnimation::QuatSphericalLinearChannel;
						//channel->setName(itAnimChannel->target.path);
						//channel->setTargetName(itAnimChannel->target.id);
						//
						////create container from channel
						//osgAnimation::QuatKeyframeContainer *container = 
						//	channel->getOrCreateSampler()->getOrCreateKeyframeContainer();
						//
						////get buffer
						//std::vector<float> input;
						//osg::ref_ptr<osg::Vec4Array> output = new osg::Vec4Array;
						//for(; itSamplersToAccessorName != itSamplersToAccessorNameEnd; itSamplersToAccessorName++){
						//	if(itSamplersToAccessorName->first == "input"){
						//		getContextAccessor(scene, itSamplersToAccessorName->second, input, err);
						//	}
						//
						//	if(itSamplersToAccessorName->first == "output"){
						//		getContextAccessor(scene, itSamplersToAccessorName->second, output, err);
						//	}
						//}
						//
						////push keyframe to container
						//float inputScalar;
						//osg::Vec4 outputVec4;
						//for(size_t i = 0; i < accessor.count; i++){
						//	inputScalar = input[i];
						//	outputVec4 = (*output)[i];
						//	container->push_back(osgAnimation::QuatKeyframe(inputScalar, outputVec4));
						//}
						//
						////add channel to animation
						//animation->addChannel(channel);
					}
					//the output is translation or scale
					else {
						osg::ref_ptr<osgAnimation::Vec3KeyframeContainer> Key = new osgAnimation::Vec3KeyframeContainer;
						osg::ref_ptr<osgAnimation::Vec3LinearSampler> Sampler = new osgAnimation::Vec3LinearSampler;
						osg::ref_ptr<osgAnimation::Vec3LinearChannel> Channel = new osgAnimation::Vec3LinearChannel(Sampler.get());
						Sampler->setKeyframeContainer(Key.get());
						Channel->setName(itAnimChannel->target.path);
						Channel->setTargetName(itAnimChannel->target.id);
						animation->addChannel(Channel.get());
						//get buffer
						std::vector<float> input;
						osg::ref_ptr<osg::Vec3Array> output = new osg::Vec3Array;
						for(; itSamplersToAccessorName != itSamplersToAccessorNameEnd; itSamplersToAccessorName++){
							if(itSamplersToAccessorName->first == "input"){
								getContextAccessor(scene, itSamplersToAccessorName->second, input, err);
							}
						
							if(itSamplersToAccessorName->first == "output"){
								getContextAccessor(scene, itSamplersToAccessorName->second, output, err);
							}
						}
						
						//push keyframe to container
						float time;
						osg::Vec3 outputVec3;
						Key->push_back(osgAnimation::Vec3Keyframe(0, (*output)[0]));
						for(size_t i = 0; i < accessor.count; i++){
							time = input[i];
							outputVec3 = (*output)[i];
							Key->push_back(osgAnimation::Vec3Keyframe(time, outputVec3));
						}
						Key->push_back(osgAnimation::Vec3Keyframe(animationEndTime, outputVec3));
					}
				}
				//the interpolation is not "LINEAR"
				else{
				}
			}
		}//end of channel

		//register animation manager
		manager->registerAnimation(animation.get());
		//play animation
		manager->playAnimation(animation.get());
	}//end of animation

	return true;
}

osg::Node* ReaderWriterGLTF::convertModelToSceneGraph(
		Scene &scene, string basePath, string &err) const{
	Group *root = new Group;
	ref_ptr<osg::MatrixTransform> transform = new MatrixTransform;

	map<string, vector<string>>::const_iterator it;
	it = scene.scenes.find(scene.defaultScene);

	vector<string>::const_iterator itNodes;
	itNodes = it->second.begin();
	for(; itNodes!=it->second.end(); itNodes++){
		Group *group = new osg::Group;
		if(!getNode(*itNodes, scene, basePath, group, err)){
			err += "Failed to decode 'scenes'\n";break;
		}
		transform->asGroup()->addChild(group);
	}
	root->addChild(transform);


	//create animation
	if(scene.animation.size() != 0){
		createKeyframeAnimation(root, scene, err);
	}


	return root;
}



ReaderWriterGLTF::ReaderWriterGLTF(){
	supportsExtension("gltf", "GL Transmission Format");
}

const char* ReaderWriterGLTF::className() const{
	return "ReaderWriterGLTF";
}

ReaderWriterGLTF::ReadResult ReaderWriterGLTF::readNode(const string &file, 
		const Options *opt) const{

	string ext = getLowerCaseFileExtension(file);
	if(!acceptsExtension(ext)){
		return ReaderWriterGLTF::ReadResult::FILE_NOT_HANDLED;
	}

	string fileName = findDataFile(file, opt);
	if(fileName.empty()){
		return ReaderWriterGLTF::ReadResult::FILE_NOT_FOUND;
	}

	Scene scene;
	TinyGLTFLoader loader;
	string err;
	loader.LoadFromFile(scene, err, file);

	if (!err.empty()) {
		printf("Err: %s\n", err.c_str());
	}

	string ERR;
	string basePath = GetBaseDir(file);
	cout << basePath << endl;
	osg::ref_ptr<osg::Node> node = convertModelToSceneGraph(scene, basePath, ERR);

	if(!ERR.empty()){
		cout << "Err: " << ERR.c_str() << endl;
	}

	return node;
}

int main(){
	ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	//viewer->setSceneData(osgDB::readNodeFile("C:\\glTF\\sampleModels\\2_cylinder_engine\\glTF\\2_cylinder_engine.gltf"));
	//viewer->setSceneData(osgDB::readNodeFile("C:\\glTF\\sampleModels\\CesiumMilkTruck\\glTF\\CesiumMilkTruck.gltf"));
	viewer->setSceneData(osgDB::readNodeFile("C:\\glTF\\sampleModels\\boxAnimated\\glTF\\glTF.gltf"));
	//viewer->setSceneData(osgDB::readNodeFile("C:\\glTF\\sampleModels\\buggy\\glTF\\buggy.gltf"));
	//viewer->setSceneData(osgDB::readNodeFile("C:\\glTF\\sampleModels\\gearbox_assy\\glTF\\gearbox_assy.gltf"));
	//viewer->setSceneData(osgDB::readNodeFile("C:\\glTF\\sampleModels\\Reciprocating_Saw\\glTF\\Reciprocating_Saw.gltf"));

	//viewer->setSceneData(osgDB::readNodeFile("C:\\glTF\\sampleModels\\new\\monkey\\monkey.gltf"));
	//viewer->setSceneData(osgDB::readNodeFile("C:\\glTF\\sampleModels\\new\\Test_Ball_Hard\\Test_Ball_Hard.gltf"));
	return viewer->run();
}
