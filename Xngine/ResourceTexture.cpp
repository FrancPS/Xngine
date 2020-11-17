#include "ResourceTexture.h"
#include "IL/il.h"
#include "IL/ilut.h"
#include "SDL.h"

ResourceTexture::ResourceTexture() {
	
}

ResourceTexture::~ResourceTexture()
{
	
}

unsigned int ResourceTexture::LoadTexture(const char* file_path) 
{
	LOG("Initialising DevIL context");

	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
		LOG("ERROR: WRONG DevIL version");
	}
	ilInit();
	iluInit();
	ilutInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	unsigned int texture, imageID;

	ilGenImages(1, &imageID); /* Generation of one image name */
	ilBindImage(imageID); /* Binding of image name */
	ILboolean success = ilLoadImage(file_path); /* Loading of image "image.jpg" */
	if (success) /* If no error occured: */
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); /* Convert every colour component into unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
		if (!success)
		{
			LOG("ERROR: Could not comvert the image '%s' to texture file", file_path);
			return false;
		}
		glGenTextures(1, &texture); /* Texture name generation */
		glBindTexture(GL_TEXTURE_2D, texture); /* Binding of texture name */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear interpolation for magnification filter */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear interpolation for minifying filter */
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData()); /* Texture specification */
	}
	else
	{
		LOG("ERROR: Could not load the image file '%s'", file_path);
		return false;
	}
	ilDeleteImages(1, &imageID); /* Because we have already copied image data into texture data we can release memory used by image. */

	ilShutDown();

	return texture;
}