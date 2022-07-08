#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Button
{
public:
	Button();

	Button(int posx, int posy, int szx, int szy, const Color& _basic, const Color& _hover, const Color& _click, Font& font, String str);

	void update(const Vector2f& curPos, const bool click);

	void draw(RenderWindow& window) const;

	void setPosition(const int x, const int y);

	void setSize(const int x, const int y);

	void setFillColors(const Color& _basic, const Color& _hover, const Color& _click);

	void setString(String str);

	bool isPress() const;

	bool isClick() const;

	bool isHover() const;

private:
	Text label;
	Color basicCol, hoverCol, clickCol;
	RectangleShape rect;
	mutable bool click;
	bool press;
	bool hover;
	bool flag;
};