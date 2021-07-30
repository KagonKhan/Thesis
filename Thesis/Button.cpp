#include "pch.h"
#include "Button.h"

Button::FuncType Button::defaultFunc = [](const sf::Event&, Button&)->void {};

Button::Button(Widget* parent)
	: Widget(parent), m_MinWidth(0), m_MinHeight(0), m_MaxWidth(0), m_MaxHeight(0)
{

}

Button::Button(float minwidth, float minheight, float maxwidth, float maxheigth, Widget* parent)
	: Widget(parent), m_MinWidth(minwidth), m_MinHeight(minheight), m_MaxWidth(maxwidth), m_MaxHeight(maxheigth)
{

}

void Button::onMouseEntered()
{
}

void Button::onMouseLeft()
{
}


bool Button::processEvent(const sf::Event& sfevent)
{
	return false;
}











bool TextButton::processEvent(const sf::Event& sfevent)
{
	bool res = false;
	if (sfevent.type == sf::Event::MouseButtonReleased) {
		if (m_Shape.getGlobalBounds().contains(sfevent.mouseButton.x, sfevent.mouseButton.y)) {
			on_click(sfevent, *this);
			res = true;
		}
	}
	else if (sfevent.type == sf::Event::MouseMoved) {
		const sf::Vector2f mouse_pos(sfevent.mouseMove.x, sfevent.mouseMove.y);

		int old_status = m_Status;
		m_Status = Status::Idle;

		if (m_Shape.getGlobalBounds().contains(mouse_pos))
			m_Status |= Status::Hover;
		if ((old_status & Status::Hover) && !(m_Status & Status::Hover))
			onMouseLeft();
		else if (!(old_status & Status::Hover) && (m_Status & Status::Hover))
			onMouseEntered();
	}

	return res;
}

void TextButton::onMouseEntered()
{
	const float light = 1.4f;
	m_Shape.setOutlineColor(sf::Color(m_OutlineColor.r * light, m_OutlineColor.g * light, m_OutlineColor.b * light));
	m_Shape.setFillColor(sf::Color(m_FillColor.r * light, m_FillColor.g * light, m_FillColor.b * light));
}

void TextButton::onMouseLeft()
{
	m_Shape.setFillColor(m_FillColor);
	m_Shape.setOutlineColor(m_OutlineColor);
}

void TextButton::updateTextPosition()
{
	sf::Vector2f position((m_Shape.getSize() - m_Label.getSize()) / 2.f);
	position += m_Shape.getPosition();
	m_Label.setPosition(position);
}

TextButton::TextButton(std::string_view text, Widget* parent)
	: Button(200, 100, 0, 0, parent), m_Label(text, this)
{
	m_FillColor = sf::Color::Red;
	m_Shape.setFillColor(m_FillColor);
	adjustSize();
}

TextButton::TextButton(std::string_view text, const sf::Font& font, unsigned int charSize, const sf::Vector2f& size,
	float outlineThicc, const sf::Color& outlinecolor, const sf::Color& fillColor, const sf::Color& textColor,
	float minwidth, float minheight, float maxwidth, float maxheigth, Widget* parent)
	: Button(minwidth, minheight, maxwidth, maxheigth, parent),
	m_Label(text, font, charSize, this)
{
	setOutlineColor(outlinecolor);
	setFillColor(fillColor);
	setTextColor(textColor);
	setOutlineThickness(outlineThicc);
	setSize(size);
}

void TextButton::setPosition(const sf::Vector2f& pos)
{
	m_Position = pos;
	m_Shape.setPosition(m_Position);
	updateTextPosition();
}

void TextButton::setPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Shape.setPosition(m_Position);

	updateTextPosition();
}

void TextButton::setOutlineColor(const sf::Color& color)
{
	m_OutlineColor = color;
	m_Shape.setOutlineColor(m_OutlineColor);
}

void TextButton::setFillColor(const sf::Color& color)
{
	m_FillColor = color;
	m_Shape.setFillColor(m_FillColor);
}

void TextButton::setTextColor(const sf::Color& color)
{
	m_Label.setTextColor(color);
}

void TextButton::setOutlineThickness(float thickness)
{
	m_Shape.setOutlineThickness(thickness);
}

void TextButton::setSize(const sf::Vector2f& size)
{
	m_Shape.setSize(size);
}

void TextButton::setLetterSpacing(float spacing)
{
	m_Label.setLetterSpacing(spacing);
	adjustSize();
}

void TextButton::adjustSize()
{
	sf::Vector2f size(0, 0);
	size.x = m_Label.getSize().x + 2 * m_Label.getCharacterSize();
	size.y = m_Label.getSize().y + 1.5 * m_Label.getCharacterSize();

	if (m_MinWidth != 0 && size.x < m_MinWidth)
		size.x = m_MinWidth;
	if (m_MaxWidth != 0 && size.x > m_MaxWidth)
		size.x = m_MaxWidth;

	if (m_MinHeight != 0 && size.y < m_MinHeight)
		size.y = m_MinHeight;
	if (m_MaxHeight != 0 && size.y > m_MaxHeight)
		size.y = m_MaxHeight;

	m_Shape.setSize(size);
}

sf::Vector2f TextButton::getSize() const
{
	return m_Shape.getSize();
}


void TextButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Shape, states);
	target.draw(m_Label, states);
}
