#include "ModuleTexture.h"
#include "IL/il.h"
#include "IL/ilut.h"


std::string ModuleTexture::getPathName(const std::string& s) {

	char sep = '/';

#ifdef _WIN32
	sep = '\\';
#endif

	size_t i = s.rfind(sep, s.length());
	if (i != std::string::npos) {
		return(s.substr(0, i));
	}

	return("");
}



ModuleTexture::ModuleTexture() {
}

ModuleTexture::~ModuleTexture()
{
	ilShutDown();
}

bool ModuleTexture::Init() {

	LOG("Initialising DevIL context");

	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
		LOG("ERROR: WRONG DevIL version");
		return false;
	}
	ilInit();
	iluInit();
	ilutInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	return true;
}


unsigned int ModuleTexture::LoadTexture(const char* file_path, const char* mesh_path) {
	unsigned int imageID;

	ilGenImages(1, &imageID);					// Generation of one image name
	ilBindImage(imageID);						// Binding of image name 
	ILboolean success = ilLoadImage(file_path); // Loading of image "image.jpg"
	if (!success)
	{
		LOG("Could not load the image file '%s' from the path described in the FBX.\nTrying in the FBX folder.\n", file_path);

		const char* filename = strrchr(file_path, '\\') ? strrchr(file_path, '\\') + 1 : file_path; // obtain the filename from the texture path

		std::string dirPath(mesh_path);
		dirPath = getPathName(dirPath);	// Path
		dirPath.append("\\");			// Path/
		dirPath.append(filename);		// Path/filename

		const char* newPath = dirPath.c_str();

		success = ilLoadImage(newPath);
		if (!success)
		{
			LOG("Could not load the image file '%s' from the same folder of the FBX.\nTrying in Textures folder.\n", newPath);

			std::string dirTex("./Textures/");
			dirTex.append(filename);
			const char* dirTexture = dirTex.c_str();
			success = ilLoadImage(dirTexture);
		}
	}
	if (success) {
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); // Convert every colour component into unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA

		ILinfo info;
		iluGetImageInfo(&info);
		if (info.Origin == IL_ORIGIN_UPPER_LEFT) {
			iluFlipImage();
		}
		texWidth = info.Width;
		texHeight = info.Height;

		if (!success)
		{
			LOG("ERROR: Could not comvert the image '%s' to a texture object", file_path);
			return false;
		}
		glGenTextures(1, &textureID);				// Texture name generation
		glBindTexture(GL_TEXTURE_2D, textureID);	// Binding of texture name
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// We will use linear interpolation for magnification filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// We will use linear interpolation for minifying filter
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData()); // Texture specification
	}
	else
	{
		LOG("ERROR: Could not load the image file '%s'", file_path);
		return false;
	}
	ilDeleteImages(1, &imageID);				// Because we have already copied image data into texture data we can release memory used by image.
	return textureID;
}