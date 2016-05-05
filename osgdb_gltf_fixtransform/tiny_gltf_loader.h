//
// Tiny glTF loader.
//
// Copyright (c) 2015, Syoyo Fujita.
// All rights reserved.
// (Licensed under 2-clause BSD liecense)
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this
//    list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//
// Version:
//  - v0.9.2 Support parsing `texture`
//  - v0.9.1 Support loading glTF asset from memory
//  - v0.9.0 Initial
//
// Tiny glTF loader is using following libraries:
//
//  - picojson: C++ JSON library.
//  - base64: base64 decode/encode library.
//  - stb_image: Image loading library.
//
#ifndef TINY_GLTF_LOADER_H_
#define TINY_GLTF_LOADER_H_

#include <string>
#include <vector>
#include <map>

namespace tinygltf {

#define TINYGLTF_MODE_POINTS (0)
#define TINYGLTF_MODE_LINE (1)
#define TINYGLTF_MODE_LINE_LOOP (2)
#define TINYGLTF_MODE_TRIANGLES (4)
#define TINYGLTF_MODE_TRIANGLE_STRIP (5)
#define TINYGLTF_MODE_TRIANGLE_FAN (6)

#define TINYGLTF_COMPONENT_TYPE_BYTE (5120)
#define TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE (5121)
#define TINYGLTF_COMPONENT_TYPE_SHORT (5122)
#define TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT (5123)
#define TINYGLTF_COMPONENT_TYPE_INT (5124)
#define TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT (5125)
#define TINYGLTF_COMPONENT_TYPE_FLOAT (5126)
#define TINYGLTF_COMPONENT_TYPE_DOUBLE (5127)

#define TINYGLTF_TYPE_VEC2 (2)
#define TINYGLTF_TYPE_VEC3 (3)
#define TINYGLTF_TYPE_VEC4 (4)
#define TINYGLTF_TYPE_MAT2 (32 + 2)
#define TINYGLTF_TYPE_MAT3 (32 + 3)
#define TINYGLTF_TYPE_MAT4 (32 + 4)
#define TINYGLTF_TYPE_SCALAR (64 + 1)
#define TINYGLTF_TYPE_VECTOR (64 + 4)
#define TINYGLTF_TYPE_MATRIX (64 + 16)

#define TECHNIQUE_PARAMETER_TYPE_BYTE (5120)
#define TECHNIQUE_PARAMETER_TYPE_UNSIGNED_BYTE (5121)
#define TECHNIQUE_PARAMETER_TYPE_SHORT (5122)
#define TECHNIQUE_PARAMETER_TYPE_UNSIGNED_SHORT (5123)
#define TECHNIQUE_PARAMETER_TYPE_INT (5124)
#define TECHNIQUE_PARAMETER_TYPE_UNSIGNED_INT (5125)
#define TECHNIQUE_PARAMETER_TYPE_FLOAT (5126)
#define TECHNIQUE_PARAMETER_TYPE_FLOAT_VEC2 (35664)
#define TECHNIQUE_PARAMETER_TYPE_FLOAT_VEC3 (35665)
#define TECHNIQUE_PARAMETER_TYPE_FLOAT_VEC4 (35666)
#define TECHNIQUE_PARAMETER_TYPE_INT_VEC2 (35667)
#define TECHNIQUE_PARAMETER_TYPE_INT_VEC3 (35668)
#define TECHNIQUE_PARAMETER_TYPE_INT_VEC4 (35669)
#define TECHNIQUE_PARAMETER_TYPE_BOOL (35670)
#define TECHNIQUE_PARAMETER_TYPE_BOOL_VEC2 (35671)
#define TECHNIQUE_PARAMETER_TYPE_BOOL_VEC3 (35672)
#define TECHNIQUE_PARAMETER_TYPE_BOOL_VEC4 (35673)
#define TECHNIQUE_PARAMETER_TYPE_FLOAT_MAT2 (35674)
#define TECHNIQUE_PARAMETER_TYPE_FLOAT_MAT3 (35675)
#define TECHNIQUE_PARAMETER_TYPE_FLOAT_MAT4 (35676)
#define TECHNIQUE_PARAMETER_TYPE_SAMPLER_2D (35678)


#define TINYGLTF_IMAGE_FORMAT_JPEG (0)
#define TINYGLTF_IMAGE_FORMAT_PNG (1)
#define TINYGLTF_IMAGE_FORMAT_BMP (2)
#define TINYGLTF_IMAGE_FORMAT_GIF (3)

#define TINYGLTF_TEXTURE_FORMAT_RGBA (6408)
#define TINYGLTF_TEXTURE_TARGET_TEXTURE2D (3553)
#define TINYGLTF_TEXTURE_TYPE_UNSIGNED_BYTE (5121)

#define TINYGLTF_TARGET_ARRAY_BUFFER (34962)
#define TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER (34963)

typedef struct {
  std::string stringValue;
  std::vector<double> numberArray;
} Parameter;

typedef std::map<std::string, Parameter> ParameterMap;

//原本的Image的数据结构

//typedef struct {
//  std::string name;
//  int width;
//  int height;
//  int component;
//  std::vector<unsigned char> image;
//} Image;

typedef struct {
	std::string name;
	std::string uri;
}Image;

typedef struct {
  int format;
  int internalFormat;
  std::string sampler;  // Required
  std::string source;   // Required
  int target;
  int type;
  std::string name;
} Texture;

typedef struct {
  std::string name;
  std::string technique;
  ParameterMap values;
} Material;

typedef struct {
  std::string name;
  std::string buffer;  // Required
  size_t byteOffset;   // Required
  size_t byteLength;   // default: 0
  int target;
} BufferView;

typedef struct {
  std::string bufferView;
  std::string name;
  size_t byteOffset;
  size_t byteStride;
  int componentType;  // One of TINYGLTF_COMPONENT_TYPE_***
  size_t count;
  int type;                       // One of TINYGLTF_TYPE_***
  std::vector<double> minValues;  // Optional
  std::vector<double> maxValues;  // Optional
} Accessor;

//着色器程序
typedef struct {
	std::vector<std::string> attributes;
	std::string fragmentShader;
	std::string vertexShader;
	std::string name;
}Program;

//着色器
typedef struct {
	std::string uri;
	double type;
	std::string name;
}Shaders;

//technique
typedef struct {
	double count;
	std::string node;
	double type;
	std::string semantic;
	std::string value;//待补充，可以是其他类型的数据
}TechniquesParameter;

///enum Function {
//};
typedef struct {
	std::vector<double> enable;
	//Function functions; //枚举类型，表示可以用到的函数，待补充
}TechniquesState;

typedef struct {
	std::map<std::string, TechniquesParameter> parameters;
	std::map<std::string, std::string> attributes;
	std::string program;
	std::map<std::string, std::string> uniforms;
	TechniquesState states; 
	std::string name;
}Techniques;

//animation
typedef struct {
	std::string id;
	std::string path;
}AnimationChannelTarget;

typedef struct {
	std::string sampler;
	AnimationChannelTarget target;
}AnimationChannel;

typedef struct {
	std::string TIME;
	std::string rotation;
	std::string scale;
	std::string translation;
}AnimationParameters;

typedef struct {
	std::string input;
	std::string interpolation;
	std::string output;
}AnimationSampler;

typedef struct {
	std::vector<AnimationChannel> channels;
	AnimationParameters parameters;
	std::map<std::string, AnimationSampler> samplers;
	std::string name;
}Animations;

typedef struct {
	double magFilter;
	double minFilter;
	double wrapS;
	double wrapT;
	std::string name;
}Samplers;

class Camera {
 public:
  Camera() {}
  ~Camera() {}

  std::string name;
  bool isOrthographic;  // false = perspective.

  // Some common properties.
  float aspectRatio;
  float yFov;
  float zFar;
  float zNear;
};

typedef struct {
  std::map<std::string, std::string> attributes;  // A dictionary object of
												  // strings, where each string
												  // is the ID of the accessor
												  // containing an attribute.
  std::string material;  // The ID of the material to apply to this primitive
						 // when rendering.
  std::string indices;   // The ID of the accessor that contains the indices.
  int mode;              // one of TINYGLTF_MODE_***
} Primitive;

typedef struct {
  std::string name;
  std::vector<Primitive> primitives;
} Mesh;

class Node {
 public:
  Node() {}
  ~Node() {}

  std::string camera;  // camera object referenced by this node.

  std::string name;
  std::string jointName;
  std::vector<std::string> children;
  std::vector<double> rotation;     // length must be 0 or 4
  std::vector<double> scale;        // length must be 0 or 3
  std::vector<double> translation;  // length must be 0 or 3
  std::vector<double> matrix;       // length must be 0 or 16
  std::vector<std::string> meshes;
  std::vector<std::string> skeletons;
  std::string skin;
};

typedef struct {
  std::string name;
  //std::string uri;
  //std::string type;
  //double bytelength;
  std::vector<unsigned char> data;
} Buffer;

typedef struct {
  std::string generator;
  std::string version;
  std::string profile_api;
  std::string profile_version;
  bool premultipliedAlpha;
} Asset;

typedef struct {
	std::vector<double> bindShapeMatrix;
	std::string inverseBindMatrices;
	std::vector<std::string> jointNames;
	std::string name;
} Skins;

class Scene {
 public:
  Scene() {}
  ~Scene() {}

  std::map<std::string, Accessor> accessors;
  std::map<std::string, Buffer> buffers;
  std::map<std::string, BufferView> bufferViews;
  std::map<std::string, Material> materials;
  std::map<std::string, Mesh> meshes;
  std::map<std::string, Node> nodes;
  std::map<std::string, Texture> textures;
  std::map<std::string, Image> images;
  std::map<std::string, std::vector<std::string> > scenes;  // list of nodes
  std::map<std::string, Program> programs;//着色器程序
  std::map<std::string, Shaders> shaders;//着色器代码
  std::map<std::string, Techniques> technique;
  std::map<std::string, Animations> animation;
  std::map<std::string, Samplers> sampler;//采样器
  std::map<std::string, Skins> skins; //皮肤
	
  std::string defaultScene;

  Asset asset;
};

class TinyGLTFLoader {
 public:
  TinyGLTFLoader() {}
  ~TinyGLTFLoader() {}

  /// Loads glTF asset from a file.
  /// Returns false and set error string to `err` if there's an error.
  bool LoadFromFile(Scene &scene, std::string &err,
					const std::string &filename);

  /// Loads glTF asset from string(memory).
  /// `length` = strlen(str);
  /// Returns false and set error string to `err` if there's an error.
  bool LoadFromString(Scene &scene, std::string &err, const char *str,
					  const unsigned int length, const std::string &baseDir);
};

}  // namespace tinygltf

#ifdef TINYGLTF_LOADER_IMPLEMENTATION
#include <sstream>
#include <fstream>
#include <cassert>
#include <algorithm>

#include "./picojson.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <wordexp.h>
#endif

namespace tinygltf {

bool FileExists(const std::string &abs_filename) {
  bool ret;
  FILE *fp = fopen(abs_filename.c_str(), "rb");
  if (fp) {
	ret = true;
	fclose(fp);
  } else {
	ret = false;
  }

  return ret;
}

std::string ExpandFilePath(const std::string &filepath) {
#ifdef _WIN32
  DWORD len = ExpandEnvironmentStringsA(filepath.c_str(), NULL, 0);
  char *str = new char[len];
  ExpandEnvironmentStringsA(filepath.c_str(), str, len);

  std::string s(str);

  delete[] str;

  return s;
#else

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
  // no expansion
  std::string s = filepath;
#else
  std::string s;
  wordexp_t p;

  if (filepath.empty()) {
	return "";
  }

  // char** w;
  int ret = wordexp(filepath.c_str(), &p, 0);
  if (ret) {
	// err
	s = filepath;
	return s;
  }

  // Use first element only.
  if (p.we_wordv) {
	s = std::string(p.we_wordv[0]);
	wordfree(&p);
  } else {
	s = filepath;
  }

#endif

  return s;
#endif
}

std::string JoinPath(const std::string &path0, const std::string &path1) {
  if (path0.empty()) {
	return path1;
  } else {
	// check '/'
	char lastChar = *path0.rbegin();
	if (lastChar != '/') {
	  return path0 + std::string("/") + path1;
	} else {
	  return path0 + path1;
	}
  }
}

std::string FindFile(const std::vector<std::string> &paths,
					 const std::string &filepath) {
  for (size_t i = 0; i < paths.size(); i++) {
	std::string absPath = ExpandFilePath(JoinPath(paths[i], filepath));
	if (FileExists(absPath)) {
	  return absPath;
	}
  }

  return std::string();
}

// std::string GetFilePathExtension(const std::string& FileName)
//{
//    if(FileName.find_last_of(".") != std::string::npos)
//        return FileName.substr(FileName.find_last_of(".")+1);
//    return "";
//}

std::string GetBaseDir(const std::string &filepath) {
  if (filepath.find_last_of("/\\") != std::string::npos)
	return filepath.substr(0, filepath.find_last_of("/\\"));
  return "";
}

// std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const &s);

/*
   base64.cpp and base64.h

   Copyright (C) 2004-2008 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
	  claim that you wrote the original source code. If you use this source code
	  in a product, an acknowledgment in the product documentation would be
	  appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
	  misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

static const std::string base64_chars =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789+/";

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(std::string const &encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && (encoded_string[in_] != '=') &&
		 is_base64(encoded_string[in_])) {
	char_array_4[i++] = encoded_string[in_];
	in_++;
	if (i == 4) {
	  for (i = 0; i < 4; i++)
		char_array_4[i] = base64_chars.find(char_array_4[i]);

	  char_array_3[0] =
		  (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
	  char_array_3[1] =
		  ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
	  char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

	  for (i = 0; (i < 3); i++) ret += char_array_3[i];
	  i = 0;
	}
  }

  if (i) {
	for (j = i; j < 4; j++) char_array_4[j] = 0;

	for (j = 0; j < 4; j++)
	  char_array_4[j] = base64_chars.find(char_array_4[j]);

	char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
	char_array_3[1] =
		((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
	char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

	for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}

bool LoadExternalFile(std::vector<unsigned char> &out, std::string &err,
					  const std::string &filename, const std::string &basedir,
					  size_t reqBytes, bool checkSize) {
  out.clear();

  std::vector<std::string> paths;
  paths.push_back(basedir);
  paths.push_back(".");

  std::string filepath = FindFile(paths, filename);
  if (filepath.empty()) {
	err += "File not found : " + filename;
	return false;
  }

  std::ifstream f(filepath.c_str(), std::ifstream::binary);
  if (!f) {
	err += "File open error : " + filepath;
	return false;
  }

  f.seekg(0, f.end);
  size_t sz = f.tellg();
  std::vector<unsigned char> buf(sz);

  f.seekg(0, f.beg);
  f.read(reinterpret_cast<char *>(&buf.at(0)), sz);
  f.close();

  if (checkSize) {
	if (reqBytes == sz) {
	  out.swap(buf);
	  return true;
	} else {
	  std::stringstream ss;
	  ss << "File size mismatch : " << filepath << ", requestedBytes "
		 << reqBytes << ", but got " << sz << std::endl;
	  err += ss.str();
	  return false;
	}
  }

  out.swap(buf);
  return true;
}

bool IsDataURI(const std::string &in) {
  std::string header = "data:application/octet-stream;base64,";
  if (in.find(header) == 0) {
	return true;
  }

  header = "data:image/png;base64,";
  if (in.find(header) == 0) {
	return true;
  }

  header = "data:image/jpeg;base64,";
  if (in.find(header) == 0) {
	return true;
  }

  return false;
}

bool DecodeDataURI(std::vector<unsigned char> &out, const std::string &in,
				   size_t reqBytes, bool checkSize) {
  std::string header = "data:application/octet-stream;base64,";
  std::string data;
  if (in.find(header) == 0) {
	data = base64_decode(in.substr(header.size()));  // cut mime string.
  }

  if (data.empty()) {
	header = "data:image/jpeg;base64,";
	if (in.find(header) == 0) {
	  data = base64_decode(in.substr(header.size()));  // cut mime string.
	}
  }

  if (data.empty()) {
	header = "data:image/png;base64,";
	if (in.find(header) == 0) {
	  data = base64_decode(in.substr(header.size()));  // cut mime string.
	}
  }

  if (data.empty()) {
	return false;
  }

  if (checkSize) {
	if (data.size() != reqBytes) {
	  return false;
	}
	out.resize(reqBytes);
  } else {
	out.resize(data.size());
  }
  std::copy(data.begin(), data.end(), out.begin());
  return true;

  return false;
}

bool ParseBooleanProperty(bool &ret, std::string &err,
						  const picojson::object &o,
						  const std::string &property, bool required) {
  picojson::object::const_iterator it = o.find(property);
  if (it == o.end()) {
	if (required) {
	  err += "'" + property + "' property is missing.\n";
	}
	return false;
  }

  if (!it->second.is<bool>()) {
	if (required) {
	  err += "'" + property + "' property is not a bool type.\n";
	}
	return false;
  }

  ret = it->second.get<bool>();

  return true;
}

bool ParseNumberProperty(double &ret, std::string &err,
						 const picojson::object &o, const std::string &property,
						 bool required) {
  picojson::object::const_iterator it = o.find(property);
  if (it == o.end()) {
	if (required) {
	  err += "'" + property + "' property is missing.\n";
	}
	return false;
  }

  if (!it->second.is<double>()) {
	if (required) {
	  err += "'" + property + "' property is not a number type.\n";
	}
	return false;
  }

  ret = it->second.get<double>();

  return true;
}

bool ParseNumberArrayProperty(std::vector<double> &ret, std::string &err,
							  const picojson::object &o,
							  const std::string &property, bool required) {
  picojson::object::const_iterator it = o.find(property);
  if (it == o.end()) {
	if (required) {
	  err += "'" + property + "' property is missing.\n";
	}
	return false;
  }

  if (!it->second.is<picojson::array>()) {
	if (required) {
	  err += "'" + property + "' property is not an array.\n";
	}
	return false;
  }

  ret.clear();
  const picojson::array &arr = it->second.get<picojson::array>();
  for (size_t i = 0; i < arr.size(); i++) {
	if (!arr[i].is<double>()) {
	  if (required) {
		err += "'" + property + "' property is not a number.\n";
	  }
	  return false;
	}
	ret.push_back(arr[i].get<double>());
  }

  return true;
}

bool ParseStringProperty(std::string &ret, std::string &err,
						 const picojson::object &o, const std::string &property,
						 bool required) {
  picojson::object::const_iterator it = o.find(property);
  if (it == o.end()) {
	if (required) {
	  err += "'" + property + "' property is missing.\n";
	}
	return false;
  }

  if (!it->second.is<std::string>()) {
	if (required) {
	  err += "'" + property + "' property is not a string type.\n";
	}
	return false;
  }

  ret = it->second.get<std::string>();

  return true;
}

bool ParseStringArrayProperty(std::vector<std::string> &ret, std::string &err,
							  const picojson::object &o,
							  const std::string &property, bool required) {
  picojson::object::const_iterator it = o.find(property);
  if (it == o.end()) {
	if (required) {
	  err += "'" + property + "' property is missing.\n";
	}
	return false;
  }

  if (!it->second.is<picojson::array>()) {
	if (required) {
	  err += "'" + property + "' property is not an array.\n";
	}
	return false;
  }

  ret.clear();
  const picojson::array &arr = it->second.get<picojson::array>();
  for (size_t i = 0; i < arr.size(); i++) {
	if (!arr[i].is<std::string>()) {
	  if (required) {
		err += "'" + property + "' property is not a string.\n";
	  }
	  return false;
	}
	ret.push_back(arr[i].get<std::string>());
  }

  return true;
}

bool ParseAsset(Asset &asset, std::string &err, const picojson::object &o) {
  ParseStringProperty(asset.generator, err, o, "generator", false);
  ParseBooleanProperty(asset.premultipliedAlpha, err, o, "premultipliedAlpha",
					   false);

  ParseStringProperty(asset.version, err, o, "version", false);

  picojson::object::const_iterator profile = o.find("profile");
  if (profile != o.end()) {
	const picojson::value &v = profile->second;
	if (v.contains("api") & v.get("api").is<std::string>()) {
	  asset.profile_api = v.get("api").get<std::string>();
	}
	if (v.contains("version") & v.get("version").is<std::string>()) {
	  asset.profile_version = v.get("version").get<std::string>();
	}
  }

  return true;
}



////原本的解析Image的函数
//bool ParseImage(Image &image, std::string &err, const picojson::object &o,
//				const std::string &basedir) {
//  std::string uri;
//  if (!ParseStringProperty(uri, err, o, "uri", true)) {
//	return false;
//  }
//
//  ParseStringProperty(image.name, err, o, "name", false);
//
//  std::vector<unsigned char> img;
//  if (IsDataURI(uri)) {
//	if (!DecodeDataURI(img, uri, 0, false)) {
//	  err += "Failed to decode 'uri'.\n";
//	  return false;
//	}
//  } else {
//	// Assume external file
//	if (!LoadExternalFile(img, err, uri, basedir, 0, false)) {
//	  err += "Failed to load external 'uri'.\n";
//	  return false;
//	}
//	if (img.empty()) {
//	  err += "File is empty.\n";
//	  return false;
//	}
//  }
//
//  int w, h, comp;
//  unsigned char *data =
//	  stbi_load_from_memory(&img.at(0), img.size(), &w, &h, &comp, 0);
//  if (!data) {
//	err += "Unknown image format.\n";
//	return false;
//  }
//
//  if (w < 1 || h < 1) {
//	err += "Unknown image format.\n";
//	return false;
//  }
//
//  image.width = w;
//  image.height = h;
//  image.component = comp;
//  image.image.resize(w * h * comp);
//  std::copy(data, data + w * h * comp, image.image.begin());
//
//  return true;
//}

bool ParseImage(Image &image, std::string &err, const picojson::object &o,
				const std::string &baseDir){
	ParseStringProperty(image.name, err, o, "name", false);
	ParseStringProperty(image.uri, err, o, "uri", false);

	return true;
}


bool ParseTexture(Texture &texture, std::string &err, const picojson::object &o,
				  const std::string &basedir) {
  if (!ParseStringProperty(texture.sampler, err, o, "sampler", true)) {
	return false;
  }

  if (!ParseStringProperty(texture.source, err, o, "source", true)) {
	return false;
  }

  ParseStringProperty(texture.name, err, o, "name", false);

  double format = TINYGLTF_TEXTURE_FORMAT_RGBA;
  ParseNumberProperty(format, err, o, "format", false);

  double internalFormat = TINYGLTF_TEXTURE_FORMAT_RGBA;
  ParseNumberProperty(internalFormat, err, o, "internalFormat", false);

  double target = TINYGLTF_TEXTURE_TARGET_TEXTURE2D;
  ParseNumberProperty(target, err, o, "target", false);

  double type = TINYGLTF_TEXTURE_TYPE_UNSIGNED_BYTE;
  ParseNumberProperty(type, err, o, "type", false);

  texture.format = static_cast<int>(format);
  texture.internalFormat = static_cast<int>(internalFormat);
  texture.target = static_cast<int>(target);
  texture.type = static_cast<int>(type);

  return true;
}

bool ParseBuffer(Buffer &buffer, std::string &err, const picojson::object &o,
				 const std::string &basedir) {
  double byteLength;
  if (!ParseNumberProperty(byteLength, err, o, "byteLength", true)) {
	return false;
  }

  std::string uri;
  if (!ParseStringProperty(uri, err, o, "uri", true)) {
	return false;
  }

  picojson::object::const_iterator type = o.find("type");
  if (type != o.end()) {
	if (type->second.is<std::string>()) {
	  const std::string &ty = (type->second).get<std::string>();
	  if (ty.compare("arraybuffer") == 0) {
		// buffer.type = "arraybuffer";
	  }
	}
  }

  size_t bytes = static_cast<size_t>(byteLength);
  if (IsDataURI(uri)) {
	if (!DecodeDataURI(buffer.data, uri, bytes, true)) {
	  err += "Failed to decode 'uri'.\n";
	  return false;
	}
  } else {
	// Assume external .bin file.
	if (!LoadExternalFile(buffer.data, err, uri, basedir, bytes, true)) {
	  return false;
	}
  }

  ParseStringProperty(buffer.name, err, o, "name", false);

  return true;
}

bool ParseBufferView(BufferView &bufferView, std::string &err,
					 const picojson::object &o) {
  std::string buffer;
  if (!ParseStringProperty(buffer, err, o, "buffer", true)) {
	return false;
  }

  double byteOffset;
  if (!ParseNumberProperty(byteOffset, err, o, "byteOffset", true)) {
	return false;
  }

  double byteLength = 0.0;
  ParseNumberProperty(byteLength, err, o, "byteLength", false);

  double target = 0.0;
  ParseNumberProperty(target, err, o, "target", false);
  int targetValue = static_cast<int>(target);
  if ((targetValue == TINYGLTF_TARGET_ARRAY_BUFFER) ||
	  (targetValue == TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER)) {
	// OK
  } else {
	targetValue = 0;
  }
  bufferView.target = targetValue;

  ParseStringProperty(bufferView.name, err, o, "name", false);

  bufferView.buffer = buffer;
  bufferView.byteOffset = static_cast<size_t>(byteOffset);
  bufferView.byteLength = static_cast<size_t>(byteLength);

  return true;
}

bool ParseAccessor(Accessor &accessor, std::string &err,
				   const picojson::object &o) {
  std::string bufferView;
  if (!ParseStringProperty(bufferView, err, o, "bufferView", true)) {
	return false;
  }

  double byteOffset;
  if (!ParseNumberProperty(byteOffset, err, o, "byteOffset", true)) {
	return false;
  }

  double componentType;
  if (!ParseNumberProperty(componentType, err, o, "componentType", true)) {
	return false;
  }

  double count = 0.0;
  if (!ParseNumberProperty(count, err, o, "count", true)) {
	return false;
  }

  std::string type;
  if (!ParseStringProperty(type, err, o, "type", true)) {
	return false;
  }

  if (type.compare("SCALAR") == 0) {
	accessor.type = TINYGLTF_TYPE_SCALAR;
  } else if (type.compare("VEC2") == 0) {
	accessor.type = TINYGLTF_TYPE_VEC2;
  } else if (type.compare("VEC3") == 0) {
	accessor.type = TINYGLTF_TYPE_VEC3;
  } else if (type.compare("VEC4") == 0) {
	accessor.type = TINYGLTF_TYPE_VEC4;
  } else if (type.compare("MAT2") == 0) {
	accessor.type = TINYGLTF_TYPE_MAT2;
  } else if (type.compare("MAT3") == 0) {
	accessor.type = TINYGLTF_TYPE_MAT3;
  } else if (type.compare("MAT4") == 0) {
	accessor.type = TINYGLTF_TYPE_MAT4;
  } else {
	std::stringstream ss;
	ss << "Unsupported `type` for accessor object. Got \"" << type << "\"\n";
	err += ss.str();
	return false;
  }

  double byteStride = 0.0;
  ParseNumberProperty(byteStride, err, o, "byteStride", false);

  ParseStringProperty(accessor.name, err, o, "name", false);

  accessor.minValues.clear();
  accessor.maxValues.clear();
  ParseNumberArrayProperty(accessor.minValues, err, o, "min", false);
  ParseNumberArrayProperty(accessor.maxValues, err, o, "max", false);

  accessor.count = static_cast<size_t>(count);
  accessor.bufferView = bufferView;
  accessor.byteOffset = static_cast<size_t>(byteOffset);
  accessor.byteStride = static_cast<size_t>(byteStride);

  {
	int comp = static_cast<size_t>(componentType);
	if (comp >= TINYGLTF_COMPONENT_TYPE_BYTE &&
		comp <= TINYGLTF_COMPONENT_TYPE_DOUBLE) {
	  // OK
	  accessor.componentType = comp;
	} else {
	  std::stringstream ss;
	  ss << "Invalid `componentType` in accessor. Got " << comp << "\n";
	  err += ss.str();
	  return false;
	}
  }

  return true;
}

bool ParsePrimitive(Primitive &primitive, std::string &err,
					const picojson::object &o) {
  if (!ParseStringProperty(primitive.material, err, o, "material", true)) {
	return false;
  }

  double mode = static_cast<double>(TINYGLTF_MODE_TRIANGLES);
  ParseNumberProperty(mode, err, o, "mode", false);

  int primMode = static_cast<int>(mode);
  if (primMode != TINYGLTF_MODE_TRIANGLES) {
	err +=
		"Currently TinyGLTFLoader doesn not support primitive mode other \n"
		"than TRIANGLES.\n";
	return false;
  }
  primitive.mode = primMode;

  primitive.indices = "";
  ParseStringProperty(primitive.indices, err, o, "indices", false);

  primitive.attributes.clear();
  picojson::object::const_iterator attribsObject = o.find("attributes");
  if ((attribsObject != o.end()) &&
	  (attribsObject->second).is<picojson::object>()) {
	const picojson::object &attribs =
		(attribsObject->second).get<picojson::object>();
	picojson::object::const_iterator it(attribs.begin());
	picojson::object::const_iterator itEnd(attribs.end());
	for (; it != itEnd; it++) {
	  const std::string &name = it->first;
	  if (!(it->second).is<std::string>()) {
		err += "attribute expects string value.\n";
		return false;
	  }
	  const std::string &value = (it->second).get<std::string>();

	  primitive.attributes[name] = value;
	}
  }

  return true;
}

bool ParseMesh(Mesh &mesh, std::string &err, const picojson::object &o) {
  ParseStringProperty(mesh.name, err, o, "name", false);

  mesh.primitives.clear();
  picojson::object::const_iterator primObject = o.find("primitives");
  if ((primObject != o.end()) && (primObject->second).is<picojson::array>()) {
	const picojson::array &primArray =
		(primObject->second).get<picojson::array>();
	for (size_t i = 0; i < primArray.size(); i++) {
	  Primitive primitive;
	  ParsePrimitive(primitive, err, primArray[i].get<picojson::object>());
	  mesh.primitives.push_back(primitive);
	}
  }

  return true;
}

bool ParseNode(Node &node, std::string &err, const picojson::object &o) {
  ParseStringProperty(node.name, err, o, "name", false);

  ParseNumberArrayProperty(node.rotation, err, o, "rotation", false);
  ParseNumberArrayProperty(node.scale, err, o, "scale", false);
  ParseNumberArrayProperty(node.translation, err, o, "translation", false);
  ParseNumberArrayProperty(node.matrix, err, o, "matrix", false);
  ParseStringArrayProperty(node.meshes, err, o, "meshes", false);
  ParseStringArrayProperty(node.skeletons, err, o, "skeletons", false);
  ParseStringProperty(node.jointName, err, o, "jointName", false);
  ParseStringProperty(node.skin, err, o, "skin", false);

  node.children.clear();
  picojson::object::const_iterator childrenObject = o.find("children");
  if ((childrenObject != o.end()) &&
	  (childrenObject->second).is<picojson::array>()) {
	const picojson::array &childrenArray =
		(childrenObject->second).get<picojson::array>();
	for (size_t i = 0; i < childrenArray.size(); i++) {
	  if (!childrenArray[i].is<std::string>()) {
		err += "Invalid `children` array.\n";
		return false;
	  }
	  const std::string &childrenNode = childrenArray[i].get<std::string>();
	  node.children.push_back(childrenNode);
	}
  }

  return true;
}

bool ParseMaterial(Material &material, std::string &err,
				   const picojson::object &o) {
  ParseStringProperty(material.name, err, o, "name", false);
  ParseStringProperty(material.technique, err, o, "technique", false);

  material.values.clear();
  picojson::object::const_iterator valuesIt = o.find("values");
  if ((valuesIt != o.end()) && (valuesIt->second).is<picojson::object>()) {
	const picojson::object &valuesObject =
		(valuesIt->second).get<picojson::object>();
	picojson::object::const_iterator it(valuesObject.begin());
	picojson::object::const_iterator itEnd(valuesObject.end());

	for (; it != itEnd; it++) {
	  // Assume number values.
	  Parameter param;
	  if (ParseStringProperty(param.stringValue, err, valuesObject, it->first,
							  false)) {
		// Found string property.
	  } else if (!ParseNumberArrayProperty(param.numberArray, err, valuesObject,
										   it->first, false)) {
		// Fallback to numer property.
		double value;
		if (ParseNumberProperty(value, err, valuesObject, it->first, false)) {
		  param.numberArray.push_back(value);
		}
	  }

	  material.values[it->first] = param;
	}
  }

  return true;
}

bool ParseProgram(Program &program, std::string &err,
				   const picojson::object &o){
  ParseStringArrayProperty(program.attributes, err, o, "attributes", false);
  ParseStringProperty(program.name, err, o, "name", false);

  if (!ParseStringProperty(program.fragmentShader, err, o, "fragmentShader", true)) {
	  return false;
  }

  if (!ParseStringProperty(program.vertexShader, err, o, "vertexShader", true)) {
	  return false;
  }

  return true;
}

bool ParseShader (Shaders &shader, std::string &err,
				  const picojson::object &o) {
  if (!ParseStringProperty(shader.uri, err, o, "uri", true)) {
		return false;
  }
  if (!ParseNumberProperty(shader.type, err, o, "type", true)) {
	  return false;
  }
  ParseStringProperty(shader.name, err, o, "name", false);

  return true;
}

bool ParseTechniqueParameter(TechniquesParameter &parameter, std::string &err, 
							 const picojson::object &o){
	if(!ParseNumberProperty(parameter.type, err, o, "type", true)){
		return false;
	}
	ParseStringProperty(parameter.node, err, o, "node", false);
	ParseStringProperty(parameter.semantic, err, o, "semantic", false);
	ParseStringProperty(parameter.value, err, o, "value", false);
	//如果没有没有定义count，则将count的值置为0
	ParseNumberProperty(parameter.count, err, o, "count", false);

	return true;
}
/*
bool ParseTechniqueState(TechniquesState &state, std::string &err, const picojson::object &o){
	picojson::object::const_iterator it = o.begin();
	for(; it != o.end(); it++){
		ParseNumberArrayProperty(state.enable, err, o, "enable", false);
	}
	return true;
}
*/

bool ParseTechnique (Techniques &technique, std::string &err,
				  const picojson::object &o) {
  ParseStringProperty(technique.name, err, o, "name", false);
  ParseStringProperty(technique.program, err, o, "program", false);
  
  //解析Techniques.parameter
  technique.parameters.clear();
  picojson::object::const_iterator itpara = o.find("parameters");
  if((itpara != o.end()) && (itpara->second).is<picojson::object>()){
	  const picojson::object &paraObject = 
		  (itpara->second).get<picojson::object>();
	  picojson::object::const_iterator it(paraObject.begin());
	  picojson::object::const_iterator itEnd(paraObject.end());
	  for(; it != itEnd; it++){
		  TechniquesParameter parameter;
		  ParseTechniqueParameter(parameter, err, (it->second).get<picojson::object>());
		  technique.parameters[it->first] = parameter;
	  }

  }

  //解析Technique.attribute
  picojson::object::const_iterator itattr = o.find("attributes"); 
  if((itattr != o.end()) && (itattr->second).is<picojson::object>()){
	  const picojson::object &attrObject = 
		  (itattr->second).get<picojson::object>();
	  picojson::object::const_iterator it(attrObject.begin());
	  picojson::object::const_iterator itEnd(attrObject.end());
	  for(; it != itEnd; it++){
		  ParseStringProperty(technique.attributes[it->first], err, 
			  (itattr->second).get<picojson::object>(), it->first, false);
	  }

  }

  //解析Technique.uniform
  picojson::object::const_iterator ituniform = o.find("uniforms");
  if((ituniform != o.end()) && (ituniform->second).is<picojson::object>()){
	  const picojson::object &uniformObject = 
		  (ituniform->second).get<picojson::object>();
	  picojson::object::const_iterator it(uniformObject.begin());
	  picojson::object::const_iterator itEnd(uniformObject.end());
	  for(; it != itEnd; it++){
		  ParseStringProperty(technique.uniforms[it->first], err, 
			  (ituniform->second).get<picojson::object>(), it->first, false);
	  }
  }

  //解析Technique.State
  picojson::object::const_iterator itstate = o.find("states");
  ParseNumberArrayProperty(technique.states.enable, err, 
	  (itstate->second).get<picojson::object>(), "enable", false);


  return true;
}

bool ParseAnimationChannelTarget(AnimationChannelTarget &target, std::string &err, 
								 const picojson::object &o){
	ParseStringProperty(target.id, err, o, "id", false);
	ParseStringProperty(target.path, err, o, "path", false);

	return true;
}


bool ParseAnimationChannel(AnimationChannel &animationchanel, std::string &err,
				  const picojson::object &o){
	ParseStringProperty(animationchanel.sampler, err, o, "sampler", false);

	picojson::object::const_iterator it = o.find("target");
	ParseAnimationChannelTarget(animationchanel.target, err, 
		(it->second).get<picojson::object>());

	return true;
}

bool ParseAnimationParameter(AnimationParameters &animationparameter, std::string &err, 
							 const picojson::object &o){
	ParseStringProperty(animationparameter.TIME, err, o, "TIME", false);
	ParseStringProperty(animationparameter.rotation, err, o, "rotation", false);
	ParseStringProperty(animationparameter.scale, err, o, "scale", false);
	ParseStringProperty(animationparameter.translation, err, o, "translation", false);

	return true;
}

bool ParseAnimationSampler(AnimationSampler &animationsampler, std::string &err, 
						   const picojson::object &o){
	ParseStringProperty(animationsampler.input, err, o, "input", false);
	ParseStringProperty(animationsampler.output, err, o, "output", false);
	ParseStringProperty(animationsampler.interpolation, err, o, "interpolation", false);

	return true;
}

bool ParseAnimation(Animations &animation, std::string &err,
				  const picojson::object &o) {
	ParseStringProperty(animation.name, err, o, "name", false);

	//解析Animation.Channel
	animation.channels.clear();
	picojson::object::const_iterator channelObject = o.find("channels");
	if((channelObject != o.end()) && (channelObject->second).is<picojson::array>()){
		const picojson::array &channelArray = (channelObject->second).get<picojson::array>();
		for(size_t i=0; i<channelArray.size(); i++){
			AnimationChannel animationchannel;
			ParseAnimationChannel(animationchannel, err, 
				channelArray[i].get<picojson::object>());
			animation.channels.push_back(animationchannel);
		}

	}

	//解析Animation.Parameter
	picojson::object::const_iterator itpara = o.find("parameters");
	if ((itpara != o.end()) && (itpara->second).is<picojson::object>()) {
		ParseAnimationParameter(animation.parameters, err, 
			(itpara->second).get<picojson::object>());
	}

	//解析Animation.Sampler
	animation.samplers.clear();
	picojson::object::const_iterator itsampler = o.find("samplers");
	if ((itsampler != o.end()) && (itsampler->second).is<picojson::object>()) {
		const picojson::object &samplerObject =
			(itsampler->second).get<picojson::object>();
		picojson::object::const_iterator it(samplerObject.begin());
		picojson::object::const_iterator itEnd(samplerObject.end());
		for (; it != itEnd; it++) {
			AnimationSampler animationsampler;
			ParseAnimationSampler(animationsampler, err, 
				(it->second).get<picojson::object>());
			animation.samplers[it->first] = animationsampler;
		}
	}

	return true;
}

bool ParseSampler(Samplers &sampler, std::string &err,
				  const picojson::object &o){
	ParseNumberProperty(sampler.magFilter, err, o, "magFilter", false);
	ParseNumberProperty(sampler.minFilter, err, o, "minFilter", false);
	ParseNumberProperty(sampler.wrapS, err, o, "wrapS", false);
	ParseNumberProperty(sampler.wrapT, err, o, "wrapT", false);
	ParseStringProperty(sampler.name, err, o, "name", false);

	return true;
}

bool ParseSkin(Skins &skin, std::string &err,
				  const picojson::object &o){
	ParseStringProperty(skin.name, err, o, "name", false);
	ParseStringProperty(skin.inverseBindMatrices, err, o, "inverseBindMatrices", false);
	ParseNumberArrayProperty(skin.bindShapeMatrix, err, o, "matrix", false);

	skin.jointNames.clear();
	picojson::object::const_iterator jointObject = o.find("jointNames");
	if ((jointObject != o.end()) &&
		(jointObject->second).is<picojson::array>()) {
		const picojson::array &jointArray =
			(jointObject->second).get<picojson::array>();
		for (size_t i = 0; i < jointArray.size(); i++) {
			if (!jointArray[i].is<std::string>()) {
			err += "Invalid `jointNames` array.\n";
			return false;
			}
			const std::string &jointName = jointArray[i].get<std::string>();
			skin.jointNames.push_back(jointName);
			printf("jointName: %s\n", jointName.c_str());
		}
	}

	return true;
}

bool TinyGLTFLoader::LoadFromString(Scene &scene, std::string &err,
									const char *str, unsigned int length,
									const std::string &baseDir) {
  picojson::value v;
  std::string perr = picojson::parse(v, str, str + length);

  if (!perr.empty()) {
	err = perr;
	return false;
  }

  if (v.contains("scene") && v.get("scene").is<std::string>()) {
	// OK
  } else {
	err += "\"scene\" object not found in .gltf\n";
	return false;
  }

  if (v.contains("scenes") && v.get("scenes").is<picojson::object>()) {
	// OK
  } else {
	err += "\"scenes\" object not found in .gltf\n";
	return false;
  }

  if (v.contains("nodes") && v.get("nodes").is<picojson::object>()) {
	// OK
  } else {
	err += "\"nodes\" object not found in .gltf\n";
	return false;
  }

  if (v.contains("accessors") && v.get("accessors").is<picojson::object>()) {
	// OK
  } else {
	err += "\"accessors\" object not found in .gltf\n";
	return false;
  }

  if (v.contains("buffers") && v.get("buffers").is<picojson::object>()) {
	// OK
  } else {
	err += "\"buffers\" object not found in .gltf\n";
	return false;
  }

  if (v.contains("bufferViews") &&
	  v.get("bufferViews").is<picojson::object>()) {
	// OK
  } else {
	err += "\"bufferViews\" object not found in .gltf\n";
	return false;
  }
  if (v.contains("programs") &&
	  v.get("programs").is<picojson::object>()) {
	// OK
  } else {
	err += "\"programs\" object not found in .gltf\n";
	return false;
  }

  scene.buffers.clear();
  scene.bufferViews.clear();
  scene.accessors.clear();
  scene.meshes.clear();
  scene.nodes.clear();
  scene.programs.clear();
  scene.shaders.clear();
  scene.animation.clear();
  scene.sampler.clear();
  scene.technique.clear();
  scene.defaultScene = "";

  // 0. Parse Asset
  if (v.contains("asset") && v.get("asset").is<picojson::object>()) {
	const picojson::object &root = v.get("asset").get<picojson::object>();

	ParseAsset(scene.asset, err, root);
  }

  // 1. Parse Buffer
  if (v.contains("buffers") && v.get("buffers").is<picojson::object>()) {
	const picojson::object &root = v.get("buffers").get<picojson::object>();

	picojson::object::const_iterator it(root.begin());
	picojson::object::const_iterator itEnd(root.end());
	for (; it != itEnd; it++) {
	  Buffer buffer;
	  if (!ParseBuffer(buffer, err, (it->second).get<picojson::object>(),
					   baseDir)) {
		return false;
	  }

	  scene.buffers[it->first] = buffer;
	}
  }

  // 2. Parse BufferView
  if (v.contains("bufferViews") &&
	  v.get("bufferViews").is<picojson::object>()) {
	const picojson::object &root = v.get("bufferViews").get<picojson::object>();

	picojson::object::const_iterator it(root.begin());
	picojson::object::const_iterator itEnd(root.end());
	for (; it != itEnd; it++) {
	  BufferView bufferView;
	  if (!ParseBufferView(bufferView, err,
						   (it->second).get<picojson::object>())) {
		return false;
	  }

	  scene.bufferViews[it->first] = bufferView;
	}
  }

  // 3. Parse Accessor
  if (v.contains("accessors") && v.get("accessors").is<picojson::object>()) {
	const picojson::object &root = v.get("accessors").get<picojson::object>();

	picojson::object::const_iterator it(root.begin());
	picojson::object::const_iterator itEnd(root.end());
	for (; it != itEnd; it++) {
	  Accessor accessor;
	  if (!ParseAccessor(accessor, err, (it->second).get<picojson::object>())) {
		return false;
	  }

	  scene.accessors[it->first] = accessor;
	}
  }

  // 4. Parse Mesh
  if (v.contains("meshes") && v.get("meshes").is<picojson::object>()) {
	const picojson::object &root = v.get("meshes").get<picojson::object>();

	picojson::object::const_iterator it(root.begin());
	picojson::object::const_iterator itEnd(root.end());
	for (; it != itEnd; it++) {
	  Mesh mesh;
	  if (!ParseMesh(mesh, err, (it->second).get<picojson::object>())) {
		return false;
	  }

	  scene.meshes[it->first] = mesh;
	}
  }

  // 5. Parse Node
  if (v.contains("nodes") && v.get("nodes").is<picojson::object>()) {
	const picojson::object &root = v.get("nodes").get<picojson::object>();

	picojson::object::const_iterator it(root.begin());
	picojson::object::const_iterator itEnd(root.end());
	for (; it != itEnd; it++) {
	  Node node;
	  if (!ParseNode(node, err, (it->second).get<picojson::object>())) {
		return false;
	  }

	  scene.nodes[it->first] = node;
	}
  }

  // 6. Parse scenes.
  if (v.contains("scenes") && v.get("scenes").is<picojson::object>()) {
	const picojson::object &root = v.get("scenes").get<picojson::object>();

	picojson::object::const_iterator it(root.begin());
	picojson::object::const_iterator itEnd(root.end());
	for (; it != itEnd; it++) {
	  const picojson::object &o = (it->second).get<picojson::object>();
	  std::vector<std::string> nodes;
	  if (!ParseStringArrayProperty(nodes, err, o, "nodes", false)) {
		return false;
	  }

	  scene.scenes[it->first] = nodes;
	}
  }

  // 7. Parse default scenes.
  if (v.contains("scene") && v.get("scene").is<std::string>()) {
	const std::string defaultScene = v.get("scene").get<std::string>();

	scene.defaultScene = defaultScene;
  }

  // 8. Parse Material
  if (v.contains("materials") && v.get("materials").is<picojson::object>()) {
	const picojson::object &root = v.get("materials").get<picojson::object>();

	picojson::object::const_iterator it(root.begin());
	picojson::object::const_iterator itEnd(root.end());
	for (; it != itEnd; it++) {
	  Material material;
	  if (!ParseMaterial(material, err, (it->second).get<picojson::object>())) {
		return false;
	  }

	  scene.materials[it->first] = material;
	}
  }

  // 9. Parse Image
  if (v.contains("images") && v.get("images").is<picojson::object>()) {
	const picojson::object &root = v.get("images").get<picojson::object>();

	picojson::object::const_iterator it(root.begin());
	picojson::object::const_iterator itEnd(root.end());
	for (; it != itEnd; it++) {
	  Image image;
	  if (!ParseImage(image, err, (it->second).get<picojson::object>(),
					  baseDir)) {
		return false;
		}

	  scene.images[it->first] = image;
	}
  }

  // 9. Parse Texture
  if (v.contains("textures") && v.get("textures").is<picojson::object>()) {
	const picojson::object &root = v.get("textures").get<picojson::object>();

	picojson::object::const_iterator it(root.begin());
	picojson::object::const_iterator itEnd(root.end());
	for (; it != itEnd; it++) {
	  Texture texture;
	  if (!ParseTexture(texture, err, (it->second).get<picojson::object>(),
						baseDir)) {
		return false;
	  }

	  scene.textures[it->first] = texture;
	}
  }

  // 10. Parse Program
  if (v.contains("programs") && v.get("programs").is<picojson::object>()) {
	  const picojson::object &root = v.get("programs").get<picojson::object>();

	  picojson::object::const_iterator it(root.begin());
	  picojson::object::const_iterator itEnd(root.end());
	  for(; it != itEnd; it++) {
		  Program program;
		  if (!ParseProgram(program, err, (it->second).get<picojson::object>())) {
				  return false;
		  }

		  scene.programs[it->first] = program;
	  }
  }

  // 11. Parse Shaders
  if (v.contains("shaders") && v.get("shaders").is<picojson::object>()) {
	  const picojson::object &root = v.get("shaders").get<picojson::object>();

	  picojson::object::const_iterator it(root.begin());
	  picojson::object::const_iterator itEnd(root.end());
	  for(; it != itEnd; it++) {
		  Shaders shader;
		  if (!ParseShader(shader, err, (it->second).get<picojson::object>())) {
			  return false;
		  }

		  scene.shaders[it->first] = shader;
	  }
  }

  // 12. Parse Techniques
  if(v.contains("techniques") && v.get("techniques").is<picojson::object>()) {
	  const picojson::object &root = v.get("techniques").get<picojson::object>();

	  picojson::object::const_iterator it(root.begin());
	  picojson::object::const_iterator itEnd(root.end());
	  for(; it != itEnd; it++) {
		  Techniques technique;
		  if(!ParseTechnique(technique, err, (it->second).get<picojson::object>())){
			  return false;
		  }

		  scene.technique[it->first] = technique;
	  }
  }

  // 13. Parse Animations
  if(v.contains("animations") && v.get("animations").is<picojson::object>()) {
	  const picojson::object &root = v.get("animations").get<picojson::object>();

	  picojson::object::const_iterator it(root.begin());
	  picojson::object::const_iterator itEnd(root.end());
	  for(; it != itEnd; it++){
		  Animations animation;
		  if(!ParseAnimation(animation, err, (it->second).get<picojson::object>())){
			  return false;
		  }

		  scene.animation[it->first] = animation;
	  }
  }

  // 14. Parse Samplers
  if(v.contains("samplers") && v.get("samplers").is<picojson::object>()) {
	  const picojson::object &root = v.get("samplers").get<picojson::object>();

	  picojson::object::const_iterator it(root.begin());
	  picojson::object::const_iterator itEnd(root.end());
	  for(; it != itEnd; it++){
		  Samplers sampler;
		  if(!ParseSampler(sampler, err, (it->second).get<picojson::object>())){
			  return false;
		  }

		  scene.sampler[it->first] = sampler;
	  }
  }
  //15. Parse Skin
  if(v.contains("skins") && v.get("skins").is<picojson::object>()) {
	  const picojson::object &root = v.get("skins").get<picojson::object>();

	  picojson::object::const_iterator it(root.begin());
	  picojson::object::const_iterator itEnd(root.end());
	  for(; it != itEnd; it++){
		  Skins skin;
		  if(!ParseSkin(skin, err, (it->second).get<picojson::object>())){
			  return false;
		  }

		  scene.skins[it->first] = skin;
	  }
  }

  return true;
}

bool TinyGLTFLoader::LoadFromFile(Scene &scene, std::string &err,
								  const std::string &filename) {
  std::stringstream ss;

  std::ifstream f(filename.c_str());
  if (!f) {
	ss << "Failed to open file: " << filename << std::endl;
	err = ss.str();
	return false;
  }

  f.seekg(0, f.end);
  size_t sz = f.tellg();
  std::vector<char> buf(sz);

  f.seekg(0, f.beg);
  f.read(&buf.at(0), sz);
  f.close();
  //获取文件路径(不包括文件名)
  std::string basedir = GetBaseDir(filename);

  bool ret = LoadFromString(scene, err, &buf.at(0), buf.size(), basedir);

  return ret;
}

}  // namespace tinygltf

#endif  // TINYGLTF_LOADER_IMPLEMENTATION

#endif  // TINY_GLTF_LOADER_H_
