#include "Button.h"

Button::Button()
{
	hover = click = press = flag = false;
}

Button::Button(int posx, int posy, int szx, int szy, const Color& _basic, const Color& _hover, const Color& _click, Font& font, String str) : Button()
{
	label.setCharacterSize(20);
	label.setFont(font);
	label.setString(str);
	label.setPosition(posx + szx / 2 - label.getGlobalBounds().width / 2, posy + 5);
	rect.setPosition(posx, posy);
	rect.setSize(Vector2f(szx, szy));
	rect.setFillColor(_basic);

	rect.setOutlineThickness(-1);
	rect.setOutlineColor(_hover);
	basicCol = _basic;
	hoverCol = _hover;
	clickCol = _click;
}

void Button::update(const Vector2f& curPos, const bool click)
{
	hover = rect.getGlobalBounds().contains(curPos);
	press = hover && click;

	if (!flag && press)
		flag = true;

	if (flag && !click)
	{
		flag = false;
		if (hover)
			this->click = true;
	}

	if (press)
	{
		rect.setFillColor(clickCol);
	}
	else if (hover)
	{
		rect.setFillColor(hoverCol);
	}
	else
	{
		rect.setFillColor(basicCol);
	}
}

void Button::draw(RenderWindow& window) const
{
	window.draw(rect);
	window.draw(label);
}

void Button::setPosition(const int x, const int y)
{
	rect.setPosition(x, y);
	label.setPosition(rect.getGlobalBounds().left + rect.getGlobalBounds().width / 2 - label.getGlobalBounds().width / 2, rect.getGlobalBounds().top + 5);
}

void Button::setSize(const int x, const int y)
{
	rect.setSize(Vector2f(x, y));
	label.setPosition(rect.getGlobalBounds().left + rect.getGlobalBounds().width / 2 - label.getGlobalBounds().width / 2, rect.getGlobalBounds().top + 5);
}

void Button::setFillColors(const Color& _basic, const Color& _hover, const Color& _click)
{
	basicCol = _basic;
	hoverCol = _hover;
	clickCol = _click;
	rect.setOutlineThickness(-1);
	rect.setOutlineColor(_hover);
}

void Button::setString(String str)
{
	label.setString(str);
	label.setPosition(rect.getGlobalBounds().left + rect.getGlobalBounds().width / 2 - label.getGlobalBounds().width / 2, rect.getGlobalBounds().top + 5);
}

bool Button::isPress() const
{
	return press;
}

bool Button::isClick() const
{
	bool t = click;
	click = false;
	return t;
}

bool Button::isHover() const
{
	return hover;
}