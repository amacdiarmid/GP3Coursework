#include "Texture.h"

GLuint loadTextureFromFile(const string& filename)
{
	SDL_Surface *imageSurface = IMG_Load(filename.c_str());
	if (!imageSurface)
	{
		cout << "can't load image" << filename << " " << IMG_GetError();
		return 0;
	}
	GLuint textureID = convertSDLSurfaceToTexture(imageSurface);
	SDL_FreeSurface(imageSurface);

	return textureID;
}

GLuint loadTextureFromFont(const string& fontFilename, int pointSize, const string& text)
{
	TTF_Font *font = TTF_OpenFont(fontFilename.c_str(), pointSize);
	if (!font)
	{
		cout << "unable to load font " << fontFilename << " " << TTF_GetError();
		return 0;
	}
	SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), { 255, 255, 255 });
	GLuint textureID = convertSDLSurfaceToTexture(textSurface);
	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);

	return textureID;
}

GLuint convertSDLSurfaceToTexture(SDL_Surface* surface)
{
	GLuint textureID = 0;
	GLint nOfColours = surface->format->BytesPerPixel;

	GLenum textureFormat = GL_RGB;
	GLenum internalFormat = GL_RGB8;

	if (nOfColours == 4) //contains an alpha channel
	{
		if (surface->format->Rmask == 0x000000ff)
		{
			textureFormat = GL_RGBA;
			internalFormat = GL_RGBA8;
		}
		else
		{
			textureFormat = GL_BGRA;
			internalFormat = GL_RGBA8;
		}
	}
	else if (nOfColours == 3)
	{
		if (surface->format->Rmask == 0x000000ff)
		{
			textureFormat = GL_RGB;
			internalFormat = GL_RGB8;
		}
		else
		{
			textureFormat = GL_BGR;
			internalFormat = GL_RGB8;
		}
	}
	else
	{
		cout << "warning: the image is not true colour..this will probable break" << endl;
		return 0;
	}

	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	

	return textureID;
}

Texture::Texture()
{
}

Texture::Texture(string tempName)
{
	name = tempName;
	tex2D = true;
}

Texture::~Texture()
{
}

void Texture::createTexture(string texturePath)
{
	//load texture & bind
	string Path = ASSET_PATH + TEXTURE_PATH + texturePath;
	textureMap = loadTextureFromFile(Path);
	
	//glGenTextures(1, &textureMap);
	glBindTexture(GL_TEXTURE_2D, textureMap);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//mipmap
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);

	GLenum err = GL_NO_ERROR;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		//Process/log the error.
		cout << "error in creating texture " << err << endl;
	}

}

void Texture::cleanUp()
{
	glDeleteTextures(1, &textureMap);
}

GLuint Texture::getTexture()
{
	return textureMap;
}

string Texture::getName()
{
	return name;
}
