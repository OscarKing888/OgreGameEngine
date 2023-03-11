/***********************************************************************
    filename:   CEGUIFalTextComponent.cpp
    created:    Sun Jun 19 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "falagard/CEGUIFalTextComponent.h"
#include "falagard/CEGUIFalXMLEnumHelper.h"
#include "CEGUIFontManager.h"
#include "CEGUIExceptions.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUIFont.h"
#include "CEGUILeftAlignedRenderedString.h"
#include "CEGUIRightAlignedRenderedString.h"
#include "CEGUICentredRenderedString.h"
#include "CEGUIJustifiedRenderedString.h"
#include "CEGUIRenderedStringWordWrapper.h"
#include <iostream>

#if defined (CEGUI_USE_FRIBIDI)
    #include "CEGUIFribidiVisualMapping.h"
#elif defined (CEGUI_USE_MINIBIDI)
    #include "CEGUIMinibidiVisualMapping.h"
#else
    #include "CEGUIBiDiVisualMapping.h"
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    TextComponent::TextComponent() :
#ifndef CEGUI_BIDI_SUPPORT
        d_bidiVisualMapping(0),
#elif defined (CEGUI_USE_FRIBIDI)
        d_bidiVisualMapping(new FribidiVisualMapping),
#elif defined (CEGUI_USE_MINIBIDI)
        d_bidiVisualMapping(new MinibidiVisualMapping),
#else
    #error "BIDI Configuration is inconsistant, check your config!"
#endif
        d_bidiDataValid(false),
        d_formattedRenderedString(new LeftAlignedRenderedString(d_renderedString)),
        d_lastHorzFormatting(HTF_LEFT_ALIGNED),
        d_vertFormatting(VTF_TOP_ALIGNED),
        d_horzFormatting(HTF_LEFT_ALIGNED)
    {}

    TextComponent::~TextComponent()
    {
        delete d_bidiVisualMapping;
    }

    TextComponent::TextComponent(const TextComponent& obj) :
        FalagardComponentBase(obj),
        d_textLogical(obj.d_textLogical),
#ifndef CEGUI_BIDI_SUPPORT
        d_bidiVisualMapping(0),
#elif defined (CEGUI_USE_FRIBIDI)
        d_bidiVisualMapping(new FribidiVisualMapping),
#elif defined (CEGUI_USE_MINIBIDI)
        d_bidiVisualMapping(new MinibidiVisualMapping),
#endif
        d_bidiDataValid(false),
        d_renderedString(obj.d_renderedString),
        d_formattedRenderedString(obj.d_formattedRenderedString),
        d_lastHorzFormatting(obj.d_lastHorzFormatting),
        d_font(obj.d_font),
        d_vertFormatting(obj.d_vertFormatting),
        d_horzFormatting(obj.d_horzFormatting),
        d_textPropertyName(obj.d_textPropertyName),
        d_fontPropertyName(obj.d_fontPropertyName)
    {
    }

    TextComponent& TextComponent::operator=(const TextComponent& other)
    {
        if (this == &other)
            return *this;

        FalagardComponentBase::operator=(other);

        d_textLogical = other.d_textLogical;
        // note we do not assign the BiDiVisualMapping object, we just mark our
        // existing one as invalid so it's data gets regenerated next time it's
        // needed.
        d_bidiDataValid = false;
        d_renderedString = other.d_renderedString;
        d_formattedRenderedString = other.d_formattedRenderedString;
        d_lastHorzFormatting = other.d_lastHorzFormatting;
        d_font = other.d_font;
        d_vertFormatting = other.d_vertFormatting;
        d_horzFormatting = other.d_horzFormatting;
        d_textPropertyName = other.d_textPropertyName;
        d_fontPropertyName = other.d_fontPropertyName;

        return *this;
    }

    const String& TextComponent::getText() const
    {
        return d_textLogical;
    }

    void TextComponent::setText(const String& text)
    {
        d_textLogical = text;
        d_bidiDataValid = false;
    }

    const String& TextComponent::getFont() const
    {
        return d_font;
    }

    void TextComponent::setFont(const String& font)
    {
        d_font = font;
    }

    VerticalTextFormatting TextComponent::getVerticalFormatting() const
    {
        return d_vertFormatting;
    }

    void TextComponent::setVerticalFormatting(VerticalTextFormatting fmt)
    {
        d_vertFormatting = fmt;
    }

    HorizontalTextFormatting TextComponent::getHorizontalFormatting() const
    {
        return d_horzFormatting;
    }

    void TextComponent::setHorizontalFormatting(HorizontalTextFormatting fmt)
    {
        d_horzFormatting = fmt;
    }

    void TextComponent::setupStringFormatter(const Window& window,
                                             const RenderedString& rendered_string) const
    {
        const HorizontalTextFormatting horzFormatting = d_horzFormatPropertyName.empty() ? d_horzFormatting :
            FalagardXMLHelper::stringToHorzTextFormat(window.getProperty(d_horzFormatPropertyName));

        // no formatting change
        if (horzFormatting == d_lastHorzFormatting)
        {
            d_formattedRenderedString->setRenderedString(rendered_string);
            return;
        }

        d_lastHorzFormatting = horzFormatting;

        switch(horzFormatting)
        {
        case HTF_LEFT_ALIGNED:
            d_formattedRenderedString =
                new LeftAlignedRenderedString(rendered_string);
            break;

        case HTF_CENTRE_ALIGNED:
            d_formattedRenderedString =
                new CentredRenderedString(rendered_string);
            break;

        case HTF_RIGHT_ALIGNED:
            d_formattedRenderedString =
                new RightAlignedRenderedString(rendered_string);
            break;

        case HTF_JUSTIFIED:
            d_formattedRenderedString =
                new JustifiedRenderedString(rendered_string);
            break;

        case HTF_WORDWRAP_LEFT_ALIGNED:
            d_formattedRenderedString =
                new RenderedStringWordWrapper
                    <LeftAlignedRenderedString>(rendered_string);
            break;

        case HTF_WORDWRAP_CENTRE_ALIGNED:
            d_formattedRenderedString =
                new RenderedStringWordWrapper
                    <CentredRenderedString>(rendered_string);
            break;

        case HTF_WORDWRAP_RIGHT_ALIGNED:
            d_formattedRenderedString =
                new RenderedStringWordWrapper
                    <RightAlignedRenderedString>(rendered_string);
            break;

        case HTF_WORDWRAP_JUSTIFIED:
            d_formattedRenderedString =
                new RenderedStringWordWrapper
                    <JustifiedRenderedString>(rendered_string);
            break;
        }
    }

    void TextComponent::render_impl(Window& srcWindow, Rect& destRect, const CEGUI::ColourRect* modColours, const Rect* clipper, bool /*clipToDisplay*/) const
    {
        // get font to use
        Font* font;

        CEGUI_TRY
        {
            font = d_fontPropertyName.empty() ?
                (d_font.empty() ? srcWindow.getFont() : &FontManager::getSingleton().get(d_font))
                : &FontManager::getSingleton().get(srcWindow.getProperty(d_fontPropertyName));
        }
        CEGUI_CATCH (UnknownObjectException&)
        {
            font = 0;
        }

        // exit if we have no font to use.
        if (!font)
            return;

        const RenderedString* rs = &d_renderedString;
        // do we fetch text from a property
        if (!d_textPropertyName.empty())
        {
            // fetch text & do bi-directional reordering as needed
            String vis;
            #ifdef CEGUI_BIDI_SUPPORT
                BiDiVisualMapping::StrIndexList l2v, v2l;
                d_bidiVisualMapping->reorderFromLogicalToVisual(
                    srcWindow.getProperty(d_textPropertyName), vis, l2v, v2l);
            #else
                vis = srcWindow.getProperty(d_textPropertyName);
            #endif
            // parse string using parser from Window.
            d_renderedString =
                srcWindow.getRenderedStringParser().parse(vis, font, modColours);
        }
        // do we use a static text string from the looknfeel
        else if (!getTextVisual().empty())
            // parse string using parser from Window.
            d_renderedString = srcWindow.getRenderedStringParser().
                parse(getTextVisual(), font, modColours);
        // do we have to override the font?
        else if (font != srcWindow.getFont())
            d_renderedString = srcWindow.getRenderedStringParser().
                parse(srcWindow.getTextVisual(), font, modColours);
        // use ready-made RenderedString from the Window itself
        else
            rs = &srcWindow.getRenderedString();

        setupStringFormatter(srcWindow, *rs);
        d_formattedRenderedString->format(destRect.getSize());

        // Get total formatted height.
        const float textHeight = d_formattedRenderedString->getVerticalExtent();

        // handle dest area adjustments for vertical formatting.
        VerticalTextFormatting vertFormatting = d_vertFormatPropertyName.empty() ? d_vertFormatting :
            FalagardXMLHelper::stringToVertTextFormat(srcWindow.getProperty(d_vertFormatPropertyName));

        switch(vertFormatting)
        {
        case VTF_CENTRE_ALIGNED:
            destRect.d_top += (destRect.getHeight() - textHeight) * 0.5f;
            break;

        case VTF_BOTTOM_ALIGNED:
            destRect.d_top = destRect.d_bottom - textHeight;
            break;

        default:
            // default is VTF_TOP_ALIGNED, for which we take no action.
            break;
        }

        // calculate final colours to be used
        ColourRect finalColours;
        initColoursRect(srcWindow, modColours, finalColours);

        // offset the font little down so that it's centered within its own spacing
//        destRect.d_top += (font->getLineSpacing() - font->getFontHeight()) * 0.5f;
        // add geometry for text to the target window.
        d_formattedRenderedString->draw(srcWindow.getGeometryBuffer(),
                                        destRect.getPosition(),
                                        &finalColours, clipper);
    }

    void TextComponent::writeXMLToStream(XMLSerializer& xml_stream) const
    {
        // opening tag
        xml_stream.openTag("TextComponent");
        // write out area
        d_area.writeXMLToStream(xml_stream);

        // write text element
        if (!d_font.empty() && !getText().empty())
        {
            xml_stream.openTag("Text");
            if (!d_font.empty())
                xml_stream.attribute("font", d_font);
            if (!getText().empty())
                xml_stream.attribute("string", getText());
            xml_stream.closeTag();
        }

        // write text property element
        if (!d_textPropertyName.empty())
        {
            xml_stream.openTag("TextProperty")
                .attribute("name", d_textPropertyName)
                .closeTag();
        }

        // write font property element
        if (!d_fontPropertyName.empty())
        {
            xml_stream.openTag("FontProperty")
                .attribute("name", d_fontPropertyName)
                .closeTag();
        }

        // get base class to write colours
        writeColoursXML(xml_stream);

        // write vert format, allowing base class to do this for us if a propety is in use
        if (!writeVertFormatXML(xml_stream))
        {
            // was not a property, so write out explicit formatting in use
            xml_stream.openTag("VertFormat")
                .attribute("type", FalagardXMLHelper::vertTextFormatToString(d_vertFormatting))
                .closeTag();
        }

        // write horz format, allowing base class to do this for us if a propety is in use
        if (!writeHorzFormatXML(xml_stream))
        {
            // was not a property, so write out explicit formatting in use
            xml_stream.openTag("HorzFormat")
                .attribute("type", FalagardXMLHelper::horzTextFormatToString(d_horzFormatting))
                .closeTag();
        }

        // closing tag
        xml_stream.closeTag();
    }

    bool TextComponent::isTextFetchedFromProperty() const
    {
        return !d_textPropertyName.empty();
    }

    const String& TextComponent::getTextPropertySource() const
    {
        return d_textPropertyName;
    }

    void TextComponent::setTextPropertySource(const String& property)
    {
        d_textPropertyName = property;
    }

    bool TextComponent::isFontFetchedFromProperty() const
    {
        return !d_fontPropertyName.empty();
    }

    const String& TextComponent::getFontPropertySource() const
    {
        return d_fontPropertyName;
    }

    void TextComponent::setFontPropertySource(const String& property)
    {
        d_fontPropertyName = property;
    }

    const String& TextComponent::getTextVisual() const
    {
        // no bidi support
        if (!d_bidiVisualMapping)
            return d_textLogical;

        if (!d_bidiDataValid)
        {
            d_bidiVisualMapping->updateVisual(d_textLogical);
            d_bidiDataValid = true;
        }

        return d_bidiVisualMapping->getTextVisual();
    }

} // End of  CEGUI namespace section
