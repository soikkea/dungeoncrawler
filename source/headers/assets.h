#pragma once

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

class Assets {
public:
	static Assets& get()
	{
		static Assets me;
		return me;
	}

	Assets(Assets const&) = delete;
	void operator=(Assets const&) = delete;

	sf::Texture& getTexture(std::string name)
	{
		return _textures[name];
	}

	sf::Font& getFont(std::string name)
	{
		return _fonts[name];
	}

	void LoadTextures();
	void LoadFonts();
private:
	Assets();
	~Assets();

	std::map<std::string, sf::Texture> _textures;
	std::map<std::string, sf::Font> _fonts;
};