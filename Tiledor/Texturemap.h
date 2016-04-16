#pragma once

//this contains the textures of all the possible tiles
class Texturemap
{
public:
	~Texturemap();
	void makeMap(const string &_file, int tileRes);	//load image and chop it into tiles of requested size

	vector<SDL_Texture*> map;	//this exists on renderer one (individual tile textures)
	SDL_Texture* fullTex;		//and this on renderer two (for displaying the bitmap)
	SDL_Rect rect;
};

