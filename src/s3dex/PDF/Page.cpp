﻿# include "Siv3Dex/Page.hpp"

namespace s3dex
{
	const Array<String>& Page::getText() const
	{
		return m_texts;
	}

	const Texture Page::getTexture() const
	{
		return m_texture;
	}

	void Page::drawTexture(double x, double y) const
	{
		m_texture.draw(x, y);
	}
}